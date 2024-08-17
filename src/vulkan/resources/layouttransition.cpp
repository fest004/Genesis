#include <vulkan/vulkan_core.h>
#include "layouttransition.hpp"
#include "frameBuffers.hpp"



void transition_image_layout(VkDevice& device, VkQueue& queue, VkCommandPool& command_pool, const VkImage& image, const VkFormat& format, const VkImageLayout& old_layout, const VkImageLayout new_layout) 
{
  VkCommandBuffer command_buffer = begin_single_time_commands(device, command_pool);

  VkImageMemoryBarrier memory_barrier{};
  memory_barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
  memory_barrier.oldLayout= old_layout;
  memory_barrier.newLayout = new_layout;

  memory_barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
  memory_barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;

  memory_barrier.image = image;
  memory_barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
  memory_barrier.subresourceRange.baseMipLevel = 0;
  memory_barrier.subresourceRange.levelCount = 1;
  memory_barrier.subresourceRange.baseArrayLayer = 0;
  memory_barrier.subresourceRange.layerCount= 1;

  
  VkPipelineStageFlags src_stage;
  VkPipelineStageFlags dst_stage;

  if (old_layout == VK_IMAGE_LAYOUT_UNDEFINED && new_layout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL)
  {
    memory_barrier.srcAccessMask = 0;
    memory_barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

    src_stage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
    dst_stage = VK_PIPELINE_STAGE_TRANSFER_BIT;
  }
  else if (old_layout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && new_layout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)
  {
    memory_barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
    memory_barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

    src_stage = VK_PIPELINE_STAGE_TRANSFER_BIT;
    dst_stage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
  }
  else 
    GenLogCritical("Invalid layout transition! In layouttransition.cpp");


  vkCmdPipelineBarrier(command_buffer, src_stage, dst_stage, 0, 0, nullptr, 0, nullptr, 1, &memory_barrier);
  
  end_single_time_commands(device, queue, command_pool, command_buffer);
}


