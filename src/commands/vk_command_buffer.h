#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <app-context/vk_device.h>
#include <commands/vk_command_pool.h>

class VulkanCommandBuffer
{
private:
    VulkanDevice* device;
    VulkanCommandPool* pool;

    VkCommandBuffer commandBuffer;

    bool initialized = false;
    bool recording = false;
public:
    VulkanCommandBuffer(VulkanDevice* device, VulkanCommandPool* pool) : device(device), pool(pool) {}
    ~VulkanCommandBuffer() {}

    void AllocateCommandBuffer();

    void Begin();
    void End();
    void Reset();

    VkCommandBuffer GetCommandBuffer() { return commandBuffer; }
};