#pragma once
#include <vector>
#include <string>
#include <optional>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

struct VulkanApplicationInfo
{
    int width;
    int height;
    std::string title;
};

struct VulkanQueueFamilyIndices
{
    std::optional<uint32_t> graphicsFamily;
    std::optional<uint32_t> presentFamily;

    bool Complete() { return graphicsFamily.has_value() && presentFamily.has_value(); }
};

struct VulkanSwapChainSupportDetails
{
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;

    bool AdequateSwapchain() { return !formats.empty() && !presentModes.empty(); }
};

class VulkanApplicationContext
{
private:
    GLFWwindow* window;
    int windowHeight;
    int windowWidth;
    std::string windowTitle;

    VkInstance instance;
    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
    VkDevice logicalDevice;
    VkSurfaceKHR surface;

    VkSwapchainKHR swapchain;
    std::vector<VkImage> swapchainImages;
    std::vector<VkImageView> swapchainImageViews;
    VkFormat swapchainImageFormat;
    VkExtent2D swapchainExtent;

    VkQueue graphicsQueue;
    VkQueue presentQueue;
private:    
    std::vector<const char*> GetInstanceExtensions();
    int RatePhysicalDevice(VkPhysicalDevice device);
    VulkanQueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device);
    bool CheckForDeviceExtensions(VkPhysicalDevice device);
    VulkanSwapChainSupportDetails QuerySwapchainSupport(VkPhysicalDevice device);

    VkSurfaceFormatKHR ChooseSwapchainSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
    VkPresentModeKHR ChooseSwapchainPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
    VkExtent2D ChooseSwapchainExtent(const VkSurfaceCapabilitiesKHR& capabilities);

    void CreateWindow(int width, int height, std::string title);
    void CreateInstance();
    void SelectPhysicalDevice();
    void CreateDevice();
    void CreateSurface();
    void CreateSwapchain();
    void CreateImageViews();
public:
    void Initialize(VulkanApplicationInfo info);
    void Destroy() {}

    GLFWwindow* GetWindow() { return window; }
    VkInstance GetInstance() { return instance; }
};