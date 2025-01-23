#import "./include/network_info_plus/FPPCaptiveNetworkInfoProvider.h"
#import <SystemConfiguration/CaptiveNetwork.h>

@implementation FPPCaptiveNetworkInfoProvider
- (void)fetchNetworkInfoWithCompletionHandler:
  (void (^)(FPPNetworkInfo *network))completionHandler {
  dispatch_async(dispatch_get_main_queue(), ^{
#if TARGET_OS_TV
  // tvOS doesn't support getting SSID/BSSID information
  completionHandler(nil);
#else
  NSArray *interfaceNames = (__bridge_transfer id)CNCopySupportedInterfaces();
  for (NSString *interfaceName in interfaceNames) {
    NSDictionary *networkInfo =
      (__bridge_transfer id)CNCopyCurrentNetworkInfo(
        (__bridge CFStringRef)interfaceName);
    if (networkInfo) {
    NSString *ssid = networkInfo[(NSString *)kCNNetworkInfoKeySSID];
    NSString *bssid = networkInfo[(NSString *)kCNNetworkInfoKeyBSSID];
    completionHandler([[FPPNetworkInfo alloc] initWithSSID:ssid
                             BSSID:bssid]);
    return;
    }
  }
  completionHandler(nil);
#endif
  });
}

@end
