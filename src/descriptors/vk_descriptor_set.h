#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <app-context/vk_device.h>
#include <memory/vk_buffer.h>

class VulkanDescriptorPool
{
private:
    std::shared_ptr<VulkanDevice> device;
    VkDescriptorPool descriptorPool;

    bool initalized = false;
public:
    VulkanDescriptorPool(std::shared_ptr<VulkanDevice> device) : device(device){}
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
    std::shared_ptr<VulkanDevice> device;
    std::shared_ptr<VulkanDescriptorPool> descriptorPool;

    VkDescriptorSet descriptorSet;
public:
    VulkanDescriptorSet(std::shared_ptr<VulkanDevice> device, std::shared_ptr<VulkanDescriptorPool> descriptorPool) : device(device), descriptorPool(descriptorPool) {}
    ~VulkanDescriptorSet() {}

    void CreateDescriptorSet(uint32_t size, VulkanBuffer buffer, VkDescriptorSetLayout layout);
    VkDescriptorSet GetDescriptorSet() { return descriptorSet; }
};