#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <app-context/vk_device.h>
#include <buffer/vk_buffer.h>

class VulkanDescriptorPool
{
private:
    VulkanDevice* device = nullptr;
    VkDescriptorPool descriptorPool;

    bool initalized = false;
public:
    VulkanDescriptorPool(VulkanDevice* device) : device(device){}
    ~VulkanDescriptorPool()
    {
        if(initalized) { Destroy(); }
    }

    void CreateDescriptorPool();
    void Destroy();

    VkDescriptorPool GetDescriptorPool() { return descriptorPool; }
};

class VulkanDescriptorSet
{
private:
    VulkanDevice* device = nullptr;
    VulkanDescriptorPool* descriptorPool = nullptr;

    VkDescriptorSet descriptorSet;
public:
    VulkanDescriptorSet(VulkanDevice* device, VulkanDescriptorPool* descriptorPool) : device(device), descriptorPool(descriptorPool) {}
    ~VulkanDescriptorSet() {}

    void CreateDescriptorSet(uint32_t size, VulkanBuffer buffer, VkDescriptorSetLayout layout);
    VkDescriptorSet GetDescriptorSet() { return descriptorSet; }
};