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


bool checkValidationLayers(const std::vector<const char*>& validationLayers);

//DebugMessenger stuff
void setupDebugMessenger(VkInstance instance, VkDebugUtilsMessengerEXT* pDebugMessenger);
VkResult CreateDebugUtilsMessengerEXT(VkInstance& instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger);
void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator);
void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback
(
  VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, 
  VkDebugUtilsMessageTypeFlagsEXT messageType, 
  const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, 
  void* pUserData
);
