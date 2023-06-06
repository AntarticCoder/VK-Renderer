#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <app-context/vk_window.h>
#include <app-context/vk_device.h>
#include <app-context/vk_swapchain.h>

#include <commands/vk_command_pool.h>
#include <commands/vk_command_buffer.h>

#include <graphics-pipeline/vk_graphics_pipeline.h>
#include <graphics-pipeline/vk_renderpass.h>

#include <buffer/vk_buffer.h>
#include <buffer/vk_descriptor_set.h>
#include <buffer/vk_descriptor_set_layout.h>

#include <definitions/vk_vertex.h>
#include <definitions/vk_uniforms.h>

class VulkanRenderer
{
private:
    VulkanWindow* window;
    VulkanDevice* device;
    VulkanSwapchain* swapchain;
    VulkanGraphicsPipeline* pipeline;

    VulkanCommandPool* commandPool;
    std::vector<VulkanCommandBuffer*> commandBuffers;

    std::vector<VkSemaphore> imageAvailableSemaphores;
    std::vector<VkSemaphore> renderFinishedSemaphores;
    std::vector<VkFence> inFlightFences;

    VulkanBuffer* vertexBuffer;
    VulkanBuffer* indexBuffer;
    std::vector<VulkanBuffer*> uniformBuffers;

    VulkanDescriptorPool* descriptorPool;
    std::vector<VulkanDescriptorSet*> descriptorSets;

    UniformBufferObject ubo{};
    uint32_t currentFrame = 0;
    const int MAX_FRAMES_IN_FLIGHT = 2;

    void CreateSynchronizationStructures();
    void DestroySynchronizationStructures();

    void CreateCommands();
    void DestroyCommands();

    void CreateBuffers();
    void DestroyBuffers();

    void CreateDescriptors(VkDescriptorSetLayout layout);
    void DestroyDescriptors();

    bool syncStructuresCreated = false;
public:
    VulkanRenderer(VulkanWindow* window, VulkanDevice* device, VulkanSwapchain* swapchain) : device(device), swapchain(swapchain), window(window) { }
    ~VulkanRenderer() {}

    void Init(VulkanDescriptorSetLayout layout);
    void UpdateUniforms();
    void Draw(VulkanGraphicsPipeline* pipeline, VulkanRenderPass* renderpass, VulkanFramebuffers* framebuffer);
    void Destroy();
};