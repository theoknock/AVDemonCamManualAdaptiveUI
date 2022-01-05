//
//  ControlConfiguration.h
//  AVDemonCamManualAdaptive
//
//  Created by Xcode Developer on 12/21/21.
//

#import <objc/runtime.h>
@import Accelerate;
#include "simd/simd.h"

#ifndef ControlConfiguration_h
#define ControlConfiguration_h


#define degreesToRadians(angleDegrees) (angleDegrees * M_PI / 180.0)
// Button state

typedef enum {
    ControlStateRenderPropertyComponentTransition = 1 << 0,
    ControlStateRenderPropertyComponent           = 1 << 1,
    ControlStateRenderValueComponentTransition    = 1 << 2,
    ControlStateRenderValueComponent              = 1 << 3
} ControlState;


#define ControlRendererStatePropertyComponentTransition  0b0001
#define ControlRendererStatePropertyComponent            0b0010
#define ControlRendererStateValueComponentTransition     0b0100
#define ControlRendererStateValueComponent               0b1000
#define State (ControlRendererStatePropertyComponentTransition | ControlRendererStatePropertyComponent | ControlRendererStateValueComponentTransition | ControlRendererStateValueComponent)

typedef struct __attribute__((objc_boxable)) ControlRendererState ControlRendererState;
static struct __attribute__((objc_boxable)) ControlRendererState {
    unsigned int control_renderer_state : 1 << 4 >> 3;
} controlRendererState = {
    .control_renderer_state = 0b0001
};

static unsigned int (^cycle_state)(void) = ^{
    return controlRendererState.control_renderer_state++;
};
static simd_uchar1 control_state_bit_vector = {1 << 0 | 1 << 1 | 1 << 2 | 1 << 3};

typedef NS_OPTIONS(uint8_t, CaptureDeviceConfigurationControlPropertyBit) {
    CaptureDeviceConfigurationControlPropertyBitTorchLevel       = 1 << 0,
    CaptureDeviceConfigurationControlPropertyBitLensPosition     = 1 << 1,
    CaptureDeviceConfigurationControlPropertyBitExposureDuration = 1 << 2,
    CaptureDeviceConfigurationControlPropertyBitISO              = 1 << 3,
    CaptureDeviceConfigurationControlPropertyBitZoomFactor       = 1 << 4,
};

typedef CaptureDeviceConfigurationControlPropertyBit               CaptureDeviceConfigurationControlPropertyBitMask;
typedef CaptureDeviceConfigurationControlPropertyBitMask           CaptureDeviceConfigurationControlPropertyBitVector;
static  CaptureDeviceConfigurationControlPropertyBitVector         property_bit_vector     = (CaptureDeviceConfigurationControlPropertyBitTorchLevel |
                                                                                              CaptureDeviceConfigurationControlPropertyBitLensPosition |
                                                                                              CaptureDeviceConfigurationControlPropertyBitExposureDuration |
                                                                                              CaptureDeviceConfigurationControlPropertyBitISO |
                                                                                              CaptureDeviceConfigurationControlPropertyBitZoomFactor);
static  CaptureDeviceConfigurationControlPropertyBitVector * const property_bit_vector_ptr = &property_bit_vector;

typedef NS_OPTIONS(uint8_t, CaptureDeviceConfigurationControlSelectedPropertyBit) {
    CaptureDeviceConfigurationControlSelectedPropertyBitTorchLevel       = 1 << 0,
    CaptureDeviceConfigurationControlSelectedPropertyBitLensPosition     = 1 << 1,
    CaptureDeviceConfigurationControlSelectedPropertyBitExposureDuration = 1 << 2,
    CaptureDeviceConfigurationControlSelectedPropertyBitISO              = 1 << 3,
    CaptureDeviceConfigurationControlSelectedPropertyBitZoomFactor       = 1 << 4,
};
typedef CaptureDeviceConfigurationControlSelectedPropertyBit               CaptureDeviceConfigurationControlSelectedPropertyBitMask;
typedef CaptureDeviceConfigurationControlSelectedPropertyBitMask           CaptureDeviceConfigurationControlSelectedPropertyBitVector;
static  CaptureDeviceConfigurationControlSelectedPropertyBitVector         selected_property_bit_vector     = (0 << 0 | 0 << 1 | 0 << 2 | 0 << 3 | 0 << 4);
static  CaptureDeviceConfigurationControlSelectedPropertyBitVector * const selected_property_bit_vector_ptr = &selected_property_bit_vector;

typedef NS_OPTIONS(uint8_t, CaptureDeviceConfigurationControlHiddenPropertyBit) {
    CaptureDeviceConfigurationControlHiddenPropertyBitTorchLevel       = 1 << 0,
    CaptureDeviceConfigurationControlHiddenPropertyBitLensPosition     = 1 << 1,
    CaptureDeviceConfigurationControlHiddenPropertyBitExposureDuration = 1 << 2,
    CaptureDeviceConfigurationControlHiddenPropertyBitISO              = 1 << 3,
    CaptureDeviceConfigurationControlHiddenPropertyBitZoomFactor       = 1 << 4,
};
typedef CaptureDeviceConfigurationControlHiddenPropertyBit               CaptureDeviceConfigurationControlHiddenPropertyBitMask;
typedef CaptureDeviceConfigurationControlHiddenPropertyBitMask           CaptureDeviceConfigurationControlHiddenPropertyBitVector;
static  CaptureDeviceConfigurationControlHiddenPropertyBitVector         hidden_property_bit_vector     = (0 << 0 | 0 << 1 | 0 << 2 | 0 << 3 | 0 << 4);
static  CaptureDeviceConfigurationControlHiddenPropertyBitVector * const hidden_property_bit_vector_ptr = &hidden_property_bit_vector;

static CaptureDeviceConfigurationControlPropertyBitVector (^mask_property_bit_vector)(CaptureDeviceConfigurationControlPropertyBitMask) = ^ CaptureDeviceConfigurationControlPropertyBitVector (CaptureDeviceConfigurationControlPropertyBitMask property_bit_mask) {
    property_bit_vector = property_bit_vector | property_bit_mask;
    return *property_bit_vector_ptr;
};

static CaptureDeviceConfigurationControlSelectedPropertyBitVector (^mask_selected_property_bit_vector)(CaptureDeviceConfigurationControlSelectedPropertyBit) = ^ CaptureDeviceConfigurationControlSelectedPropertyBitVector (CaptureDeviceConfigurationControlSelectedPropertyBit selected_property_bit) {
    selected_property_bit_vector   = selected_property_bit_vector & 0;
    selected_property_bit_vector   = selected_property_bit_vector | selected_property_bit;
    hidden_property_bit_vector    ^= ~((selected_property_bit_vector & ~selected_property_bit_vector));

    return *selected_property_bit_vector_ptr;
};

static CaptureDeviceConfigurationControlHiddenPropertyBitVector (^mask_hidden_property_bit_vector)(CaptureDeviceConfigurationControlSelectedPropertyBitVector) = ^ (CaptureDeviceConfigurationControlSelectedPropertyBitMask selected_property_bit_mask) {
    return *hidden_property_bit_vector_ptr;
};

// Button rendering

#define degreesToRadians(angleDegrees) (angleDegrees * M_PI / 180.0)

typedef NS_OPTIONS(NSUInteger, CaptureDeviceConfigurationControlProperty) {
    CaptureDeviceConfigurationControlPropertyTorchLevel       = 0,
    CaptureDeviceConfigurationControlPropertyLensPosition     = 1,
    CaptureDeviceConfigurationControlPropertyExposureDuration = 2,
    CaptureDeviceConfigurationControlPropertyISO              = 3,
    CaptureDeviceConfigurationControlPropertyZoomFactor       = 4,
    CaptureDeviceConfigurationControlPropertyNone             = 5
};

static NSArray<NSArray<NSString *> *> * const CaptureDeviceConfigurationControlPropertyImageNames = @[@[@"bolt.circle",
                                                                                                        @"viewfinder.circle",
                                                                                                        @"timer",
                                                                                                        @"camera.aperture",
                                                                                                        @"magnifyingglass.circle"],@[@"bolt.circle.fill",
                                                                                                                                     @"viewfinder.circle.fill",
                                                                                                                                     @"timer",
                                                                                                                                     @"camera.aperture",
                                                                                                                                     @"magnifyingglass.circle.fill"]];

static NSArray<NSString *> * const CaptureDeviceConfigurationControlPropertyImageKeys = @[@"CaptureDeviceConfigurationControlPropertyTorchLevel",
                                                                                          @"CaptureDeviceConfigurationControlPropertyLensPosition",
                                                                                          @"CaptureDeviceConfigurationControlPropertyExposureDuration",
                                                                                          @"CaptureDeviceConfigurationControlPropertyISO",
                                                                                          @"CaptureDeviceConfigurationControlPropertyZoomFactor"];

typedef enum : NSUInteger {
    CaptureDeviceConfigurationControlStateDeselected,
    CaptureDeviceConfigurationControlStateSelected,
    CaptureDeviceConfigurationControlStateHighlighted,
    CaptureDeviceConfigurationControlStateAny
} CaptureDeviceConfigurationControlState;

static NSString * (^CaptureDeviceConfigurationControlPropertySymbol)(CaptureDeviceConfigurationControlProperty, CaptureDeviceConfigurationControlState) = ^ NSString * (CaptureDeviceConfigurationControlProperty property, CaptureDeviceConfigurationControlState state) {
    return CaptureDeviceConfigurationControlPropertyImageNames[state][property];
};

static NSString * (^CaptureDeviceConfigurationControlPropertyString)(CaptureDeviceConfigurationControlProperty) = ^ NSString * (CaptureDeviceConfigurationControlProperty property) {
    return CaptureDeviceConfigurationControlPropertyImageKeys[property];
};

static UIImageSymbolConfiguration * (^CaptureDeviceConfigurationControlPropertySymbolImageConfiguration)(CaptureDeviceConfigurationControlState) = ^ UIImageSymbolConfiguration * (CaptureDeviceConfigurationControlState state) {
    switch (state) {
        case CaptureDeviceConfigurationControlStateDeselected: {
            UIImageSymbolConfiguration * symbol_palette_colors = [UIImageSymbolConfiguration configurationWithHierarchicalColor:[UIColor colorWithRed:4/255 green:51/255 blue:255/255 alpha:1.0]];
            UIImageSymbolConfiguration * symbol_font_weight    = [UIImageSymbolConfiguration configurationWithWeight:UIImageSymbolWeightLight];
            UIImageSymbolConfiguration * symbol_font_size      = [UIImageSymbolConfiguration configurationWithPointSize:42.0 weight:UIImageSymbolWeightUltraLight];
            UIImageSymbolConfiguration * symbol_configuration  = [symbol_font_size configurationByApplyingConfiguration:[symbol_palette_colors configurationByApplyingConfiguration:symbol_font_weight]];
            return symbol_configuration;
        }
            break;
            
        case CaptureDeviceConfigurationControlStateSelected: {
            UIImageSymbolConfiguration * symbol_palette_colors_selected = [UIImageSymbolConfiguration configurationWithHierarchicalColor:[UIColor colorWithRed:255/255 green:252/255 blue:121/255 alpha:1.0]];// configurationWithPaletteColors:@[[UIColor yellowCollor], [UIColor clearColor], [UIColor yellowCollor]]];
            UIImageSymbolConfiguration * symbol_font_weight_selected    = [UIImageSymbolConfiguration configurationWithWeight:UIImageSymbolWeightRegular];
            UIImageSymbolConfiguration * symbol_font_size_selected      = [UIImageSymbolConfiguration configurationWithPointSize:42.0 weight:UIImageSymbolWeightLight];
            UIImageSymbolConfiguration * symbol_configuration_selected  = [symbol_font_size_selected configurationByApplyingConfiguration:[symbol_palette_colors_selected configurationByApplyingConfiguration:symbol_font_weight_selected]];
            
            return symbol_configuration_selected;
        }
            
        case CaptureDeviceConfigurationControlStateHighlighted: {
            UIImageSymbolConfiguration * symbol_palette_colors_highlighted = [UIImageSymbolConfiguration configurationWithHierarchicalColor:[UIColor colorWithRed:255/255 green:252/255 blue:121/255 alpha:1.0]];// configurationWithPaletteColors:@[[UIColor yellowCollor], [UIColor clearColor], [UIColor yellowCollor]]];
            UIImageSymbolConfiguration * symbol_font_weight_highlighted    = [UIImageSymbolConfiguration configurationWithWeight:UIImageSymbolWeightRegular];
            UIImageSymbolConfiguration * symbol_font_size_highlighted      = [UIImageSymbolConfiguration configurationWithPointSize:84.0 weight:UIImageSymbolWeightLight];
            UIImageSymbolConfiguration * symbol_configuration_highlighted  = [symbol_font_size_highlighted configurationByApplyingConfiguration:[symbol_palette_colors_highlighted configurationByApplyingConfiguration:symbol_font_weight_highlighted]];
            
            return symbol_configuration_highlighted;
        }
            break;
        default:
            return nil;
            break;
    }
};

static UIImage * (^CaptureDeviceConfigurationControlPropertySymbolImage)(CaptureDeviceConfigurationControlProperty, CaptureDeviceConfigurationControlState) = ^ UIImage * (CaptureDeviceConfigurationControlProperty property, CaptureDeviceConfigurationControlState state) {
    return [UIImage systemImageNamed:CaptureDeviceConfigurationControlPropertySymbol(property, state) withConfiguration:CaptureDeviceConfigurationControlPropertySymbolImageConfiguration(state)];
};

static int (^mask)(unsigned int) = ^ (unsigned int bit_index) {
    return (int)(1 << bit_index);
};

static int (^get_byte)(int, int) = ^ (int x, int n) {
  return (x >> (n<<5)) & 0xff;
};

static void (^print_byte)(CaptureDeviceConfigurationControlPropertyBitVector, CaptureDeviceConfigurationControlPropertyBit) = ^ (CaptureDeviceConfigurationControlPropertyBitVector bit_vector, CaptureDeviceConfigurationControlPropertyBit bit) {
    printf("\n\t%d\n", ((BOOL)(get_byte(bit_vector, bit) & mask(bit)) ? 1 : 0));
};


static const UIButton * (^buttons[5])(void);
static const UIButton * (^(^(^button_group)(CaptureDeviceConfigurationControlPropertyBitMask, CaptureDeviceConfigurationControlSelectedPropertyBitMask, CaptureDeviceConfigurationControlHiddenPropertyBitMask))(CaptureDeviceConfigurationControlProperty))(void) =  ^ (CaptureDeviceConfigurationControlPropertyBitMask property_bit_mask, CaptureDeviceConfigurationControlSelectedPropertyBitMask selected_property_bit_mask, CaptureDeviceConfigurationControlHiddenPropertyBitMask hidden_property_bit_mask) {
    static simd_uchar2 selected_hidden_bit_vector_pair;
    for (unsigned int property_tag = 0; property_bit_vector; property_bit_vector >>= 1) {
        UIButton * (^button)(void);
        button = ^{
            UIButton * button;
            [button = [UIButton new] setTag:property_tag];
            [button setBackgroundColor:[UIColor clearColor]];
            [button setImage:[UIImage systemImageNamed:CaptureDeviceConfigurationControlPropertyImageNames[0][property_tag] withConfiguration:CaptureDeviceConfigurationControlPropertySymbolImageConfiguration(CaptureDeviceConfigurationControlStateDeselected)] forState:UIControlStateNormal];
            [button setImage:[UIImage systemImageNamed:CaptureDeviceConfigurationControlPropertyImageNames[1][property_tag] withConfiguration:CaptureDeviceConfigurationControlPropertySymbolImageConfiguration(CaptureDeviceConfigurationControlStateSelected)] forState:UIControlStateSelected];
            [button setTitle:[NSString stringWithFormat:@"%d - %d",
                              (BOOL)(get_byte(selected_property_bit_vector, property_tag) & mask(property_tag)), //(selected_property_bit_vector | (CaptureDeviceConfigurationControlSelectedPropertyBit)(property_tag)),
                              (BOOL)(get_byte(hidden_property_bit_vector,   property_tag) & mask(property_tag))] forState:UIControlStateNormal];
            [button sizeToFit];
            [[button titleLabel] setAdjustsFontSizeToFitWidth:CGRectGetWidth([[button titleLabel] frame])];
            [button setBounds:CGRectMake(0.0, 0.0, [button intrinsicContentSize].width, [button intrinsicContentSize].height)];
            [button setUserInteractionEnabled:TRUE];
            
            void (^eventHandlerBlock)(void) = ^{

//                _simd_pow_d2(<#simd_double2 x#>, <#simd_double2 y#>)
//                _simd_atan2_d2(<#simd_double2 y#>, <#simd_double2 x#>)
//                simd_slerp(<#simd_quatf q0#>, <#simd_quatf q1#>, <#float t#>)
//                simd_negate(<#simd_quatd q#>)
//                simd_min(<#simd_double8 x#>, <#simd_double8 y#>)
//                simd_max(<#simd_double8 x#>, <#simd_double8 y#>)
//                simd_incircle(<#simd_float2 __x#>, <#simd_float2 __a#>, <#simd_float2 __b#>, <#simd_float2 __c#>) // possibly how to determine where the touch and the arc meet
//                simd_sign(<#simd_double4 x#>)  // Use this to narrow the proximity-to-button touch point calculation to once and for all
//                simd_angle(<#simd_quatd q#>) // compute all angles with this, all at the same time
//                simd_bitselect(<#simd_uchar2 x#>, <#simd_uchar2 y#>, <#simd_char2 mask#>)
//                simd_bezier(<#simd_quatd q0#>, <#simd_quatd q1#>, <#simd_quatd q2#>, <#simd_quatd q3#>, <#double t#>) //Use this to calculate all bezier paths for all components, simultaneously
                selected_hidden_bit_vector_pair = simd_make_uchar2(*selected_property_bit_vector_ptr, *hidden_property_bit_vector_ptr);
                selected_hidden_bit_vector_pair[0] &= ~(selected_hidden_bit_vector_pair[0]);
                selected_hidden_bit_vector_pair[0] |= mask(property_tag);
                selected_hidden_bit_vector_pair[1] ^= ~((selected_hidden_bit_vector_pair[0] & get_byte(selected_hidden_bit_vector_pair[0], mask(buttons[property_tag]().tag))));
                selected_property_bit_vector = selected_hidden_bit_vector_pair[0];
                hidden_property_bit_vector   = selected_hidden_bit_vector_pair[1];
                for (int property = 0; property < 5; property++) {
                    [buttons[property]() setTitle:[NSString stringWithFormat:@"%d - %d",
                                                   (BOOL)(get_byte(selected_hidden_bit_vector_pair[0], property_tag) & mask(buttons[property]().tag)), //(selected_hidden_bit_vector_pair[0] | (CaptureDeviceConfigurationControlSelectedPropertyBit)(property_tag)),
                                                   (BOOL)(get_byte(hidden_property_bit_vector,   property_tag) & mask(buttons[property]().tag))] forState:UIControlStateNormal];
                    [buttons[property]() setSelected:(BOOL)(get_byte(selected_hidden_bit_vector_pair[0], property_tag) & mask(buttons[property]().tag))]; //(selected_hidden_bit_vector_pair[0] | (CaptureDeviceConfigurationControlSelectedPropertyBit)(property_tag)),
                };
            };
            objc_setAssociatedObject(button, @selector(invoke), eventHandlerBlock, OBJC_ASSOCIATION_RETAIN_NONATOMIC);
            [button addTarget:eventHandlerBlock action:@selector(invoke) forControlEvents:UIControlEventTouchUpInside];
            
            return ^ UIButton * (void) {
                [button setSelected:(BOOL)(get_byte(selected_property_bit_vector, property_tag) & mask(button.tag))]; //(selected_property_bit_vector | (CaptureDeviceConfigurationControlSelectedPropertyBit)(property_tag)),
                    [button setHidden:(BOOL)(get_byte(hidden_property_bit_vector, property_tag) & mask(button.tag))];
                return button;
            };
        }();
        buttons[property_tag] = button;
        property_tag += property_bit_vector & 1;
    }
    
    return ^ (CaptureDeviceConfigurationControlProperty property_index) {
        return buttons[property_index];
    };
};

static UIButton * (^(^property_button)(CaptureDeviceConfigurationControlProperty))(void);

#define BITMASK(b) (1 << ((b) % CHAR_BIT))
#define BITSLOT(b) ((b) / CHAR_BIT)
#define BITSET(a, b) ((a)[BITSLOT(b)] |= BITMASK(b))
#define BITCLEAR(a, b) ((a)[BITSLOT(b)] &= ~BITMASK(b))
#define BITTEST(a, b) ((a)[BITSLOT(b)] & BITMASK(b))
#define BITNSLOTS(nb) ((nb + CHAR_BIT - 1) / CHAR_BIT)


//static UIButton * (^CaptureDeviceConfigurationPropertyButton)(CaptureDeviceConfigurationControlProperty);



#endif /* ControlConfiguration_h */
