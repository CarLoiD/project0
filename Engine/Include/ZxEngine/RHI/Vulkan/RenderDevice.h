#ifndef VULKAN_RENDER_DEVICE_H
#define VULKAN_RENDER_DEVICE_H

#include "RHI/RenderDevice.h"
#include <vulkan/vulkan.h>

namespace RHI {
    struct VkQueueContainer {
        VkQueue handle = VK_NULL_HANDLE;
        int32_t index = -1;
    };
    
    struct VkCoreContext {
        VkInstance instance;
        VkPhysicalDevice physicalDevice;
        VkDevice logicalDevice;
        VkSurfaceKHR surface;
        VkSurfaceFormatKHR surfaceFormat;
        VkSwapchainKHR swapChain;
    };
    
    struct VkCommandContext {
        VkCommandBuffer* current = nullptr;
        VkCommandPool pool;
        VkCommandBuffer* buffers = nullptr;
        uint32_t numBuffers {};
    };
    
    struct VkSyncResources {
        VkSemaphore imageSema;
        VkSemaphore finishSema;
        VkFence* fences = nullptr;
        uint32_t numFences {};
    };
    
    struct VkDepthStencilContext {
        VkDeviceMemory memory;
        VkImage image;
        VkImageView view;
        VkFormat format = VK_FORMAT_UNDEFINED;
    };
    
    struct VkFramebufferContext {
        int32_t index = -1;
        VkExtent2D resolution;
        VkFramebuffer* buffers = nullptr;
        uint32_t numBuffers {};
        VkImage* images = nullptr;
        uint32_t numImages {};
        VkImageView* views = nullptr;
        uint32_t numViews {};
    };
    
    struct VkInitializeInfo {
        const void* nativeWindow = nullptr;
        uint32_t displayWidth;
        uint32_t displayHeight;
        VkFormat colorFormat;
        VkFormat depthStencilFormat;
    };

    class VkRenderDevice final : public RHI::RenderDevice {
    private:
        void createInstance();
        void createPhysicalDevice();
        void createSurface(const void* nativeWindow);
        void createLogicalDevice();
        void createSwapChain(const VkExtent2D resolution, const VkFormat colorFormat);
        void createFrameImageViews();
        void createDepthStencilView(const VkFormat depthStencilFormat);
        void createRenderPass();
        void createFrameBuffer();
        void createCommandPool();
        void createSyncResources();
        void destroySwapChain();
        bool getMappedMemoryIndex(const uint32_t bits, const VkFlags flags, uint32_t& mappedIndex);
        
        void initializeVulkanApi(
            const void* nativeWindow,
            const VkExtent2D resolution,
            const VkFormat colorFormat,
            const VkFormat depthStencilFormat);
            
        void terminateVulkanApi();
        
    public:
        VkRenderDevice(const VkInitializeInfo& initInfo);
        ~VkRenderDevice();
        
        void clearFrameBuffer(const RHI::Color& clearColor) override;
        void swapBuffers() override;
        
    private:
        bool mInitialized;
        VkCoreContext mCore;
        VkQueueContainer mGraphicsQueue;
        VkQueueContainer mSurfaceQueue;
        VkRenderPass mCurrentPass;
        VkCommandContext mCommand;
        VkSyncResources mSync;
        VkDepthStencilContext mDepthStencil;
        VkFramebufferContext mFrame;
    };
}

#endif // VULKAN_RENDER_DEVICE_H