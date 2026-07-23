import AVFoundation

public class VolumeController {
  private let audioSession: AVAudioSession
  
  init(audioSession: AVAudioSession) {
    self.audioSession = audioSession
  }

  public func getVolume() -> Float {
    return audioSession.getVolume()
  }

  public func setVolume(volume: Float, showSystemUI: Bool) {
    // AVAudioSession.outputVolume is read-only on tvOS, and UISlider (used by
    // the iOS MPVolumeView workaround) is unavailable. System volume must be
    // changed by the Siri Remote, HDMI-CEC receiver, or television.
  }

  public func isMuted() -> Bool {
    return getVolume() == 0
  }

  public func setMute(isMute: Bool, showSystemUI: Bool) {
    // Muting the system output programmatically is unsupported on tvOS.
  }
}
