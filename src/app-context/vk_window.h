#pragma once
#include <string>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <app-context/vk_instance.h>

class VulkanWindow
{
private:
    VulkanInstance* context;

    GLFWwindow* window;
    VkSurfaceKHR surface;

    std::string title;
    unsigned int width;
    unsigned int height;

    bool initialized = false;
public:
    VulkanWindow(std::string windowTitle, int windowWidth, int windowHeight) : title(windowTitle), width(windowWidth), height(windowHeight) {}
    ~VulkanWindow()
    {
        if(initialized) { Destroy(); }
    }

    void CreateWindow();
    void CreateSurface(VulkanInstance* appContext);
    void Destroy();
    
    GLFWwindow* GetWindow() { return window; }
    VkSurfaceKHR GetSurface() { return surface; }
};