#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <app-context/vk_device.h>
#include <app-context/vk_swapchain.h>
#include <graphics-pipeline/vk_renderpass.h>

class VulkanFramebuffers
{
private:
    VulkanDevice* device;
    VulkanSwapchain* swapchain;
    VulkanRenderPass* renderpass;

    std::vector<VkFramebuffer> framebuffers;

    bool initialized = false;
public:
    VulkanFramebuffers(VulkanDevice* device, VulkanSwapchain* swapchain, VulkanRenderPass* renderpass) : device(device), swapchain(swapchain), renderpass(renderpass) {}
    ~VulkanFramebuffers()
    {
        if(initialized) { Destroy(); }
    }

    void CreateFramebuffers();
    void Destroy();
};