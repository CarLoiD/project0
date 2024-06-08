#include <engine.h>

#include "vk_device.h"

VulkanDevice api;

void GameCallbacks::OnCreate() {
    NativeWindow* window = Engine::GetWindow();  
    api.Initialize(window->handle, window->display_width, window->display_height, VK_FORMAT_R8G8B8A8_UNORM);
}

void GameCallbacks::OnUpdate(const float dt) {
    api.ClearFrameBuffer(VkColor(0x2D, 0x2D, 0x2D), 1.0f, 0x00);
    api.SwapBuffers();
}

void GameCallbacks::OnDestroy() {
}
