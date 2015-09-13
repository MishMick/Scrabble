#import <UIKit/UIKit.h>

@class RootViewController;

@interface AppController : NSObject <UIApplicationDelegate> {
    UIWindow *window;
}

@property(nonatomic, readonly) RootViewController* viewController;

@end
static NSString * const kClientId = @"105131900840-qhfg9vsjrskp99i7ot8esaovilpfe113.apps.googleusercontent.com";




