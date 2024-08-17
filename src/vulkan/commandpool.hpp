#pragma once
#include "../core.h"


void record_command_buffer(Gen_Graphics& graphics_info, Gen_Swapchain& swapchain, 
                         Gen_Buffers& buffer_info, 
                         Gen_DescriptorSet& desc, const std::vector<uint16_t> indices,
                         uint32_t& current_frame, uint32_t& index);


void create_command_buffers(VkDevice& device, VkCommandPool& command_pool, std::vector<VkCommandBuffer>& command_buffers);

void create_command_pool(Gen_Devices& devices, VkSurfaceKHR& surface, VkCommandPool& command_pool);
