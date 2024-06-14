#ifndef ENGINE_APP_BASE_H
#define ENGINE_APP_BASE_H

#include <cstdint>

#define ZX_CONSTRUCT_APP(appClass) \
    appClass::appClass(int argc, char* argv[]) \
        : Application::AppBase(argc, argv) {} \
    appClass::~appClass() {}

namespace Application {
    struct Window {
        uint32_t displayWidth;
        uint32_t displayHeight;
        void* nativeHandle = nullptr;
        const char* titlebarText = nullptr;
    };

    class AppBase {
    private:
        bool isProcessingEvents();
        void createNativeWindow(const uint32_t displayWidth, const uint32_t displayHeight);
        void destroyNativeWindow();
        
    public:
        AppBase() = delete;
        AppBase(int argc, char* argv[]);
        
        virtual ~AppBase() {};
        
        // To be implemented by the application
        virtual void onCreate() = 0;
        virtual void onUpdate(const float deltaTime) = 0;
        virtual void onDestroy() = 0;
        
        // Handled by the engine
        void setWindowTitle(const char* title);
        void run();
        
    private:
        bool mInitialized;
        Application::Window mWindow;
    };   
} // namespace Application

#endif // ENGINE_APP_BASE_H