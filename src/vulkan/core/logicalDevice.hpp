#include "../../core.h"
#include "queues.hpp"


void createLogicalDevice(Gen_Devices& devices, 
                         VkQueue& graphicsQueue, 
                         VkQueue& presentQueue,
                        const std::vector<const char*>& validationLayers, 
                         std::vector<const char*> deviceExtensions, 
                         VkSurfaceKHR& surface
                         );

