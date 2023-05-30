#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <app-context/vk_device.h>
#include <app-context/vk_swapchain.h>

#include <commands/vk_command_buffer.h>

class VulkanFramebuffers;
class VulkanRenderPass;

class VulkanRenderPass
{  
private:
    VulkanDevice* device;
    VulkanSwapchain* swapchain;

    VkRenderPass renderpass;

    bool initialized = false;
    bool active = false;
public:
    VulkanRenderPass(VulkanDevice* device, VulkanSwapchain* swapchain) : device(device), swapchain(swapchain) {}
    ~VulkanRenderPass()
    {
        if(initialized) { Destroy(); }
    }

    void CreateRenderpass();
    void Destroy();

    void Begin(VulkanCommandBuffer* commandBuffer, VulkanSwapchain* swapchain,  std::vector<VkFramebuffer> framebuffers, uint32_t imageIndex);
    void End(VulkanCommandBuffer* commandBuffer);

    VkRenderPass GetRenderPass() { return renderpass; }
};

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

    std::vector<VkFramebuffer> GetFramebuffers() { return framebuffers; }
};