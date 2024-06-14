#include <vulkan/vulkan_core.h>
#include "layouttransition.hpp"
#include "frameBuffers.hpp"



void transitionImageLayout(VkDevice& device, VkQueue& queue, VkCommandPool& commandPool, const VkImage& image, const VkFormat& format, const VkImageLayout& oldLayout, const VkImageLayout newLayout) 
{
  VkCommandBuffer commandBuffer = beginSingleTimeCommands(device, commandPool);

  VkImageMemoryBarrier memoryBarrier{};
  memoryBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
  memoryBarrier.oldLayout = oldLayout;
  memoryBarrier.newLayout = newLayout;

  memoryBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
  memoryBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;

  memoryBarrier.image = image;
  memoryBarrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
  memoryBarrier.subresourceRange.baseMipLevel = 0;
  memoryBarrier.subresourceRange.levelCount = 1;
  memoryBarrier.subresourceRange.baseArrayLayer = 0;
  memoryBarrier.subresourceRange.layerCount = 1;

  
  VkPipelineStageFlags srcStage;
  VkPipelineStageFlags dstStage;

  if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL)
  {
    memoryBarrier.srcAccessMask = 0;
    memoryBarrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

    srcStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
    dstStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
  }
  else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)
  {
    memoryBarrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
    memoryBarrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

    srcStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
    dstStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
  }
  else 
    GenLogCritical("Invalid layout transition! In layouttransition.cpp");


  vkCmdPipelineBarrier(commandBuffer, srcStage, dstStage, 0, 0, nullptr, 0, nullptr, 1, &memoryBarrier);
  
  endSingleTimeCommands(device, queue, commandPool, commandBuffer);
}


