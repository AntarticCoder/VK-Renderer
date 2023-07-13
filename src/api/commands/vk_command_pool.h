#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <api/app-context/vk_device.h>
#include <utils/vk_api_object.h>

class VulkanCommandPool : public VulkanAPIObject
{
private:
    std::shared_ptr<VulkanDevice> device;

    VkCommandPool commandPool;
public:
    VulkanCommandPool(std::shared_ptr<VulkanDevice> device) : device(device) {}
    ~VulkanCommandPool() {}

    void CreateCommandPool();
    void Destroy() override;

    VkCommandPool GetCommandPool() { return commandPool; }
};