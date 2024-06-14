#include "RHI/Vulkan/VkRenderDevice.h"
#include "Core/Utils.h"
#include "Core/Macros.h"

#if defined(ZX_WINDOWS)
#include <windows.h>
#endif

constexpr const char* kInstanceExtensions[] = {
    "VK_KHR_surface",
    "VK_KHR_win32_surface",
}; 
constexpr uint32_t kInstanceExtensionCount = SIZEOF_ARRAY_PTR(kInstanceExtensions);

constexpr const char* kDeviceExtensions[] = {
    "VK_KHR_swapchain",
}; 
constexpr uint32_t kDeviceExtensionCount = SIZEOF_ARRAY_PTR(kDeviceExtensions);

constexpr const char* kValidationLayers[] = {
    "VK_LAYER_KHRONOS_validation",
}; 
constexpr uint32_t kValidationLayerCount = SIZEOF_ARRAY_PTR(kValidationLayers);

struct VkQueueContainer {
    VkQueue handle = VK_NULL_HANDLE;
    int32_t index = -1;
};

using namespace RHI;

void VkRenderDevice::createInstance() {
    VkApplicationInfo appInfo {};
    appInfo.sType               = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName    = "Project0";
    appInfo.applicationVersion  = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName         = "ZX-ENGINE";
    appInfo.engineVersion       = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion          = VK_API_VERSION_1_0;
    
    VkInstanceCreateInfo createInfo {};
    createInfo.sType                    = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo         = &appInfo;
    createInfo.enabledExtensionCount    = kInstanceExtensionCount;
    createInfo.ppEnabledExtensionNames  = kInstanceExtensions;
    createInfo.enabledLayerCount        = kValidationLayerCount;
    createInfo.ppEnabledLayerNames      = kValidationLayers;
    
    VkResult result = vkCreateInstance(&createInfo, nullptr, &m_core.instance);
    assert(result == VK_SUCCESS, "VkRenderDevice::createInstance() - vkCreateInstance() failed");
}

void VkRenderDevice::createPhysicalDevice() {
  if (uint32_t deviceCount {};
      vkEnumeratePhysicalDevices(m_core.instance, &deviceCount, nullptr) == VK_SUCCESS)
}

void VkRenderDevice::createPhysicalDevice() {
    if (uint32_t deviceCount {};
        vkEnumeratePhysicalDevices(m_core.instance, &deviceCount, nullptr) == VK_SUCCESS)
    {
        VkPhysicalDevice* devices = new VkPhysicalDevice[deviceCount];
        vkEnumeratePhysicalDevices(m_core.instance, &deviceCount, devices);
        
        // TODO: Pick the GPU physical device based on capabilities
        for (uint32_t index = 0; index < deviceCount; ++index) {
            VkPhysicalDeviceProperties props;
            vkGetPhysicalDeviceProperties(devices[index], &props);
        }
        
        m_core.physicalDevice = devices[0];
        
        delete[] devices;
    }
    
    assert(m_core.physicalDevice, "VkRenderDevice::createPhysicalDevice() - failed to find a VkPhysicalDevice");
}

void VkRenderDevice::createSurface(void* nativeWindow) {
    VkResult result {};
    
#if defined(ZX_WINDOWS)
    VkWin32SurfaceCreateInfoKHR surfaceInfo {};
    surfaceInfo.sType       = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
    surfaceInfo.hwnd        = reinterpret_cast<HWND>(nativeWindow);
    surfaceInfo.hinstance   = GetModuleHandle(nullptr);
    
    result = vkCreateWin32SurfaceKHR(m_core.instance, &surfaceInfo, nullptr, &m_core.surface);
#endif

    assert(result == VK_SUCCESS, "VkRenderDevice::createSurface() - vkCreate<platform>SurfaceKHR() failed");
}

void VkRenderDevice::createLogicalDevice() {
    VkResult result {};
    
    uint32_t familyCount {};
    vkGetPhysicalDeviceQueueFamilyProperties(m_core.physicalDevice, &familyCount, nullptr);
    
    auto familyProps = new VkQueueFamilyProperties[familyCount];
    vkGetPhysicalDeviceQueueFamilyProperties(m_core.physicalDevice, &familyCount, familyProps);
    
    for (uint32_t index = 0; index < familyCount; ++index) {
        auto& at = familyProps[index];
        if (at.queueCount > 0) {
            if (at.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
                m_graphicsQueue.index = index;
            }
            
            VkBool32 hasSurface {};
            
            result = vkGetPhysicalDeviceSurfaceSupportKHR(m_core.physicalDevice, index, m_core.surface, &hasSurface);
            assert(result == VK_SUCCESS, "VkRenderDevice::createLogicalDevice() - failed to get surface support");
            
            if (hasSurface) {
                m_surfaceQueue.index = index;
            }
            
            // Break condition
            if (m_graphicsQueue.index != -1 && m_surfaceQueue.index != -1) { break; }
        }
    }
    
    delete[] familyProps;
    
    VkDeviceQueueCreateInfo queueInfo {};
    float queuePriority = 1.0f;
    queueInfo.sType             = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueInfo.pQueuePriorities  = &queuePriority;
    queueInfo.queueCount        = 1;
    
    VkDeviceCreateInfo deviceInfo {};
    deviceInfo.sType                    = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    deviceInfo.queueCreateInfoCount     = 1;
    deviceInfo.pQueueCreateInfos        = &queueInfo;
    deviceInfo.enabledExtensionCount    = kDeviceExtensionCount;
    deviceInfo.ppEnabledExtensionNames  = kDeviceExtensions;
    
    result = vkCreateDevice(m_core.physicalDevice, &deviceInfo, nullptr, &m_core.logicalDevice);
    assert(result == VK_SUCCESS, "VkRenderDevice::createLogicalDevice() - vkCreateDevice() failed");
        
    // Fetch queues at the obtained indices
    vkGetDeviceQueue(m_core.logicalDevice, m_graphicsQueue.index, 0, &m_graphicsQueue.handle);
    vkGetDeviceQueue(m_core.logicalDevice, m_surfaceQueue.index, 0, &m_surfaceQueue.handle);
}

void VkRenderDevice::createSwapChain(const VkExtent2D resolution, const VkFormat colorFormat) {
}

void VkRenderDevice::createFrameImageViews() {
}

void VkRenderDevice::createDepthStencilView(const VkFormat depthStencilFormat) {
}

void VkRenderDevice::createRenderPass() {
}

void VkRenderDevice::createFrameBuffer() {
}

void VkRenderDevice::createCommandPool() {
}

void VkRenderDevice::createSyncResources() {
}

void VkRenderDevice::destroySwapChain() {
}

bool VkRenderDevice::getMappedMemoryIndex(const uint32_t bits, const VkFlags flags, uint32_t& mappedIndex) {
    return true;
}

void VkRenderDevice::initializeVulkanApi(
    void* nativeWindow,
    const VkExtent2D resolution,
    const VkFormat colorFormat,
    const VkFormat depthStencilFormat
) {
    createInstance();
    createPhysicalDevice();
    createSurface(nativeWindow);
    createLogicalDevice();
    createSwapChain(resolution, colorFormat);
}

void VkRenderDevice::terminateVulkanApi() {
}

VkRenderDevice::VkRenderDevice(const RHI::VkInitializeInfo& initInfo) {
    VkExtent2D resolution { initInfo.displayWidth, initInfo.displayHeight };
    initializeVulkanApi(
        initInfo.nativeWindow,
        resolution,
        initInfo.colorFormat,
        initInfo.depthStencilFormat);
}

VkRenderDevice::~VkRenderDevice() {
}

void VkRenderDevice::clearFrameBuffer(const RHI::Color& clearColor) {
}

void VkRenderDevice::swapBuffers() {
}