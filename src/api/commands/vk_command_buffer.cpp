#include <cassert>

#include <api/commands/vk_command_buffer.h>
#include <utils/vk_utils.h>

void VulkanCommandBuffer::AllocateCommandBuffer()
{
    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = pool->GetCommandPool();
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = 1;

    VkResult result = vkAllocateCommandBuffers(device->GetLogicalDevice(), &allocInfo, &commandBuffer);
    VK_CHECK(result);

    initialized = true;
}

void VulkanCommandBuffer::Begin()
{
    assert(initialized);
    assert(!recording);

    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = 0;
    beginInfo.pInheritanceInfo = nullptr;

    VkResult result = vkBeginCommandBuffer(commandBuffer, &beginInfo);
    VK_CHECK(result);

    recording = true;
}

void VulkanCommandBuffer::End()
{
    assert(recording);
    VkResult result = vkEndCommandBuffer(commandBuffer);
    VK_CHECK(result);

    recording = false;
}

void VulkanCommandBuffer::Reset()
{
    vkResetCommandBuffer(commandBuffer, 0);
}