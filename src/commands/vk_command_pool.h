#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <app-context/vk_device.h>

class VulkanCommandPool
{
private:
    VulkanDevice* device;

    VkCommandPool commandPool;

     bool initialized = false;
public:
    VulkanCommandPool(VulkanDevice* device) : device(device) {}
    ~VulkanCommandPool()
    {
        if(initialized) { Destroy(); }
    }

    void CreateCommandPool();
    void Destroy();

    VkCommandPool GetCommandPool() { return commandPool; }
};