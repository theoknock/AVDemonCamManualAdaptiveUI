//
//  ViewController.h
//  AVDemonCamManualAdaptiveUI
//
//  Created by Xcode Developer on 1/1/22.
//

@import UIKit;
@import AVFoundation;

#import "ControlView.h"
static ControlView * _Nonnull control_view;


NS_ASSUME_NONNULL_BEGIN

#ifndef ViewController_h
#define ViewController_h


static AVCaptureDevice * _Nonnull capture_device;
static CADisplayLink * _Nonnull display_link;


static void (^(^touch_handler)(UITouch * _Nonnull))(void);
static void (^handle_touch)(void);


//@interface ControlView: UIView
//
//@end
//
NS_ASSUME_NONNULL_END

NS_ASSUME_NONNULL_BEGIN

@interface CameraView : UIView

@end

NS_ASSUME_NONNULL_END

NS_ASSUME_NONNULL_BEGIN


@interface ViewController : UIViewController <UIGestureRecognizerDelegate>

@property(nonatomic, strong) CameraView * view;

@end
#endif /* ViewController_h */

NS_ASSUME_NONNULL_END


