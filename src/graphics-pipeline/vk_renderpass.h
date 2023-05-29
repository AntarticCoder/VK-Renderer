#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <app-context/vk_device.h>
#include <app-context/vk_swapchain.h>

class VulkanRenderPass
{  
private:
    VulkanDevice* device;
    VulkanSwapchain* swapchain;

    VkRenderPass renderpass;

    bool initialized = false;
public:
    VulkanRenderPass(VulkanDevice* device, VulkanSwapchain* swapchain) : device(device), swapchain(swapchain) {}
    ~VulkanRenderPass()
    {
        if(initialized) { Destroy(); }
    }

    void CreateRenderpass();
    void Destroy();

    VkRenderPass GetRenderPass() { return renderpass; }
};