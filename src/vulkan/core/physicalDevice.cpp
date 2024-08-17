#include "physicalDevice.hpp"
#include "queues.hpp"
#include "../vulkan.hpp"
#include <iostream>
#include <limits>
#include <set>
#include <vulkan/vulkan_core.h>


//Pick a GPU we can use for our intentions
int pick_physical_device(VkInstance& instance, Gen_Devices& devices, VkSurfaceKHR& surface, std::vector<const char*> device_extensions)
{
  uint32_t device_count = 0;
  vkEnumeratePhysicalDevices(instance, &device_count, nullptr);

  if (device_count < 1)
    GenLogCritical("No physical devices found with Vulkan Support! Exiting...");

  std::vector<VkPhysicalDevice> physical_devices(device_count);
  vkEnumeratePhysicalDevices(instance, &device_count, physical_devices.data());

  GenLogTrace("device_count: {}", device_count);

  for (const auto& phys_device : physical_devices) 
  {
    if (is_device_suitable(phys_device, surface, device_extensions)) 
    {
      devices.physical_device = phys_device;
      return 1;
    }
  }

  // If we don't find a suitable device, we exit
  GenLogCritical("No devices fit our requirements!");
  return 0;
}

bool is_device_suitable(VkPhysicalDevice phys_device, VkSurfaceKHR& surface, std::vector<const char*> device_extensions) 
{

    std::cout << &phys_device << std::endl;

    QueueFamilyIndices indices = find_queue_families(phys_device, surface);

    bool extensions_supported = check_device_extension_support(phys_device, device_extensions);

    bool swap_chain_adequate = false;
    if (extensions_supported)
    {
      SwapChainSupportDetails swap_chain_support = query_swapchain_support(phys_device, surface);
      swap_chain_adequate = !swap_chain_support.formats.empty() && !swap_chain_support.presentModes.empty();
    }
    
    VkPhysicalDeviceFeatures supported_features;
    vkGetPhysicalDeviceFeatures(phys_device, &supported_features);

    return indices.is_complete() && extensions_supported && swap_chain_adequate && supported_features.samplerAnisotropy;
}

bool check_device_extension_support(VkPhysicalDevice& device, std::vector<const char*> device_extensions)
{
  uint32_t extension_count;
  vkEnumerateDeviceExtensionProperties(device, nullptr, &extension_count, nullptr);

  std::vector<VkExtensionProperties> available_extensions(extension_count);
  vkEnumerateDeviceExtensionProperties(device, nullptr, &extension_count, available_extensions.data());

  std::set<std::string> required_extensions(device_extensions.begin(), device_extensions.end());

  for (const auto& extension : available_extensions)
  {
    required_extensions.erase(extension.extensionName);
  }

  return required_extensions.empty();
}


//Function that rates how good our device is for our needs
int get_device_suitability(VkPhysicalDevice& physical_device, VkSurfaceKHR& surface, std::vector<const char*> device_extensions)
{
  VkPhysicalDeviceProperties device_properties;
  vkGetPhysicalDeviceProperties(physical_device, &device_properties);

  VkPhysicalDeviceFeatures device_features;
  vkGetPhysicalDeviceFeatures(physical_device, &device_features);


  int score = 0;

  if (device_properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
    score += 1000;

  score += device_properties.limits.maxImageDimension2D;

  if (!device_features.geometryShader)
    return 0;

   if (!is_device_suitable(physical_device, surface, device_extensions))
    return 0;


  return score;
}

