#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <api/app-context/vk_device.h>

class VulkanCommandPool
{
private:
    std::shared_ptr<VulkanDevice> device;

    VkCommandPool commandPool;

     bool initialized = false;
public:
    VulkanCommandPool(std::shared_ptr<VulkanDevice> device) : device(device) {}
    ~VulkanCommandPool()
    {
        if(initialized) { Destroy(); }
    }

    void CreateCommandPool();
    void Destroy();

    VkCommandPool GetCommandPool() { return commandPool; }
};