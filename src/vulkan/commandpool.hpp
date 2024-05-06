#include "../core.h"




void recordCommandBuffer(VkPipeline& pipeline, VkExtent2D& extent, std::vector<VkFramebuffer>& swapChainFrameBuffers, VkRenderPass& renderpass, VkCommandBuffer& commandBuffer, uint32_t index);

void createCommandBuffer(VkDevice device, VkCommandPool& commandPool, VkCommandBuffer& commandBuffer);

void createCommandPool(VkDevice device, VkPhysicalDevice& physicalDevice, VkSurfaceKHR& surface, 
VkCommandPool& commandPool);
