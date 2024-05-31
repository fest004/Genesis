#include "../../core.h"


VkCommandBuffer beginSingleTimeCommands(VkDevice& device, VkCommandPool& commandPool);
void endSingleGameCommands(VkDevice& device, VkCommandPool& commandPool, VkQueue& queue, VkCommandBuffer& commandBuffer);
void transitionImageLayout(VkDevice& device, VkQueue& queue, VkCommandPool& commandPool, const VkImage& image, const VkFormat& format, const VkImageLayout& oldLayout, const VkImageLayout newLayout) ;

