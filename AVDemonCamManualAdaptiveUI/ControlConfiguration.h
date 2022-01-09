//
//  ControlConfiguration.h
//  AVDemonCamManualAdaptive
//
//  Created by Xcode Developer on 12/21/21.
//

#include <objc/runtime.h>
#include <stdio.h>
#include <limits.h>



#ifndef ControlConfiguration_h
#define ControlConfiguration_h

#define CONTROL_STATES_COUNT     4
#define CONTROL_STATES_INDEX_MIN 0
#define CONTROL_STATES_INDEX_MAX 3

#define CONTROL_PROPERTIES_COUNT     5
#define CONTROL_PROPERTIES_INDEX_MIN 0
#define CONTROL_PROPERTIES_INDEX_MAX 4


typedef enum : const simd_uchar1 {
    ControlStateKeyTransition        = 0x01,
    ControlStateKey                  = 0x02,
    ControlStateValueTransition      = 0x04,
    ControlStateValue                = 0x08
} ControlState;
static ControlState control_states[CONTROL_STATES_COUNT] = {[CONTROL_STATES_INDEX_MIN ... CONTROL_STATES_INDEX_MAX] = 0x00};

typedef enum : const simd_uchar1 {
    ControlPropertyTorchLevel        = 0x01,
    ControlPropertyExposureDuration  = 0x02,
    ControlPropertyISO               = 0x04,
    ControlPropertyLensPosition      = 0x08,
    ControlPropertyZoomFactor        = 0x10,
    ControlPropertyAll               = 0x20,
    ControlPropertyVisible           = 0x40,
    ControlPropertyEnable            = 0x80
} ControlProperty;
static ControlProperty control_property[CONTROL_PROPERTIES_COUNT] = {[CONTROL_PROPERTIES_INDEX_MIN ... CONTROL_PROPERTIES_INDEX_MAX] = 0x01};
static ControlProperty selected_control[CONTROL_PROPERTIES_COUNT] = {[CONTROL_PROPERTIES_INDEX_MIN ... CONTROL_PROPERTIES_INDEX_MAX] = 0x00};
static ControlProperty enabled_controls[CONTROL_PROPERTIES_COUNT] = {[CONTROL_PROPERTIES_INDEX_MIN ... CONTROL_PROPERTIES_INDEX_MAX] = 0x01};
static const simd_uchar8 control_properties = (ControlPropertyTorchLevel | ControlPropertyLensPosition | ControlPropertyExposureDuration | ControlPropertyISO | ControlPropertyZoomFactor);

static CGPoint center_point;
static CGPoint touch_point;
static CGFloat touch_angle;


static double (^button_angles[CONTROL_PROPERTIES_COUNT])(void)  = {[CONTROL_PROPERTIES_INDEX_MIN ... CONTROL_PROPERTIES_INDEX_MAX] = ^{
    static int property_tags = 0x00;
    
    return ^ (const unsigned int property_tag) {
        static double touch_angle;
        
        touch_angle = (atan2((touch_point).y - (center_point).y,
                             (touch_point).x - (center_point).x)) * (180.0 / M_PI);
        if (touch_angle < 0.0) touch_angle += 360.0;
        touch_angle = fmaxf(180.0, fminf(touch_angle, 270.0));
        
        touch_angle = (double)(180.0 + (90.0 * ((double)property_tag / 4.0)));
        printf("touch_angle\t\t%f\n", touch_angle);
        return ^ double {
            return touch_angle;
        }();
    }(property_tags++);
}};

/*
 
 */

#define degreesToRadians(angleDegrees) (angleDegrees * M_PI / 180.0)

static CGFloat radius_min;
static CGFloat radius_max;
static int touch_property;
static CGFloat radius;

/*
 
 */

static NSArray<NSArray<NSString *> *> * const CaptureDeviceConfigurationControlPropertyImageSymbolValues = @[@[@"bolt.circle",
                                                                                                               @"timer",
                                                                                                               @"camera.aperture",
                                                                                                               @"viewfinder.circle",
                                                                                                               @"magnifyingglass.circle"], @[@"bolt.circle.fill",
                                                                                                                                             @"timer",
                                                                                                                                             @"camera.aperture",
                                                                                                                                             @"viewfinder.circle.fill",
                                                                                                                                             @"magnifyingglass.circle.fill"]];

static NSArray<NSString *> * const CaptureDeviceConfigurationControlPropertyImageSymbolKeys = @[@"CaptureDeviceConfigurationControlPropertyTorchLevel",
                                                                                                @"CaptureDeviceConfigurationControlPropertyExposureDuration",
                                                                                                @"CaptureDeviceConfigurationControlPropertyISO",
                                                                                                @"CaptureDeviceConfigurationControlPropertyLensPosition",
                                                                                                @"CaptureDeviceConfigurationControlPropertyZoomFactor"];

static NSString * (^CaptureDeviceConfigurationControlPropertySymbol)(UIControlState, int) = ^ NSString * (UIControlState state, int property) {
    return CaptureDeviceConfigurationControlPropertyImageSymbolValues[state][property];
};

static NSString * (^CaptureDeviceConfigurationControlPropertyString)(int) = ^ NSString * (int property) {
    return CaptureDeviceConfigurationControlPropertyImageSymbolKeys[property];
};

static UIImageSymbolConfiguration * (^CaptureDeviceConfigurationControlPropertySymbolImageConfiguration)(UIControlState) = ^ UIImageSymbolConfiguration * (UIControlState state) {
    UIImageSymbolWeight symbol_weight = UIImageSymbolWeightLight;
    switch (state) {
        case UIControlStateNormal: {
            symbol_weight = UIImageSymbolWeightLight;
        }
            break;
        case UIControlStateSelected: {
            symbol_weight = UIImageSymbolWeightThin;
        }
            break;
        case UIControlStateHighlighted: {
            symbol_weight = UIImageSymbolWeightRegular;
        }
            break;
        case UIControlStateDisabled: {
            symbol_weight = UIImageSymbolWeightUltraLight;
        }
            break;
        default: {
            symbol_weight = UIImageSymbolWeightLight;
        }
            break;
    }
    return [[[UIImageSymbolConfiguration configurationWithPointSize:42.0] configurationByApplyingConfiguration:[UIImageSymbolConfiguration configurationWithWeight:symbol_weight]] configurationByApplyingConfiguration:[UIImageSymbolConfiguration configurationWithHierarchicalColor:[UIColor blueColor]  ]];
};

static UIImage * (^CaptureDeviceConfigurationControlPropertySymbolImage)(int, UIControlState) = ^ UIImage * (int property, UIControlState state) {
    return [UIImage systemImageNamed:CaptureDeviceConfigurationControlPropertyImageSymbolValues[state][property] withConfiguration:CaptureDeviceConfigurationControlPropertySymbolImageConfiguration(state)];
};

#define BITMASK(b) (1 << ((b) % CHAR_BIT))
#define BITSLOT(b) ((b) / CHAR_BIT)
#define BITSET(a, b) ((a)[BITSLOT(b)] |= BITMASK(b))
#define BITCLEAR(a, b) ((a)[BITSLOT(b)] &= ~BITMASK(b))
#define BITTEST(a, b) ((a)[BITSLOT(b)] & BITMASK(b))
#define BITNSLOTS(nb) ((nb + CHAR_BIT - 1) / CHAR_BIT)


static UIButton * (^control_buttons_init[CONTROL_PROPERTIES_COUNT])(void) = {[CONTROL_PROPERTIES_INDEX_MIN ... CONTROL_PROPERTIES_INDEX_MAX] =
    ^{
        static int property_tags = 0x00;
        printf("\n%d\n", (int)property_tags);
        static dispatch_once_t onceToken[5];
        __block UIButton * b;
        return ^ (const unsigned long property_tag) {
            dispatch_once(&onceToken[property_tag], ^{
                b = [UIButton new];
                [b setTag:property_tag];
                [b setBackgroundColor:[UIColor clearColor]];
                [b setImage:[UIImage systemImageNamed:CaptureDeviceConfigurationControlPropertyImageSymbolValues[0][property_tag] withConfiguration:CaptureDeviceConfigurationControlPropertySymbolImageConfiguration(UIControlStateNormal)]   forState:UIControlStateNormal];
                [b setImage:[UIImage systemImageNamed:CaptureDeviceConfigurationControlPropertyImageSymbolValues[1][property_tag] withConfiguration:CaptureDeviceConfigurationControlPropertySymbolImageConfiguration(UIControlStateSelected)] forState:UIControlStateSelected];
                [b setImage:[UIImage systemImageNamed:CaptureDeviceConfigurationControlPropertyImageSymbolValues[0][property_tag] withConfiguration:CaptureDeviceConfigurationControlPropertySymbolImageConfiguration(UIControlStateDisabled)] forState:UIControlStateDisabled];
                [b sizeToFit];
//                [[b titleLabel] setAdjustsFontSizeToFitWidth:CGRectGetWidth([[b titleLabel] frame])];
//                [b setBounds:CGRectMake(0.0, 0.0, [b intrinsicContentSize].width, [b intrinsicContentSize].height)];
            });
            return ^ UIButton * {
                return b;
            }();
        }(property_tags++);
    }};

static dispatch_queue_t byte_queue;
static void (^(^enumerate)(ControlProperty[CONTROL_PROPERTIES_COUNT], ControlProperty[CONTROL_PROPERTIES_COUNT], ControlProperty[CONTROL_PROPERTIES_COUNT], const unsigned long))(void (^__strong)(simd_uchar1, simd_uchar1, simd_uchar1, UIButton *, const unsigned long)) = ^ (ControlProperty * _Nonnull control_property_bit_array, ControlProperty * _Nonnull selected_control_bit_array, ControlProperty * _Nonnull enabled_controls_bit_array, const unsigned long bit_count) {
    if (!byte_queue) byte_queue = dispatch_queue_create("byte_rw", DISPATCH_QUEUE_CONCURRENT);
    dispatch_queue_t serial_queue = dispatch_queue_create_with_target("com.bush.james", DISPATCH_QUEUE_SERIAL, dispatch_get_main_queue());
    return ^ (void(^enumeration)(simd_uchar1, simd_uchar1, simd_uchar1, UIButton *, const unsigned long)) {
        dispatch_apply(CONTROL_PROPERTIES_COUNT, byte_queue, ^(size_t iteration) {
            dispatch_async(serial_queue, ^{
                enumeration(control_property_bit_array[iteration],
                            selected_control_bit_array[iteration],
                            enabled_controls_bit_array[iteration],
                            control_buttons_init[iteration](),
                            bit_count);
            });
        });
    };
};

// TO-DO: enumerate_bit should optionally return id
//        this can be done using either a block that returns void or a block that returns id
//        to choose which of the two blocks to execute:
//        typedef both blocks
//        make a generic pointer to each
//        require a passing of a pointer to one of the kinds of blocks for the enumerate block
//        test the pointer to determine its equality with either block:
//              (true ? block_returning_id : block_pointer_to_block_returning_id)(enumerate_bit_block)
// Abstract enumerator using GCD Blocks


static double rescale(double old_value, double old_min, double old_max, double new_min, double new_max) {
    return (new_max - new_min) * /*(fmax(old_min, fmin(old_value, old_max))*/ (old_value - old_min) / (old_max - old_min) + new_min;
};

static double (^CaptureDeviceConfigurationPropertyButtonAngle)(const int) = ^ double (const int property) {
    static double button_angle;
    button_angle = (double)(180.0 + (90.0 * ((double)property / 4.0)));
    
    return button_angle;
};

static dispatch_queue_t byte_queue;
static const char (^get_byte)(char, char) = ^ (const char bit_vector, char bit) {
    __block char byte;
    dispatch_sync(byte_queue, ^{
        byte = (bit_vector >> (bit << 5)) & 0xff;
    });
    return byte;
};

static const void (^select_button)(const char *, const char *, const char) = ^ (const char * selected_buttons, const char * enabled_buttons, const char selected_button) {
    dispatch_barrier_async(byte_queue, ^{
        //        BITCLEAR(&selected_buttons, selected_button);
        //        *selected_buttons &= ~(*selected_buttons);
        //        *selected_buttons |=    selected_button;
    });
};

//static int (^mask)(unsigned int) = ^ (unsigned int bit_index) {
//    __block int byte;
//    dispatch_sync(byte_queue, ^{
//        byte = (int)(1 << bit_index);
//    });
//    return byte;
//};

//static void (^print_byte)(char, int) = ^ (char bit_vector, int bit) {
//    printf("\n\t%d\n", ((BOOL)(get_byte(bit_vector, bit) & mask(bit)) ? 1 : 0));
//};


//^ (void (^control_renderers[4])(void)) {
//    void (^(^(^draw_control_components)(void))(void))(void) = ^{
//        return ^{
//            return ^{
//                [control_view setNeedsDisplay];
//            };
//        };
//    };
//
//    control_renderer[4] = {
//        ^{
//            return ^{
//
//            };
//        }(),
//        ^{
//            return ^{
//
//            };
//        }(),
//        ^{
//            return ^{
//
//            };
//        }(),
//        ^{
//            return ^{
//
//            };
//        }()
//    };
//};

//static const UIButton * (^button[5])(void);

//static UIButton * (^(^(^button_group[5])(void))(ControlProperty[8], ControlProperty[8], ControlProperty[8], unsigned int))(UIButton * (^__strong)(simd_uchar1, simd_uchar1, simd_uchar1, unsigned int)) = ^{
//    ^{ return };
//
//    return ^ UIButton * (ControlProperty * _Nonnull control_property_bit_array, ControlProperty * _Nonnull selected_control_bit_array, ControlProperty * _Nonnull enabled_controls_bit_array, unsigned int bit_count) {
//    static const UIButton * buttons[PROPERTY_BUTTONS_COUNT];
//    return ^ UIButton[5]* (UIButton * (^enumerate_bit)(simd_uchar1 control_property_bit_array, simd_uchar1 selected_control_bit_array, simd_uchar1 enabled_controls_bit_array, unsigned int bit_count)) {
//        for (unsigned int property = 0; property < bit_count; property++) { // TO-DO: iterate per the bit count -- not the supplied count
//            buttons[property] = enumerate_bit(control_property_bit_array[property], // TO-DO: enumerate_bit should optionally return id
//                                              selected_control_bit_array[property],
//                                              enabled_controls_bit_array[property],
//                                              bit_count);
//        };
//        return buttons;
//    };
//};

//    for (int property_tag = 0; property_tag < 5; property_tag++) {
//        button[property_tag]  = ^{
//            UIButton * b;
//            [b = [UIButton new] setTag:property_tag];
//            [b setBackgroundColor:[UIColor clearColor]];
//            [b setImage:[UIImage systemImageNamed:CaptureDeviceConfigurationControlPropertyImageSymbolValues[0][property_tag] withConfiguration:CaptureDeviceConfigurationControlPropertySymbolImageConfiguration(UIControlStateNormal)]   forState:UIControlStateNormal];
//            [b setImage:[UIImage systemImageNamed:CaptureDeviceConfigurationControlPropertyImageSymbolValues[1][property_tag] withConfiguration:CaptureDeviceConfigurationControlPropertySymbolImageConfiguration(UIControlStateSelected)] forState:UIControlStateSelected];
//            [b setImage:[UIImage systemImageNamed:CaptureDeviceConfigurationControlPropertyImageSymbolValues[0][property_tag] withConfiguration:CaptureDeviceConfigurationControlPropertySymbolImageConfiguration(UIControlStateDisabled)] forState:UIControlStateDisabled];
//            [b setTitle:[NSString stringWithFormat:@"%d - %d",
//                         (BOOL)(get_byte(*selected_buttons, property_tag) & property_tag),
//                         (BOOL)(get_byte(*enabled_buttons,  property_tag) & property_tag)] forState:UIControlStateNormal];
//            [b sizeToFit];
//            [[b titleLabel] setAdjustsFontSizeToFitWidth:CGRectGetWidth([[b titleLabel] frame])];
//            [b setBounds:CGRectMake(0.0, 0.0, [b intrinsicContentSize].width, [b intrinsicContentSize].height)];
//            [b setUserInteractionEnabled:TRUE];
//
//            void (^eventHandlerBlock)(void) = ^{
//                select_button(selected_buttons, enabled_buttons, (const char)b.tag);
//                //                *enabled_buttons   ^= ~((*selected_buttons & *selected_buttons, button.tag)));
//                for (char button_tag = 0; button_tag < 5; button_tag++) {
//                    [button[button_tag]() setTitle:[NSString stringWithFormat:@"%d - %d", (BOOL)(get_byte(*selected_buttons, button_tag) & button[button_tag]().tag), (BOOL)(get_byte(*enabled_buttons, property_tag) & button[button_tag]().tag)] forState:UIControlStateNormal];
//                    [button[button_tag]() setSelected:(BOOL)(get_byte(*selected_buttons, property_tag) & button[button_tag]().tag)];
//                };
//            };
//            objc_setAssociatedObject(b, @selector(invoke), eventHandlerBlock, OBJC_ASSOCIATION_RETAIN_NONATOMIC);
//            [b addTarget:eventHandlerBlock action:@selector(invoke) forControlEvents:UIControlEventAllTouchEvents];
//
//            return ^ UIButton * (void) {
//                [b setSelected:(BOOL)(get_byte(*selected_buttons, property_tag) & b.tag)];
//                //                [button setEnabled:(BOOL)(get_byte(enabled_property_bit_vector, property_tag) & mask(button.tag))];
//                return b;
//            };
//        }();
//    }
//
//    return ^ (void(^enumerate_bit)(simd_uchar1, simd_uchar1, simd_uchar1, unsigned int)) {
//
//    };
//    return ^ (int property_index) {
//        return button[property_index];
//    };
//
//};

static UIButton * (^(^property_button)(int))(void);


//static UIButton * (^CaptureDeviceConfigurationPropertyButton)(CaptureDeviceConfigurationControlProperty);



#endif /* ControlConfiguration_h */
