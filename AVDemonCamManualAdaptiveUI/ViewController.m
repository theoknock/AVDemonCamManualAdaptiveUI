//
//  ViewController.m
//  AVDemonCamManualAdaptiveUI
//
//  Created by Xcode Developer on 1/1/22.
//

#import "ViewController.h"
#import "ControlConfiguration.h"

#import <objc/runtime.h>


#define k_whole_round_time (2.0)

extern AVCaptureDevice * _Nonnull capture_device;
extern ControlView * _Nonnull control_view;
extern CADisplayLink * _Nonnull display_link;

@interface ControlView ()

@property (assign) NSTimeInterval frameDuration;

@end


/// <#Description#>
@implementation ControlView

static double rescale(double old_value, double old_min, double old_max, double new_min, double new_max) {
    return (new_max - new_min) * /*(fmax(old_min, fmin(old_value, old_max))*/ (old_value - old_min) / (old_max - old_min) + new_min;
};

void validateRadius(int a, int b)
{
    // test (test a and b) and c
    int c = a - b;
    int k = (c >> 31) & 0x1;
    int max = a - k * c;
    int min = b + k * c;
    printf("max = %d\nmin = %d",max,min);
}

static double (^CaptureDeviceConfigurationPropertyButtonAngle)(CaptureDeviceConfigurationControlProperty) = ^ double (CaptureDeviceConfigurationControlProperty property) {
    static double button_angle;
    button_angle = (double)(180.0 + (90.0 * ((double)property / 4.0)));
    
    return button_angle;
};
//static double (^CaptureDeviceConfigurationPropertyValueAngle)(double[3]) = ^ double (double capture_device_configuration_property_value_angle_arguments[3]) {
//    double rescaled_angle = rescale(capture_device_configuration_property_value_angle_arguments[0], capture_device_configuration_property_value_angle_arguments[1], capture_device_configuration_property_value_angle_arguments[2], CaptureDeviceConfigurationPropertyButtonAngle(CaptureDeviceConfigurationControlPropertyTorchLevel), CaptureDeviceConfigurationPropertyButtonAngle(CaptureDeviceConfigurationControlPropertyZoomFactor));
//    rescaled_angle = fmax(CaptureDeviceConfigurationPropertyButtonAngle(CaptureDeviceConfigurationControlPropertyTorchLevel), fmin(rescaled_angle, CaptureDeviceConfigurationPropertyButtonAngle(CaptureDeviceConfigurationControlPropertyZoomFactor)));
//
//    return rescaled_angle;
//};

static void (^(^(^property_component_renderer)(ControlRendererState))(void))(void);
static void (^(^render_property_component_init)(void))(void);
static void (^render_property_component)(void);
static void (^(^(^(^property_component_renderer_init)(void))(ControlRendererState))(void))(void) = ^{
    id objects[] = {
        ^{
            return ^ {
                for (CaptureDeviceConfigurationControlProperty property = CaptureDeviceConfigurationControlPropertyTorchLevel; property < CaptureDeviceConfigurationControlPropertySelected; property++) {
                    [CaptureDeviceConfigurationPropertyButton(property) setHidden:!([CaptureDeviceConfigurationPropertyButton(property) isHidden])];
                    [CaptureDeviceConfigurationPropertyButton(property) setSelected:FALSE];
                }
            };
        },
        ^{
            return ^ {
                [CaptureDeviceConfigurationPropertyButton(touch_property) setSelected:FALSE];
                for (CaptureDeviceConfigurationControlProperty property = CaptureDeviceConfigurationControlPropertyTorchLevel; property < CaptureDeviceConfigurationControlPropertySelected; property++) {
                    static CGFloat angle;
                    angle = CaptureDeviceConfigurationPropertyButtonAngle(property);
                    [CaptureDeviceConfigurationPropertyButton(property) setCenter:[[UIBezierPath bezierPathWithArcCenter:center_point radius:radius startAngle:degreesToRadians(angle) endAngle:degreesToRadians(angle) clockwise:FALSE] currentPoint]];
                    [CaptureDeviceConfigurationPropertyButton(property) setSelected:(property == touch_property)];
                    [CaptureDeviceConfigurationPropertyButton(property) setHidden:FALSE];
                }
            };
        },
        ^{
            return ^ {
                for (CaptureDeviceConfigurationControlProperty property = CaptureDeviceConfigurationControlPropertyTorchLevel; property < CaptureDeviceConfigurationControlPropertySelected; property++) {
                    [CaptureDeviceConfigurationPropertyButton(property) setHidden:!([CaptureDeviceConfigurationPropertyButton(property) isSelected])];
                }
            };
        },
        ^{
            CaptureDeviceConfigurationControlProperty touch_property_fixed = touch_property;
            CGFloat radius_fixed = radius;
            return ^ {
                [CaptureDeviceConfigurationPropertyButton(touch_property_fixed) setCenter:[[UIBezierPath bezierPathWithArcCenter:center_point radius:radius_fixed startAngle:degreesToRadians(touch_angle) endAngle:degreesToRadians(touch_angle) clockwise:FALSE] currentPoint]];
            };
        },
    };
    
    NSUInteger count = sizeof(objects) / sizeof(id);
    __block NSArray<void(^(^)(void))(void)> * property_component_renderers = [NSArray arrayWithObjects:objects count:count];
    return ^ (ControlRendererState property_component_renderer_state) {
        return [property_component_renderers objectAtIndex:property_component_renderer_state % 4];
    };
};

static void (^(^(^value_component_renderer)(ControlRendererState))(void))(void);
static void (^(^render_value_component_init)(void))(void);
static void (^render_value_component)(void);
static void (^(^(^(^value_component_renderer_init)(void))(ControlRendererState))(void))(void) = ^{
    
    static int angle;
    static int offset;
    void (^eventHandlerBlock)(void) = ^{
        ++angle;
        angle = angle % 180;
        for (CaptureDeviceConfigurationControlProperty property = CaptureDeviceConfigurationControlPropertyTorchLevel; property < CaptureDeviceConfigurationControlPropertySelected; property++) {
            offset = (CaptureDeviceConfigurationPropertyButtonAngle(property) - 90) + angle;
            [CaptureDeviceConfigurationPropertyButton(property) setCenter:[[UIBezierPath bezierPathWithArcCenter:center_point radius:radius startAngle:degreesToRadians(offset) endAngle:degreesToRadians(offset) clockwise:FALSE] currentPoint]];
        }
        if (angle >= 90.0)
        {
            [display_link invalidate];
            [display_link removeFromRunLoop:[NSRunLoop mainRunLoop] forMode:NSRunLoopCommonModes];
            angle = 0.0;
            offset = 0.0;
        }
    };
    
    void (^(^animation)(void))(void) = ^{
        const NSUInteger frameInterval = 360;
        [display_link invalidate];
        display_link = [CADisplayLink displayLinkWithTarget:eventHandlerBlock selector:@selector(invoke)];
        display_link.preferredFramesPerSecond = frameInterval;
        
        [display_link addToRunLoop:[NSRunLoop mainRunLoop] forMode:NSRunLoopCommonModes];
        return ^ {
            
        };
    };
    id objects[] = { animation , ^{ return ^{}; }, ^{ return ^{}; }, ^{ return ^{}; } };
    
    
    NSUInteger count = sizeof(objects) / sizeof(id);
    __block NSArray<void(^(^)(void))(void)> * value_component_renderers = [NSArray arrayWithObjects:objects count:count];
    
    return ^ (ControlRendererState value_component_renderer_state) {
        return [value_component_renderers objectAtIndex:(value_component_renderer_state % 4)];
    };
};



- (void)drawRect:(CGRect)rect {
    //    render_value_component();
}

static void (^(^(^touch_handler_init)(void))(UITouch * _Nonnull))(void) = ^{
    CaptureDeviceConfigurationPropertyButton = CaptureDeviceConfigurationPropertyButtons();
    center_point = CGPointMake(CGRectGetMaxX(UIScreen.mainScreen.bounds) - CaptureDeviceConfigurationPropertyButton(CaptureDeviceConfigurationControlPropertyTorchLevel).center.x,
                               CGRectGetMaxY(UIScreen.mainScreen.bounds) - CaptureDeviceConfigurationPropertyButton(CaptureDeviceConfigurationControlPropertyZoomFactor).center.x);
    radius_min = center_point.x - CGRectGetMidX(CaptureDeviceConfigurationPropertyButton(CaptureDeviceConfigurationControlPropertyTorchLevel).bounds);
    radius_max = CGRectGetMidX(UIScreen.mainScreen.bounds) - CGRectGetMaxX(CaptureDeviceConfigurationPropertyButton(CaptureDeviceConfigurationControlPropertyTorchLevel).bounds);
    radius = radius_max;
    control_renderer_state = ControlRendererStateProperty;
    render_property_component = (render_property_component_init = (property_component_renderer = property_component_renderer_init())(control_renderer_state))();
    render_value_component = (render_value_component_init = (value_component_renderer = value_component_renderer_init())(control_renderer_state))();
    
    return ^ (UITouch * _Nonnull touch) {
        return ^{
            touch_point = [touch preciseLocationInView:(touch).view];
            
            touch_angle = (atan2((touch_point).y - (center_point).y,
                                 (touch_point).x - (center_point).x)) * (180.0 / M_PI);
            if (touch_angle < 0.0) touch_angle += 360.0;
            touch_angle = fmaxf(CaptureDeviceConfigurationPropertyButtonAngle(CaptureDeviceConfigurationControlPropertyTorchLevel), fminf(touch_angle, CaptureDeviceConfigurationPropertyButtonAngle(CaptureDeviceConfigurationControlPropertyZoomFactor)));
            
            touch_property = (CaptureDeviceConfigurationControlProperty)round(rescale(touch_angle, CaptureDeviceConfigurationPropertyButtonAngle(CaptureDeviceConfigurationControlPropertyTorchLevel), CaptureDeviceConfigurationPropertyButtonAngle(CaptureDeviceConfigurationControlPropertyZoomFactor), CaptureDeviceConfigurationControlPropertyTorchLevel, CaptureDeviceConfigurationControlPropertyZoomFactor));
            
            radius = (radius != 0.0) ? fmaxf(fminf(center_point.x - CGRectGetMidX(CaptureDeviceConfigurationPropertyButton(CaptureDeviceConfigurationControlPropertyTorchLevel).bounds), (sqrt(pow((touch_point).x - (center_point).x, 2.0) + pow((touch_point).y - (center_point).y, 2.0)))), CGRectGetMidX(control_view.bounds) - CGRectGetMaxX(CaptureDeviceConfigurationPropertyButton(CaptureDeviceConfigurationControlPropertyTorchLevel).bounds))
            : CGRectGetMidX(control_view.bounds) - CGRectGetMaxX(CaptureDeviceConfigurationPropertyButton(CaptureDeviceConfigurationControlPropertyTorchLevel).bounds);
            
            render_property_component();
//            render_value_component();
        };
    };
};

- (void)touchesBegan:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event {
    //    (handle_touch = (touch_handler = touch_handler_init())(touches.anyObject));
    (handle_touch = touch_handler(touches.anyObject))();
    
}

- (void)touchesMoved:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event {
    handle_touch();
}

- (void)touchesEnded:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event {
    handle_touch();
    ++control_renderer_state;
    render_value_component = (render_value_component_init = (value_component_renderer = value_component_renderer_init())(control_renderer_state))();
    render_property_component = (render_property_component_init = (property_component_renderer = property_component_renderer_init())(control_renderer_state))();
    handle_touch();
    ++control_renderer_state;
    render_value_component = (render_value_component_init = (value_component_renderer = value_component_renderer_init())(control_renderer_state))();
    render_property_component = (render_property_component_init = (property_component_renderer = property_component_renderer_init())(control_renderer_state))();
    handle_touch();
}

+ (Class)layerClass {
    return [CAShapeLayer class];
}

- (instancetype)initWithFrame:(CGRect)frame {
    if (self == [super initWithFrame:frame]) {
        {
            [self setTranslatesAutoresizingMaskIntoConstraints:FALSE];
            [self setBackgroundColor:[UIColor clearColor]];
            [self setOpaque:FALSE];
            [self setClipsToBounds:FALSE];
        };

        {
            [self setUserInteractionEnabled:FALSE];
            {
                touch_handler = touch_handler_init();
//                [UIView animateWithDuration:2.0 delay:0.0 usingSpringWithDamping:1.0 initialSpringVelocity:1.0 options:UIViewAnimationOptionBeginFromCurrentState animations:^{
                    for (CaptureDeviceConfigurationControlProperty property = CaptureDeviceConfigurationControlPropertyTorchLevel; property < CaptureDeviceConfigurationControlPropertySelected; property++) {
                        [self addSubview:CaptureDeviceConfigurationPropertyButton(property)];
                        [CaptureDeviceConfigurationPropertyButton(property) setCenter:
                         [[UIBezierPath bezierPathWithArcCenter:center_point
                                                         radius:radius
                                                     startAngle:degreesToRadians(CaptureDeviceConfigurationPropertyButtonAngle(property)) - 90.0
                                                       endAngle:degreesToRadians(CaptureDeviceConfigurationPropertyButtonAngle(property)) - 90.0
                                                      clockwise:FALSE] currentPoint]];
                    }
//                } completion:^(BOOL finished) {
//                    [control_view setNeedsDisplay];
//                }];
            }
            [self setUserInteractionEnabled:TRUE];
        };
        
        
    }
    
    return self;
}


@end

@interface CameraView ()

@end

@implementation CameraView

+ (Class)layerClass {
    return  [AVCaptureVideoPreviewLayer class];
}

@end

@interface ViewController ()
{
    AVCaptureSession           * captureSession;
    AVCaptureDeviceInput       * captureInput;
    AVCaptureConnection        * captureConnection;
    AVCaptureVideoPreviewLayer * capturePreview;
    CGSize                       videoDimensions;
}

//@property (nonatomic, strong) CameraView * view;

@end

@implementation ViewController

//extern AVCaptureDevice * _Nonnull const * _Nonnull capture_device;
//extern __kindof UIView * _Nonnull control_view;

@dynamic view;

- (void)loadView {
    self.view = (CameraView *)[[CameraView alloc] initWithFrame:UIScreen.mainScreen.bounds];
    [(CameraView *)self.view setContentMode:UIViewContentModeScaleAspectFit];
    
    CGRect control_view_rect = CGRectMake(CGRectGetMinX(UIScreen.mainScreen.bounds),
                                          CGRectGetMaxY(UIScreen.mainScreen.bounds) - CGRectGetWidth(UIScreen.mainScreen.bounds),
                                          CGRectGetWidth(UIScreen.mainScreen.bounds),
                                          CGRectGetWidth(UIScreen.mainScreen.bounds));
    control_view = [[ControlView alloc] initWithFrame:UIScreen.mainScreen.bounds];
    [control_view.layer setBorderColor:[UIColor blueColor].CGColor];
    [control_view.layer setBorderWidth:0.5];
    [(CameraView *)self.view addSubview:control_view];
    
    //    CaptureDeviceConfigurationPropertyButton = CaptureDeviceConfigurationPropertyButtons();
    //    center_point = CGPointMake(CGRectGetMaxX(control_view.bounds) - CaptureDeviceConfigurationPropertyButton(CaptureDeviceConfigurationControlPropertyTorchLevel).center.x, CGRectGetMaxY(control_view.bounds) - CaptureDeviceConfigurationPropertyButton(CaptureDeviceConfigurationControlPropertyZoomFactor).center.y);
    //    radius = CGRectGetMidX(control_view.bounds) - CGRectGetMaxX(CaptureDeviceConfigurationPropertyButton(CaptureDeviceConfigurationControlPropertyTorchLevel).bounds);
    //    property_component_renderer = property_component_renderers();
    //    render_property_component = property_component_renderer();
    //    [UIView animateWithDuration:2.0 delay:0.0 usingSpringWithDamping:1.0 initialSpringVelocity:1.0 options:UIViewAnimationOptionBeginFromCurrentState animations:^{
    //        for (CaptureDeviceConfigurationControlProperty property = CaptureDeviceConfigurationControlPropertyTorchLevel; property < CaptureDeviceConfigurationControlPropertySelected; property++) {
    //            [control_view addSubview:CaptureDeviceConfigurationPropertyButton(property)];
    //            [CaptureDeviceConfigurationPropertyButton(property) setCenter:[[UIBezierPath bezierPathWithArcCenter:center_point
    //                                                                                                          radius:radius
    //                                                                                                      startAngle:degreesToRadians(CaptureDeviceConfigurationPropertyButtonAngle(property))
    //                                                                                                        endAngle:degreesToRadians(CaptureDeviceConfigurationPropertyButtonAngle(property))
    //                                                                                                       clockwise:FALSE] currentPoint]];
    //        }
    //    } completion:^(BOOL finished) {
    //        [control_view setNeedsDisplay];
    //    }];
    //    touch_handler = touch_handler_init();
}

- (void)viewDidLoad {
    [super viewDidLoad];
    
    [captureSession = [[AVCaptureSession alloc] init] setSessionPreset:([captureSession canSetSessionPreset:AVCaptureSessionPreset3840x2160]) ? AVCaptureSessionPreset3840x2160 : AVCaptureSessionPreset1920x1080];
    [captureSession beginConfiguration];
    {
        [captureInput  = [AVCaptureDeviceInput deviceInputWithDevice:[capture_device = [AVCaptureDevice defaultDeviceWithDeviceType:AVCaptureDeviceTypeBuiltInDualCamera mediaType:AVMediaTypeVideo position:AVCaptureDevicePositionBack] self] error:nil] setUnifiedAutoExposureDefaultsEnabled:TRUE];
        [captureSession addInput:([captureSession canAddInput:captureInput]) ? captureInput : nil];
        
        videoDimensions = CMVideoFormatDescriptionGetPresentationDimensions(capture_device.activeFormat.formatDescription, TRUE, FALSE);
        //        CGFloat video_maxY   = videoDimensions.height;
        //        CGFloat height_scale = (CGRectGetHeight(control_view.bounds) / videoDimensions.height); // the screen height is this many times smaller than the video height
        
        [capturePreview = (AVCaptureVideoPreviewLayer *)[(CameraView *)self.view layer] setSessionWithNoConnection:captureSession];
        [capturePreview setSessionWithNoConnection:captureSession];
        
        [captureConnection   = [[AVCaptureConnection alloc] initWithInputPort:captureInput.ports.firstObject videoPreviewLayer:capturePreview] setVideoOrientation:AVCaptureVideoOrientationPortrait];
        [captureSession addConnection:([captureSession canAddConnection:captureConnection]) ? captureConnection : nil];
    }
    [captureSession commitConfiguration];
    [captureSession startRunning];
}

@end
