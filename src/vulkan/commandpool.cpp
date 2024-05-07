#include "commandpool.hpp"
#include "queues.hpp"
#include <iostream>
#include <vulkan/vulkan_core.h>



void recordCommandBuffer(VkPipeline& pipeline, VkExtent2D& extent, std::vector<VkFramebuffer>& swapChainFrameBuffers, VkRenderPass& renderpass, VkCommandBuffer& commandBuffer, uint32_t index)
{
  VkCommandBufferBeginInfo beginInfo{};
  beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
  beginInfo.flags = 0;
  beginInfo.pInheritanceInfo = nullptr;

  if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS)
    GenLogCritical(("Failed to begin command buffer! In commandpool.cpp"));

  VkRenderPassBeginInfo renderPassInfo{};
  renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
  renderPassInfo.renderPass = renderpass;

  renderPassInfo.framebuffer = swapChainFrameBuffers[index];
  renderPassInfo.renderArea.offset = {0, 0};
  renderPassInfo.renderArea.extent = extent;
   
  VkClearValue clearColor = 
  {
    {{0.0f, 0.0f, 0.0f, 1.0f}} //TODO experiment with this IG
  };
  renderPassInfo.clearValueCount = 1;
  renderPassInfo.pClearValues = &clearColor;

  vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
  vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);

  VkViewport viewport{};
  viewport.x = 0.0f;
  viewport.y = 0.0f;
  viewport.width = static_cast<uint32_t>(extent.width);
  viewport.height= static_cast<uint32_t>(extent.height);
  viewport.minDepth = 0.0f;
  viewport.maxDepth = 1.0f;
  vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

  VkRect2D scissor{};
  scissor.offset = {0, 0};
  scissor.extent = extent;
  vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

  //1200 lines of code, this is the one that sends the draw command...
  vkCmdDraw(commandBuffer, 3, 1, 0, 0);

  vkCmdEndRenderPass(commandBuffer);

  if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS)
    GenLogCritical("Failed to end command buffer! In file commandpool.cpp");

}

void createCommandBuffers(VkDevice& device, VkCommandPool& commandPool, std::vector<VkCommandBuffer>& commandBuffers)
{
  commandBuffers.resize(MAX_FRAMES_IN_FLIGHT);

  VkCommandBufferAllocateInfo createInfo{};
  createInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
  createInfo.commandPool = commandPool;
  createInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
  createInfo.commandBufferCount = 1;
  createInfo.commandBufferCount = (uint32_t) commandBuffers.size();

  if (vkAllocateCommandBuffers(device, &createInfo, commandBuffers.data())!= VK_SUCCESS)
    GenLogCritical("Failed to allocate command buffers! In commandpool.cpp")

}

void createCommandPool(VkDevice device, VkPhysicalDevice& physicalDevice, VkSurfaceKHR& surface, VkCommandPool& commandPool)
{
  QueueFamilyIndices familyIndeces = findQueueFamilies(physicalDevice, surface);

  VkCommandPoolCreateInfo createInfo{};
  createInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
  createInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT; //Record command buffer every frame
  createInfo.queueFamilyIndex = familyIndeces.graphicsFamily.value();

  if (vkCreateCommandPool(device, &createInfo, nullptr, &commandPool) != VK_SUCCESS)
    GenLogCritical("Failed to create Command Pool! In commandpool.cpp");
}
