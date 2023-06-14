#include <iostream>
#include <memory>

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

    std::shared_ptr<VulkanWindow> window = std::make_shared<VulkanWindow>("VK Renderer", 800, 600);
    std::shared_ptr<VulkanInstance> instance = std::make_shared<VulkanInstance>();
    std::shared_ptr<VulkanDevice> device = std::make_shared<VulkanDevice>(instance, window);
    std::shared_ptr<VulkanSwapchain> swapchain = std::make_shared<VulkanSwapchain>(window, device);
    std::shared_ptr<VulkanRenderer> renderer = std::make_shared<VulkanRenderer>(instance, window, device, swapchain);

    window->CreateWindow();
    instance->CreateInstance();
    window->CreateSurface(instance);
    device->SelectPhysicalDevice();
    device->CreateDevice();
    swapchain->CreateSwapchain();

    std::shared_ptr<VulkanShaderModule> vertex= std::make_shared<VulkanShaderModule>(device);
    std::shared_ptr<VulkanShaderModule> fragment = std::make_shared<VulkanShaderModule>(device);
    std::shared_ptr<VulkanRenderPass> renderPass = std::make_shared<VulkanRenderPass>(device, swapchain);
    std::shared_ptr<VulkanDescriptorSetLayout> descriptorSetLayout = std::make_shared<VulkanDescriptorSetLayout>(device, VERTEX_SHADER_STAGE);
    std::shared_ptr<VulkanGraphicsPipeline> graphicsPipeline = std::make_shared<VulkanGraphicsPipeline>(device, renderPass);
    std::shared_ptr<VulkanFramebuffers> framebuffers = std::make_shared<VulkanFramebuffers>(device, swapchain, renderPass);

    renderPass->CreateRenderpass();

    vertex->CreateShaderModule(VERTEX_SHADER_STAGE, "/Users/nick/Programming/Graphics-Programming/VK-Renderer/resources/shaders/compiled/vert.spv");
    fragment->CreateShaderModule(FRAGMENT_SHADER_STAGE, "/Users/nick/Programming/Graphics-Programming/VK-Renderer/resources/shaders/compiled/frag.spv");

    descriptorSetLayout->CreateDescriptorSetLayout(0, false);

    graphicsPipeline->AddShaderStage(vertex);
    graphicsPipeline->AddShaderStage(fragment);

    graphicsPipeline->CreatePipeline(descriptorSetLayout);

    framebuffers->CreateFramebuffers();

    renderer->Init(descriptorSetLayout);

    while(!glfwWindowShouldClose(window->GetWindow()))
    {
        renderer->Draw(graphicsPipeline, renderPass, framebuffers);
        glfwPollEvents();
    }
    renderer->Destroy();

    descriptorSetLayout->Destroy();

    framebuffers->Destroy();

    graphicsPipeline->Destroy();

    vertex->Destroy();
    fragment->Destroy();

    renderPass->Destroy();

    swapchain->Destroy();
    device->Destroy();
    window->Destroy();
    instance->Destroy();

    std::cout << "VK Renderer Shutting Down!" << std::endl;
    return 0;
}