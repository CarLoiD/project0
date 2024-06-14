#include "AppBase.h"
#include "Core/Macros.h"
#include "Core/Utils.h"

#if defined(ZX_RENDER_API_VULKAN)
#include "RHI/Vulkan/VkRenderDevice.h"
#endif

static constexpr bool kStartFullscreen = false;
static constexpr uint32_t kDefaultDisplayW = 1280;
static constexpr uint32_t kDefaultDisplayH = 720;

#if defined(ZX_WINDOWS)
#include <windows.h>
static constexpr uint32_t kBorderlessStyle = WS_POPUP | WS_SYSMENU | WS_MAXIMIZE;
static constexpr const char* kDefaultTitlebarText = "Project0 - Vulkan";
#endif

using namespace Application;

bool AppBase::isProcessingEvents() {
#if defined(ZX_WINDOWS)
    MSG handler;
    if (PeekMessageA(&handler, nullptr, 0, 0, PM_REMOVE)) {
        TranslateMessage(&handler);
        DispatchMessageA(&handler);
    }
    return handler.message != WM_QUIT;
#endif
}

void AppBase::createNativeWindow(const uint32_t displayWidth, const uint32_t displayHeight) {
    Application::Window* wnd = &m_window;
    if (!wnd->nativeHandle) {
    #if defined(ZX_WINDOWS)
        auto windowProc = [](HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
            // This is kinda of hacky, but it's better than having outside platform specific code
            AppBase* app = reinterpret_cast<AppBase*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
        
            switch (msg) {
                case WM_DESTROY:
                    app->onDestroy(); // Call AppBase::onDestroy()
                    PostQuitMessage(0);
                break;
            }
            return DefWindowProcA(hwnd, msg, wparam, lparam);  
        };
        
        WNDCLASSEXA wclass {};
        wclass.cbSize         = sizeof(wclass);
        wclass.lpfnWndProc    = windowProc;
        wclass.hCursor        = LoadCursorA(nullptr, IDC_ARROW);
        wclass.lpszClassName  = "Project0-WndClass";
        
        RegisterClassExA(&wclass);
        
        const char* wndTitle = wnd->titlebarText ? wnd->titlebarText : kDefaultTitlebarText;
        const uint32_t wndPosition = kStartFullscreen ? 0 : CW_USEDEFAULT;
        const uint32_t wndStyle = kStartFullscreen ? kBorderlessStyle : WS_OVERLAPPEDWINDOW;
        
        HWND wndHandle = CreateWindowExA(
            0,
            wclass.lpszClassName,
            wndTitle,
            wndStyle,
            wndPosition, wndPosition,
            static_cast<int32_t>(displayWidth),
            static_cast<int32_t>(displayHeight),
            nullptr,
            nullptr,
            GetModuleHandle(nullptr),   // hInstance
            nullptr
        );
        
        assert(wndHandle != nullptr, "AppBase::createNativeWindow() - CreateWindowExA() failed");
        
        // Store the pointer to this class as window user data
        SetWindowLongPtr(wndHandle, GWLP_USERDATA, reinterpret_cast<unsigned long>(this));
        
        ShowWindow(wndHandle, SW_SHOW);
        UpdateWindow(wndHandle);
        
        if (kStartFullscreen) {
            ShowCursor(false); // Hide the cursor on fullscreen
        }
        
        m_window.nativeHandle  = reinterpret_cast<void*>(wndHandle);
        m_window.displayWidth  = displayWidth;
        m_window.displayHeight = displayHeight;
    #endif
    }
}

void AppBase::attachGraphicsApi() {
#if defined(ZX_RENDER_API_VULKAN)
    assert(m_window.nativeHandle != nullptr, "AppBase::attachGraphicsApi() window native handle must be valid");

    RHI::VkInitializeInfo info;
    info.nativeWindow        = m_window.nativeHandle;
    info.displayWidth        = m_window.displayWidth;
    info.displayHeight       = m_window.displayHeight;
    info.colorFormat         = VK_FORMAT_R8G8B8A8_UNORM;
    info.depthStencilFormat  = VK_FORMAT_D24_UNORM_S8_UINT;
    
    m_api = new RHI::VkRenderDevice(info);
#endif
}

AppBase::AppBase() {
}

AppBase::~AppBase() {
    if (m_api) {
        delete m_api;
        m_api = nullptr;
    }
}

void AppBase::run(int argc, char* argv[]) {
    // TODO: Check for window parameters on application args
    uint32_t w { kDefaultDisplayW };
    uint32_t h { kDefaultDisplayH };
    
    createNativeWindow(w, h);
    attachGraphicsApi();
    
    onCreate();
    
    // TODO: Implement
    while (isProcessingEvents()) {
        onUpdate(0.0f);
    }
}

void AppBase::setWindowTitle(const char* title) {
    m_window.titlebarText = title;
    // TODO: If native window is created, update the window title dynamically
}
