#include <commands/vk_command_pool.h>
#include <utils/vk_utils.h>

void VulkanCommandPool::CreateCommandPool()
{
    VulkanQueueFamilyIndices indices = device->GetQueueFamilyIndices();

    VkCommandPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    poolInfo.queueFamilyIndex = indices.graphicsFamily.value();

    VkResult result = vkCreateCommandPool(device->GetLogicalDevice(), &poolInfo, nullptr, &commandPool);
    VK_CHECK(result);

    initialized = true;
}

void VulkanCommandPool::Destroy()
{
    assert(initialized);
    vkDestroyCommandPool(device->GetLogicalDevice(), commandPool, nullptr);

    initialized = false;
}