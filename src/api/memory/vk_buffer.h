#pragma once
#include <cstring>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <api/app-context/vk_device.h>
#include <api/memory/vk_vma_allocator.h>
#include <utils/vk_api_object.h>

enum VulkanBufferUsage
{
    NONE_BUFFER = 0,
    VERTEX_BUFFER = 1,
    INDEX_BUFFER = 2,
    UNIFORM_BUFFER = 3
};

class VulkanBuffer : public VulkanAPIObject
{
private:
    std::shared_ptr<VulkanDevice> device;
    std::shared_ptr<VulkanMemoryAllocator> vmaAllocator;

    VkBuffer buffer;
    VmaAllocation allocation;

    void* bufferData;
public:
    VulkanBuffer(std::shared_ptr<VulkanDevice> device, std::shared_ptr<VulkanMemoryAllocator> vmaAllocator) : device(device), vmaAllocator(vmaAllocator) {}
    ~VulkanBuffer() {}

    void CreateBuffer(void* data, uint32_t size, VulkanBufferUsage bufferUsage);
    void UpdateBuffer(void* data, uint32_t size);
    void Destroy() override;

    VkBuffer GetBuffer() { return buffer; }
};