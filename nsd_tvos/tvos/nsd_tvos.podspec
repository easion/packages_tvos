#
# To learn more about a Podspec see http://guides.cocoapods.org/syntax/podspec.html.
# Run `pod lib lint nsd_tvos.podspec` to validate before publishing.
#
Pod::Spec.new do |s|
  s.name             = 'nsd_tvos'
  s.version          = '0.0.1'
  s.summary          = 'Flutter plugin for network service discovery'
  s.description      = <<-DESC
Flutter plugin for network service discovery (NSD/DNS-SD/Bonjour/mDNS)
                       DESC
  s.homepage         = 'https://github.com/sebastianhaberey/nsd'
  s.license          = { :type => 'MIT', :file => '../LICENSE' }
  s.author           = { 'Sebastian Haberey' => 'sebastian@haberey.com' }
  s.source           = { :path => '.' }
  s.source_files = 'nsd_tvos/Sources/nsd_tvos/**/*'
  s.dependency 'Flutter'
  s.platform = :tvos, '12.0'
  s.pod_target_xcconfig = { 'DEFINES_MODULE' => 'YES' }
  s.swift_version = '5.0'
end
