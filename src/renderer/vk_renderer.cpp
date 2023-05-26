#include <utils/vk_utils.h>
#include <renderer/vk_renderer.h>

void VulkanRenderer::CreateCommands()
{
    // TODO: Abstract command pool creation
    VkCommandPoolCreateInfo commandPoolInfo = {};
	commandPoolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	commandPoolInfo.pNext = nullptr;

	commandPoolInfo.queueFamilyIndex = device->FindQueueFamilies(device->GetPhysicalDevice()).graphicsFamily.value();
	commandPoolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

    VkResult commandPoolResult = vkCreateCommandPool(device->GetLogicalDevice(), &commandPoolInfo, nullptr, &commandPool);
    VK_CHECK(commandPoolResult);

    // TODO: Abstract command buffer creation
    VkCommandBufferAllocateInfo commandAllocInfo = {};
	commandAllocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	commandAllocInfo.pNext = nullptr;

	commandAllocInfo.commandPool = commandPool;
	commandAllocInfo.commandBufferCount = 1;
	commandAllocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;

    VkResult primaryCommandBufferResult = vkAllocateCommandBuffers(device->GetLogicalDevice(), &commandAllocInfo, &primaryCommandBuffer);
    VK_CHECK(primaryCommandBufferResult);

    commandsInitialized = true;
}

void VulkanRenderer::CreateDefaultRenderPass()
{
    VkAttachmentDescription colorAttachment = {};
	colorAttachment.format = swapchain->GetFormat();
	colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
	colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;

	colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;

	colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    VkAttachmentReference colorAttachmentRef = {};
	colorAttachmentRef.attachment = 0;
	colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	VkSubpassDescription subpass = {};
	subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	subpass.colorAttachmentCount = 1;
	subpass.pColorAttachments = &colorAttachmentRef;

    VkRenderPassCreateInfo renderPassInfo = {};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	renderPassInfo.attachmentCount = 1;
	renderPassInfo.pAttachments = &colorAttachment;
	renderPassInfo.subpassCount = 1;
	renderPassInfo.pSubpasses = &subpass;

    VkResult result = vkCreateRenderPass(device->GetLogicalDevice(), &renderPassInfo, nullptr, &renderpass);
	VK_CHECK(result);

    renderpassInitialized = true;
}

void VulkanRenderer::CreateFramebuffers()
{
  	VkFramebufferCreateInfo framebufferInfo = {};
	framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
	framebufferInfo.pNext = nullptr;

	framebufferInfo.renderPass = renderpass;
	framebufferInfo.attachmentCount = 1;
	framebufferInfo.width = swapchain->GetExtent().width;
	framebufferInfo.height = swapchain->GetExtent().height;
	framebufferInfo.layers = 1;

	const uint32_t swapchainLength = swapchain->GetSwapchainImageViewLength();
	framebuffers = std::vector<VkFramebuffer>(swapchainLength);

	for(int i = 0; i < swapchainLength; i++)
    {
		VkImageView imageView[] = { swapchain->GetImageViews()[i] };
		framebufferInfo.pAttachments = imageView;
        VkResult result = vkCreateFramebuffer(device->GetLogicalDevice(), &framebufferInfo, nullptr, &framebuffers[i]);
		VK_CHECK(result);
	}

    framebufferInitialized = true;
}

void VulkanRenderer::IntializeSyncStructures()
{
    VkFenceCreateInfo fenceCreateInfo = {};
	fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	fenceCreateInfo.pNext = nullptr;
	fenceCreateInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

	VkResult fenceResult = vkCreateFence(device->GetLogicalDevice(), &fenceCreateInfo, nullptr, &renderFence);
	VK_CHECK(fenceResult);

	VkSemaphoreCreateInfo semaphoreCreateInfo = {};
	semaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
	semaphoreCreateInfo.pNext = nullptr;
	semaphoreCreateInfo.flags = 0;

	VkResult presentSemaphoreResult = vkCreateSemaphore(device->GetLogicalDevice(), &semaphoreCreateInfo, nullptr, &presentSemaphore);
	VkResult renderSemaphoreResult = vkCreateSemaphore(device->GetLogicalDevice(), &semaphoreCreateInfo, nullptr, &renderSemaphore);

	VK_CHECK(presentSemaphoreResult);
	VK_CHECK(renderSemaphoreResult);
}

void VulkanRenderer::Draw()
{
	VkResult result = VK_SUCCESS;

	result = vkWaitForFences(device->GetLogicalDevice(), 1, &renderFence, true, 1000000000);
	VK_CHECK(result);

	result = vkResetFences(device->GetLogicalDevice(), 1, &renderFence);
	VK_CHECK(result);

	uint32_t imageIndex = 0;
	result = vkAcquireNextImageKHR(device->GetLogicalDevice(), swapchain->GetSwapchain(), 1000000000, presentSemaphore, nullptr, &imageIndex);
	VK_CHECK(result);

	result = vkResetCommandBuffer(primaryCommandBuffer, 0);
	VK_CHECK(result);

	VkCommandBuffer cmd = primaryCommandBuffer;

	VkCommandBufferBeginInfo cmdBeginInfo = {};
	cmdBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	cmdBeginInfo.pNext = nullptr;

	cmdBeginInfo.pInheritanceInfo = nullptr;
	cmdBeginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

	result = vkBeginCommandBuffer(cmd, &cmdBeginInfo);
	VK_CHECK(result);

	VkClearValue clearValue;
	clearValue.color = { { 172.0f/255.0f, 22.0f/255.0f, 44.0f/255.0f, 1.0f } };

	VkRenderPassBeginInfo renderpassInfo = {};
	renderpassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	renderpassInfo.pNext = nullptr;

	renderpassInfo.renderPass = renderpass;
	renderpassInfo.renderArea.offset.x = 0;
	renderpassInfo.renderArea.offset.y = 0;
	renderpassInfo.renderArea.extent = swapchain->GetExtent();
	renderpassInfo.framebuffer = framebuffers[imageIndex];

	renderpassInfo.clearValueCount = 1;
	renderpassInfo.pClearValues = &clearValue;

	vkCmdBeginRenderPass(cmd, &renderpassInfo, VK_SUBPASS_CONTENTS_INLINE);
	vkCmdEndRenderPass(cmd);

	result = vkEndCommandBuffer(cmd);
	VK_CHECK(result);

	VkSubmitInfo submit = {};
	submit.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submit.pNext = nullptr;

	VkPipelineStageFlags waitStage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;

	submit.pWaitDstStageMask = &waitStage;

	submit.waitSemaphoreCount = 1;
	submit.pWaitSemaphores = &presentSemaphore;

	submit.signalSemaphoreCount = 1;
	submit.pSignalSemaphores = &renderSemaphore;

	submit.commandBufferCount = 1;
	submit.pCommandBuffers = &cmd;

	result = vkQueueSubmit(device->GetGraphicsQueue(), 1, &submit, renderFence);
	VK_CHECK(result);

	VkPresentInfoKHR presentInfo = {};
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	presentInfo.pNext = nullptr;

	VkSwapchainKHR swapchainKHR = swapchain->GetSwapchain();
	presentInfo.pSwapchains = &swapchainKHR;
	presentInfo.swapchainCount = 1;
	presentInfo.pWaitSemaphores = &renderSemaphore;
	presentInfo.waitSemaphoreCount = 1;
	presentInfo.pImageIndices = &imageIndex;

	result = vkQueuePresentKHR(device->GetGraphicsQueue(), &presentInfo);
    VK_CHECK(result);
}

void VulkanRenderer::DestroyCommands()
{
    assert(commandsInitialized);
    vkDestroyCommandPool(device->GetLogicalDevice(), commandPool, nullptr);
}

void VulkanRenderer::DestroyFramebuffers()
{
    assert(framebufferInitialized);

    for(int i = 0; i < framebuffers.size(); i++)
    {
        vkDestroyFramebuffer(device->GetLogicalDevice(), framebuffers[i], nullptr);
    }
}

void VulkanRenderer::DestroyRenderPass()
{
    assert(renderpassInitialized);
    vkDestroyRenderPass(device->GetLogicalDevice(), renderpass, nullptr);
}

void VulkanRenderer::DestroySyncStructures()
{
	vkWaitForFences(device->GetLogicalDevice(), 1, &renderFence, true, 1000000000);

	vkDestroySemaphore(device->GetLogicalDevice(), presentSemaphore, nullptr);
	vkDestroySemaphore(device->GetLogicalDevice(), renderSemaphore, nullptr);
	vkDestroyFence(device->GetLogicalDevice(), renderFence, nullptr);
}