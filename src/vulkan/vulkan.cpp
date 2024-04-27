#include "vulkan.hpp"
#include <vulkan/vulkan_core.h>

int Vulkan::initVulkan()
{
  createInstance(m_VkInstance);

  return 1;
}


void Vulkan::cleanup()
{
  //Do stuff
  vkDestroyInstance(m_VkInstance, nullptr);
}

