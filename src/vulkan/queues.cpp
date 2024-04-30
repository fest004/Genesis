#include "queues.hpp"
#include <vulkan/vulkan_core.h>


QueueFamilyIndices findQueueFamilies(VkPhysicalDevice& device, VkSurfaceKHR& surface)
{
  QueueFamilyIndices indices;

  //Same old pattern of finding stuff
  uint32_t queueFamilyCount = 0;
  vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

  std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
  vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

  int i = 0;
  for (const auto& queueFamily : queueFamilies)
  {
    if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
      indices.graphicsFamily = i;

    VkBool32 presentSupport = false;
    vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);

    if (presentSupport == 1)
      indices.presentFamily = i;


    if (indices.isComplete())
      break;

    i++;
  }

  return indices;
}
