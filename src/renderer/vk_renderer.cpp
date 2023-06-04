#include <utils/vk_utils.h>
#include <renderer/vk_renderer.h>
#include <graphics-pipeline/vk_vertex.h>

void VulkanRenderer::CreateSynchronizationStructures()
{
	VkDevice vkDevice = device->GetLogicalDevice();

	imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
	renderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
	inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);

	VkSemaphoreCreateInfo semaphoreInfo{};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

	VkFenceCreateInfo fenceInfo{};
	fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

	for(int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
	{
		vkCreateSemaphore(vkDevice, &semaphoreInfo, nullptr, &imageAvailableSemaphores[i]);
		vkCreateSemaphore(vkDevice, &semaphoreInfo, nullptr, &renderFinishedSemaphores[i]);
		vkCreateFence(vkDevice, &fenceInfo, nullptr, &inFlightFences[i]);
	}

	syncStructuresCreated = true;
}

void VulkanRenderer::DestroySynchronizationStructures()
{
	assert(syncStructuresCreated);
	VkDevice vkDevice = device->GetLogicalDevice();

	for(int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
	{
		vkDestroySemaphore(vkDevice, imageAvailableSemaphores[i], nullptr);
		vkDestroySemaphore(vkDevice, renderFinishedSemaphores[i], nullptr);
		vkDestroyFence(vkDevice, inFlightFences[i], nullptr);
	}

	syncStructuresCreated = false;
}

void VulkanRenderer::Init()
{
	CreateSynchronizationStructures();

	commandPool = new VulkanCommandPool(device);
	commandPool->CreateCommandPool();

	commandBuffers.resize(MAX_FRAMES_IN_FLIGHT);

	for(int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
	{
		commandBuffers[i] = new VulkanCommandBuffer(device, commandPool);
		commandBuffers[i]->AllocateCommandBuffer();
	}

	const std::vector<class Vertex> vertices =
	{
    	{{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},
		{{0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
		{{0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
		{{-0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}}
	};

	const std::vector<uint16_t> indices =
	{
		0, 1, 2, 2, 3, 0
	};

	void* vertexData;
	vertexData = (void*)&vertices[0];

	vertexBuffer = new VulkanBuffer(device);
	vertexBuffer->CreateBuffer(vertexData, sizeof(vertices[0]) * vertices.size(), VERTEX_BUFFER);

	indexBuffer = new VulkanBuffer(device);
	indexBuffer->CreateBuffer((void*)&indices[0], sizeof(indices[0]) * indices.size(), INDEX_BUFFER);
}

void VulkanRenderer::Draw(VulkanGraphicsPipeline* pipeline, VulkanRenderPass* renderpass, VulkanFramebuffers* framebuffer)
{
	VulkanCommandBuffer* vulkanCommandBuffer = commandBuffers[currentFrame];

	VkDevice vkDevice = device->GetLogicalDevice();
	VkCommandBuffer vkCommandBuffer = vulkanCommandBuffer->GetCommandBuffer();

	VkQueue vkGraphicsQueue = device->GetGraphicsQueue();
	VkQueue vkPresentQueue = device->GetPresentQueue();

	VkSemaphore renderFinishedSemaphore = renderFinishedSemaphores[currentFrame];
	VkSemaphore imageAvailableSemaphore = imageAvailableSemaphores[currentFrame];
	VkFence inFlightFence = inFlightFences[currentFrame];
	
	vkWaitForFences(vkDevice, 1, &inFlightFence, VK_TRUE, UINT64_MAX);
	vkResetFences(vkDevice, 1, &inFlightFence);

	uint32_t imageIndex;
    VkResult result = vkAcquireNextImageKHR(vkDevice, swapchain->GetSwapchain(), UINT64_MAX, imageAvailableSemaphore, VK_NULL_HANDLE, &imageIndex);
	
	if(VK_CHECK_SWAPCHAIN_OUT_OF_DATE(result) || window->GetFramebufferResize())
	{
		vkDeviceWaitIdle(vkDevice);

		swapchain->Destroy();
		framebuffer->Destroy();
		swapchain->CreateSwapchain();
		framebuffer->CreateFramebuffers();			
	}

	vkResetFences(vkDevice, 1, &inFlightFences[currentFrame]);

	vulkanCommandBuffer->Reset();
	vulkanCommandBuffer->Begin();
	renderpass->Begin(vulkanCommandBuffer, swapchain, framebuffer->GetFramebuffers(), imageIndex);

	vkCmdBindPipeline(vkCommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline->GetPipeline());

	VkViewport viewport{};
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = (float) swapchain->GetExtent().width;
	viewport.height = (float) swapchain->GetExtent().height;
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;
	vkCmdSetViewport(vkCommandBuffer, 0, 1, &viewport);

	VkRect2D scissor{};
	scissor.offset = {0, 0};
	scissor.extent = swapchain->GetExtent();
	vkCmdSetScissor(vkCommandBuffer, 0, 1, &scissor);           

	VkBuffer vertexBuffers[] = {vertexBuffer->GetBuffer()};
	VkDeviceSize offsets[] = {0};
	vkCmdBindVertexBuffers(vkCommandBuffer, 0, 1, vertexBuffers, offsets); 
	vkCmdBindIndexBuffer(vkCommandBuffer, indexBuffer->GetBuffer(), 0, VK_INDEX_TYPE_UINT16);

	// vkCmdDraw(vkCommandBuffer, 3, 1, 0, 0);
	vkCmdDrawIndexed(vkCommandBuffer, 6, 1, 0, 0, 0);

	renderpass->End(vulkanCommandBuffer);
	vulkanCommandBuffer->End();

	VkSubmitInfo submitInfo{};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

	VkSemaphore waitSemaphores[] = {imageAvailableSemaphore};
	VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
	submitInfo.waitSemaphoreCount = 1;
	submitInfo.pWaitSemaphores = waitSemaphores;
	submitInfo.pWaitDstStageMask = waitStages;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &vkCommandBuffer;

	VkSemaphore signalSemaphores[] = {renderFinishedSemaphore};
	submitInfo.signalSemaphoreCount = 1;	
	submitInfo.pSignalSemaphores = signalSemaphores;

	result = vkQueueSubmit(vkGraphicsQueue, 1, &submitInfo, inFlightFence);
	VK_CHECK(result);

	VkPresentInfoKHR presentInfo{};
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

	presentInfo.waitSemaphoreCount = 1;
	presentInfo.pWaitSemaphores = signalSemaphores;

	VkSwapchainKHR swapChains[] = {swapchain->GetSwapchain()};
	presentInfo.swapchainCount = 1;
	presentInfo.pSwapchains = swapChains;
	presentInfo.pImageIndices = &imageIndex;

	presentInfo.pResults = nullptr;

	result = vkQueuePresentKHR(vkPresentQueue, &presentInfo);

	if(VK_CHECK_SWAPCHAIN_OUT_OF_DATE(result) || window->GetFramebufferResize())
	{
		vkDeviceWaitIdle(vkDevice);

		swapchain->Destroy();
		framebuffer->Destroy();
		swapchain->CreateSwapchain();
		framebuffer->CreateFramebuffers();		
	}

	currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
}

void VulkanRenderer::Destroy()
{
	vkDeviceWaitIdle(device->GetLogicalDevice());

	DestroySynchronizationStructures();
	commandPool->Destroy();
	vertexBuffer->Destroy();
	indexBuffer->Destroy();
}