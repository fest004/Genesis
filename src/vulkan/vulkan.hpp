#pragma once

#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>
#include "instance.hpp"




class Vulkan
{
  public:
    int initVulkan(); //Initializes all relevant Vulkan modules
    void cleanup();

    //Getters and setters
    VkInstance getInstance() { return m_VkInstance; }; 

  private:

    VkInstance m_VkInstance;

};
