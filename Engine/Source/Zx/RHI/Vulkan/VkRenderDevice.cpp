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

    auto checkQueueIndices = [&, this](void) {
        return m_graphicsQueue.index != -1 && m_surfaceQueue.index != -1;
    };

    for (uint32_t index = 0; index < familyCount; ++index) {
        auto& at = familyProps[index];
        if (at.queueCount > 0) {
            if (at.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
                m_graphicsQueue.index = index;
            }

            if (VkBool32 hasSurface {};
                vkGetPhysicalDeviceSurfaceSupportKHR(m_core.physicalDevice, index, m_core.surface, &hasSurface) == VK_SUCCESS) 
            {
                m_surfaceQueue.index = index;
            }

            // Break condition
            if (checkQueueIndices()) break;
        }
    }
    
    assert(checkQueueIndices(), "VkRenderDevice::createLogicalDevice() - failed to get surface support");
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
    uint32_t formatCount = 0;
    vkGetPhysicalDeviceSurfaceFormatsKHR(m_core.physicalDevice, m_core.surface, &formatCount, nullptr);

    auto formats = new VkSurfaceFormatKHR[formatCount];
    vkGetPhysicalDeviceSurfaceFormatsKHR(m_core.physicalDevice, m_core.surface, &formatCount, formats);

    int32_t formatIndex = -1;
    for (int32_t index = 0; index < formatCount; ++index) {
        if (formats[index].format == colorFormat) {
            formatIndex = index;
            break;
        }
    }
    
    assert(formatIndex >= 0, "VkRenderDevice::createSwapChain() - internal surface does not support the requested color format");

    // Copy over the format
    m_core.surfaceFormat = formats[index];
    delete[] formats; // Can now safely delete

    m_frameBuffer.resolution = resolution;

    const uint32_t minImageCount = [](void) { VkSurfaceCapabilitiesKHR surfaceCaps;
        vkGetPhysicalDeviceSurfaceCapabilitesKHR(m_core.physicalDevice, m_core.surface, &surfaceCaps);
        return surfaceCaps.minImageCount;
    }();

    VkSwapchainCreateInfoKHR createInfo {};
    createInfo.sType                    = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface                  = m_core.surface;
    createInfo.minImageCount            = minImageCount;
    createInfo.imageFormat              = m_core.surfaceFormat.format;
    createInfo.imageColorSpace          = m_core.surfaceFormat.colorSpace;
    createInfo.imageExtent              = m_frameBuffer.resolution;
    createInfo.imageArrayLayers         = 1;
    createInfo.imageUsage               = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    createInfo.imageSharingMode         = VK_SHARING_MODE_EXCLUSIVE;
    createInfo.queueFamilyIndexCount    = 1;
    createInfo.pQueueFamilyIndices      = &m_graphicsQueue.index;
    createInfo.preTransform             = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
    createInfo.compositeAlpha           = VK_COMPOSITE_ALPHA_INHERIT_BIT_KHR;
    createInfo.presentMode              = VK_PRESENT_MODE_FIFO_KHR;

    VkResult result = vkCreateSwapchainKHR(m_core.logicalDevice, &createInfo, nullptr, &m_core.swapChain);
    assert(result == VK_SUCCESS, "VkRenderDevice::createSwapChain() - vkCreateSwapChainKHR() failed");

    const uint32_t imageCount = [&, this](void) { uint32_t ret {};
        vkGetSwapchainImagesKHR(m_core.logicalDevice, m_core.swapChain, &ret, nullptr);
        return ret;
    }();

    assert(imageCount, "VkRenderDevice::createSwapChain() - image count on swap chain is invalid");

    // Reserve memory
    m_frameBuffer.buffers  = new VkFramebuffer[imageCount];
    m_sync.fences          = new VkFence[imageCount];
    m_frameBuffer.images   = new VkImage[imageCount];
    m_frameBuffer.views    = new VkImageView[imageCount];
    
    m_frameBuffer.bufferCount = imageCount;
    m_sync.fenceCount = imageCount;
    m_frameBuffer.imageCount = imageCount;
    m_frameBuffer.viewCount = imageCount;

    result = vkGetSwapchainImagesKHR(m_core.logicalDevice, m_core.swapChain, &imageCount, m_frameBuffer.images);
    assert(result == VK_SUCCESS, "VkRenderDevice::createSwapChain() - vkGetSwapchainImagesKHR() failed");
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
