#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <api/app-context/vk_device.h>
#include <api/commands/vk_command_pool.h>

class VulkanCommandBuffer
{
private:
    std::shared_ptr<VulkanDevice> device;
    std::shared_ptr<VulkanCommandPool> pool;

    VkCommandBuffer commandBuffer;

    bool initialized = false;
    bool recording = false;
public:
    VulkanCommandBuffer(std::shared_ptr<VulkanDevice> device, std::shared_ptr<VulkanCommandPool> pool) : device(device), pool(pool) {}
    ~VulkanCommandBuffer() {}

    void AllocateCommandBuffer();

    void Begin();
    void End();
    void Reset();

    VkCommandBuffer GetCommandBuffer() { return commandBuffer; }
};