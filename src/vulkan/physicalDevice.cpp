#include "physicalDevice.hpp"
#include "queues.hpp"
#include "vulkan.hpp"
#include <iostream>
#include <limits>
#include <set>
#include <vulkan/vulkan_core.h>
#include "swapchain.hpp"

//Pick a GPU we can use for our intentions
int pickPhysicalDevice(VkInstance& instance, VkDevice& device, VkSurfaceKHR& surface, VkPhysicalDevice& physicalDevice, std::vector<const char*> deviceExtensions)
{
  uint32_t deviceCount = 0;
  vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

  if (deviceCount < 1)
    GenLogCritical("No physical devices found with Vulkan Support! Exiting...");

  std::vector<VkPhysicalDevice> physicalDevices(deviceCount);
  vkEnumeratePhysicalDevices(instance, &deviceCount, physicalDevices.data());

  for (const auto& physDevice : physicalDevices) 
  {
    if (isDeviceSuitable(physDevice, surface, deviceExtensions)) 
    {
      physicalDevice = physDevice;
      return 1;
    }
  }

  // If we don't find a suitable device, we exit
  GenLogCritical("No devices fit our requirements!");
  return 0;
}

bool isDeviceSuitable(VkPhysicalDevice device, VkSurfaceKHR& surface, std::vector<const char*> deviceExtensions) 
{
    QueueFamilyIndices indices = findQueueFamilies(device, surface);

    bool extensionsSupported = checkDeviceExtensionSupport(device, deviceExtensions);

    bool swapChainAdequate = false;
    if (extensionsSupported)
    {
      SwapChainSupportDetails swapChainSupport = querySwapchainSupport(device, surface);
      swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
    }
    
    VkPhysicalDeviceFeatures supportedFeatures;
    vkGetPhysicalDeviceFeatures(device, &supportedFeatures);

    return indices.isComplete() && extensionsSupported && swapChainAdequate && supportedFeatures.samplerAnisotropy;
}

bool checkDeviceExtensionSupport(VkPhysicalDevice& device, std::vector<const char*> deviceExtensions)
{
  uint32_t extensionCount;
  vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

  std::vector<VkExtensionProperties> availableExtensions(extensionCount);
  vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

  std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

  for (const auto& extension : availableExtensions)
  {
    requiredExtensions.erase(extension.extensionName);
  }

  return requiredExtensions.empty();
}


//Function that rates how good our device is for our needs
int getDeviceSuitability(VkPhysicalDevice& physicalDevice, VkSurfaceKHR& surface, std::vector<const char*> deviceExtensions)
{
  VkPhysicalDeviceProperties deviceProperties;
  vkGetPhysicalDeviceProperties(physicalDevice, &deviceProperties);

  VkPhysicalDeviceFeatures deviceFeatures;
  vkGetPhysicalDeviceFeatures(physicalDevice, &deviceFeatures);


  int score = 0;

  if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
    score += 1000;

  score += deviceProperties.limits.maxImageDimension2D;

  if (!deviceFeatures.geometryShader)
    return 0;

   if (!isDeviceSuitable(physicalDevice, surface, deviceExtensions))
    return 0;


  return score;
}

