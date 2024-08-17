#include "../../core.h"
#include <cstring>
#include <vulkan/vulkan_core.h>
#include "frameBuffers.hpp"
#include "layouttransition.hpp"


void create_image(Gen_Devices& devices, VkDeviceMemory& texture_image_mem, uint32_t width, uint32_t height, VkFormat format, 
                 VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image);

void create_image_texture(Gen_Devices& devices, Gen_ImageTexture& image_info, VkQueue& graphics_queue, VkCommandPool command_pool,  const char* filepath);

void copy_buffer_to_image(VkDevice& device, VkQueue& graphics_queue, VkCommandPool& command_pool, VkBuffer& buffer, VkImage& image, uint32_t width, uint32_t height);

void create_texture_image_view(VkDevice& device, Gen_ImageTexture& image_info);


void create_texture_sampler(Gen_Devices& devices, VkSampler& sampler);
