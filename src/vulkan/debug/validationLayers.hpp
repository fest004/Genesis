#include <cstdint>
#include <vector>
#include <vulkan/vulkan_core.h>
#include "../../core.h"

/*
Validation Layers notice: 
- Argument validity
- Tracks object creation and deletion (resource tracking)
- Thread safety
- Tracking Vulkan calls
- 
*/


bool check_validation_layers(const std::vector<const char*>& validation_layers);

//DebugMessenger stuff
void setup_debug_messenger(VkInstance instance, VkDebugUtilsMessengerEXT* p_debug_messenger);


VkResult create_debug_utils_messengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* p_create_info, const VkAllocationCallbacks* p_allocator, VkDebugUtilsMessengerEXT* p_debug_messenger);

void destroy_debug_utils_messengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debug_messenger, const VkAllocationCallbacks* p_allocator);
void populate_debug_messenger_create_info(VkDebugUtilsMessengerCreateInfoEXT& create_info);

static VKAPI_ATTR VkBool32 VKAPI_CALL debug_callback
(
  VkDebugUtilsMessageSeverityFlagBitsEXT message_severity, 
  VkDebugUtilsMessageTypeFlagsEXT message_type, 
  const VkDebugUtilsMessengerCallbackDataEXT* p_callback_data, 
  void* p_user_data
);
