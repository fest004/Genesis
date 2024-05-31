#pragma once
#include "../core.h"

void recordCommandBuffer(VkPipeline& pipeline, VkPipelineLayout& layout, VkBuffer& vertexBuffer, VkBuffer& indexBuffer, const std::vector<Vertex>& vertices, VkExtent2D& extent, std::vector<VkFramebuffer>& swapChainFrameBuffers, VkRenderPass& renderpass, VkCommandBuffer& commandBuffer, uint32_t index, const std::vector<uint16_t>& indices, uint32_t currentFrame, std::vector<VkDescriptorSet>& descriptorSets);

void createCommandBuffers(VkDevice& device, VkCommandPool& commandPool, std::vector<VkCommandBuffer>& commandBuffers);

void createCommandPool(Gen_Devices& devices, VkSurfaceKHR& surface, VkCommandPool& commandPool);
