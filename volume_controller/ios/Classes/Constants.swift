struct ChannelName {
  static let methodChannel = "com.kurenai7968.volume_controller.method"
  static let eventChannel = "com.kurenai7968.volume_controller.volume_listener_event"
}

struct MethodName {
  static let getVolume = "getVolume"
  static let setVolume = "setVolume"
}

struct MethodArgument {
  static let volume = "volume"
  static let showSystemUI = "showSystemUI"
}

struct EventArgument {
  static let fetchInitialVolume = "fetchInitialVolume"
}
