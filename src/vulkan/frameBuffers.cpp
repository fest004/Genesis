#include "frameBuffers.hpp"
#include <vulkan/vulkan_core.h>


void createFrameBuffers(const VkDevice& device, VkExtent2D& extent, std::vector<VkFramebuffer>& swapChainFramebuffers, std::vector<VkImageView>& swapChainImageViews, VkRenderPass& renderpass)
{

swapChainFramebuffers.resize(swapChainImageViews.size());

  GenLogTrace("Size of swapchainimagebuffers: {}", swapChainFramebuffers.size());

  for (size_t i = 0; i < swapChainImageViews.size(); i++)
  {
    VkImageView attachments[] = 
    {
      swapChainImageViews[i]
    };

 VkFramebufferCreateInfo frameBufferInfo{};
    frameBufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    frameBufferInfo.renderPass = renderpass; 
    frameBufferInfo.attachmentCount = 1;
    frameBufferInfo.pAttachments = attachments;
    frameBufferInfo.width = extent.width;
    frameBufferInfo.height = extent.height;
    frameBufferInfo.layers = 1;


   if (vkCreateFramebuffer(device, &frameBufferInfo, nullptr, &swapChainFramebuffers[i]) != VK_SUCCESS)
      GenLogCritical("Failed to create framebuffer! In framebuffer.cpp");


   
 }
  GenLogTrace("Size of swapchainimageviews: {}", swapChainImageViews.size());



}


