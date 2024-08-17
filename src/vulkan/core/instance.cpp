#include "instance.hpp"
#include <iostream>
#include <vulkan/vulkan_core.h>
#include <vector>
#include <algorithm>
#include "../debug/validationLayers.hpp"
//First VK interfacing I do, will be overcommented for sake of learning as I go


int create_instance(VkInstance& instance, const std::vector<const char*>& validation_layers)
{
    if (DEBUG && !check_validation_layers(validation_layers))
    {
        GenLogCritical("Requested validation layers not supported! Error in file: validation_layers.cpp");
        return 0;
    }

    // Structs are used to pass data instead of eternally long list of function parameters
    VkApplicationInfo app_info{}; // app_info struct, use {} to initialize unset fields to 0 values
    app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO; // We choose the type of app_info to match the one we want
    app_info.pApplicationName = "Genesis";
    app_info.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    app_info.pEngineName = "YOLO";
    app_info.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    app_info.apiVersion = VK_API_VERSION_1_0;

    // Create another struct to hold argument info of the instance itself
    VkInstanceCreateInfo create_instance_info{};
    create_instance_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    create_instance_info.pApplicationInfo = &app_info;

    /*
        Vulkan is "platform agnostic", so it needs to be given a way to interact with the
        window system, which for us is done through GLFW. These next two arguments are used 
        to tell Vulkan about our GLFW extensions used to render to a window
    */
    auto extensions = get_required_extensions();

    create_instance_info.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
    create_instance_info.ppEnabledExtensionNames = extensions.data();

    // Enable validation layers if DEBUG is on
    VkDebugUtilsMessengerCreateInfoEXT debug_create_info{};
    if (DEBUG)
    {
        create_instance_info.enabledLayerCount = static_cast<uint32_t>(validation_layers.size());
        create_instance_info.ppEnabledLayerNames = validation_layers.data();

        populate_debug_messenger_create_info(debug_create_info);
        create_instance_info.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debug_create_info;
    }
    else 
    {
        create_instance_info.enabledLayerCount = 0;
        create_instance_info.pNext = nullptr;
    }

    // Now we have all the info we need to actually create the instance with the result stored at &instance
    if (vkCreateInstance(&create_instance_info, nullptr, &instance) != VK_SUCCESS)
    {
        GenLogCritical("Failed to create instance. Instance.cpp");
        return 0;
    }

    return 1;
}

// Checks if all the GLFW extensions are supported by the current setup
int check_external_extensions(const char** external_extensions, int arr_size, std::vector<VkExtensionProperties>& vk_extensions)
{
    int success = 1;
    for (int i = 0; i < arr_size; i++)
    {
        std::string external_extension(external_extensions[i]);

        bool extension_found = false;
        for (const auto& vk_extension : vk_extensions)
        {
            if (external_extension == vk_extension.extensionName)
            {
                extension_found = true;
                continue;
            }
        }

        if (!extension_found)
        {
            success = 0;
            GenLogCritical("The following extension is not supported by your Vulkan Extensions: {}", external_extension);
        }
    }
    return success;
}

std::vector<const char*> get_required_extensions()
{
    uint32_t glfw_extension_count = 0;
    const char** glfw_extensions;
    glfw_extensions = glfwGetRequiredInstanceExtensions(&glfw_extension_count);

    std::vector<const char*> extensions(glfw_extensions, glfw_extensions + glfw_extension_count);

    if (DEBUG)
    {
        extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    }

    return extensions;
}

