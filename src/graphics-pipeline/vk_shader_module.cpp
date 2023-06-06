#include <graphics-pipeline/vk_shader_module.h>
#include <utils/vk_utils.h>
#include <utils/vk_io.h>

void VulkanShaderModule::CreateShaderModule(VulkanShaderStage stage, const std::string filePath, std::string entryPoint)
{
    shaderStage = stage; 
    shaderPath = filePath;
    shaderEntryPoint = entryPoint;

    std::vector<char> shaderBytes = ReadFile(shaderPath);

    VkShaderModuleCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = shaderBytes.size();
    createInfo.pCode = reinterpret_cast<const uint32_t*>(shaderBytes.data());

    VkResult result = vkCreateShaderModule(device->GetLogicalDevice(), &createInfo, nullptr, &shaderModule);
    VK_CHECK(result);

    shaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    switch(shaderStage)
    {
        case NONE_SHADER_STAGE: std::cout << "Invalid Shader Stage: None" << std::endl; abort();
        case VERTEX_SHADER_STAGE: shaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT; break;
        case FRAGMENT_SHADER_STAGE: shaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT; break;
        default: std::cout << "Undefined Shader Stage" << std::endl; abort();
    }

    shaderStageInfo.module = shaderModule;
    shaderStageInfo.pName = shaderEntryPoint.c_str();

    initialized = true;

    shaderStage = stage;
}

void VulkanShaderModule::Destroy()
{
    assert(initialized);
    vkDestroyShaderModule(device->GetLogicalDevice(), shaderModule, nullptr);

    initialized = false;
}