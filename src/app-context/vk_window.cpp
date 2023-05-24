#include <iostream>

#include <utils/vk_utils.h>
#include <app-context/vk_window.h>

void VulkanWindow::CreateWindow()
{
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);

    if(window == NULL) { std::cout << "Failed to create GLFW Window: " << glfwGetError(NULL) << std::endl; }
}

void VulkanWindow::CreateSurface(VulkanInstance* appContext)
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