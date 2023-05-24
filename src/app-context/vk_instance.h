#pragma once
#include <vector>
#include <string>
#include <optional>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

class VulkanInstance
{
private:
    VkInstance instance;
    bool intialized = false;

    std::vector<const char*> GetInstanceExtensions();
public:
    void CreateInstance();
    void Destroy();

    VkInstance GetInstance() { return instance; }
};