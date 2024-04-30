#pragma once

#include "../core.h"
#include <cstdint>
#include <map>
#include "queues.hpp"





int pickPhysicalDevice(VkInstance& instance, VkDevice& device, VkSurfaceKHR& surface, VkPhysicalDevice& physicalDevice, std::vector<const char*> deviceExtensions);
bool isDeviceSuitable(VkPhysicalDevice device, VkSurfaceKHR& surface, std::vector<const char*> deviceExtensions);
bool checkDeviceExtensionSupport(VkPhysicalDevice& device, std::vector<const char*> deviceExtensions);
int getDeviceSuitability(VkPhysicalDevice& physicalDevice, VkSurfaceKHR& surface, std::vector<const char*> deviceExtensions);

//

