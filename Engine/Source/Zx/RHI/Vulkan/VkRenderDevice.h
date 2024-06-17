#ifndef VULKAN_RENDER_DEVICE_H
#define VULKAN_RENDER_DEVICE_H

#include "RHI/RenderDevice.h"
#include "RHI/Vulkan/VkDeviceStruct.h"

namespace RHI {        
    struct VkInitializeInfo {
        void* nativeWindow = nullptr;
        uint32_t displayWidth;
        uint32_t displayHeight;
        VkFormat colorFormat;
        VkFormat depthStencilFormat;
    };

    struct CommandBuffer() {
        virtual ~CommandBuffer() {}
        virtual void Submit() = 0;
        virtual void map() = 0;
        virtual void unmap() = 0;
    };

    class VkRenderDevice final : public RHI::RenderDevice {
    private:
        void createInstance();
        void createPhysicalDevice();
        void createSurface(void* nativeWindow);
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
            void* nativeWindow,
            const VkExtent2D resolution,
            const VkFormat colorFormat,
            const VkFormat depthStencilFormat);

        void terminateVulkanApi();

    public:
        VkRenderDevice(const RHI::VkInitializeInfo& initInfo);
        ~VkRenderDevice();

        void clearFrameBuffer(const RHI::Color& clearColor) override;
        void swapBuffers() override;

    private:
        bool mInitialized;

        RHI::VkCoreContext m_core;
        RHI::VkQueueContainer m_graphicsQueue;
        RHI::VkQueueContainer m_surfaceQueue;
        RHI::VkCommandContext m_command;
        RHI::VkSyncResources m_sync;
        RHI::VkDepthStencilContext m_depthStencil;
        RHI::VkFrameBufferContext m_frameBuffer;
    };
}

#endif // VULKAN_RENDER_DEVICE_H
