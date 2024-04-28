#include "physicalDevice.hpp"

//Pick a GPU we can use for our intentions
int pickPhysicalDevice(VkInstance instance, VkPhysicalDevice& physicalDevice)
{
  uint32_t deviceCount = 0;
  vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

  if (deviceCount < 1)
    GenLogCritical("No physical devices found with Vulkan Support! Exiting...");


  std::vector<VkPhysicalDevice> devices(deviceCount);
  vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());


  //Map orders ratings for us automatically
  std::multimap<int, VkPhysicalDevice> candidates;

  for (const auto& device : devices) 
  {
      int score = getDeviceSuitability(device);
      candidates.insert(std::make_pair(score, device));
  }

  // If best device is suitable we can just return it
  if (candidates.rbegin()->first > 0) 
  {
      physicalDevice = candidates.rbegin()->second;
      return 1;
  } 

  //If we dont find a good device, we exit
  GenLogCritical("No devices fit our requirements!");

  return 0; //Wont ever reach here
}

bool isDeviceSuitable(VkPhysicalDevice device) 
{
    QueueFamilyIndices indices = findQueueFamilies(device);

    return indices.isComplete();
}


//Function that rates how good our device is for our needs
int getDeviceSuitability(VkPhysicalDevice physicalDevice)
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

   if (!isDeviceSuitable(physicalDevice))
    return 0;


  return score;
}


