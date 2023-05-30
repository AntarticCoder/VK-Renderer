#include <graphics-pipeline/vk_framebuffer.h>
#include <utils/vk_utils.h>

void VulkanFramebuffers::CreateFramebuffers()
{
    framebuffers.resize(swapchain->GetSwapchainLength());

    for (size_t i = 0; i < framebuffers.size(); i++)
    {
        VkImageView attachments[] = {
            swapchain->GetImageViews()[i]
        };

        VkFramebufferCreateInfo framebufferInfo{};
        framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass = renderpass->GetRenderPass();
        framebufferInfo.attachmentCount = 1;
        framebufferInfo.pAttachments = attachments;
        framebufferInfo.width = swapchain->GetExtent().width;
        framebufferInfo.height = swapchain->GetExtent().height;
        framebufferInfo.layers = 1;

        VkResult result = vkCreateFramebuffer(device->GetLogicalDevice(), &framebufferInfo, nullptr, &framebuffers[i]);
        VK_CHECK(result);
    }

    initialized = true;
}

void VulkanFramebuffers::Destroy()
{
    assert(initialized);
    for (auto framebuffer : framebuffers)
    {
        vkDestroyFramebuffer(device->GetLogicalDevice(), framebuffer, nullptr);
    }

    initialized = false;
}