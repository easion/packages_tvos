// Copyright 2013 The Flutter Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

import Foundation

#if os(iOS) || os(tvOS)
  import Flutter
#elseif os(macOS)
  import FlutterMacOS
#endif

public class PathProviderPlugin: NSObject, FlutterPlugin, PathProviderApi {
  public static func register(with registrar: FlutterPluginRegistrar) {
    let instance = PathProviderPlugin()
    //print("PathProviderPlugin() calling...")
    // Workaround for https://github.com/flutter/flutter/issues/118103.
    #if os(iOS) || os(tvOS)
      let messenger = registrar.messenger()
    #else
      let messenger = registrar.messenger
    #endif
    PathProviderApiSetup.setUp(binaryMessenger: messenger, api: instance)
  }

	func getDirectoryPath(type: DirectoryType) -> String? {
		let directory = fileManagerDirectoryForType(type)
		guard let path = getDirectory(ofType: directory, type: type) else {
			return nil
		}
		
		var finalPath = path
		
		#if os(macOS)
		if type == .applicationSupport || type == .applicationCache {
			guard let bundleId = Bundle.main.bundleIdentifier else {
				return nil
			}
			let basePathURL = URL(fileURLWithPath: path)
			finalPath = basePathURL.appendingPathComponent(bundleId).path
		}
		#endif
		
		if type != .temp && !FileManager.default.fileExists(atPath: finalPath) {
			do {
				try FileManager.default.createDirectory(
					atPath: finalPath,
					withIntermediateDirectories: true,
					attributes: nil
				)
			} catch {
				print("Failed to create directory: \(error.localizedDescription)")
				return nil
			}
		}
		
		return finalPath
	}

  // Returns the path for the container of the specified app group.
  func getContainerPath(appGroupIdentifier: String) -> String? {
    return FileManager.default.containerURL(
      forSecurityApplicationGroupIdentifier: appGroupIdentifier)?.path
  }
}

/// Returns the FileManager constant corresponding to the given type.
private func fileManagerDirectoryForType(_ type: DirectoryType) -> FileManager.SearchPathDirectory {
    #if os(tvOS)
    switch type {
    case .applicationDocuments, .applicationSupport, .downloads:
        return .cachesDirectory
    case .applicationCache:
        return .cachesDirectory
    case .library:
        return .libraryDirectory
    case .temp:
        return .cachesDirectory 
    }
    #else
    switch type {
    case .applicationCache:
        return .cachesDirectory
    case .applicationDocuments:
        return .documentDirectory
    case .applicationSupport:
        return .applicationSupportDirectory
    case .downloads:
        return .downloadsDirectory
    case .library:
        return .libraryDirectory
    case .temp:
        return .cachesDirectory
    }
    #endif
}

/// Returns the user-domain directory of the given type.
private func getDirectory(ofType directory: FileManager.SearchPathDirectory, type: DirectoryType) -> String? {
    #if os(tvOS)
    if type == .temp {
        return NSTemporaryDirectory()
    }
    let paths = NSSearchPathForDirectoriesInDomains(
        directory,
        .userDomainMask,
        true)
    
    guard let basePath = paths.first else {
        return nil
    }
    
    let baseURL = URL(fileURLWithPath: basePath)
    switch type {
    case .applicationDocuments:
        return baseURL.appendingPathComponent("Documents").path
    case .applicationSupport:
        return baseURL.appendingPathComponent("AppSupport").path
    case .downloads:
        return baseURL.appendingPathComponent("Downloads").path
    case .applicationCache:
        return basePath
    case .library:
        return basePath
    case .temp:
        return NSTemporaryDirectory()
    }
    #else
    if type == .temp {
        return NSTemporaryDirectory()
    }
    let paths = NSSearchPathForDirectoriesInDomains(
        directory,
        .userDomainMask,
        true)
    return paths.first
    #endif
}

