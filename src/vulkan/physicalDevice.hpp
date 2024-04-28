#include "../core.h"
#include <cstdint>
#include <map>
#include "queues.hpp"





bool isDeviceSuitable(VkPhysicalDevice device);
int pickPhysicalDevice(VkInstance instance, VkPhysicalDevice& physicalDevice);
bool isDeviceSuitable(VkPhysicalDevice physicalDevice);
int getDeviceSuitability(VkPhysicalDevice physicalDevice);
