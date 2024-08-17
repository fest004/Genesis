#include "../../core.h"
#include "frameBuffers.hpp"
#include <algorithm>
#include <cstddef>
#include <cstring>
#include <iostream>
#include <ostream>
#include <vulkan/vulkan_core.h>



void create_framebuffers(const VkDevice& device, Gen_Swapchain& swap_chain_info, VkRenderPass& renderpass)
{
    swap_chain_info.swap_chain_framebuffers.resize(swap_chain_info.swap_chain_image_views.size());

    for (size_t i = 0; i < swap_chain_info.swap_chain_image_views.size(); i++)
    {
        VkImageView attachments[] = { swap_chain_info.swap_chain_image_views[i] };

        VkFramebufferCreateInfo framebuffer_info{};
        framebuffer_info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebuffer_info.renderPass = renderpass;
        framebuffer_info.attachmentCount = 1;
        framebuffer_info.pAttachments = attachments;
        framebuffer_info.width = swap_chain_info.swap_chain_extent.width;
        framebuffer_info.height = swap_chain_info.swap_chain_extent.height;
        framebuffer_info.layers = 1;

        if (vkCreateFramebuffer(device, &framebuffer_info, nullptr, &swap_chain_info.swap_chain_framebuffers[i]) != VK_SUCCESS) 
        {
            GenLogCritical("Failed to create framebuffer! In framebuffer.cpp");
        }
    }
}

void create_buffer(Gen_Devices& devices, VkBuffer& buffer, VkDeviceMemory& device_memory, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties)
{
    VkBufferCreateInfo buffer_info{};
    buffer_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    buffer_info.size = size;
    buffer_info.usage = usage;
    buffer_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    if (vkCreateBuffer(devices.logical_device, &buffer_info, nullptr, &buffer) != VK_SUCCESS) 
    {
        throw std::runtime_error("Failed to create buffer!");
    }

    VkMemoryRequirements mem_requirements;
    vkGetBufferMemoryRequirements(devices.logical_device, buffer, &mem_requirements);

    VkMemoryAllocateInfo alloc_info{};
    alloc_info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    alloc_info.allocationSize = mem_requirements.size;
    alloc_info.memoryTypeIndex = find_memory_type(devices.physical_device, mem_requirements.memoryTypeBits, properties);

    // TODO: Implement a better memory allocation strategy for larger applications
    if (vkAllocateMemory(devices.logical_device, &alloc_info, nullptr, &device_memory) != VK_SUCCESS) 
    {
        throw std::runtime_error("Failed to allocate buffer memory!");
    }

    vkBindBufferMemory(devices.logical_device, buffer, device_memory, 0);
}

void create_index_buffer(Gen_Devices& devices, Gen_Buffers& buffer_info, VkQueue& graphics_queue, VkCommandPool& command_pool, const std::vector<uint16_t>& indices)
{
    VkDeviceSize buffer_size = sizeof(indices[0]) * indices.size();

    VkBuffer staging_buffer;
    VkDeviceMemory staging_buffer_memory;

    create_buffer(devices, staging_buffer, staging_buffer_memory, buffer_size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

    void* data;
    vkMapMemory(devices.logical_device, staging_buffer_memory, 0, buffer_size, 0, &data);
    memcpy(data, indices.data(), static_cast<size_t>(buffer_size));
    vkUnmapMemory(devices.logical_device, staging_buffer_memory);

    create_buffer(devices, buffer_info.index_buffer, buffer_info.index_buffer_memory, buffer_size, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

    copy_buffer(devices.logical_device, command_pool, graphics_queue, staging_buffer, buffer_info.index_buffer, buffer_size);

    vkDestroyBuffer(devices.logical_device, staging_buffer, nullptr);
    vkFreeMemory(devices.logical_device, staging_buffer_memory, nullptr);
}

void create_vertex_buffer(Gen_Devices& devices, Gen_Buffers& buffers, VkQueue& graphics_queue, VkCommandPool& command_pool, const std::vector<Vertex>& vertices)
{
    VkDeviceSize buffer_size = sizeof(vertices[0]) * vertices.size();

    VkBuffer staging_buffer;
    VkDeviceMemory staging_buffer_memory;

    create_buffer(devices, staging_buffer, staging_buffer_memory, buffer_size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

    void* data;
    vkMapMemory(devices.logical_device, staging_buffer_memory, 0, buffer_size, 0, &data);
    memcpy(data, vertices.data(), static_cast<size_t>(buffer_size));
    vkUnmapMemory(devices.logical_device, staging_buffer_memory);

    create_buffer(devices, buffers.vertex_buffer, buffers.vertex_buffer_memory, buffer_size, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

    copy_buffer(devices.logical_device, command_pool, graphics_queue, staging_buffer, buffers.vertex_buffer, buffer_size);

    vkDestroyBuffer(devices.logical_device, staging_buffer, nullptr);
    vkFreeMemory(devices.logical_device, staging_buffer_memory, nullptr);
}

void copy_buffer(VkDevice device, VkCommandPool& command_pool, VkQueue& queue, VkBuffer src_buffer, VkBuffer dst_buffer, VkDeviceSize size)
{
    VkCommandBuffer command_buffer = begin_single_time_commands(device, command_pool);

    VkBufferCopy copy_region{};
    copy_region.size = size;
    vkCmdCopyBuffer(command_buffer, src_buffer, dst_buffer, 1, &copy_region);

    end_single_time_commands(device, queue, command_pool, command_buffer);
}

uint32_t find_memory_type(VkPhysicalDevice& physical_device, uint32_t type_filter, VkMemoryPropertyFlags properties)
{
    VkPhysicalDeviceMemoryProperties mem_properties;
    vkGetPhysicalDeviceMemoryProperties(physical_device, &mem_properties);

    for (uint32_t i = 0; i < mem_properties.memoryTypeCount; i++)
    {
        if ((type_filter & (1 << i)) && (mem_properties.memoryTypes[i].propertyFlags & properties) == properties)
        {
            return i;
        }
    }

    GenLogCritical("Failed to find suitable memory type!");
    return UINT32_MAX;  // Return a constant for failure instead of an arbitrary number
}

VkCommandBuffer begin_single_time_commands(VkDevice& device, VkCommandPool& command_pool)
{
    VkCommandBufferAllocateInfo alloc_info{};
    alloc_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    alloc_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    alloc_info.commandPool = command_pool;
    alloc_info.commandBufferCount = 1;

    VkCommandBuffer command_buffer;
    vkAllocateCommandBuffers(device, &alloc_info, &command_buffer);

    VkCommandBufferBeginInfo begin_info{};
    begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    begin_info.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    vkBeginCommandBuffer(command_buffer, &begin_info);

    return command_buffer;
}

void end_single_time_commands(VkDevice& device, VkQueue& graphics_queue, VkCommandPool& command_pool, VkCommandBuffer& command_buffer)
{
    vkEndCommandBuffer(command_buffer);

    VkSubmitInfo submit_info{};
    submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submit_info.commandBufferCount = 1;
    submit_info.pCommandBuffers = &command_buffer;

    vkQueueSubmit(graphics_queue, 1, &submit_info, VK_NULL_HANDLE);
    vkQueueWaitIdle(graphics_queue);

    vkFreeCommandBuffers(device, command_pool, 1, &command_buffer);
}

