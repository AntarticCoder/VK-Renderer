#pragma once
#include <optional>
#include <vector>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <app-context/vk_instance.h>
#include <app-context/vk_window.h>

struct VulkanQueueFamilyIndices
{
    std::optional<uint32_t> graphicsFamily;
    std::optional<uint32_t> presentFamily;

    bool Complete() { return graphicsFamily.has_value() && presentFamily.has_value(); }
};

class VulkanDevice
{
private:
    VulkanInstance* appContext;
    VulkanWindow* window;

    VkPhysicalDevice physicalDevice;
    VkDevice logicalDevice;

    VulkanQueueFamilyIndices queueFamilyIndices;

    VkQueue graphicsQueue;
    VkQueue presentQueue;

    bool intialized = false;

    int RatePhysicalDevice(VkPhysicalDevice device);
    bool CheckForDeviceExtensions(VkPhysicalDevice device);
    bool SwapchainSupported(VkPhysicalDevice device);
public:
    VulkanDevice(VulkanInstance* appContext, VulkanWindow* window) : appContext(appContext), window(window) {}
    ~VulkanDevice()
    {
        if(intialized) { Destroy(); }
    }

    VulkanQueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device);

    void SelectPhysicalDevice();
    void CreateDevice();

    void Destroy();

    VkPhysicalDevice GetPhysicalDevice() { return physicalDevice; }
    VkDevice GetLogicalDevice() { return logicalDevice; }
};