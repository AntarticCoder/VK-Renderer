#pragma once
#include <string>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <app-context/vk_instance.h>

class VulkanWindow
{
private:
    std::shared_ptr<VulkanInstance> context;

    GLFWwindow* window;
    VkSurfaceKHR surface;

    std::string title;
    unsigned int width;
    unsigned int height;

    static bool framebufferResized;
    bool initialized = false;
public:
    VulkanWindow(std::string windowTitle, int windowWidth, int windowHeight) : title(windowTitle), width(windowWidth), height(windowHeight) {}
    ~VulkanWindow()
    {
        if(initialized) { Destroy(); }
    }

    void CreateWindow();
    void CreateSurface(std::shared_ptr<VulkanInstance> appContext);
    void Destroy();
    
    GLFWwindow* GetWindow() { return window; }
    VkSurfaceKHR GetSurface() { return surface; }

    bool GetFramebufferResize()
    {
        if(framebufferResized == true)
        { 
            framebufferResized = false;
            return true;
        }

        framebufferResized = false;
        return false;
    }
    
    static void FramebufferResizeCallback(GLFWwindow* window, int width, int height)
    {
        framebufferResized = true;
    }
};