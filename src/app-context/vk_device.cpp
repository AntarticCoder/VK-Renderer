#include <set>

#include <utils/vk_utils.h>
#include <app-context/vk_device.h>
#include <app-context/vk_window.h>

std::vector<const char*> requiredDeviceExtensions = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

bool VulkanDevice::CheckForDeviceExtensions(VkPhysicalDevice device)
{
    uint32_t extensionCount;
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

    std::vector<VkExtensionProperties> availableExtensions(extensionCount);
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

    std::set<std::string> requiredExtensions(requiredDeviceExtensions.begin(), requiredDeviceExtensions.end());

    for(const auto& extension : availableExtensions) { requiredExtensions.erase(extension.extensionName); }
    return requiredExtensions.empty();
}

VulkanQueueFamilyIndices VulkanDevice::FindQueueFamilies(VkPhysicalDevice device)
{
    VulkanQueueFamilyIndices indices;

    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

    int i = 0;
    for (const auto& queueFamily : queueFamilies)
    {
        if(queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) { indices.graphicsFamily = i; }

        VkBool32 presentSupport = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(device, i, window->GetSurface(), &presentSupport);

        if(presentSupport) { indices.presentFamily = i; }
        if(indices.Complete()) { break; }
        i++;
    }

    return indices;
}

int VulkanDevice::RatePhysicalDevice(VkPhysicalDevice device)
{
    int score = 0;
    VulkanQueueFamilyIndices indices = FindQueueFamilies(device);

    if(!indices.Complete()) { return -1; }
    if(!CheckForDeviceExtensions(device)) { return -1; }
    if(!SwapchainSupported(device)) { return -1; }

    VkPhysicalDeviceProperties deviceProperties;
    VkPhysicalDeviceFeatures deviceFeatures;
    vkGetPhysicalDeviceProperties(device, &deviceProperties);
    vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

    if(deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_CPU) { score += 25; }
    if(deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU) { score += 50; }
    if(deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) { score += 100; }

    std::cout << "Found Qualified Physical Device: " << deviceProperties.deviceName << " with a score of: " << score << std::endl;
    return score;
}

bool VulkanDevice::SwapchainSupported(VkPhysicalDevice device)
{
    VkSurfaceCapabilitiesKHR capabilities;
    VkSurfaceKHR surface = window->GetSurface();

    uint32_t formatCount;
    uint32_t presentModeCount;

    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &capabilities);
    vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);
    vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr);

    if(formatCount == 0) { return false; }
    if(presentModeCount == 0) { return false; }

    return true;
}

void VulkanDevice::SelectPhysicalDevice()
{
    uint32_t deviceCount;
    vkEnumeratePhysicalDevices(appContext->GetInstance(), &deviceCount, nullptr);
    assert(deviceCount > 0);

    std::vector<VkPhysicalDevice> physicalDevices(deviceCount);
    vkEnumeratePhysicalDevices(appContext->GetInstance(), &deviceCount, &physicalDevices[0]);

    int bestRatedDeviceScore = 0;
    VkPhysicalDevice bestRatedDevice = VK_NULL_HANDLE;

    for(auto device: physicalDevices)
    {
        int rating = RatePhysicalDevice(device);
        if(rating > bestRatedDeviceScore)
        { 
            bestRatedDeviceScore = rating;
            bestRatedDevice = device;
        }
    }

    assert(bestRatedDevice != VK_NULL_HANDLE);
    physicalDevice = bestRatedDevice;
    queueFamilyIndices = FindQueueFamilies(physicalDevice);
    return;
}

void VulkanDevice::CreateDevice()
{
    float graphicsQueuePriority = 1.0f;
    float presentQueuePriority = 1.0f;

    VkDeviceQueueCreateInfo graphicsQueueCreateInfo{};
    graphicsQueueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    graphicsQueueCreateInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();
    graphicsQueueCreateInfo.queueCount = 1;
    graphicsQueueCreateInfo.pQueuePriorities = &graphicsQueuePriority;

    VkDeviceQueueCreateInfo presentQueueCreateInfo{};
    presentQueueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    presentQueueCreateInfo.queueFamilyIndex = queueFamilyIndices.presentFamily.value();
    presentQueueCreateInfo.queueCount = 1;
    presentQueueCreateInfo.pQueuePriorities = &presentQueuePriority;

    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos = { graphicsQueueCreateInfo, presentQueueCreateInfo};

    #ifdef __APPLE__ 
        requiredDeviceExtensions.push_back("VK_KHR_portability_subset");
    #endif

    VkPhysicalDeviceFeatures deviceFeatures{};

    VkDeviceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createInfo.queueCreateInfoCount = queueCreateInfos.size();
    createInfo.pQueueCreateInfos = &queueCreateInfos[0];
    createInfo.pEnabledFeatures = &deviceFeatures;

    createInfo.enabledExtensionCount = requiredDeviceExtensions.size();
    createInfo.ppEnabledExtensionNames = &requiredDeviceExtensions[0];
    createInfo.enabledLayerCount = 0;

    VkResult result = vkCreateDevice(physicalDevice, &createInfo, nullptr, &logicalDevice);
    VK_CHECK(result);

    vkGetDeviceQueue(logicalDevice, queueFamilyIndices.graphicsFamily.value(), 0, &graphicsQueue);
    vkGetDeviceQueue(logicalDevice, queueFamilyIndices.presentFamily.value(), 0, &presentQueue);

    intialized = true;
    return;
}

void VulkanDevice::Destroy()
{
    assert(intialized);
    vkDestroyDevice(logicalDevice, nullptr);
    
    intialized = false;
}