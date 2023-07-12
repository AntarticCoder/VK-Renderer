#include <api/memory/vk_vma_allocator.h>

void VulkanMemoryAllocator::CreateAllocator()
{
    assert(!initalized);

    VmaAllocatorCreateInfo allocatorCreateInfo = {};
    allocatorCreateInfo.vulkanApiVersion = VK_API_VERSION_1_2;
    allocatorCreateInfo.physicalDevice = device->GetPhysicalDevice();
    allocatorCreateInfo.device = device->GetLogicalDevice();
    allocatorCreateInfo.instance = instance->GetInstance();
    allocatorCreateInfo.pVulkanFunctions = nullptr;

    vmaCreateAllocator(&allocatorCreateInfo, &vmaAllocator);

    initalized = true;
}

void VulkanMemoryAllocator::Destroy()
{
    assert(initalized);
    vmaDestroyAllocator(vmaAllocator);

    initalized = false;
}