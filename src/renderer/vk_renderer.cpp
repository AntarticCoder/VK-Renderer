#include <cassert>
#include <chrono>

#include <utils/vk_utils.h>
#include <renderer/vk_renderer.h>

#include <imgui/backends/imgui_impl_vulkan.h>
#include <imgui/backends/imgui_impl_glfw.h>

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

void VulkanRenderer::CreateCommands()
{
	commandPool = std::make_shared<VulkanCommandPool>(device);
	commandPool->CreateCommandPool();

	commandBuffers.resize(MAX_FRAMES_IN_FLIGHT);

	for(int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
	{
		commandBuffers[i] = std::make_shared<VulkanCommandBuffer>(device, commandPool);
		commandBuffers[i]->AllocateCommandBuffer();
	}
}

void VulkanRenderer::DestroyCommands()
{
	commandPool->Destroy();
}

void VulkanRenderer::CreateBuffers()
{
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

	UniformBufferObject ubo{};
	ubo.model = glm::mat4(0);
	ubo.view = glm::mat4(0);
	ubo.proj = glm::mat4(0);

	allocator = std::make_shared<VulkanMemoryAllocator>(instance, device);
	allocator->CreateAllocator();

	void* vertexData;
	vertexData = (void*)&vertices[0];

	vertexBuffer = std::make_shared<VulkanBuffer>(device, allocator);
	vertexBuffer->CreateBuffer(vertexData, sizeof(vertices[0]) * vertices.size(), VERTEX_BUFFER);

	indexBuffer = std::make_shared<VulkanBuffer>(device, allocator);
	indexBuffer->CreateBuffer((void*)&indices[0], sizeof(indices[0]) * indices.size(), INDEX_BUFFER);

	uniformBuffers.resize(MAX_FRAMES_IN_FLIGHT);
	for(int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
	{
		uniformBuffers[i] = std::make_shared<VulkanBuffer>(device, allocator);
		uniformBuffers[i]->CreateBuffer(&ubo, sizeof(ubo), UNIFORM_BUFFER);
	}
}

void VulkanRenderer::DestroyBuffers()
{
	vertexBuffer->Destroy();
	indexBuffer->Destroy();

	for(int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
	{
		uniformBuffers[i]->Destroy();
	}

	allocator->Destroy();
}

void VulkanRenderer::CreateDescriptors(VkDescriptorSetLayout layout)
{
	descriptorPool = std::make_shared<VulkanDescriptorPool>(device);
	descriptorPool->CreateDescriptorPool();

	descriptorSets.resize(MAX_FRAMES_IN_FLIGHT);
	for(int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
	{
		descriptorSets[i] = std::make_shared<VulkanDescriptorSet>(device, descriptorPool);
		descriptorSets[i]->CreateDescriptorSet(sizeof(uniformBuffers[i]), *uniformBuffers[i], layout);
	}
}

void VulkanRenderer::DestroyDescriptors()
{
	descriptorPool->Destroy();
}

void VulkanRenderer::InitImgui()
{
	VkDescriptorPoolSize pool_sizes[] =
	{
		{ VK_DESCRIPTOR_TYPE_SAMPLER, 1000 },
		{ VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000 },
		{ VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1000 },
		{ VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1000 },
		{ VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1000 },
		{ VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1000 },
		{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000 },
		{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1000 },
		{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000 },
		{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000 },
		{ VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1000 }
	};

	VkDescriptorPoolCreateInfo pool_info = {};
	pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	pool_info.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
	pool_info.maxSets = 1000;
	pool_info.poolSizeCount = std::size(pool_sizes);
	pool_info.pPoolSizes = pool_sizes;

	VkResult result = vkCreateDescriptorPool(device->GetLogicalDevice(), &pool_info, nullptr, &imguiPool);
    VK_CHECK(result);

    ImGui_ImplVulkan_InitInfo init_info = {};
	init_info.Instance = instance->GetInstance();
	init_info.PhysicalDevice = device->GetPhysicalDevice();
	init_info.Device = device->GetLogicalDevice();
	init_info.Queue = device->GetGraphicsQueue();
	init_info.DescriptorPool = imguiPool;
	init_info.MinImageCount = 3;
	init_info.ImageCount = 3;
	init_info.MSAASamples = VK_SAMPLE_COUNT_1_BIT;

	IMGUI_CHECKVERSION();
    ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  

    ImGui_ImplGlfw_InitForVulkan(window->GetWindow(), true);

	ImGui_ImplVulkan_Init(&init_info, renderpass->GetRenderPass());

	std::unique_ptr<VulkanCommandBuffer> cmd = std::make_unique<VulkanCommandBuffer>(device, commandPool);
	cmd->AllocateCommandBuffer();

	cmd->Begin();
	ImGui_ImplVulkan_CreateFontsTexture(cmd->GetCommandBuffer());
	cmd->End();

	ImGui_ImplVulkan_DestroyFontUploadObjects();
}

void VulkanRenderer::DrawImgui(VkCommandBuffer cmd)
{
	ImGui_ImplVulkan_NewFrame();
	ImGui_ImplGlfw_NewFrame();

	ImGui::NewFrame();
	ImGui::ShowDemoWindow();
	ImGui::Render();

	ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), cmd);
}

void VulkanRenderer::DestroyImgui()
{
	vkDestroyDescriptorPool(device->GetLogicalDevice(), imguiPool, nullptr);
	ImGui_ImplVulkan_Shutdown();
}

void VulkanRenderer::Init(std::shared_ptr<VulkanDescriptorSetLayout> layout)
{
	CreateSynchronizationStructures();
	CreateCommands();
	CreateBuffers();
	CreateDescriptors(layout->GetDescriptorLayout());

	InitImgui();
}

void VulkanRenderer::UpdateUniforms()
{
	static auto startTime = std::chrono::high_resolution_clock::now();

    auto currentTime = std::chrono::high_resolution_clock::now();
    float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

	ubo.model = glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f) * 1.5f, glm::vec3(0.0f, 0.0f, 1.0f));
	ubo.view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	ubo.proj = glm::perspective(glm::radians(45.0f), swapchain->GetExtent().width / (float) swapchain->GetExtent().width, 0.1f, 10.0f);

	ubo.proj[1][1] *= -1;
	uniformBuffers[currentFrame]->UpdateBuffer(&ubo, sizeof(ubo));
}

void VulkanRenderer::Draw(std::weak_ptr<VulkanGraphicsPipeline> pipelinePTR, std::weak_ptr<VulkanRenderPass> renderpassPTR, std::weak_ptr<VulkanFramebuffers> framebufferPTR)
{
	std::weak_ptr<VulkanCommandBuffer> vulkanCommandBufferPTR = commandBuffers[currentFrame];

	if(vulkanCommandBufferPTR.expired()) { std::cout << "Vulkan Command Buffer weak pointer is invalid" << std::endl; return; }
	if(pipelinePTR.expired()) { std::cout << "Vulkan Graphics Pipeline weak pointer is invalid" << std::endl; return; }
	if(renderpassPTR.expired()) { std::cout << "Vulkan Render Pass weak pointer is invalid" << std::endl; return; }
	if(framebufferPTR.expired()) { std::cout << "Vulkan Framebuffer weak pointer is invalid" << std::endl; return; }

	auto vulkanCommandBuffer = vulkanCommandBufferPTR.lock();
	auto pipeline = pipelinePTR.lock();
	auto renderpass = renderpassPTR.lock();
	auto framebuffer = framebufferPTR.lock();

	VkDevice vkDevice = device->GetLogicalDevice();
	VkCommandBuffer vkCommandBuffer = vulkanCommandBuffer->GetCommandBuffer();

	VkQueue vkGraphicsQueue = device->GetGraphicsQueue();
	VkQueue vkPresentQueue = device->GetPresentQueue();

	VkDescriptorSet vkDescriptorSet = descriptorSets[currentFrame]->GetDescriptorSet();

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

	UpdateUniforms();

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

	vkCmdBindDescriptorSets(vkCommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline->GetPipelineLayout(), 0, 1, &vkDescriptorSet, 0, nullptr);
	vkCmdDrawIndexed(vkCommandBuffer, 6, 1, 0, 0, 0);

	DrawImgui(vkCommandBuffer);

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
	DestroyCommands();
	DestroyBuffers();
	DestroyDescriptors();

	DestroyImgui();
}