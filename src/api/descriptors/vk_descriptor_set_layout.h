#pragma once
#include <cassert>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <api/app-context/vk_device.h>
#include <api/memory/vk_buffer.h>
#include <api/graphics-pipeline/vk_shader_module.h>

class VulkanDescriptorSetLayout
{
private:
    std::shared_ptr<VulkanDevice> device;
    VulkanShaderStage shaderStage;

    VkDescriptorSetLayout descriptorSetLayout;
    bool initalized = false;
public:
    VulkanDescriptorSetLayout(std::shared_ptr<VulkanDevice> device, VulkanShaderStage shaderStage) : device(device), shaderStage(shaderStage) {}
    ~VulkanDescriptorSetLayout()
    {
        if(initalized) { Destroy(); }
    }

    void CreateDescriptorSetLayout(uint32_t binding, bool allShaderStages = false);
    void Destroy();

    VkDescriptorSetLayout GetDescriptorLayout()
    { 
        assert(initalized);
        return descriptorSetLayout;
    }
};
