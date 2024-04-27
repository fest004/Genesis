#pragma once

#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>
#include "instance.hpp"
#include "validationLayers.hpp"
#include "physicalDevice.hpp"




class Vulkan
{
  public:
    int initVulkan(); //Initializes all relevant Vulkan modules
    void cleanup();


    //Getters and setters
    VkInstance getInstance() { return m_VkInstance; }; 
    const std::vector<const char*> getValidationLayers() { return m_ValidationLayers; }
  private:

    VkInstance m_VkInstance;
    const std::vector<const char*> m_ValidationLayers = { "VK_LAYER_KHRONOS_validation" };
    VkDebugUtilsMessengerEXT m_DebugMessenger;
    VkPhysicalDevice m_PhysicalDevice = VK_NULL_HANDLE;

};
