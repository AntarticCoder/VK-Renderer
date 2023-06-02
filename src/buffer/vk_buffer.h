#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <app-context/vk_device.h>

enum VulkanBufferUsage
{
    VERTEX_BUFFER = 0,
    INDEX_BUFFER = 1
};

class VulkanBuffer
{
private:
    VulkanDevice* device = nullptr;

    VkBuffer buffer;
    VkDeviceMemory bufferMemory;

    bool initalized = false;
public:
    VulkanBuffer(VulkanDevice* device) : device(device) {}
    ~VulkanBuffer()
    {
        if(initalized) { Destroy(); }
    }

    void CreateBuffer(void* data, uint32_t size, VulkanBufferUsage bufferUsage);
    void Destroy();

    VkBuffer GetBuffer() { return buffer; }
};