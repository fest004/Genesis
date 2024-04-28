#include "vulkan.hpp"
#include <vulkan/vulkan_core.h>

int Vulkan::initVulkan()
{
  createInstance(m_VkInstance, m_ValidationLayers);
  setupDebugMessenger(m_VkInstance, &m_DebugMessenger);
  pickPhysicalDevice(m_VkInstance, m_PhysicalDevice);
  createLogicalDevice(m_PhysicalDevice, m_Device, m_GraphicsQueue, m_ValidationLayers);

  return 1;
}


void Vulkan::cleanup()
{

  vkDestroyDevice(m_Device, nullptr);

  if (DEBUG)
  {
    DestroyDebugUtilsMessengerEXT(m_VkInstance, m_DebugMessenger, nullptr);
  }

  vkDestroyInstance(m_VkInstance, nullptr);
}







