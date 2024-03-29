#include <cassert>
#include <algorithm>
#include <limits>

#include <utils/vk_utils.h>
#include <api/app-context/vk_swapchain.h>

VkSurfaceFormatKHR VulkanSwapchain::ChooseSwapchainSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats)
{
    for (const auto& availableFormat : availableFormats)
    {
        if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
        {
            return availableFormat;
        }
    }
    return availableFormats[0];
}

VkPresentModeKHR VulkanSwapchain::ChooseSwapchainPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes)
{
    for (const auto& availablePresentMode : availablePresentModes)
    {
        if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) { return availablePresentMode; }
    }
    return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D VulkanSwapchain::ChooseSwapchainExtent(const VkSurfaceCapabilitiesKHR& capabilities)
{
    if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
    {
        return capabilities.currentExtent;
    }
    else
    {
        int width, height;
        glfwGetFramebufferSize(window->GetWindow(), &width, &height);

        VkExtent2D actualExtent =
        {
            static_cast<uint32_t>(width),
            static_cast<uint32_t>(height)
        };

        actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
        actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);
        return actualExtent;
    }
    return VkExtent2D();
}

VulkanSwapchainSupportDetails VulkanSwapchain::GetSwapchainSupportDetails()
{
    VulkanSwapchainSupportDetails details;

    VkPhysicalDevice physicalDevice = device->GetPhysicalDevice();
    VkSurfaceKHR surface = window->GetSurface();

    uint32_t formatCount;
    uint32_t presentModeCount;

    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface, &details.capabilities);
    vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatCount, nullptr);
    vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentModeCount, nullptr);

    if (formatCount != 0)
    {
        details.formats.resize(formatCount);
        vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatCount, details.formats.data());
    }

    if (presentModeCount != 0)
    {
        details.presentModes.resize(presentModeCount);
        vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentModeCount, details.presentModes.data());
    }
    return details;
}

void VulkanSwapchain::CreateImageViews()
{
    swapchainImageViews.resize(swapchainImages.size());

    for (size_t i = 0; i < swapchainImages.size(); i++)
    {
        VkImageViewCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        createInfo.image = swapchainImages[i];

        createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        createInfo.format = swapchainImageFormat;

        createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

        createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        createInfo.subresourceRange.baseMipLevel = 0;
        createInfo.subresourceRange.levelCount = 1;
        createInfo.subresourceRange.baseArrayLayer = 0;
        createInfo.subresourceRange.layerCount = 1;

        VkResult result = vkCreateImageView(device->GetLogicalDevice(), &createInfo, nullptr, &swapchainImageViews[i]);
        VK_CHECK(result);
    }
    return;
}

void VulkanSwapchain::CreateSwapchain()
{
    assert(!initialized);
    
    VulkanSwapchainSupportDetails swapChainSupport = GetSwapchainSupportDetails();
    VulkanQueueFamilyIndices indices = device->FindQueueFamilies(device->GetPhysicalDevice());

    VkDevice vkDevice = device->GetLogicalDevice();
    VkSurfaceKHR vkSurface = window->GetSurface();

    VkSurfaceFormatKHR surfaceFormat = ChooseSwapchainSurfaceFormat(swapChainSupport.formats);
    VkPresentModeKHR presentMode = ChooseSwapchainPresentMode(swapChainSupport.presentModes);
    VkExtent2D extent = ChooseSwapchainExtent(swapChainSupport.capabilities);

    uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
    if(swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount)
    {
        imageCount = swapChainSupport.capabilities.maxImageCount;
    }

    VkSwapchainCreateInfoKHR createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface = vkSurface;
    createInfo.minImageCount = imageCount;
    createInfo.imageFormat = surfaceFormat.format;
    createInfo.imageColorSpace = surfaceFormat.colorSpace;
    createInfo.imageExtent = extent;
    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    uint32_t queueFamilyIndices[] = {indices.graphicsFamily.value(), indices.presentFamily.value()};

    if (indices.graphicsFamily != indices.presentFamily)
    {
        createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        createInfo.queueFamilyIndexCount = 2;
        createInfo.pQueueFamilyIndices = queueFamilyIndices;
    }else
    {
        createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        createInfo.queueFamilyIndexCount = 0;
        createInfo.pQueueFamilyIndices = nullptr;
    }

    createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    createInfo.presentMode = presentMode;
    createInfo.clipped = VK_TRUE;
    createInfo.oldSwapchain = VK_NULL_HANDLE;

    swapchainImageFormat = surfaceFormat.format;
    swapchainExtent = extent;

    VkResult result = vkCreateSwapchainKHR(vkDevice, &createInfo, nullptr, &swapchain);
    VK_CHECK(result);

    vkGetSwapchainImagesKHR(vkDevice, swapchain, &imageCount, nullptr);
    swapchainImages.resize(imageCount);
    vkGetSwapchainImagesKHR(vkDevice, swapchain, &imageCount, swapchainImages.data());

    CreateImageViews();

    initialized = true;
    return;
}

void VulkanSwapchain::RecreateSwapchain()
{
    vkDeviceWaitIdle(device->GetLogicalDevice());
    
    Destroy();
    CreateSwapchain();
}

void VulkanSwapchain::Destroy()
{
    assert(initialized);
    VkDevice vkDevice = device->GetLogicalDevice();

    for(auto imageView : swapchainImageViews)
    {
        vkDestroyImageView(vkDevice, imageView, nullptr);
    }
    vkDestroySwapchainKHR(vkDevice, swapchain, nullptr);
    
    initialized = false;
}