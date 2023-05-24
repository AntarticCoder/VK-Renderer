#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <app-context/vk_application_context.h>

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
    VulkanApplicationContext* appContext;
    VulkanSwapchainSupportDetails supportDetails;

    VkSwapchainKHR swapchain;
    VkFormat swapchainImageFormat;
    VkExtent2D swapchainExtent;

    std::vector<VkImage> swapchainImages;
    std::vector<VkImageView> swapchainImageViews;

    VulkanSwapchainSupportDetails GetSwapchainSupportDetails();
    VkSurfaceFormatKHR ChooseSwapchainSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
    VkPresentModeKHR ChooseSwapchainPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
    VkExtent2D ChooseSwapchainExtent(const VkSurfaceCapabilitiesKHR& capabilities);

    void CreateImageViews();
public:
    VulkanSwapchain(VulkanApplicationContext context) { appContext = &context; }
    ~VulkanSwapchain() {}

    void CreateSwapchain();
    void RecreateSwapchain() {}
    void Destroy();
};