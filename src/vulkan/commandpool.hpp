#pragma once
#include "../core.h"


void recordCommandBuffer(Gen_Graphics& graphicsInfo, Gen_Swapchain& swapchain, 
                         Gen_Buffers& bufferInfo, 
                         Gen_DescriptorSet& desc, const std::vector<uint16_t> indices,
                         uint32_t& currentFrame, uint32_t& index);


void createCommandBuffers(VkDevice& device, VkCommandPool& commandPool, std::vector<VkCommandBuffer>& commandBuffers);

void createCommandPool(Gen_Devices& devices, VkSurfaceKHR& surface, VkCommandPool& commandPool);
