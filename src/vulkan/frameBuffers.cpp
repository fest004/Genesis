#include "frameBuffers.hpp"
#include <vulkan/vulkan_core.h>


void createFrameBuffers(const VkDevice& device, VkExtent2D extent, std::vector<VkFramebuffer>& swapChainFramebuffers, std::vector<VkImageView>& swapChainImageViews, VkRenderPass& renderpass)
{
  swapChainFramebuffers.resize(swapChainImageViews.size());

  for (int i = 0; i < swapChainImageViews.size(); i++)
  {
    VkImageView attachments[] = 
    { swapChainImageViews[i] };

    VkFramebufferCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    createInfo.renderPass = renderpass;
    createInfo.attachmentCount = 1;
    createInfo.pAttachments = attachments;
    createInfo.width = extent.width;
    createInfo.height = extent.height;
    createInfo.layers = 1;



    if (vkCreateFramebuffer(device, &createInfo, nullptr, &swapChainFramebuffers[i]) != VK_SUCCESS)
      GenLogCritical("Failed to create frame buffer! In frameBuffers.cpp");

  }
}


