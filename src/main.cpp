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
    std::cout << "VK-Renderer Starting Up!" << std::endl;

    VulkanWindow window("VK Renderer", 800, 600);
    VulkanInstance context;
    VulkanDevice device(&context, &window);
    VulkanSwapchain swapchain(&window, &device);
    VulkanRenderer renderer(&window, &device, &swapchain);

    window.CreateWindow();
    context.CreateInstance();
    window.CreateSurface(&context);
    device.SelectPhysicalDevice();
    device.CreateDevice();
    swapchain.CreateSwapchain();

    VulkanShaderModule vertex(&device);
    VulkanShaderModule fragment(&device);
    VulkanRenderPass renderPass(&device, &swapchain);
    VulkanDescriptorSetLayout descriptorSetLayout(&device, VERTEX_SHADER_STAGE);
    VulkanGraphicsPipeline graphicsPipeline(&device, &renderPass);
    VulkanFramebuffers framebuffers(&device, &swapchain, &renderPass);

    renderPass.CreateRenderpass();

    vertex.CreateShaderModule(VERTEX_SHADER_STAGE, "/Users/nick/Programming/Graphics-Programming/VK-Renderer/resources/shaders/compiled/vert.spv");
    fragment.CreateShaderModule(FRAGMENT_SHADER_STAGE, "/Users/nick/Programming/Graphics-Programming/VK-Renderer/resources/shaders/compiled/frag.spv");

    descriptorSetLayout.CreateDescriptorSetLayout(0, false);

    graphicsPipeline.AddShaderStage(&vertex);
    graphicsPipeline.AddShaderStage(&fragment);

    graphicsPipeline.CreatePipeline(&descriptorSetLayout);

    framebuffers.CreateFramebuffers();

    renderer.Init(descriptorSetLayout);

    while(!glfwWindowShouldClose(window.GetWindow()))
    {
        renderer.Draw(&graphicsPipeline, &renderPass, &framebuffers);
        glfwPollEvents();
    }
    renderer.Destroy();

    descriptorSetLayout.Destroy();

    framebuffers.Destroy();

    graphicsPipeline.Destroy();

    vertex.Destroy();
    fragment.Destroy();

    renderPass.Destroy();

    swapchain.Destroy();
    device.Destroy();
    window.Destroy();
    context.Destroy();

    std::cout << "VK Renderer Shutting Down!" << std::endl;
    return 0;
}