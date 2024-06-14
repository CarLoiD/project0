#ifndef VULKAN_DEVICE_STRUCT_H
#define VULKAN_DEVICE_STRUCT_H

#include <vulkan/vulkan.h>

namespace RHI {
    struct VkCoreContext {
        VkInstance instance;
        VkPhysicalDevice physicalDevice;
        VkDevice logicalDevice;
        VkSurfaceKHR surface;
        VkSurfaceFormatKHR surfaceFormat;
        VkSwapchainKHR swapChain;
        VkRenderPass currentPass;
    };

    struct VkQueueContainer {
        VkQueue handle = VK_NULL_HANDLE;
        int32_t index = -1;
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
}

#endif // VULKAN_DEVICE_STRUCT_H
