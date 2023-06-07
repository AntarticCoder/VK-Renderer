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
    std::shared_ptr<VulkanDevice> device;
    std::shared_ptr<VulkanSwapchain> swapchain;

    VkRenderPass renderpass;

    bool initialized = false;
    bool active = false;
public:
    VulkanRenderPass(std::shared_ptr<VulkanDevice> device, std::shared_ptr<VulkanSwapchain> swapchain) : device(device), swapchain(swapchain) {}
    ~VulkanRenderPass()
    {
        if(initialized) { Destroy(); }
    }

    void CreateRenderpass();
    void Destroy();

    void Begin(std::shared_ptr<VulkanCommandBuffer> commandBuffer, std::shared_ptr<VulkanSwapchain> swapchain, std::vector<VkFramebuffer> framebuffers, uint32_t imageIndex);
    void End(std::shared_ptr<VulkanCommandBuffer> commandBuffer);

    VkRenderPass GetRenderPass() { return renderpass; }
};

class VulkanFramebuffers
{
private:
    std::shared_ptr<VulkanDevice> device;
    std::shared_ptr<VulkanSwapchain> swapchain;
    std::shared_ptr<VulkanRenderPass> renderpass;

    std::vector<VkFramebuffer> framebuffers;

    bool initialized = false;
public:
    VulkanFramebuffers(std::shared_ptr<VulkanDevice> device, std::shared_ptr<VulkanSwapchain> swapchain, std::shared_ptr<VulkanRenderPass> renderpass) : device(device), swapchain(swapchain), renderpass(renderpass) {}
    ~VulkanFramebuffers()
    {
        if(initialized) { Destroy(); }
    }

    void CreateFramebuffers();
    void Destroy();

    std::vector<VkFramebuffer> GetFramebuffers() { return framebuffers; }
};