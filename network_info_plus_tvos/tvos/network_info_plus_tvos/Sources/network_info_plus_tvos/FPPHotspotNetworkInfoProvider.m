#import "./include/network_info_plus_tvos/FPPHotspotNetworkInfoProvider.h"

@implementation FPPHotspotNetworkInfoProvider

- (void)fetchNetworkInfoWithCompletionHandler:
    (void (^)(FPPNetworkInfo *network))completionHandler {
  dispatch_async(dispatch_get_main_queue(), ^{
    // NEHotspotNetwork is explicitly unavailable on tvOS.
    completionHandler(nil);
  });
}

@end
