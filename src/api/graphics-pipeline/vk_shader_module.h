#pragma once
#include <string>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <api/app-context/vk_device.h>
#include <utils/vk_api_object.h>

enum VulkanShaderStage
{
    NONE_SHADER_STAGE = 0,
    VERTEX_SHADER_STAGE = 1,
    FRAGMENT_SHADER_STAGE = 2
};

class VulkanShaderModule : public VulkanAPIObject
{
private:
    std::shared_ptr<VulkanDevice> device;

    VkShaderModule shaderModule;
    VkPipelineShaderStageCreateInfo shaderStageInfo{};
    VulkanShaderStage shaderStage;

    std::string shaderPath;
    std::string shaderEntryPoint; 
public:
    VulkanShaderModule(std::shared_ptr<VulkanDevice> device) : device(device) {}
    ~VulkanShaderModule() {}

    void CreateShaderModule(VulkanShaderStage stage, const std::string filePath, std::string entryPoint = "main");
    void Destroy() override;

    VkShaderModule GetShaderModule() { return shaderModule; }
    VkPipelineShaderStageCreateInfo GetShaderStageCreateInfo() { return shaderStageInfo; }
    VulkanShaderStage GetShaderStage() { return shaderStage; }
};