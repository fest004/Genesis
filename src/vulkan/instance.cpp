#include "instance.hpp"
#include <iostream>
#include <vulkan/vulkan_core.h>
#include <vector>
#include <algorithm>
#include "validationLayers.hpp"
//First VK interfacing I do, will be overcommented for sake of learning as I go

int createInstance(VkInstance& instance, const std::vector<const char*>& validationLayers)
{
  if (DEBUG && !checkValidationLayers(validationLayers))
  {
    GenLogCritical("Requested validation layers not supported! Error in file: validationLayers.cpp");
    return 0;
  }


  //Structs are used to pass data instead of eternally long list of function paramaters
  VkApplicationInfo appInfo{}; //appInfo struct, use {} to initialize unset fields to 0 values
  appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO; //We choose the type of appInfo to match the one we want
  appInfo.pApplicationName = "Genesis";
  appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
  appInfo.pEngineName = "YOLO";
  appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
  appInfo.apiVersion = VK_API_VERSION_1_0;

  //Then we create another struct to hold argument info of the instance itself
  VkInstanceCreateInfo createInstanceInfo{};
  createInstanceInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  createInstanceInfo.pApplicationInfo = &appInfo;

  /*
   
     Vulkan is "platform agnostic", so it needs to be given a way to interact with the
     window system, which for us is done through GLFW. These next two arguments are used 
     to tell Vulkan about our glfw extensions used to render to a window

  */
  auto extensions = getRequiredExtensions();

  createInstanceInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
  createInstanceInfo.ppEnabledExtensionNames = extensions.data();



  //Enable validation layers if DEBUG is on
  VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
  if (DEBUG)
  {
    createInstanceInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
    createInstanceInfo.ppEnabledLayerNames = validationLayers.data();

    populateDebugMessengerCreateInfo(debugCreateInfo);
    createInstanceInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*) &debugCreateInfo;
  }
  else 
  {
    createInstanceInfo.enabledLayerCount = 0;
    createInstanceInfo.pNext = nullptr;
  }

  

  //Now we have all the info we need to actually create the instance with the result stored at &instance which is m_Instance from the Vulkan class
  if (vkCreateInstance(&createInstanceInfo, nullptr, &instance) != VK_SUCCESS)
  {
    GenLogCritical("Failed to create instance. Instance.cpp");
    return 0;
  }

  return 1;
}


//Checks if all the glfw extensions are supported by current setup
int checkExternalExtensions(const char** externalExtensions, int arrSize, std::vector<VkExtensionProperties>& vkExtensions)
{
    int success = 1;
    for (int i = 0; i < arrSize; i++)
    {
        std::string externalExtension(externalExtensions[i]);

        bool extensionFound = false;
        for (const auto& vkExtension : vkExtensions)
        {
            if (externalExtension == vkExtension.extensionName)
            {
                extensionFound = true;
                continue;
            }
        }

        if (!extensionFound)
        {
            success = 0;
            GenLogCritical("The following extension is not supported by your Vulkan Extensions: {}", externalExtension);
        }
        
    }
    return success;
}

std::vector<const char*> getRequiredExtensions()
{
  uint32_t glfwExtensionCount = 0;
  const char** glfwExtensions;
  glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

  std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

  if (DEBUG)
  {
    extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
  }

  return extensions;
}





