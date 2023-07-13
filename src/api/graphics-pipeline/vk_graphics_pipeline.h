#pragma once
#include <vector>
#include <iostream>
#include <cassert>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <api/app-context/vk_device.h>
#include <api/graphics-pipeline/vk_shader_module.h>
#include <api/graphics-pipeline/vk_renderpass.h>
#include <api/descriptors/vk_descriptor_set_layout.h>
#include <utils/vk_api_object.h>

class VulkanGraphicsPipeline : public VulkanAPIObject
{
private:
    std::shared_ptr<VulkanDevice> device;
    std::shared_ptr<VulkanRenderPass> renderpass;

    VkPipeline pipeline;
    VkPipelineLayout layout;

    std::vector<VkDescriptorSetLayout> pipelineDescriptorSetLayouts;
    std::vector<VkPipelineShaderStageCreateInfo> shaderStages;
public:
    VulkanGraphicsPipeline(std::shared_ptr<VulkanDevice> device, std::shared_ptr<VulkanRenderPass> renderpass) : device(device), renderpass(renderpass) {}
    ~VulkanGraphicsPipeline() {}

    void AddShaderStage(std::shared_ptr<VulkanShaderModule> module)
    { 
        assert(!initialized);
        shaderStages.push_back(module->GetShaderStageCreateInfo());
    }

    void CreatePipeline(std::weak_ptr<VulkanDescriptorSetLayout> descriptorSetLayoutPTR);
    void Destroy() override;

    VkPipeline GetPipeline() { return pipeline; }
    VkPipelineLayout GetPipelineLayout() { return layout; }
};