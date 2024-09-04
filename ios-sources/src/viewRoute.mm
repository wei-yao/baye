#import "viewRoute.h"
#import <UIKit/UIKit.h>

@interface APLViewController: UIViewController <UINavigationControllerDelegate, UIImagePickerControllerDelegate>
{ }
@end

@implementation APLViewController

- (void)setInterfaceOrientation:(UIInterfaceOrientation)orientation
{
    if ([[UIDevice currentDevice] respondsToSelector:@selector(setOrientation:)])
    {
        SEL selector = NSSelectorFromString(@"setOrientation:");
        NSInvocation *invocation = [NSInvocation invocationWithMethodSignature:[UIDevice instanceMethodSignatureForSelector:selector]];
        [invocation setSelector:selector];
        [invocation setTarget:[UIDevice currentDevice]];
        int val = orientation;
        [invocation setArgument:&val atIndex:2];
        [invocation invoke];
    }
}

@end

void routeView(int a)
{
    void *context = [[APLViewController alloc] init];
    if(a == 0)
    {
        [(id) context setInterfaceOrientation:UIInterfaceOrientationLandscapeRight];

    }
    else
    {
        [(id) context setInterfaceOrientation:UIInterfaceOrientationPortrait];
    }
}
