#include <cassert>

#include <graphics-pipeline/vk_renderpass.h>
#include <utils/vk_utils.h>

#include <commands/vk_command_buffer.h>
#include <app-context/vk_swapchain.h>

void VulkanRenderPass::CreateRenderpass()
{
    VkAttachmentDescription colorAttachment{};
    colorAttachment.format = swapchain->GetFormat();
    colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;

    colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;

    colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;

    colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    VkAttachmentReference colorAttachmentRef{};
    colorAttachmentRef.attachment = 0;
    colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkSubpassDescription subpass{};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;

    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &colorAttachmentRef;

    VkRenderPassCreateInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassInfo.attachmentCount = 1;
    renderPassInfo.pAttachments = &colorAttachment;
    renderPassInfo.subpassCount = 1;
    renderPassInfo.pSubpasses = &subpass;
    renderPassInfo.dependencyCount = 1;

    VkSubpassDependency dependency{};
	dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
	dependency.dstSubpass = 0;

	dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependency.srcAccessMask = 0;

	dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
    renderPassInfo.pDependencies = &dependency;

    VkResult result = vkCreateRenderPass(device->GetLogicalDevice(), &renderPassInfo, nullptr, &renderpass);
    VK_CHECK(result);

    initialized = true;
}

void VulkanRenderPass::Destroy()
{
    assert(initialized);
    vkDestroyRenderPass(device->GetLogicalDevice(), renderpass, nullptr);

    initialized = false;
}

void VulkanRenderPass::Begin(std::shared_ptr<VulkanCommandBuffer> commandBuffer, std::shared_ptr<VulkanSwapchain> swapchain, std::vector<VkFramebuffer> framebuffers, uint32_t imageIndex)
{
    assert(initialized);
    assert(!active);

    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = renderpass;
    renderPassInfo.framebuffer = framebuffers[imageIndex];
    renderPassInfo.renderArea.offset = {0, 0};
    renderPassInfo.renderArea.extent = swapchain->GetExtent();

    VkClearValue clearColor = {{{0.0f, 0.0f, 0.0f, 1.0f}}};
    renderPassInfo.clearValueCount = 1;
    renderPassInfo.pClearValues = &clearColor;

    vkCmdBeginRenderPass(commandBuffer->GetCommandBuffer(), &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

    active = true;
}

void VulkanRenderPass::End(std::shared_ptr<VulkanCommandBuffer> commandBuffer)
{
    assert(active);
    vkCmdEndRenderPass(commandBuffer->GetCommandBuffer());

    active = false;
}

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