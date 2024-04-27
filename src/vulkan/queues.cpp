#include "queues.hpp"


QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device)
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

    if (indices.isComplete())
      break;

    i++;
  }

  return indices;
}
