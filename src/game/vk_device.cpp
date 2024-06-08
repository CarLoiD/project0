#include "vk_device.h"
#include "shared/utils.h"

static constexpr const char* TAG = "VK-DEVICE";

const std::vector<const char*> instance_extensions = {
    "VK_KHR_surface",
    "VK_KHR_win32_surface",
};

const std::vector<const char*> device_extensions = {
    "VK_KHR_swapchain",
};

const std::vector<const char*> validation_layers = {
    "VK_LAYER_KHRONOS_validation",
};

void VulkanDevice::CreateInstance() {
    VkApplicationInfo app_info {};
    app_info.sType               = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    app_info.pApplicationName    = "Project0";
    app_info.applicationVersion  = VK_MAKE_VERSION(1, 0, 0);
    app_info.pEngineName         = "ZX-ENGINE";
    app_info.engineVersion       = VK_MAKE_VERSION(1, 0, 0);
    app_info.apiVersion          = VK_API_VERSION_1_0;
    
    VkInstanceCreateInfo create_info {};
    create_info.sType                    = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    create_info.pApplicationInfo         = &app_info;
    create_info.enabledExtensionCount    = static_cast<uint32_t>(instance_extensions.size());
    create_info.ppEnabledExtensionNames  = instance_extensions.data();
    create_info.enabledLayerCount        = static_cast<uint32_t>(validation_layers.size());
    create_info.ppEnabledLayerNames      = validation_layers.data();
    
    VkResult result = vkCreateInstance(&create_info, nullptr, &m_core.instance);
    Util::Assert(result == VK_SUCCESS, "vkCreateInstance() failed");
}    

void VulkanDevice::CreatePhysicalDevice() {
    if (uint32_t device_count {}; 
        vkEnumeratePhysicalDevices(m_core.instance, &device_count, nullptr) == VK_SUCCESS) 
    {
        std::vector<VkPhysicalDevice> devices(device_count);
        vkEnumeratePhysicalDevices(m_core.instance, &device_count, devices.data());
        
        // TODO : Pick the GPU physical device based on capabilities
        VkPhysicalDeviceProperties props;
        for (auto& device : devices) {
            vkGetPhysicalDeviceProperties(device, &props);
        }
        
        m_core.physical_device = devices.at(0);
    }
    
    Util::Assert(m_core.physical_device != VK_NULL_HANDLE, "Failed to find a capable Vulkan physical device");
}

void VulkanDevice::CreateSurface(void* window) {
    VkResult result {};
    
#if defined(ZX_WINDOWS)
    VkWin32SurfaceCreateInfoKHR surface_info {};
    surface_info.sType      = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
    surface_info.hwnd       = reinterpret_cast<HWND>(window);
    surface_info.hinstance  = GetModuleHandle(nullptr);
    
    result = vkCreateWin32SurfaceKHR(m_core.instance, &surface_info, nullptr, &m_core.surface);
#endif

    Util::Assert(result == VK_SUCCESS, "vkCreatePlatformSurfaceKHR() failed");    
}

void VulkanDevice::CreateDevice() {
    VkResult result {};

    uint32_t family_count {};
    vkGetPhysicalDeviceQueueFamilyProperties(m_core.physical_device, &family_count, nullptr);

    std::vector<VkQueueFamilyProperties> props(family_count);
    vkGetPhysicalDeviceQueueFamilyProperties(m_core.physical_device, &family_count, props.data());
    
    for (uint32_t index = 0; index < props.size(); ++index) {
        auto& family = props.at(index);
        if (family.queueCount > 0) {
            if (family.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
                m_graphics_queue.index = index;
            }
            
            VkBool32 has_surface {};
            result = vkGetPhysicalDeviceSurfaceSupportKHR(
                m_core.physical_device, 
                index, 
                m_core.surface, 
                &has_surface);
                
            Util::Assert(result == VK_SUCCESS, "vkGetPhysicalDeviceSurfaceSupportKHR() failed");
            
            if (has_surface) {
                m_surface_queue.index = index;
            }
            
            // Finish condition
            if (m_graphics_queue.index != -1 && m_surface_queue.index != -1) {
                break;
            }
        }
    }
    
    float queue_priority = 1.0f;
    
    VkDeviceQueueCreateInfo queue_info {};
    queue_info.sType             = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queue_info.pQueuePriorities  = &queue_priority;
    queue_info.queueCount        = 1;
    
    VkDeviceCreateInfo device_info {};
    device_info.sType                    = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    device_info.queueCreateInfoCount     = 1;
    device_info.pQueueCreateInfos        = &queue_info;
    device_info.enabledExtensionCount    = static_cast<uint32_t>(device_extensions.size());
    device_info.ppEnabledExtensionNames  = device_extensions.data();
    
    result = vkCreateDevice(m_core.physical_device, &device_info, nullptr, &m_core.device);
    Util::Assert(result == VK_SUCCESS, "vkCreateDevice() failed");
    
    // Fetch queues at the obtained indices
    vkGetDeviceQueue(m_core.device, m_graphics_queue.index, 0, &m_graphics_queue.handle);
    vkGetDeviceQueue(m_core.device, m_surface_queue.index, 0, &m_surface_queue.handle);
}

void VulkanDevice::CreateSwapChain(const VkExtent2D resolution, const VkFormat color_format
) {

}

void VulkanDevice::CreateDepthStencilView() {
}

VulkanDevice::VulkanDevice() {
    CreateInstance();
    CreatePhysicalDevice();
}

VulkanDevice::~VulkanDevice() {
    Destroy();
}

void VulkanDevice::Initialize(
    void* window, 
    const uint32_t display_width, 
    const uint32_t display_height, 
    const VkFormat color_format
) {
    CreateSurface(window);
    CreateDevice();
    CreateSwapChain(display_width, display_height, color_format);
}

void VulkanDevice::Destroy() {

}

void VulkanDevice::ClearFrameBuffer(const VkColor& clear_color, const float depth, const uint8_t stencil_mask) {
    
}

void VulkanDevice::SwapBuffers() {

}
