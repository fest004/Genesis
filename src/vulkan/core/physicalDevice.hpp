#pragma once

#include "../../core.h"
#include <cstdint>
#include <map>
#include "queues.hpp"





int pickPhysicalDevice(VkInstance& instance, Gen_Devices& devices, VkSurfaceKHR& surface, std::vector<const char*> deviceExtensions);
bool isDeviceSuitable(VkPhysicalDevice& device, VkSurfaceKHR& surface, std::vector<const char*> deviceExtensions);
bool checkDeviceExtensionSupport(VkPhysicalDevice& device, std::vector<const char*> deviceExtensions);
int getDeviceSuitability(VkPhysicalDevice& physicalDevice, VkSurfaceKHR& surface, std::vector<const char*> deviceExtensions);

//

