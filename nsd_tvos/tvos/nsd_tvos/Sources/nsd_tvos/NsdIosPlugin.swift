import Flutter

private let channelName = "com.haberey/nsd"

public class NsdtvosPlugin: NSObject, FlutterPlugin, NetServiceBrowserDelegate, NetServiceDelegate {

    // NetServiceBrowser is deprecated but Network Framework only provides equivalent functionality since tvOS 13
    // see https://developer.apple.com/forums/thread/682744

    private var methodChannel: FlutterMethodChannel
    private var serviceBrowsers: [String: NetServiceBrowser] = [:]
    private var services: [String: NetService] = [:]

    init(methodChannel: FlutterMethodChannel) {
        self.methodChannel = methodChannel
        super.init()
        log("plugin initialized; bundle=\(Bundle.main.bundleIdentifier ?? "<nil>"), tvOS=\(ProcessInfo.processInfo.operatingSystemVersionString)")
    }

    public static func register(with registrar: FlutterPluginRegistrar) {
        let methodChannel = FlutterMethodChannel(name: channelName, binaryMessenger: registrar.messenger())
        let instance = NsdtvosPlugin(methodChannel: methodChannel)
        registrar.addMethodCallDelegate(instance, channel: methodChannel)
        instance.log("method channel registered: \(channelName)")
    }

    public func handle(_ methodCall: FlutterMethodCall, result: @escaping FlutterResult) {

        switch methodCall.method {

        case "startDiscovery":
            startDiscovery(methodCall.arguments, result)

        case "stopDiscovery":
            stopDiscovery(methodCall.arguments, result)

        case "register":
            register(methodCall.arguments, result)

        case "resolve":
            resolve(methodCall.arguments, result)

        case "unregister":
            unregister(methodCall.arguments, result)

        default:
            result(FlutterMethodNotImplemented)
        }
    }

    private func startDiscovery(_ arguments: Any?, _ result: FlutterResult) {

        guard let handle = deserializeHandle(arguments) else {
            result(FlutterError(code: ErrorCause.illegalArgument.code, message: "Handle cannot be null", details: nil))
            return
        }

        guard let serviceType = deserializeServiceType(arguments) else {
            result(FlutterError(code: ErrorCause.illegalArgument.code, message: "Service type cannot be null", details: nil))
            return
        }

        let serviceBrowser = NetServiceBrowser()
        serviceBrowser.delegate = self
        serviceBrowsers[handle] = serviceBrowser // set before invoking search so that callback methods can access it
        let configuredTypes = Bundle.main.object(forInfoDictionaryKey: "NSBonjourServices") as? [String] ?? []
        log("start discovery; handle=\(handle), type=\(serviceType), domain=local., mainThread=\(Thread.isMainThread), configuredBonjourTypes=\(configuredTypes)")
        serviceBrowser.searchForServices(ofType: serviceType, inDomain: "local.")
        result(nil)
    }

    private func stopDiscovery(_ arguments: Any?, _ result: FlutterResult) {
        guard let handle = deserializeHandle(arguments) else {
            result(FlutterError(code: ErrorCause.illegalArgument.code, message: "Handle cannot be null", details: nil))
            return
        }

        guard let serviceBrowser = serviceBrowsers[handle] else {
            log("stop discovery rejected; unknown handle=\(handle), activeHandles=\(Array(serviceBrowsers.keys))")
            result(FlutterError(code: ErrorCause.illegalArgument.code, message: "Unknown handle: \(handle)", details: nil))
            return
        }

        log("stop discovery; handle=\(handle)")
        serviceBrowser.stop()
        result(nil)
    }

    private func resolve(_ arguments: Any?, _ result: FlutterResult) {
        guard let handle = deserializeHandle(arguments) else {
            result(FlutterError(code: ErrorCause.illegalArgument.code, message: "Handle cannot be null", details: nil))
            return
        }

        guard let service = deserializeService(arguments) else {
            result(FlutterError(code: ErrorCause.illegalArgument.code, message: "Invalid service info", details: nil))
            return
        }

        service.delegate = self
        services[handle] = service // set before invoking search so that callback methods can access it
        log("resolve service; handle=\(handle), name=\(service.name), type=\(service.type), domain=\(service.domain)")
        service.resolve(withTimeout: 10)
        result(nil)
    }

    private func register(_ arguments: Any?, _ result: FlutterResult) {
        guard let handle = deserializeHandle(arguments) else {
            result(FlutterError(code: ErrorCause.illegalArgument.code, message: "Handle cannot be null", details: nil))
            return
        }

        guard let service = deserializeService(arguments) else {
            result(FlutterError(code: ErrorCause.illegalArgument.code, message: "Invalid service info", details: nil))
            return
        }

        service.delegate = self
        services[handle] = service // set before invoking search so that callback methods can access it
        service.publish(options: [])
        result(nil)
    }

    private func unregister(_ arguments: Any?, _ result: FlutterResult) {
        guard let handle = deserializeHandle(arguments) else {
            result(FlutterError(code: ErrorCause.illegalArgument.code, message: "Handle cannot be null", details: nil))
            return
        }

        let service: NetService? = services[handle];
        service?.stop();
        result(nil);
    }

    public func netServiceBrowserWillSearch(_ serviceBrowser: NetServiceBrowser) {
        guard let handle = getHandle(serviceBrowser) else {
            log("browser will search but no handle was found")
            return
        }

        log("discovery started by OS; handle=\(handle)")
        methodChannel.invokeMethod("onDiscoveryStartSuccessful", arguments: serializeHandle(handle))
    }

    public func netServiceBrowser(_ serviceBrowser: NetServiceBrowser, didNotSearch errorDict: [String: NSNumber]) {
        guard let handle = getHandle(serviceBrowser) else {
            log("discovery failed but no handle was found; error=\(errorDict)")
            return
        }

        let errorCode = getErrorCode(errorDict["NSNetServicesErrorCode"])
        log("discovery failed; handle=\(handle), error=\(errorDict), mappedMessage=\(getErrorMessage(errorCode))")
        let arguments = serializeHandle(handle)
            .merging(serializeErrorCause(getErrorCause(errorCode)))
            .merging(serializeErrorMessage(getErrorMessage(errorCode)))
        methodChannel.invokeMethod("onDiscoveryStartFailed", arguments: arguments)
        serviceBrowser.delegate = nil
        serviceBrowsers[handle] = nil
    }

    public func netServiceBrowserDidStopSearch(_ serviceBrowser: NetServiceBrowser) {
        guard let handle = getHandle(serviceBrowser) else {
            log("browser stopped but no handle was found")
            return
        }

        log("discovery stopped by OS; handle=\(handle)")
        methodChannel.invokeMethod("onDiscoveryStopSuccessful", arguments: serializeHandle(handle))
        serviceBrowser.delegate = nil
        serviceBrowsers[handle] = nil
    }

    public func netServiceBrowser(_ serviceBrowser: NetServiceBrowser, didFind service: NetService, moreComing: Bool) {
        guard let handle = getHandle(serviceBrowser) else {
            log("service found but no browser handle was found; name=\(service.name), type=\(service.type)")
            return
        }

        log("service found; handle=\(handle), name=\(service.name), type=\(service.type), domain=\(service.domain), moreComing=\(moreComing)")
        let arguments = serializeHandle(handle).merging(serializeService(service))
        methodChannel.invokeMethod("onServiceDiscovered", arguments: arguments)
    }

    public func netServiceBrowser(_ serviceBrowser: NetServiceBrowser, didRemove service: NetService, moreComing: Bool) {
        guard let handle = getHandle(serviceBrowser) else {
            log("service removed but no browser handle was found; name=\(service.name), type=\(service.type)")
            return
        }

        log("service removed; handle=\(handle), name=\(service.name), type=\(service.type), moreComing=\(moreComing)")
        let arguments = serializeHandle(handle).merging(serializeService(service))
        methodChannel.invokeMethod("onServiceLost", arguments: arguments)
    }

    public func netServiceDidPublish(_ service: NetService) {
        guard let handle = getHandle(service) else {
            return
        }

        let arguments = serializeHandle(handle).merging(serializeService(service))
        methodChannel.invokeMethod("onRegistrationSuccessful", arguments: arguments)
    }

    public func netService(_ service: NetService, didNotPublish errorDict: [String: NSNumber]) {
        guard let handle = getHandle(service) else {
            return
        }

        let errorCode = getErrorCode(errorDict["NSNetServicesErrorCode"])

        let arguments = serializeHandle(handle)
                .merging(serializeErrorCause(getErrorCause(errorCode)))
                .merging(serializeErrorMessage(getErrorMessage(errorCode)))
        methodChannel.invokeMethod("onRegistrationFailed", arguments: arguments)
    }

    public func netServiceDidStop(_ service: NetService) {
        guard let handle = getHandle(service) else {
            return
        }

        service.delegate = nil
        services[handle] = nil

        methodChannel.invokeMethod("onUnregistrationSuccessful", arguments: serializeHandle(handle))
    }

    public func netServiceDidResolveAddress(_ service: NetService) {
        guard let handle = getHandle(service) else {
            log("service resolved but no resolve handle was found; name=\(service.name)")
            return
        }

        let serialized = serializeService(service)
        let selectedAddress = serialized["service.addresses"] as? String ?? "<nil>"
        log("service resolved; handle=\(handle), name=\(service.name), host=\(service.hostName ?? "<nil>"), port=\(service.port), nativeAddressCount=\(service.addresses?.count ?? 0), selectedAddress=\(selectedAddress)")
        service.delegate = nil
        services[handle] = nil

        let arguments = serializeHandle(handle).merging(serialized)
        methodChannel.invokeMethod("onResolveSuccessful", arguments: arguments)
    }

    public func netServiceDidNotResolve(_ service: NetService, didNotResolve errorDict: [String: NSNumber]) {
        guard let handle = getHandle(service) else {
            log("service resolve failed but no handle was found; name=\(service.name), error=\(errorDict)")
            return
        }

        service.delegate = nil
        services[handle] = nil

        let errorCode = getErrorCode(errorDict["NSNetServicesErrorCode"])
        log("service resolve failed; handle=\(handle), name=\(service.name), error=\(errorDict), mappedMessage=\(getErrorMessage(errorCode))")

        let arguments = serializeHandle(handle)
                .merging(serializeErrorCause(getErrorCause(errorCode)))
                .merging(serializeErrorMessage(getErrorMessage(errorCode)))
        methodChannel.invokeMethod("onResolveFailed", arguments: arguments)
    }

    private func getHandle(_ serviceBrowser: NetServiceBrowser) -> String? {
        serviceBrowsers.first(where: { $1 === serviceBrowser })?.key
    }

    private func getHandle(_ service: NetService) -> String? {
        services.first(where: { $1 === service })?.key
    }

    private func log(_ message: String) {
        NSLog("[nsd_tvos] %@", message)
    }
}

extension Dictionary {
    func merging(_ item: [Key: Value]) -> [Key: Value] {
        merging(item) { (a, b) in
            b
        }
    }
}
