#import "./include/network_info_plus/FPPHotspotNetworkInfoProvider.h"
#import <NetworkExtension/NetworkExtension.h>

@implementation FPPHotspotNetworkInfoProvider

- (void)fetchNetworkInfoWithCompletionHandler:
  (void (^)(FPPNetworkInfo *network))completionHandler {
#if TARGET_OS_TV
  // tvOS doesn't support hotspot functionality
  dispatch_async(dispatch_get_main_queue(), ^{
    completionHandler(nil);
  });
#else
  if (@available(iOS 14, *)) {
    [NEHotspotNetwork fetchCurrentWithCompletionHandler:^(
              NEHotspotNetwork *network) {
      dispatch_async(dispatch_get_main_queue(), ^{
        if (network) {
          completionHandler([[FPPNetworkInfo alloc] initWithSSID:network.SSID
                                  BSSID:network.BSSID]);
          return;
        }
        completionHandler(nil);
      });
    }];
  } else {
    dispatch_async(dispatch_get_main_queue(), ^{
      completionHandler(nil);
    });
  }
#endif
}

@end
