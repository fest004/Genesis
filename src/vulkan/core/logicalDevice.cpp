#include "logicalDevice.hpp"
#include "queues.hpp"
#include <iostream>
#include <set>
#include <vulkan/vulkan_core.h>


void create_logical_device(Gen_Devices& devices, 
                           VkQueue& graphics_queue, 
                           VkQueue& present_queue,
                           const std::vector<const char*>& validation_layers, 
                           std::vector<const char*> device_extensions, 
                           VkSurfaceKHR& surface)
{
    QueueFamilyIndices indices = find_queue_families(devices.physical_device, surface);

    std::vector<VkDeviceQueueCreateInfo> queue_create_infos;
    std::set<uint32_t> unique_queue_families = {indices.graphics_family.value(), indices.present_family.value()};

    float queue_priority = 1.0f;
    for (uint32_t queue_family : unique_queue_families) {
        VkDeviceQueueCreateInfo queue_create_info{};
        queue_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queue_create_info.queueFamilyIndex = queue_family;
        queue_create_info.queueCount = 1;
        queue_create_info.pQueuePriorities = &queue_priority;
        queue_create_infos.push_back(queue_create_info);
    }

    VkPhysicalDeviceFeatures device_features{};
    device_features.samplerAnisotropy = VK_TRUE;

    VkDeviceCreateInfo create_info{};
    create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    create_info.queueCreateInfoCount = static_cast<uint32_t>(queue_create_infos.size());
    create_info.pQueueCreateInfos = queue_create_infos.data();
    create_info.pEnabledFeatures = &device_features;
    create_info.enabledExtensionCount = static_cast<uint32_t>(device_extensions.size());
    create_info.ppEnabledExtensionNames = device_extensions.data();

    if (DEBUG) 
    {
        create_info.enabledLayerCount = static_cast<uint32_t>(validation_layers.size());
        create_info.ppEnabledLayerNames = validation_layers.data();
    } 
    else 
    {
        create_info.enabledLayerCount = 0;
    }

    if (vkCreateDevice(devices.physical_device, &create_info, nullptr, &devices.logical_device) != VK_SUCCESS) 
    {
        GenLogCritical("Failed to create logical device!");
    }

    vkGetDeviceQueue(devices.logical_device, indices.graphics_family.value(), 0, &graphics_queue);
    vkGetDeviceQueue(devices.logical_device, indices.present_family.value(), 0, &present_queue);
}

