#pragma once
#include <vector>
#include <iostream>
#include <cassert>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <app-context/vk_device.h>
#include <graphics-pipeline/vk_shader_module.h>
#include <graphics-pipeline/vk_renderpass.h>

#include <buffer/vk_descriptor_set_layout.h>

class VulkanGraphicsPipeline
{
private:
    std::shared_ptr<VulkanDevice> device;
    std::shared_ptr<VulkanRenderPass> renderpass;

    VkPipeline pipeline;
    VkPipelineLayout layout;

    std::vector<VkDescriptorSetLayout> pipelineDescriptorSetLayouts;
    std::vector<VkPipelineShaderStageCreateInfo> shaderStages;

    bool initialized = false;
public:
    VulkanGraphicsPipeline(std::shared_ptr<VulkanDevice> device, std::shared_ptr<VulkanRenderPass> renderpass) : device(device), renderpass(renderpass) {}
    ~VulkanGraphicsPipeline()
    {
        if(initialized) { Destroy(); }
    }

    void AddShaderStage(std::shared_ptr<VulkanShaderModule> module)
    { 
        assert(!initialized);
        shaderStages.push_back(module->GetShaderStageCreateInfo());
    }

    void CreatePipeline(std::weak_ptr<VulkanDescriptorSetLayout> descriptorSetLayoutPTR);
    void Destroy();

    VkPipeline GetPipeline() { return pipeline; }
    VkPipelineLayout GetPipelineLayout() { return layout; }
};