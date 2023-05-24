#pragma once
#include <iostream>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

static void VK_CHECK(VkResult result) 
{                                                                                
    if(result != VK_SUCCESS) { std::cout << "Detected Vulkan error: " << result << std::endl; }                                                           
}