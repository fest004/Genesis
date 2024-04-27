#include "vulkan.hpp"
#include <vulkan/vulkan_core.h>

int Vulkan::initVulkan()
{
  createInstance(m_VkInstance, m_ValidationLayers);
  setupDebugMessenger(m_VkInstance, &m_DebugMessenger);
  pickPhysicalDevice(m_VkInstance);

  return 1;
}


void Vulkan::cleanup()
{
  //Do stuff
if (DEBUG)
{
  DestroyDebugUtilsMessengerEXT(m_VkInstance, m_DebugMessenger, nullptr);
}

  vkDestroyInstance(m_VkInstance, nullptr);
}







