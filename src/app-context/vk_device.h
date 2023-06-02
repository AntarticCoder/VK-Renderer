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
    VulkanInstance* appContext = nullptr;
    VulkanWindow* window = nullptr;

    VkPhysicalDevice physicalDevice;
    VkDevice logicalDevice;

    VulkanQueueFamilyIndices queueFamilyIndices;

    VkQueue commonQueue = VK_NULL_HANDLE;
    VkQueue graphicsQueue = VK_NULL_HANDLE;
    VkQueue presentQueue = VK_NULL_HANDLE;

    VkPhysicalDeviceMemoryProperties memoryProperties;

    bool commonQueueFamily = false;
    bool initialized = false;

    int RatePhysicalDevice(VkPhysicalDevice device);
    bool CheckForDeviceExtensions(VkPhysicalDevice device);
    bool SwapchainSupported(VkPhysicalDevice device);
public:
    VulkanDevice(VulkanInstance* appContext, VulkanWindow* window) : appContext(appContext), window(window) {}
    ~VulkanDevice()
    {
        if(initialized) { Destroy(); }
    }

    VulkanQueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device);
    uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

    void SelectPhysicalDevice();
    void CreateDevice();

    void Destroy();

    VkPhysicalDevice GetPhysicalDevice() { return physicalDevice; }
    VkDevice GetLogicalDevice() { return logicalDevice; }

    VkQueue GetCommonQueue();
    VkQueue GetGraphicsQueue();
    VkQueue GetPresentQueue();

    VkPhysicalDeviceMemoryProperties GetMemoryProperties() { return memoryProperties; }
    VulkanQueueFamilyIndices GetQueueFamilyIndices() { return queueFamilyIndices; }
};