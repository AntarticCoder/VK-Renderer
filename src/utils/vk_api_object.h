#pragma once
#include <iostream>

class VulkanAPIObject
{
protected:
    bool initialized = false;
public:
    ~VulkanAPIObject() { if(initialized) { Destroy(); }}

    virtual void Destroy()
    {
        std::cout << "Vulkan API Object Destroyed." << std::endl;
    }
};