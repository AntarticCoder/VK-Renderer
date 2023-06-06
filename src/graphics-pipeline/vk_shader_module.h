#pragma once
#include <string>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <app-context/vk_device.h>

enum VulkanShaderStage
{
    NONE_SHADER_STAGE = 0,
    VERTEX_SHADER_STAGE = 1,
    FRAGMENT_SHADER_STAGE = 2
};

class VulkanShaderModule
{
private:
    VulkanDevice* device = nullptr;

    VkShaderModule shaderModule;
    VkPipelineShaderStageCreateInfo shaderStageInfo{};
    VulkanShaderStage shaderStage;

    std::string shaderPath;
    std::string shaderEntryPoint; 
    bool initialized = false;
public:
    VulkanShaderModule(VulkanDevice* device) : device(device) {}
    ~VulkanShaderModule()
    {
        if(initialized) { Destroy(); }
    }

    void CreateShaderModule(VulkanShaderStage stage, const std::string filePath, std::string entryPoint = "main");
    void Destroy();

    VkShaderModule GetShaderModule() { return shaderModule; }
    VkPipelineShaderStageCreateInfo GetShaderStageCreateInfo() { return shaderStageInfo; }
    VulkanShaderStage GetShaderStage() { return shaderStage; }
};