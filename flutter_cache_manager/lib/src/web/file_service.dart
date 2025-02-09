import 'dart:developer' as developer;
import 'dart:async';
import 'dart:io';

import 'package:clock/clock.dart';
import 'package:flutter_cache_manager/src/web/mime_converter.dart';
import 'package:http/http.dart' as http;

///Flutter Cache Manager
///Copyright (c) 2019 Rene Floor
///Released under MIT License.

/// Defines the interface for a file service.
/// Most common file service will be an [HttpFileService], however one can
/// also make something more specialized. For example you could fetch files
/// from other apps or from local storage.
abstract class FileService {
  int concurrentFetches = 10;

  Future<FileServiceResponse> get(String url, {Map<String, String>? headers});
}

class GlobalProxyHttpClient extends http.BaseClient {
  final http.Client _inner;
  GlobalProxyHttpClient() : _inner = http.Client();

  @override
  Future<http.StreamedResponse> send(http.BaseRequest request) async {
    //developer.log('Sending request to: ${request.url}');
    //developer.log('Request headers: ${request.headers}');

    HttpClient httpClient = HttpClient();

    httpClient.findProxy = (uri) {
      Map<String, String> environment = {};

      if (HttpOverrides.current == null) {
        //developer.log('No http proxy for ${uri}!');
        return 'DIRECT';
      }
      final proxy =
          HttpOverrides.current!.findProxyFromEnvironment(uri, environment);
      //developer.log('Using proxy: $proxy');
      return proxy;
    };

    try {
      final ioRequest = await httpClient.openUrl(request.method, request.url);

      // Copy all headers from the original request
      request.headers.forEach((key, value) {
        ioRequest.headers.set(key, value);
      });

      // If it's a POST request, make sure to set the content length
      if (request is http.Request) {
        final bodyBytes = request.bodyBytes;
        ioRequest.contentLength = bodyBytes.length;
        ioRequest.add(bodyBytes);
      }

      //developer.log('Sending IO request...');
      final ioResponse = await ioRequest.close();
      //developer
      //    .log('Received response with status code: ${ioResponse.statusCode}');

      // Convert HttpHeaders to Map<String, String>
      final headers = <String, String>{};
      ioResponse.headers.forEach((name, values) {
        headers[name] = values.join(',');
      });

      //developer.log('Response headers: $headers');

      int? contentLength;
      if (ioResponse.contentLength >= 0) {
        contentLength = ioResponse.contentLength;
        //developer
        //    .log('Response ioResponse.contentLength: ${contentLength ?? -1}');
      }

      return http.StreamedResponse(
        ioResponse.cast<List<int>>(),
        ioResponse.statusCode,
        contentLength: contentLength,
        request: request,
        headers: headers,
        isRedirect: ioResponse.isRedirect,
        persistentConnection: ioResponse.persistentConnection,
        reasonPhrase: ioResponse.reasonPhrase,
      );
    } catch (e, bt) {
      developer.log('proxy Error occurred: $e', error: e, stackTrace: bt);
      //developer.log(bt.);
      rethrow;
    } finally {
      httpClient.close();
    }
  }

  @override
  void close() {
    _inner.close();
    super.close();
  }
}

/// [HttpFileService] is the most common file service and the default for
/// [WebHelper]. One can easily adapt it to use dio or any other http client.
class HttpFileService extends FileService {
  final http.Client _httpClient;

  HttpFileService({http.Client? httpClient})
      : _httpClient = httpClient ?? GlobalProxyHttpClient();
  //     : _httpClient = httpClient ?? http.Client();

  @override
  Future<FileServiceResponse> get(String url,
      {Map<String, String>? headers}) async {
    final req = http.Request('GET', Uri.parse(url));
    if (headers != null) {
      req.headers.addAll(headers);
    }
    final httpResponse = await _httpClient.send(req);

    return HttpGetResponse(httpResponse);
  }
}

/// Defines the interface for a get result of a [FileService].
abstract class FileServiceResponse {
  /// [content] is a stream of bytes
  Stream<List<int>> get content;

  /// [contentLength] is the total size of the content.
  /// If the size is not known beforehand contentLength is null.
  int? get contentLength;

  /// [statusCode] is expected to conform to an http status code.
  int get statusCode;

  /// Defines till when the cache should be assumed to be valid.
  DateTime get validTill;

  /// [eTag] is used when asking to update the cache
  String? get eTag;

  /// Used to save the file on the storage, includes a dot. For example '.jpeg'
  String get fileExtension;
}

/// Basic implementation of a [FileServiceResponse] for http requests.
class HttpGetResponse implements FileServiceResponse {
  HttpGetResponse(this._response);

  final DateTime _receivedTime = clock.now();

  final http.StreamedResponse _response;

  @override
  int get statusCode => _response.statusCode;

  String? _header(String name) {
    return _response.headers[name];
  }

  @override
  Stream<List<int>> get content => _response.stream;

  @override
  int? get contentLength => _response.contentLength;

  @override
  DateTime get validTill {
    // Without a cache-control header we keep the file for a week
    var ageDuration = const Duration(days: 7);
    final controlHeader = _header(HttpHeaders.cacheControlHeader);
    if (controlHeader != null) {
      final controlSettings = controlHeader.split(',');
      for (final setting in controlSettings) {
        final sanitizedSetting = setting.trim().toLowerCase();
        if (sanitizedSetting == 'no-cache') {
          ageDuration = Duration.zero;
        }
        if (sanitizedSetting.startsWith('max-age=')) {
          final validSeconds =
              int.tryParse(sanitizedSetting.split('=')[1]) ?? 0;
          if (validSeconds > 0) {
            ageDuration = Duration(seconds: validSeconds);
          }
        }
      }
    }

    return _receivedTime.add(ageDuration);
  }

  @override
  String? get eTag => _header(HttpHeaders.etagHeader);

  @override
  String get fileExtension {
    var fileExtension = '';
    final contentTypeHeader = _header(HttpHeaders.contentTypeHeader);
    if (contentTypeHeader != null) {
      final contentType = ContentType.parse(contentTypeHeader);
      fileExtension = contentType.fileExtension;
    }
    return fileExtension;
  }
}
