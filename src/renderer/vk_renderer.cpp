#include <utils/vk_utils.h>
#include <renderer/vk_renderer.h>

void VulkanRenderer::CreateSynchronizationStructures()
{
	VkSemaphoreCreateInfo semaphoreInfo{};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

	VkFenceCreateInfo fenceInfo{};
	fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

	vkCreateSemaphore(device->GetLogicalDevice(), &semaphoreInfo, nullptr, &imageAvailableSemaphore);
    vkCreateSemaphore(device->GetLogicalDevice(), &semaphoreInfo, nullptr, &renderFinishedSemaphore);
    vkCreateFence(device->GetLogicalDevice(), &fenceInfo, nullptr, &inFlightFence);

	syncStructuresCreated = true;
}

void VulkanRenderer::DestroySynchronizationStructures()
{
	assert(syncStructuresCreated);

	vkDestroySemaphore(device->GetLogicalDevice(), imageAvailableSemaphore, nullptr);
    vkDestroySemaphore(device->GetLogicalDevice(), renderFinishedSemaphore, nullptr);
    vkDestroyFence(device->GetLogicalDevice(), inFlightFence, nullptr);

	syncStructuresCreated = false;
}

void VulkanRenderer::Init()
{
	CreateSynchronizationStructures();

	commandPool = new VulkanCommandPool(device);
	commandBuffer = new VulkanCommandBuffer(device, commandPool);

	commandPool->CreateCommandPool();
	commandBuffer->AllocateCommandBuffer();
}

void VulkanRenderer::Draw(VulkanGraphicsPipeline* pipeline, VulkanRenderPass* renderpass, VulkanFramebuffers* framebuffer)
{
	vkWaitForFences(device->GetLogicalDevice(), 1, &inFlightFence, VK_TRUE, UINT64_MAX);
	vkResetFences(device->GetLogicalDevice(), 1, &inFlightFence);

	uint32_t imageIndex;
    vkAcquireNextImageKHR(device->GetLogicalDevice(), swapchain->GetSwapchain(), UINT64_MAX, imageAvailableSemaphore, VK_NULL_HANDLE, &imageIndex);

	vkResetCommandBuffer(commandBuffer->GetCommandBuffer(), 0);

	commandBuffer->Begin();
	renderpass->Begin(commandBuffer, swapchain, framebuffer->GetFramebuffers(), imageIndex);

	vkCmdBindPipeline(commandBuffer->GetCommandBuffer(), VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline->GetPipeline());

	VkViewport viewport{};
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = (float) swapchain->GetExtent().width;
	viewport.height = (float) swapchain->GetExtent().height;
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;
	vkCmdSetViewport(commandBuffer->GetCommandBuffer(), 0, 1, &viewport);

	VkRect2D scissor{};
	scissor.offset = {0, 0};
	scissor.extent = swapchain->GetExtent();
	vkCmdSetScissor(commandBuffer->GetCommandBuffer(), 0, 1, &scissor);            

	vkCmdDraw(commandBuffer->GetCommandBuffer(), 3, 1, 0, 0);

	renderpass->End(commandBuffer);
	commandBuffer->End();

	VkSubmitInfo submitInfo{};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

	VkSemaphore waitSemaphores[] = {imageAvailableSemaphore};
	VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
	submitInfo.waitSemaphoreCount = 1;
	submitInfo.pWaitSemaphores = waitSemaphores;
	submitInfo.pWaitDstStageMask = waitStages;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = commandBuffer->GetCommandBufferAddress();

	VkSemaphore signalSemaphores[] = {renderFinishedSemaphore};
	submitInfo.signalSemaphoreCount = 1;	
	submitInfo.pSignalSemaphores = signalSemaphores;

	VkResult result = vkQueueSubmit(device->GetGraphicsQueue(), 1, &submitInfo, inFlightFence);

	VkPresentInfoKHR presentInfo{};
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

	presentInfo.waitSemaphoreCount = 1;
	presentInfo.pWaitSemaphores = signalSemaphores;

	VkSwapchainKHR swapChains[] = {swapchain->GetSwapchain()};
	presentInfo.swapchainCount = 1;
	presentInfo.pSwapchains = swapChains;
	presentInfo.pImageIndices = &imageIndex;

	presentInfo.pResults = nullptr;

	vkQueuePresentKHR(device->GetGraphicsQueue(), &presentInfo);
}

void VulkanRenderer::Destroy()
{
	vkDeviceWaitIdle(device->GetLogicalDevice());

	DestroySynchronizationStructures();
	commandPool->Destroy();
}