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

    VulkanQueueFamilyIndices queueFamilyIndices;

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
    bool SwapchainSupported(VkPhysicalDevice device);

    void CreateWindow(int width, int height, std::string title);
    void CreateSurface();
    void CreateInstance();
    void SelectPhysicalDevice();
    void CreateDevice();
public:
    void Initialize(VulkanApplicationInfo info);
    void Destroy() {}

    GLFWwindow* GetWindow() { return window; }
    VkSurfaceKHR GetSurface() { return surface; }
    VkInstance GetInstance() { return instance; }
    VkPhysicalDevice GetPhysicalDevice() { return physicalDevice; }
    VkDevice GetLogicalDevice() { return logicalDevice; }
    VulkanQueueFamilyIndices GetQueueFamilyIndices() { return queueFamilyIndices; }
};