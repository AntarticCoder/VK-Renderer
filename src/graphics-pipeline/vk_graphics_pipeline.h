#pragma once
#include <vector>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <app-context/vk_device.h>
#include <graphics-pipeline/vk_shader_module.h>
#include <graphics-pipeline/vk_renderpass.h>

class VulkanGraphicsPipeline
{
private:
    VulkanDevice* device;
    VulkanRenderPass* renderpass;

    VkPipeline pipeline;
    VkPipelineLayout layout;

    std::vector<VkPipelineShaderStageCreateInfo> shaderStages;

    bool initialized = false;
public:
    VulkanGraphicsPipeline(VulkanDevice* device, VulkanRenderPass* renderpass) : device(device), renderpass(renderpass) {}
    ~VulkanGraphicsPipeline()
    {
        if(initialized) { Destroy(); }
    }

    void AddShaderStage(VulkanShaderModule* module) { shaderStages.push_back(module->GetShaderStageCreateInfo()); }

    void CreatePipeline();
    void Destroy();

    VkPipeline GetPipeline() { return pipeline; }
};
