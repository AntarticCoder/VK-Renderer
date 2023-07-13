#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <api/app-context/vk_device.h>
#include <api/memory/vk_buffer.h>
#include <utils/vk_api_object.h>

class VulkanDescriptorPool : public VulkanAPIObject
{
private:
    std::shared_ptr<VulkanDevice> device;
    VkDescriptorPool descriptorPool;
public:
    VulkanDescriptorPool(std::shared_ptr<VulkanDevice> device) : device(device){}
    ~VulkanDescriptorPool() {}

    void CreateDescriptorPool();
    void Destroy() override;

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