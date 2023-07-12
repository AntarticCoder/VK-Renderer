#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <cassert>

#include <utils/vk_utils.h>
#include <api/app-context/vk_instance.h>

std::vector<const char*> VulkanInstance::GetInstanceExtensions()
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

void VulkanInstance::CreateInstance()
{
    assert(!initialized);

    VkApplicationInfo applicationInfo = {};
    applicationInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    applicationInfo.pApplicationName = "VK Renderer";
    applicationInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    applicationInfo.pEngineName = "No Engine";
    applicationInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    applicationInfo.apiVersion = VK_API_VERSION_1_2;

    std::vector<const char*> instanceExtensions = GetInstanceExtensions();
    const std::vector<const char*> validationLayers = { "VK_LAYER_KHRONOS_validation" };

    VkInstanceCreateInfo instanceCreateInfo = {};
    instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instanceCreateInfo.pApplicationInfo = &applicationInfo; 

    #ifdef __APPLE__
        instanceCreateInfo.flags |= VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;
    #endif

    instanceCreateInfo.enabledExtensionCount = instanceExtensions.size();
    instanceCreateInfo.ppEnabledExtensionNames = &instanceExtensions[0];

    instanceCreateInfo.enabledLayerCount = validationLayers.size();
    instanceCreateInfo.ppEnabledLayerNames = &validationLayers[0];

    VkResult result = vkCreateInstance(&instanceCreateInfo, nullptr, &instance);
    VK_CHECK(result);

    initialized = true;
    return;
}

void VulkanInstance::Destroy()
{
    assert(initialized);
    vkDestroyInstance(instance, nullptr);

    initialized = false;
}