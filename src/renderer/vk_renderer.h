#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <app-context/vk_window.h>
#include <app-context/vk_device.h>
#include <app-context/vk_swapchain.h>

class VulkanRenderer
{
private:
    VulkanWindow* window;
    VulkanDevice* device;
    VulkanSwapchain* swapchain;

    VkCommandPool commandPool;
    VkCommandBuffer primaryCommandBuffer;

    VkRenderPass renderpass;
	std::vector<VkFramebuffer> framebuffers;

    VkSemaphore presentSemaphore;
    VkSemaphore renderSemaphore;
	VkFence renderFence;

    bool commandsInitialized = false;
    bool renderpassInitialized = false;
    bool framebufferInitialized = false;
public:
    VulkanRenderer(VulkanWindow* window, VulkanDevice* device, VulkanSwapchain* swapchain) : device(device), swapchain(swapchain), window(window) { }
    ~VulkanRenderer() {}

    void CreateCommands();
    void CreateDefaultRenderPass();
    void CreateFramebuffers();
    void IntializeSyncStructures();

    void Draw();

    void DestroyCommands();
    void DestroyRenderPass();
    void DestroyFramebuffers();
    void DestroySyncStructures();
};