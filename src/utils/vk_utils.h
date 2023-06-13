#pragma once
#include <iostream>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

static void VK_CHECK(VkResult result, int line = __builtin_LINE(), std::string function = __builtin_FUNCTION(), std::string file = __builtin_FILE()) 
{                                                                                
    if(result != VK_SUCCESS) { std::cout << "Detected Vulkan error: " << result << " at: " << file << ':' << line << " in function: " << function << std::endl; }                                                           
}

static bool VK_CHECK_SWAPCHAIN_OUT_OF_DATE(VkResult result, int line = __builtin_LINE(), std::string function = __builtin_FUNCTION(), std::string file = __builtin_FILE()) 
{                                                                                
    if(result != VK_SUCCESS) { std::cout << "Detected Vulkan error: " << result << " at: " << file << ':' << line << " in function: " << function << std::endl; }              
    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR) { return true; }
    return false;                                                       
}