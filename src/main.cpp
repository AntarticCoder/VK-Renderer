#include <iostream>

#include <app-context/vk_application_context.h>

int main()
{
    std::cout << "Hello World!" << std::endl;

    VulkanApplicationInfo info = {};
    info.width = 800;
    info.height = 600;
    info.title = "VK Renderer";

    VulkanApplicationContext context;
    context.Initialize(info);

    while(glfwWindowShouldClose(context.GetWindow())) 
    {
        glfwPollEvents();
    }
}