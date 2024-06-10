#include "engine.h"
#include "shared/utils.h"

static constexpr const char* TAG = "ENGINE"; 
static constexpr const uint32_t DISPLAY_W = 1280;
static constexpr const uint32_t DISPLAY_H = 720;

static const bool start_fullscreen = false;
static NativeWindow window;

static void ConstructNativeWindow(const uint32_t display_width, const uint32_t display_height) {
    if (!window.handle) {
    #if defined(ZX_WINDOWS)
        auto window_proc = [](HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
            switch (msg) {
                case WM_DESTROY:
                    PostQuitMessage(0);
                break;
            }
            return DefWindowProcA(hwnd, msg, wparam, lparam);
        };
        
        WNDCLASSEXA wclass = {};
        wclass.cbSize           = sizeof(WNDCLASSEXA);
        wclass.lpfnWndProc      = window_proc;
        wclass.hCursor          = LoadCursorA(nullptr, IDC_ARROW);
        wclass.lpszClassName    = "Project0-WndClass";
        
        RegisterClassExA(&wclass);
        
        const uint32_t position = start_fullscreen ? 0 : CW_USEDEFAULT;
        const uint32_t wndstyle = start_fullscreen ? WS_POPUP | WS_SYSMENU | WS_MAXIMIZE : WS_OVERLAPPEDWINDOW;
        
        window.handle = CreateWindowExA(
            0,
            wclass.lpszClassName,
            "Project0 - Vulkan",
            wndstyle,
            position,
            position,
            static_cast<int32_t>(display_width),
            static_cast<int32_t>(display_height),
            nullptr,
            nullptr,
            nullptr,
            nullptr);
            
        Util::Assert(window.handle != nullptr, "ConstructNativeWindow() - CreateWindowExA() failed");
        
        ShowWindow(window.handle, SW_SHOW);
        UpdateWindow(window.handle);
        
        if (start_fullscreen) {
            ShowCursor(false); // Hide the cursor on fullscreen
        }

        window.display_width  = display_width;
        window.display_height = display_height;
    #endif
    }
}

static void DestructNativeWindow() {
#if defined(ZX_WINDOWS)
#endif
}

NativeWindow* Engine::GetWindow() {
    return &window;
}

bool Engine::IsProcessingEvents() {
#if defined(ZX_WINDOWS)
    MSG handler;
    if (PeekMessageA(&handler, nullptr, 0, 0, PM_REMOVE)) {
        TranslateMessage(&handler);
        DispatchMessageA(&handler);
    }
    
    return handler.message != WM_QUIT;
#endif
}

#if   defined(ZX_WINDOWS)
int main(int argc, char* argv[]) {
#elif defined(ZX_ANDROID)
void* android_main(void* args) {
#endif
    // Initialize the main window (if needed) so that the graphics context can attach to it
    window.handle = nullptr;
    ConstructNativeWindow(DISPLAY_W, DISPLAY_H);
    
    GameCallbacks::OnCreate();
    
    float dt = 0.0f;
    while (Engine::IsProcessingEvents()) {
        GameCallbacks::OnUpdate(dt);
    }
    
    GameCallbacks::OnDestroy();
    DestructNativeWindow();
    
    return 0;
}


