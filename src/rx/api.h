#ifndef RX_API_H
#define RX_API_H

#include "rhi/render_api.h"
#include "format.h"

#if defined(ZX_WINDOWS)
    #define RX_API_LIBRARY
    #define RX_API_CALL     __fastcall
#endif

enum class RxResult : int32_t {
    /// Operation was successful
    Success = 0,
    
    /// Undefined means an API call or operation didn't worked as expected
    ErrorUndefined = -1,
    
    /// One or more arguments being passed incorrectly
    /// (e.g: RxInitializeInfo::application_name == nullptr)
    ErrorInvalidArguments = -2,
    
    /// Exclusive to non-console devices 
    /// The requested Graphics API isn't supported at the desired feature set 
    /// (e.g: Vulkan API version)
    ErrorUnsupportedGPU = -3,
    
    /// User has requested Debug Mode and the driver couldn't create such debug instance
    /// (e.g: Vulkan Validation Layers not supported)
    ErrorUnsupportedDebugMode = -4,
};

enum class RxState : uint32_t {
    Disable = 0,
    Enable = 1,
};

enum class RxDynamicState : uint32_t {
    ClippingPlanes = 0,
    ClippingTestEnable = 1,
    ScissorEnable = 2,
    DepthMask = 3,
    DepthTestEnable = 4,
    DepthTestCompareOp = 5,
    AlphaMask = 6,
    AlphaTestEnable = 7,
    AlphaTestCompareOp = 8,
    StencilMask = 9,
    StencilTestEnable = 10,
    PrimitiveTopology = 11,
    PrimitiveRestartEnable = 12,
    BackfaceCullEnable = 13,
    PolygonFillMode = 14,
};

enum class RxPrimitiveTopology : uint32_t {
    PointList = 0,
    LineList = 1,
    LineStrip = 2,
    TriangleList = 3,
    TriangleStrip = 4,
    TriangleFan = 5,
};

enum class RxPolygonFillMode : uint32_t {
    Solid = 0,
    Wireframe = 1,
    Point = 2,
};

enum class RxFilter : uint32_t {
    Nearest = 0,
    Linear = 1,
};

enum class RxCommandBufferLevel : uint32_t {
    Primary = 0,
    Secondary = 1,  
};

enum class RxIndexType : uint32_t {
    Uint16 = 0,
    Uint32 = 1,
};

typedef enum {
    RX_FORMAT_UNDEFINED,
    RX_FORMAT_RGBA_8888_UINT,
    RX_FORMAT_RGBA_8888_SINT,
    RX_FORMAT_RGBA_8888_UNORM,
    RX_FORMAT_RGBA_8888_SNORM,
    RX_FORMAT_DEPTH_32,
    RX_FORMAT_DEPTH_24,
    RX_FORMAT_DEPTH_16_UINT,
    RX_FORMAT_DEPTH_16_SINT,
    RX_FORMAT_INDEX_8,
    RX_FORMAT_INDEX_4,
    RX_FORMAT_MAX_ENUM = 0x7FFFFFFF, // Align to 32bit boundary
} RxFormat;

struct RxInitializeInfo {  
    const char*     application_name = nullptr;
    uint32_t        display_width {};
    uint32_t        display_height {}; 
    RxFormat        color_format = RX_FORMAT_UNDEFINED;
    RxFormat        depth_format = RX_FORMAT_UNDEFINED;
};

struct RxViewport {
    uint32_t    x;
    uint32_t    y;
    uint32_t    width;
    uint32_t    height;
    float       z_min;
    float       z_max;
    
    RxViewport() = default;
    RxViewport(const uint32_t x, 
               const uint32_t y, 
               const uint32_t width, 
               const uint32_t height, 
               const float z_min, 
               const float z_max);
};

struct RxInstance {
    uint32_t flags;
};

union RxColor {
    struct { uint8_t b, g, r, a; } elem;
    uint32_t ucolor = 0;

    RxColor() = default;
    RxColor(const uint32_t ucolor);
    RxColor(const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t a);
};

#endif // RX_API_H