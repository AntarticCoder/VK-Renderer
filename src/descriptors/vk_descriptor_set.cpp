#include <descriptors/vk_descriptor_set.h>
#include <memory/vk_buffer.h>
#include <utils/vk_utils.h>

void VulkanDescriptorPool::CreateDescriptorPool()
{
    VkDescriptorPoolSize poolSize{};
    poolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    poolSize.descriptorCount = 3;

    VkDescriptorPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.poolSizeCount = 1;
    poolInfo.pPoolSizes = &poolSize;
    poolInfo.maxSets = 3;
    
    VkResult result = vkCreateDescriptorPool(device->GetLogicalDevice(), &poolInfo, nullptr, &descriptorPool);
    VK_CHECK(result);
}

void VulkanDescriptorPool::Destroy()
{
    vkDestroyDescriptorPool(device->GetLogicalDevice(), descriptorPool, nullptr);
}

void VulkanDescriptorSet::CreateDescriptorSet(uint32_t size, VulkanBuffer buffer, VkDescriptorSetLayout layout)
{
    VkDescriptorSetLayout descriptorSetLayouts[1] = { layout };

    VkDescriptorSetAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.descriptorPool = descriptorPool->GetDescriptorPool();
    allocInfo.descriptorSetCount = 1;
    allocInfo.pSetLayouts = &descriptorSetLayouts[0];

    VkResult result = vkAllocateDescriptorSets(device->GetLogicalDevice(), &allocInfo, &descriptorSet);
    VK_CHECK(result);

    VkDescriptorBufferInfo bufferInfo{};
    bufferInfo.buffer = buffer.GetBuffer();
    bufferInfo.offset = 0;
    bufferInfo.range = size;

    VkWriteDescriptorSet descriptorWrite{};
    descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    descriptorWrite.dstSet = descriptorSet;
    descriptorWrite.dstBinding = 0;
    descriptorWrite.dstArrayElement = 0;

    descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    descriptorWrite.descriptorCount = 1;

    descriptorWrite.pBufferInfo = &bufferInfo;
    descriptorWrite.pImageInfo = nullptr;
    descriptorWrite.pTexelBufferView = nullptr;

    vkUpdateDescriptorSets(device->GetLogicalDevice(), 1, &descriptorWrite, 0, nullptr);
}

