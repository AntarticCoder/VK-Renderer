#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <api/app-context/vk_window.h>
#include <api/app-context/vk_device.h>
#include <api/app-context/vk_swapchain.h>

#include <api/commands/vk_command_pool.h>
#include <api/commands/vk_command_buffer.h>

#include <api/graphics-pipeline/vk_graphics_pipeline.h>
#include <api/graphics-pipeline/vk_renderpass.h>

#include <api/memory/vk_buffer.h>
#include <api/descriptors/vk_descriptor_set.h>
#include <api/descriptors/vk_descriptor_set_layout.h>

#include <api/definitions/vk_vertex.h>
#include <api/definitions/vk_uniforms.h>

class VulkanRenderer
{
private:
    std::shared_ptr<VulkanWindow> window;
    std::shared_ptr<VulkanInstance> instance;
    std::shared_ptr<VulkanDevice> device;
    std::shared_ptr<VulkanSwapchain> swapchain;
    std::shared_ptr<VulkanGraphicsPipeline> pipeline;

    std::shared_ptr<VulkanCommandPool> commandPool;
    std::vector<std::shared_ptr<VulkanCommandBuffer>> commandBuffers;

    std::vector<VkSemaphore> imageAvailableSemaphores;
    std::vector<VkSemaphore> renderFinishedSemaphores;
    std::vector<VkFence> inFlightFences;

    std::shared_ptr<VulkanBuffer> vertexBuffer;
    std::shared_ptr<VulkanBuffer> indexBuffer;
    std::vector<std::shared_ptr<VulkanBuffer>> uniformBuffers;

    std::shared_ptr<VulkanMemoryAllocator> allocator;

    std::shared_ptr<VulkanDescriptorPool> descriptorPool;
    std::vector<std::shared_ptr<VulkanDescriptorSet>> descriptorSets;

    std::shared_ptr<VulkanRenderPass> renderpass;

    UniformBufferObject ubo{};
    uint32_t currentFrame = 0;
    const int MAX_FRAMES_IN_FLIGHT = 2;

    VkDescriptorPool imguiPool;

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
    VulkanRenderer(
        std::shared_ptr<VulkanInstance> instance,
        std::shared_ptr<VulkanWindow> window, 
        std::shared_ptr<VulkanDevice> device, 
        std::shared_ptr<VulkanSwapchain> swapchain,
        std::shared_ptr<VulkanRenderPass> renderpass
    )  : instance(instance), device(device), swapchain(swapchain), window(window), renderpass(renderpass) { }
    
    ~VulkanRenderer() {}

    void InitImgui();
    void DrawImgui(VkCommandBuffer cmd);
    void DestroyImgui();

    void Init(std::shared_ptr<VulkanDescriptorSetLayout> layout);
    void UpdateUniforms();
    void Draw(std::weak_ptr<VulkanGraphicsPipeline> pipelinePTR, std::weak_ptr<VulkanRenderPass> renderpassPTR, std::weak_ptr<VulkanFramebuffers> framebufferPTR);
    void Destroy();
};