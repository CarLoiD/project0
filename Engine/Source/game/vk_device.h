#ifndef GAME_VK_DEVICE_H
#define GAME_VK_DEVICE_H

#include "shared/types.h"

#include <windows.h>
#include <vector>
#include <vulkan/vulkan.h>

struct VkQueueContainer {
    VkQueue handle = VK_NULL_HANDLE;
    int32_t index = -1;
};

struct VkCoreContext {
    VkInstance instance;
    VkPhysicalDevice physical_device;
    VkDevice device;
    VkSurfaceKHR surface;
    VkSurfaceFormatKHR surface_format;
    VkSwapchainKHR swap_chain;
};

struct VkCommandContext {
    VkCommandBuffer* current = nullptr;
    VkCommandPool pool;
    std::vector<VkCommandBuffer> buffers;
};

struct VkSyncResources {
    VkSemaphore image_sema;
    VkSemaphore finish_sema;
    std::vector<VkFence> fences;
};

struct VkDepthStencilContext {
    VkDeviceMemory memory;
    VkImage image;
    VkImageView view;
    VkFormat format = VK_FORMAT_UNDEFINED;
};

struct VkFramebufferContext {
    uint32_t index;
    VkExtent2D resolution;
    std::vector<VkFramebuffer> buffers;
    std::vector<VkImage> images;
    std::vector<VkImageView> views;
};

struct VulkanInitializeInfo {
    const void* native_window = ;
    uint32_t display_width;
    uint32_t display_height;
    VkFormat color_format;
    VkFormat depth_stencil_format;
};

union VkColor {
    struct { uint8_t b, g, r, a; } elem;
    uint32_t ucolor;
    
    VkColor(const uint32_t ucolor) { 
        this->ucolor = ucolor; 
    }
    
    VkColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 0xFF) {
        elem.r = r;
        elem.g = g;
        elem.b = b;
        elem.a = a;
    }
};

class VulkanDevice final {
private:
    void CreateInstance();
    void CreatePhysicalDevice();
    void CreateSurface(const void* window);
    void CreateDevice();
    void CreateSwapChain(const VkExtent2D resolution, const VkFormat color_format);
    void CreateFrameImageViews();
    void CreateDepthStencilView(const VkFormat format);
    void CreateRenderPass();
    void CreateFrameBuffer();
    void CreateCommandPool();
    void CreateSyncResources();
    void DestroySwapChain();
    bool GetMappedMemoryIndex(const uint32_t bits, const VkFlags flags, uint32_t* mapped_index);
    
    void InitializeVulkanApi(
        const void* window, 
        const VkExtent2D resolution, 
        const VkFormat color_format, 
        const VkFormat depth_stencil_format);
        
    void TerminateVulkanApi();

public:
    VulkanDevice();
    ~VulkanDevice();
    
    void Initialize(const VulkanInitializeInfo& init_info);
    void Destroy();
    void ClearFrameBuffer(const VkColor& clear_color, const float depth = 1.0f, const uint8_t stencil_mask = 0x00);
    void SwapBuffers();
    
private:
    bool m_initialized {};
    VkCoreContext m_core;
    VkQueueContainer m_graphics_queue;
    VkQueueContainer m_surface_queue;
    VkRenderPass m_current_pass; // TODO: Eliminate that by using dynamic rendering
    VkCommandContext m_command;
    VkSyncResources m_sync;
    VkDepthStencilContext m_depth_stencil;
    VkFramebufferContext m_frame;
};

#endif // GAME_VK_DEVICE_H