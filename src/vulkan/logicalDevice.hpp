#include "../core.h"
#include "queues.hpp"


void createLogicalDevice(VkPhysicalDevice& physicalDevice, VkDevice& device, 
    VkQueue& graphicsQueue, const std::vector<const char*>& validationLayers, 
    std::vector<const char*> deviceExtensions, VkSurfaceKHR& surface, VkQueue& presentQueue
    );

