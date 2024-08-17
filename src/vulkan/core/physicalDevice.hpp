#pragma once

#include "../../core.h"
#include <cstdint>
#include <map>
#include "queues.hpp"





int pick_physical_device(VkInstance& instance, Gen_Devices& devices, VkSurfaceKHR& surface, std::vector<const char*> device_extensions);
bool is_device_suitable(VkPhysicalDevice device, VkSurfaceKHR& surface, std::vector<const char*> device_extensions);
bool check_device_extension_support(VkPhysicalDevice& device, std::vector<const char*> device_extensions);
int get_device_suitability(VkPhysicalDevice& physical_device, VkSurfaceKHR& surface, std::vector<const char*> device_extensions);

//

