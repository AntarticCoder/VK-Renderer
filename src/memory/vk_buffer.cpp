#include <iostream>

#include <memory/vk_buffer.h>
#include <utils/vk_utils.h>

void VulkanBuffer::CreateBuffer(void* data, uint32_t size, VulkanBufferUsage bufferUsage)
{
    VkBufferCreateInfo bufferInfo{};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = size;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    switch(bufferUsage)
    {
        case NONE_BUFFER: std::cout << "Invalid Buffer Usage: None" << std::endl; abort();
        case VERTEX_BUFFER: bufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT; break;
        case INDEX_BUFFER: bufferInfo.usage = VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT; break;
        case UNIFORM_BUFFER: bufferInfo.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT; break;
        default: std::cout << "Undefined Buffer Usage" << std::endl; abort();
    }

    VmaAllocationCreateInfo allocInfo = {};
    allocInfo.usage = VMA_MEMORY_USAGE_AUTO;
    allocInfo.flags = VMA_ALLOCATION_CREATE_HOST_ACCESS_RANDOM_BIT | VMA_ALLOCATION_CREATE_MAPPED_BIT;
    
    VkResult result = vmaCreateBuffer(vmaAllocator->GetAllocator(), &bufferInfo, &allocInfo, &buffer, &allocation, nullptr);
    VK_CHECK(result);

    vmaMapMemory(vmaAllocator->GetAllocator(), allocation, &bufferData);
    memcpy(bufferData, data, (size_t) bufferInfo.size);
    vmaUnmapMemory(vmaAllocator->GetAllocator(), allocation);
}

void VulkanBuffer::UpdateBuffer(void* data, uint32_t size)
{
    memcpy(bufferData, data, (size_t) size);
}

void VulkanBuffer::Destroy()
{
    vmaDestroyBuffer(vmaAllocator->GetAllocator(), buffer, allocation);
}