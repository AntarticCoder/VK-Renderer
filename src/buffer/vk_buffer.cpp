#include <iostream>

#include <buffer/vk_buffer.h>
#include <utils/vk_utils.h>

void VulkanBuffer::CreateBuffer(void* data, uint32_t size, VulkanBufferUsage bufferUsage)
{
    VkDevice vkDevice = device->GetLogicalDevice();

    VkBufferCreateInfo bufferInfo{};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = size;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    switch(bufferUsage)
    {
        case VERTEX_BUFFER: bufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT; break;
        default: std::cout << "Buffer Usage not specified, aborting" << std::endl; abort();
    }
    
    VkResult result = vkCreateBuffer(vkDevice, &bufferInfo, nullptr, &buffer);
    VK_CHECK(result);

    VkMemoryRequirements memoryRequirements;
    vkGetBufferMemoryRequirements(vkDevice, buffer, &memoryRequirements);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memoryRequirements.size;
    allocInfo.memoryTypeIndex = device->FindMemoryType(memoryRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

    result = vkAllocateMemory(vkDevice, &allocInfo, nullptr, &bufferMemory);
    VK_CHECK(result);

    result = vkBindBufferMemory(vkDevice, buffer, bufferMemory, 0);
    VK_CHECK(result);

    void* bufferData;
    vkMapMemory(vkDevice, bufferMemory, 0, bufferInfo.size, 0, &bufferData);
    memcpy(bufferData, data, (size_t) bufferInfo.size);
    vkUnmapMemory(vkDevice, bufferMemory);
}

void VulkanBuffer::Destroy()
{
    VkDevice vkDevice = device->GetLogicalDevice();

    vkDestroyBuffer(vkDevice, buffer, nullptr);
    vkFreeMemory(vkDevice, bufferMemory, nullptr);
}