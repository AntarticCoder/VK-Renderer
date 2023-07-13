#pragma once
#include <cassert>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <api/app-context/vk_device.h>
#include <api/memory/vk_buffer.h>
#include <api/graphics-pipeline/vk_shader_module.h>
#include <utils/vk_api_object.h>

class VulkanDescriptorSetLayout : public VulkanAPIObject
{
private:
    std::shared_ptr<VulkanDevice> device;
    VulkanShaderStage shaderStage;

    VkDescriptorSetLayout descriptorSetLayout;
public:
    VulkanDescriptorSetLayout(std::shared_ptr<VulkanDevice> device, VulkanShaderStage shaderStage) : device(device), shaderStage(shaderStage) {}
    ~VulkanDescriptorSetLayout() {}

    void CreateDescriptorSetLayout(uint32_t binding, bool allShaderStages = false);
    void Destroy() override;

    VkDescriptorSetLayout GetDescriptorLayout()
    { 
        assert(initialized);
        return descriptorSetLayout;
    }
};
