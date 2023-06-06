#pragma once
#include <vector>
#include <iostream>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <app-context/vk_device.h>
#include <graphics-pipeline/vk_shader_module.h>
#include <graphics-pipeline/vk_renderpass.h>

#include <buffer/vk_descriptor_set_layout.h>

class VulkanGraphicsPipeline
{
private:
    VulkanDevice* device;
    VulkanRenderPass* renderpass;

    VkPipeline pipeline;
    VkPipelineLayout layout;
    VulkanDescriptorSetLayout* pipelineDescriptorSetLayout;

    std::vector<VkDescriptorSetLayout> pipelineDescriptorSetLayouts;
    std::vector<VkPipelineShaderStageCreateInfo> shaderStages;

    bool initialized = false;
public:
    VulkanGraphicsPipeline(VulkanDevice* device, VulkanRenderPass* renderpass) : device(device), renderpass(renderpass) {}
    ~VulkanGraphicsPipeline()
    {
        if(initialized) { Destroy(); }
    }

    void AddShaderStage(VulkanShaderModule* module)
    { 
        assert(!initialized);
        shaderStages.push_back(module->GetShaderStageCreateInfo());
    }

    void CreatePipeline(VulkanDescriptorSetLayout* descriptorSetLayout);
    void Destroy();

    VkPipeline GetPipeline() { return pipeline; }
    VkPipelineLayout GetPipelineLayout() { return layout; }
    VulkanDescriptorSetLayout* GetDescriptorSetLayout()
    { 
        return pipelineDescriptorSetLayout;
    }
};