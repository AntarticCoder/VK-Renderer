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
        case VulkanShaderStage::Vertex:
            shaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
            break;
        case VulkanShaderStage::Fragment:
            shaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
            break;
        default:
            throw std::runtime_error("Invalid shader stage!");
    }

    shaderStageInfo.module = shaderModule;
    shaderStageInfo.pName = shaderEntryPoint.c_str();

    initalized = true;
}

void VulkanShaderModule::Destroy()
{
    assert(initalized);
    vkDestroyShaderModule(device->GetLogicalDevice(), shaderModule, nullptr);
}