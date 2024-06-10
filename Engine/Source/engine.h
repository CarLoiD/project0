#pragma once

#include <shared/utils.h>
#include <rx/api.h> 

#if   defined(ZX_WINDOWS)
    #include <windows.h>
    using NativeHandle = HWND;
#elif defined(ZX_ANDROID)
    using NativeHandle = ANativeWindow*;
#endif

struct NativeWindow {
    NativeHandle handle;
    uint32_t display_width;
    uint32_t display_height;
};

namespace GameCallbacks {
    extern void OnCreate();
    extern void OnUpdate(const float dt);
    extern void OnDestroy();
} // namespace GameCallbacks

namespace Engine {
    bool IsProcessingEvents();
    NativeWindow* GetWindow();
} // namespace Engine
