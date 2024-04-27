#include "../core.h"
#include <cstdint>
#include <map>
#include "queues.hpp"





bool isDeviceSuitable(VkPhysicalDevice device);
VkPhysicalDevice pickPhysicalDevice(VkInstance instance);
bool isDeviceSuitable(VkPhysicalDevice physicalDevice);
int getDeviceSuitability(VkPhysicalDevice physicalDevice);
