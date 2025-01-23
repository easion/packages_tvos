class ChannelName {
  static const String methodChannel =
      'com.kurenai7968.volume_controller.method';
  static const String eventChannel =
      'com.kurenai7968.volume_controller.volume_listener_event';
}

class MethodName {
  static const String getVolume = 'getVolume';
  static const String setVolume = 'setVolume';
}

class MethodArgument {
  static const String volume = 'volume';
  static const String showSystemUI = 'showSystemUI';
}

class EventArgument {
  static const String fetchInitialVolume = 'fetchInitialVolume';
}
