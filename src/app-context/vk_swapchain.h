#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <app-context/vk_window.h>
#include <app-context/vk_device.h>

struct VulkanSwapchainSupportDetails
{
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;

    bool UseableSwapchain() { return !formats.empty() && !presentModes.empty(); }
};

class VulkanSwapchain
{
private:
    VulkanWindow* window = nullptr;
    VulkanDevice* device = nullptr;

    VulkanSwapchainSupportDetails supportDetails;

    VkSwapchainKHR swapchain;
    VkFormat swapchainImageFormat;
    VkExtent2D swapchainExtent;

    std::vector<VkImage> swapchainImages;
    std::vector<VkImageView> swapchainImageViews;

    bool initialized = false;

    VulkanSwapchainSupportDetails GetSwapchainSupportDetails();
    VkSurfaceFormatKHR ChooseSwapchainSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
    VkPresentModeKHR ChooseSwapchainPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
    VkExtent2D ChooseSwapchainExtent(const VkSurfaceCapabilitiesKHR& capabilities);

    void CreateImageViews();
public:
    VulkanSwapchain(VulkanWindow* window, VulkanDevice* device) : window(window), device(device) {}
    ~VulkanSwapchain()
    { 
        if(initialized) { Destroy(); }
    }

    void CreateSwapchain();
    void RecreateSwapchain();
    void Destroy();

    VkFormat GetFormat() { return swapchainImageFormat; }
    VkExtent2D GetExtent() { return swapchainExtent; }

    VkSwapchainKHR GetSwapchain() { return swapchain; }
    std::vector<VkImage> GetImages() { return swapchainImages; }
    std::vector<VkImageView> GetImageViews() { return swapchainImageViews; }

    int GetSwapchainLength() { return swapchainImages.size(); }
    int GetSwapchainImageViewLength() { return swapchainImageViews.size(); }
};