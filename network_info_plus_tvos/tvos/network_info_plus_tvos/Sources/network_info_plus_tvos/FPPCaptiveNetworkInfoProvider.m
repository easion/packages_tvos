#import "./include/network_info_plus_tvos/FPPCaptiveNetworkInfoProvider.h"

@implementation FPPCaptiveNetworkInfoProvider

- (void)fetchNetworkInfoWithCompletionHandler:
    (void (^)(FPPNetworkInfo *network))completionHandler {
  dispatch_async(dispatch_get_main_queue(), ^{
    // CaptiveNetwork Wi-Fi identity APIs are explicitly unavailable on tvOS.
    completionHandler(nil);
  });
}

@end
