#pragma once
#include "../core.h"

#include "../shaders/vertices.hpp"



void recordCommandBuffer(VkPipeline& pipeline, VkBuffer& vertexBuffer, const std::vector<Vertex>& vertices, VkExtent2D& extent, std::vector<VkFramebuffer>& swapChainFrameBuffers, VkRenderPass& renderpass, VkCommandBuffer& commandBuffer, uint32_t index);

void createCommandBuffers(VkDevice& device, VkCommandPool& commandPool, std::vector<VkCommandBuffer>& commandBuffers);

void createCommandPool(VkDevice device, VkPhysicalDevice& physicalDevice, VkSurfaceKHR& surface, 
VkCommandPool& commandPool);
