#include "instance.hpp"
#include <iostream>
#include <vulkan/vulkan_core.h>
#include <vector>
#include <algorithm>

//First VK interfacing I do, will be overcommented for sake of learning as I go

int createInstance(VkInstance& instance)
{
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
  uint32_t glfwExtensionCount = 0; //Nr of extensions
  vkEnumerateInstanceExtensionProperties(nullptr, &glfwExtensionCount, nullptr); //Just the nr of extensions supported
  std::vector<VkExtensionProperties> extensions(glfwExtensionCount);

  vkEnumerateInstanceExtensionProperties(nullptr, &glfwExtensionCount, extensions.data());

  const char** glfwExtensions; //Their names
  glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);


  //Self made function to check wether or not the glfw extensions needed are supported
  checkExternalExtensions(glfwExtensions, glfwExtensionCount, extensions);

  createInstanceInfo.enabledExtensionCount = glfwExtensionCount;
  createInstanceInfo.ppEnabledExtensionNames = glfwExtensions;
  createInstanceInfo.enabledLayerCount = 0;


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

