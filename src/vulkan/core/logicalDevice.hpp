#include "../../core.h"
#include "queues.hpp"


void create_logical_device(Gen_Devices& devices, 
                         VkQueue& graphics_queue, 
                         VkQueue& present_queue,
                        const std::vector<const char*>& validation_layers, 
                         std::vector<const char*> device_extensions, 
                         VkSurfaceKHR& surface
                         );

