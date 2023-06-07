#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <app-context/vk_device.h>

enum VulkanBufferUsage
{
    NONE_BUFFER = 0,
    VERTEX_BUFFER = 1,
    INDEX_BUFFER = 2,
    UNIFORM_BUFFER = 3
};

class VulkanBuffer
{
private:
    std::shared_ptr<VulkanDevice> device;

    VkBuffer buffer;
    VkDeviceMemory bufferMemory;

    void* bufferData;
    bool initalized = false;
public:
    VulkanBuffer(std::shared_ptr<VulkanDevice> device) : device(device) {}
    ~VulkanBuffer()
    {
        if(initalized) { Destroy(); }
    }

    void CreateBuffer(void* data, uint32_t size, VulkanBufferUsage bufferUsage);
    void UpdateBuffer(void* data, uint32_t size);
    void Destroy();

    VkBuffer GetBuffer() { return buffer; }
};