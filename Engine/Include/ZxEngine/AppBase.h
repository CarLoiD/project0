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
        void destroyNativeWindow();
        
    public:
        AppBase();
        virtual ~AppBase() {};
        
        // To be implemented by the application
        virtual void onCreate() = 0;
        virtual void onUpdate(const float deltaTime) = 0;
        virtual void onDestroy() = 0;
        
        // Handled by the engine
        void setWindowTitle(const char* title);
        void run(int argc, char* argv[]);
        
        RHI::RenderDevice* getRenderApi() const;
        
    private:
        bool mInitialized;
        Application::Window mWindow;
        RHI::RenderDevice* mRenderApi = nullptr;
    };   
} // namespace Application

#endif // ENGINE_APP_BASE_H