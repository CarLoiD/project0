#ifndef CORE_MACROS_H
#define CORE_MACROS_H

#if defined(ZX_WINDOWS) || defined(ZX_ANDROID)
    #define ZX_RENDER_API_VULKAN
#elif defined(ZX_PLAYSTATION_2)
    #define ZX_RENDER_API_RX
#endif

#define SIZEOF_ARRAY_PTR(x) \
    sizeof(x) / sizeof(unsigned long)

#define SIZEOF_ARRAY(x) \
    sizeof(x) / sizeof(x[0])

#endif // CORE_MACROS_H