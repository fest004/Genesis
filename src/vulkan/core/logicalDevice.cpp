#include "logicalDevice.hpp"
#include "queues.hpp"
#include <iostream>
#include <set>
#include <vulkan/vulkan_core.h>


void createLogicalDevice(Gen_Devices& devices, 
                         VkQueue& graphicsQueue, 
                         VkQueue& presentQueue,
                        const std::vector<const char*>& validationLayers, 
                         std::vector<const char*> deviceExtensions, 
                         VkSurfaceKHR& surface
                         )
{
  QueueFamilyIndices indices = findQueueFamilies(devices.physicalDevice, surface);

  std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
  std::set<uint32_t> uniqueQueueFamilies = {indices.graphicsFamily.value(), indices.presentFamily.value()};

  float queuePriority = 1.0f;
  for (uint32_t queueFamily : uniqueQueueFamilies) {
      VkDeviceQueueCreateInfo queueCreateInfo{};
      queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
      queueCreateInfo.queueFamilyIndex = queueFamily;
      queueCreateInfo.queueCount = 1;
      queueCreateInfo.pQueuePriorities = &queuePriority;
      queueCreateInfos.push_back(queueCreateInfo);
  }

  VkPhysicalDeviceFeatures deviceFeatures{};
  deviceFeatures.samplerAnisotropy = VK_TRUE;

  VkDeviceCreateInfo createInfo{};
  createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
  createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
  createInfo.pQueueCreateInfos = queueCreateInfos.data();

  createInfo.pEnabledFeatures = &deviceFeatures;

  createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
  createInfo.ppEnabledExtensionNames = deviceExtensions.data();

  if (DEBUG) 
  {
      createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
      createInfo.ppEnabledLayerNames = validationLayers.data();
  } 
  else 
  {
      createInfo.enabledLayerCount = 0;
  }

  if (vkCreateDevice(devices.physicalDevice, &createInfo, nullptr, &devices.logicalDevice) != VK_SUCCESS) 
  {
    GenLogCritical("Failed to create logical device!");
  }

  vkGetDeviceQueue(devices.logicalDevice, indices.graphicsFamily.value(), 0, &graphicsQueue);
  vkGetDeviceQueue(devices.logicalDevice, indices.presentFamily.value(), 0, &presentQueue);

}
