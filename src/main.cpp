#include <iostream>

#include <app-context/vk_instance.h>
#include <app-context/vk_device.h>
#include <app-context/vk_swapchain.h>
#include <app-context/vk_window.h>

int main()
{
    std::cout << "Hello World!" << std::endl;

    VulkanWindow window("VK Renderer", 800, 600);
    VulkanInstance context;
    VulkanDevice device(&context, &window);
    VulkanSwapchain swapchain(&context, &window, &device);

    window.CreateWindow();
    context.CreateInstance();
    window.CreateSurface(&context);
    device.SelectPhysicalDevice();
    device.CreateDevice();
    swapchain.CreateSwapchain();

    while(!glfwWindowShouldClose(window.GetWindow()))
    {
        glfwPollEvents();
    }

    swapchain.Destroy();
    device.Destroy();
    window.Destroy();
    context.Destroy();

    return 0;
}