#pragma once
#include <cassert>

#include <api/app-context/vk_instance.h>
#include <api/app-context/vk_device.h>

#define VMA_VULKAN_VERSION 1002000
#include "vk_mem_alloc.h"

class VulkanMemoryAllocator
{
private:
    std::shared_ptr<VulkanInstance> instance;
    std::shared_ptr<VulkanDevice> device;

    VmaAllocator vmaAllocator;

    bool initalized = false;
public:
    VulkanMemoryAllocator(std::shared_ptr<VulkanInstance> instance, std::shared_ptr<VulkanDevice> device) : instance(instance), device(device) {}
    ~VulkanMemoryAllocator()
    {
        if(initalized) { Destroy(); }
    }

    void CreateAllocator();
    void Destroy();

    VmaAllocator GetAllocator()
    {
        assert(initalized);
        return vmaAllocator;
    }
};