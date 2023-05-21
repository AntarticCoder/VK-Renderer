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
    std::optional<uint32_t> computeFamily;
    bool Complete() { return graphicsFamily.has_value() && computeFamily.has_value(); }
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

    VkQueue graphicsQueue;
    VkQueue computeQueue;
private:    
    std::vector<const char*> GetInstanceExtensions();
    int RatePhysicalDevice(VkPhysicalDevice device);
    VulkanQueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device);

    void CreateWindow(int width, int height, std::string title);
    void CreateInstance();
    void SelectPhysicalDevice();
    void CreateDevice();
public:
    void Initialize(VulkanApplicationInfo info);
    void Destroy() {}

    GLFWwindow* GetWindow() { return window; }
    VkInstance GetInstance() { return instance; }
};