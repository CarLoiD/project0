#ifndef ENGINE_APP_BASE_H
#define ENGINE_APP_BASE_H

#include "RHI/RenderDevice.h"

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
        void attachGraphicsApi();
        void destroyNativeWindow();
        
    public:
        AppBase();
        virtual ~AppBase();
        
        // To be implemented by the application
        virtual void onCreate() = 0;
        virtual void onUpdate(const float deltaTime) = 0;
        virtual void onDestroy() = 0;
        
        // Handled by the engine
        void setWindowTitle(const char* title);
        void run(int argc, char* argv[]);
        
    protected:
        RHI::RenderDevice* m_api = nullptr;
        
    private:
        bool m_initialized;
        Application::Window m_window;
    };   
} // namespace Application

#endif // ENGINE_APP_BASE_H