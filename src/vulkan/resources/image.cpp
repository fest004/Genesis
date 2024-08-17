#include "image.hpp"
#include <iostream>
#include <ostream>
#include <vulkan/vulkan_core.h>
#include "frameBuffers.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "../../../libs/stb_image.h"


void create_image_texture(Gen_Devices& devices, Gen_ImageTexture& image_info, VkQueue& graphics_queue, VkCommandPool command_pool, const char* filepath)
{
    int tex_width, tex_height, tex_channels;

    // Load the image with stb_image
    stbi_uc* pixels = stbi_load(filepath, &tex_width, &tex_height, &tex_channels, STBI_rgb_alpha);
    if (!pixels) {
        throw std::runtime_error("Failed to load image with stb!");
    }
    
    VkDeviceSize image_size = tex_width * tex_height * 4;  // Assuming 4 bytes per pixel

    // Create a staging buffer
    VkBuffer staging_buffer;
    VkDeviceMemory staging_buffer_memory;
    create_buffer(devices, staging_buffer, staging_buffer_memory, image_size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

    // Copy image data to staging buffer
    void* data;
    vkMapMemory(devices.logical_device, staging_buffer_memory, 0, image_size, 0, &data);
    memcpy(data, pixels, static_cast<size_t>(image_size));
    vkUnmapMemory(devices.logical_device, staging_buffer_memory);
    stbi_image_free(pixels);

    // Create the image
    create_image(devices, image_info.texture_image_memory, tex_width, tex_height, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, image_info.image);

    // Transition image layout, copy buffer to image, and transition layout again
    transition_image_layout(devices.logical_device, graphics_queue, command_pool, image_info.image, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
    copy_buffer_to_image(devices.logical_device, graphics_queue, command_pool, staging_buffer, image_info.image, tex_width, tex_height);
    transition_image_layout(devices.logical_device, graphics_queue, command_pool, image_info.image, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

    // Cleanup staging buffer
    vkDestroyBuffer(devices.logical_device, staging_buffer, nullptr);
    vkFreeMemory(devices.logical_device, staging_buffer_memory, nullptr);
}

void create_image(Gen_Devices& devices, VkDeviceMemory& texture_image_memory, uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image)
{
    VkImageCreateInfo image_info{};
    image_info.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    image_info.imageType = VK_IMAGE_TYPE_2D;
    image_info.extent.width = width;
    image_info.extent.height = height;
    image_info.extent.depth = 1;
    image_info.mipLevels = 1;
    image_info.arrayLayers = 1;
    image_info.format = format;
    image_info.tiling = tiling;
    image_info.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    image_info.usage = usage;
    image_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    image_info.samples = VK_SAMPLE_COUNT_1_BIT;
    image_info.flags = 0;

    if (vkCreateImage(devices.logical_device, &image_info, nullptr, &image) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create image!");
    }

    VkMemoryRequirements mem_requirements;
    vkGetImageMemoryRequirements(devices.logical_device, image, &mem_requirements);

    VkMemoryAllocateInfo alloc_info{};
    alloc_info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    alloc_info.allocationSize = mem_requirements.size;
    alloc_info.memoryTypeIndex = find_memory_type(devices.physical_device, mem_requirements.memoryTypeBits, properties);

    if (vkAllocateMemory(devices.logical_device, &alloc_info, nullptr, &texture_image_memory) != VK_SUCCESS) {
        throw std::runtime_error("Failed to allocate memory!");
    }

    vkBindImageMemory(devices.logical_device, image, texture_image_memory, 0);
}

void copy_buffer_to_image(VkDevice& device, VkQueue& graphics_queue, VkCommandPool& command_pool, VkBuffer& buffer, VkImage& image, uint32_t width, uint32_t height)
{
    VkCommandBuffer command_buffer = begin_single_time_commands(device, command_pool);

    VkBufferImageCopy region{};
    region.bufferOffset = 0;
    region.bufferRowLength = 0;
    region.bufferImageHeight = 0;
    region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    region.imageSubresource.mipLevel = 0;
    region.imageSubresource.baseArrayLayer = 0;
    region.imageSubresource.layerCount = 1;
    region.imageOffset = {0, 0, 0};
    region.imageExtent = {width, height, 1};

    vkCmdCopyBufferToImage(command_buffer, buffer, image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);
    end_single_time_commands(device, graphics_queue, command_pool, command_buffer);
}

void create_texture_image_view(VkDevice& device, Gen_ImageTexture& image_info)
{
    VkImageViewCreateInfo view_info{};
    view_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    view_info.image = image_info.image;
    view_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
    view_info.format = VK_FORMAT_R8G8B8A8_SRGB;
    view_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    view_info.subresourceRange.baseMipLevel = 0;
    view_info.subresourceRange.levelCount = 1;
    view_info.subresourceRange.baseArrayLayer = 0;
    view_info.subresourceRange.layerCount = 1;

    if (vkCreateImageView(device, &view_info, nullptr, &image_info.texture_image_view) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create image view!");
    }
}

void create_texture_sampler(Gen_Devices& devices, VkSampler& sampler)
{
    VkSamplerCreateInfo sampler_info{};
    sampler_info.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
    sampler_info.magFilter = VK_FILTER_LINEAR;
    sampler_info.minFilter = VK_FILTER_LINEAR;
    sampler_info.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    sampler_info.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    sampler_info.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    sampler_info.anisotropyEnable = VK_TRUE;

    VkPhysicalDeviceProperties phys_properties{};
    vkGetPhysicalDeviceProperties(devices.physical_device, &phys_properties);

    sampler_info.maxAnisotropy = phys_properties.limits.maxSamplerAnisotropy;
    sampler_info.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
    sampler_info.unnormalizedCoordinates = VK_FALSE;
    sampler_info.compareEnable = VK_FALSE;
    sampler_info.compareOp = VK_COMPARE_OP_ALWAYS;
    sampler_info.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
    sampler_info.mipLodBias = 0.0f;
    sampler_info.minLod = 0.0f;
    sampler_info.maxLod = 0.0f;

    if (vkCreateSampler(devices.logical_device, &sampler_info, nullptr, &sampler) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create sampler!");
    }
}

