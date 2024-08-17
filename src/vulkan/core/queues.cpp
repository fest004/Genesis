#include "queues.hpp"
#include <vulkan/vulkan_core.h>


QueueFamilyIndices find_queue_families(VkPhysicalDevice& device, VkSurfaceKHR& surface)
{
  QueueFamilyIndices indices;

  //Same old pattern of finding stuff
  uint32_t queue_family_count = 0;
  vkGetPhysicalDeviceQueueFamilyProperties(device, &queue_family_count, nullptr);

  std::vector<VkQueueFamilyProperties> queue_families(queue_family_count);
  vkGetPhysicalDeviceQueueFamilyProperties(device, &queue_family_count, queue_families.data());

  int i = 0;
  for (const auto& queue_family : queue_families)
  {
    if (queue_family.queueFlags & VK_QUEUE_GRAPHICS_BIT)
      indices.graphics_family = i;

    VkBool32 present_support = false;
    vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &present_support);


    if (present_support == 1)
      indices.present_family = i;


    if (indices.is_complete())
      break;

    i++;
  }

  return indices;
}
