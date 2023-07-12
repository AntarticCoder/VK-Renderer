#include <iostream>

#include <utils/vk_utils.h>
#include <api/app-context/vk_window.h>

bool VulkanWindow::framebufferResized = false;

void VulkanWindow::CreateWindow()
{
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
    glfwSetWindowUserPointer(window, this);
    glfwSetFramebufferSizeCallback(window, FramebufferResizeCallback);

    if(window == NULL) { std::cout << "Failed to create GLFW Window: " << glfwGetError(NULL) << std::endl; }
}

void VulkanWindow::CreateSurface(std::shared_ptr<VulkanInstance> appContext)
{
    VkResult result = glfwCreateWindowSurface(appContext->GetInstance(), window, nullptr, &surface);
    VK_CHECK(result);

    context = appContext;
}

void VulkanWindow::Destroy()
{
    vkDestroySurfaceKHR(context->GetInstance(), surface, nullptr);
    glfwDestroyWindow(window);
    glfwTerminate();
}