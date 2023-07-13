#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <api/app-context/vk_device.h>
#include <api/app-context/vk_swapchain.h>
#include <api/commands/vk_command_buffer.h>
#include <utils/vk_api_object.h>

class VulkanFramebuffers;
class VulkanRenderPass;

// Need to decouple herer
class VulkanRenderPass : public VulkanAPIObject
{  
private:
    std::shared_ptr<VulkanDevice> device;
    std::shared_ptr<VulkanSwapchain> swapchain;

    VkRenderPass renderpass;

    bool active = false;
public:
    VulkanRenderPass(std::shared_ptr<VulkanDevice> device, std::shared_ptr<VulkanSwapchain> swapchain) : device(device), swapchain(swapchain) {}
    ~VulkanRenderPass() {}

    void CreateRenderpass();
    void Destroy() override;

    void Begin(std::shared_ptr<VulkanCommandBuffer> commandBuffer, std::shared_ptr<VulkanSwapchain> swapchain, std::vector<VkFramebuffer> framebuffers, uint32_t imageIndex);
    void End(std::shared_ptr<VulkanCommandBuffer> commandBuffer);

    VkRenderPass GetRenderPass() { return renderpass; }
};

class VulkanFramebuffers : public VulkanAPIObject
{
private:
    std::shared_ptr<VulkanDevice> device;
    std::shared_ptr<VulkanSwapchain> swapchain;
    std::shared_ptr<VulkanRenderPass> renderpass;

    std::vector<VkFramebuffer> framebuffers;
public:
    VulkanFramebuffers(std::shared_ptr<VulkanDevice> device, std::shared_ptr<VulkanSwapchain> swapchain, std::shared_ptr<VulkanRenderPass> renderpass) : device(device), swapchain(swapchain), renderpass(renderpass) {}
    ~VulkanFramebuffers() {}

    void CreateFramebuffers();
    void Destroy() override;

    std::vector<VkFramebuffer> GetFramebuffers() { return framebuffers; }
};