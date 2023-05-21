#include <iostream>
#include <vector>
#include <map>

#include <app-context/vk_application_context.h>

void VK_CHECK(VkResult result) 
{                                                                                
    if(result != VK_SUCCESS) { std::cout << "Detected Vulkan error: " << result << std::endl; abort(); }                                                           
}

void VulkanApplicationContext::CreateWindow(int width, int height, std::string title)
{
    windowWidth = width;
    windowHeight = height;
    windowTitle = title;

    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwCreateWindow(windowWidth, windowHeight, title.c_str(), NULL, NULL);
}

std::vector<const char*> VulkanApplicationContext::GetInstanceExtensions()
{
    uint32_t glfwExtensionCount;
    const char** glfwExtensions;    
    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    std::vector<const char*> instanceExtensions;
    for(uint32_t i = 0; i < glfwExtensionCount; i++)
    { 
        instanceExtensions.emplace_back(glfwExtensions[i]);
    }

    #ifdef __APPLE__ 
        instanceExtensions.emplace_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME); 
    #endif

    instanceExtensions.emplace_back(VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME);
    return instanceExtensions;
}

void VulkanApplicationContext::CreateInstance()
{
    VkApplicationInfo applicationInfo = {};
    applicationInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    applicationInfo.pApplicationName = "VK Renderer";
    applicationInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    applicationInfo.pEngineName = "No Engine";
    applicationInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    applicationInfo.apiVersion = VK_API_VERSION_1_0;

    std::vector<const char*> instanceExtensions = GetInstanceExtensions();

    VkInstanceCreateInfo instanceCreateInfo = {};
    instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instanceCreateInfo.pApplicationInfo = &applicationInfo; 
    instanceCreateInfo.flags |= VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;

    instanceCreateInfo.enabledExtensionCount = instanceExtensions.size();
    instanceCreateInfo.ppEnabledExtensionNames = &instanceExtensions[0];

    instanceCreateInfo.enabledLayerCount = 0;

    VkResult result = vkCreateInstance(&instanceCreateInfo, nullptr, &instance);
    VK_CHECK(result);
    return;
}

int VulkanApplicationContext::RatePhysicalDevice(VkPhysicalDevice device)
{
    int score = 0;

    VkPhysicalDeviceProperties deviceProperties;
    VkPhysicalDeviceFeatures deviceFeatures;
    vkGetPhysicalDeviceProperties(device, &deviceProperties);
    vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

    if(deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_CPU) { score += 25; }
    if(deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU) { score += 50; }
    if(deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) { score += 100; }

    VulkanQueueFamilyIndices indices = FindQueueFamilies(device);
    if(!indices.Complete()) { score = -1; }
    
    std::cout << "Found Physical Device: " << deviceProperties.deviceName << " with a score of: " << score << std::endl;
    return score;
}

void VulkanApplicationContext::SelectPhysicalDevice()
{
    uint32_t deviceCount;
    vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);
    assert(deviceCount > 0);

    std::vector<VkPhysicalDevice> physicalDevices(deviceCount);
    vkEnumeratePhysicalDevices(instance, &deviceCount, &physicalDevices[0]);

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
    return;
}

VulkanQueueFamilyIndices VulkanApplicationContext::FindQueueFamilies(VkPhysicalDevice device)
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
        if(queueFamily.queueFlags & VK_QUEUE_COMPUTE_BIT) { indices.computeFamily = i; }
        i++;
    }

    return indices;
}

void VulkanApplicationContext::CreateDevice()
{
    VulkanQueueFamilyIndices indices = FindQueueFamilies(physicalDevice);

    float graphicsQueuePriority = 1.0f;
    float computeQueuePriority = 0.5f;

    VkDeviceQueueCreateInfo graphicsQueueCreateInfo{};
    graphicsQueueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    graphicsQueueCreateInfo.queueFamilyIndex = indices.graphicsFamily.value();
    graphicsQueueCreateInfo.queueCount = 1;
    graphicsQueueCreateInfo.pQueuePriorities = &graphicsQueuePriority;

    VkDeviceQueueCreateInfo computeQueueCreateInfo{};
    computeQueueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    computeQueueCreateInfo.queueFamilyIndex = indices.computeFamily.value();
    computeQueueCreateInfo.queueCount = 1;
    computeQueueCreateInfo.pQueuePriorities = &computeQueuePriority;

    std::vector<const VkDeviceQueueCreateInfo> queues = { graphicsQueueCreateInfo, computeQueueCreateInfo };
    std::vector<const char*> deviceExtensions = {};

    #ifdef __APPLE__ 
        deviceExtensions.push_back("VK_KHR_portability_subset");
    #endif

    VkPhysicalDeviceFeatures deviceFeatures{};

    VkDeviceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createInfo.pQueueCreateInfos = &graphicsQueueCreateInfo;
    createInfo.queueCreateInfoCount = 1;
    createInfo.pEnabledFeatures = &deviceFeatures;

    createInfo.enabledExtensionCount = deviceExtensions.size();
    createInfo.ppEnabledExtensionNames = &deviceExtensions[0];
    createInfo.enabledLayerCount = 0;

    VkResult result = vkCreateDevice(physicalDevice, &createInfo, nullptr, &logicalDevice);
    VK_CHECK(result);

    vkGetDeviceQueue(logicalDevice, indices.graphicsFamily.value(), 0, &graphicsQueue);
    return;
}

void VulkanApplicationContext::Initialize(VulkanApplicationInfo info)
{
    CreateWindow(info.width, info.height, info.title);
    CreateInstance();
    SelectPhysicalDevice();
    CreateDevice();
}