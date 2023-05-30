#include <iostream>

#include <app-context/vk_instance.h>
#include <app-context/vk_device.h>
#include <app-context/vk_swapchain.h>
#include <app-context/vk_window.h>

#include <graphics-pipeline/vk_shader_module.h>
#include <graphics-pipeline/vk_renderpass.h>
#include <graphics-pipeline/vk_graphics_pipeline.h>


#include <renderer/vk_renderer.h>

int main()
{
    std::cout << "Hello World!" << std::endl;

    VulkanWindow window("VK Renderer", 800, 600);
    VulkanInstance context;
    VulkanDevice device(&context, &window);
    VulkanSwapchain swapchain(&context, &window, &device);
    VulkanRenderer renderer(&window, &device, &swapchain);

    VulkanShaderModule vertex(&device);
    VulkanShaderModule fragment(&device);
    VulkanRenderPass renderPass(&device, &swapchain);
    VulkanGraphicsPipeline graphicsPipeline(&device, &renderPass);
    VulkanFramebuffers framebuffers(&device, &swapchain, &renderPass);

    window.CreateWindow();
    context.CreateInstance();
    window.CreateSurface(&context);
    device.SelectPhysicalDevice();
    device.CreateDevice();
    swapchain.CreateSwapchain();

    renderPass.CreateRenderpass();

    vertex.CreateShaderModule(Vertex, "/Users/nick/Programming/Graphics-Programming/VK-Renderer/resources/shaders/compiled/vert.spv");
    fragment.CreateShaderModule(Fragment, "/Users/nick/Programming/Graphics-Programming/VK-Renderer/resources/shaders/compiled/frag.spv");

    graphicsPipeline.AddShaderStage(&vertex);
    graphicsPipeline.AddShaderStage(&fragment);

    graphicsPipeline.CreatePipeline();

    framebuffers.CreateFramebuffers();

    renderer.Init();

    while(!glfwWindowShouldClose(window.GetWindow()))
    {
        renderer.Draw(&graphicsPipeline, &renderPass, &framebuffers);
        glfwPollEvents();
    }
    renderer.Destroy();

    framebuffers.Destroy();

    graphicsPipeline.Destroy();

    vertex.Destroy();
    fragment.Destroy();

    renderPass.Destroy();

    swapchain.Destroy();
    device.Destroy();
    window.Destroy();
    context.Destroy();

    return 0;
}