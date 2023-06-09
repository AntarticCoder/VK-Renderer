#include <cassert>

#include <buffer/vk_descriptor_set_layout.h>
#include <utils/vk_utils.h>

void VulkanDescriptorSetLayout::CreateDescriptorSetLayout(uint32_t binding, bool allShaderStages)
{
    assert(!initalized);

    VkDescriptorSetLayoutBinding uboLayoutBinding{};
    uboLayoutBinding.binding = 0;
    uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    uboLayoutBinding.descriptorCount = 1;

    switch(shaderStage)
    {
        case NONE_SHADER_STAGE: std::cout << "Invalid Shader Stage: None" << std::endl; abort();
        case VERTEX_SHADER_STAGE: uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT; break;
        case FRAGMENT_SHADER_STAGE: uboLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT; break;
        default: std::cout << "Shader Stage Undefined" << std::endl; abort();
    }   
    uboLayoutBinding.pImmutableSamplers = nullptr;

    VkDescriptorSetLayoutCreateInfo layoutInfo{};
    layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layoutInfo.bindingCount = 1;
    layoutInfo.pBindings = &uboLayoutBinding;

    VkResult result = vkCreateDescriptorSetLayout(device->GetLogicalDevice(), &layoutInfo, nullptr, &descriptorSetLayout);
    VK_CHECK(result);

    initalized = true;
}

void VulkanDescriptorSetLayout::Destroy()
{
    assert(initalized);
    vkDestroyDescriptorSetLayout(device->GetLogicalDevice(), descriptorSetLayout, nullptr);

    initalized = false;
}
