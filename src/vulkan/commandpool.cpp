#include "commandpool.hpp"
#include "core/queues.hpp"
#include <array>
#include <iostream>
#include <vulkan/vulkan_core.h>



void recordCommandBuffer(Gen_Graphics& graphicsInfo, Gen_Swapchain& swapchain, 
                         Gen_Buffers& bufferInfo, 
                         Gen_DescriptorSet& desc, const std::vector<uint16_t> indices,
                         uint32_t& currentFrame, uint32_t& index)
{
   VkCommandBufferBeginInfo beginInfo{};
  beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
  beginInfo.flags = 0;
  beginInfo.pInheritanceInfo = nullptr;

  VkCommandBuffer comBuffer = bufferInfo.commandBuffers[currentFrame];

  if (vkBeginCommandBuffer(comBuffer, &beginInfo) != VK_SUCCESS)
    GenLogCritical(("Failed to begin command buffer! In commandpool.cpp"));

  VkRenderPassBeginInfo renderPassInfo{};
  renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
  renderPassInfo.renderPass = graphicsInfo.renderPass;

  renderPassInfo.framebuffer = swapchain.swapChainFramebuffers[index];
  renderPassInfo.renderArea.offset = {0, 0};
  renderPassInfo.renderArea.extent = swapchain.swapChainExtent;
   
  VkClearValue clearColor = 
  {
    {{0.0f, 0.0f, 0.0f, 1.0f}} //TODO experiment with this IG
  };
  renderPassInfo.clearValueCount = 1;
  renderPassInfo.pClearValues = &clearColor;

  vkCmdBeginRenderPass(bufferInfo.commandBuffers[currentFrame], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
  vkCmdBindPipeline(comBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsInfo.graphicsPipeline);

  VkViewport viewport{};
  viewport.x = 0.0f;
  viewport.y = 0.0f;
  viewport.width = static_cast<float>(swapchain.swapChainExtent.width);
  viewport.height= static_cast<float>(swapchain.swapChainExtent.height);
  viewport.minDepth = 0.0f;
  viewport.maxDepth = 1.0f;
  vkCmdSetViewport(comBuffer, 0, 1, &viewport);

  VkRect2D scissor{};
  scissor.offset = {0, 0};
  scissor.extent = swapchain.swapChainExtent;
  vkCmdSetScissor(comBuffer, 0, 1, &scissor);

  VkBuffer vertexBuffers[] = { bufferInfo.vertexBuffer };
  VkDeviceSize offsets[] = {0};


  vkCmdBindVertexBuffers(comBuffer, 0, 1, vertexBuffers, offsets);
  vkCmdBindIndexBuffer(comBuffer, bufferInfo.indexBuffer, 0, VK_INDEX_TYPE_UINT16);
  vkCmdBindDescriptorSets(comBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsInfo.pipelineLayout, 0, 1, &desc.descriptorsSets[currentFrame], 0, nullptr);



  vkCmdDrawIndexed(comBuffer, static_cast<uint32_t>(indices.size()), 1, 0, 0, 0);

  vkCmdEndRenderPass(comBuffer);

  if (vkEndCommandBuffer(comBuffer) != VK_SUCCESS)
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

void createCommandPool(Gen_Devices& devices, VkSurfaceKHR& surface, VkCommandPool& commandPool)
{
  QueueFamilyIndices familyIndeces = findQueueFamilies(devices.physicalDevice, surface);

  VkCommandPoolCreateInfo createInfo{};
  createInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
  createInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT; //Record command buffer every frame
  createInfo.queueFamilyIndex = familyIndeces.graphicsFamily.value();

  if (vkCreateCommandPool(devices.logicalDevice, &createInfo, nullptr, &commandPool) != VK_SUCCESS)
    GenLogCritical("Failed to create Command Pool! In commandpool.cpp");
}

/*
VkCommandBuffer beginSingleTimeCommands(VkDevice& device, VkCommandPool& commandPool) {
    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool = commandPool;
    allocInfo.commandBufferCount = 1;

    VkCommandBuffer commandBuffer;
    vkAllocateCommandBuffers(device, &allocInfo, &commandBuffer);

    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    vkBeginCommandBuffer(commandBuffer, &beginInfo);

    return commandBuffer;
}

void endSingleTimeCommands(VkDevice& device, VkQueue& graphicsQueue, VkCommandPool& commandPool, VkCommandBuffer& commandBuffer) {
    vkEndCommandBuffer(commandBuffer);

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffer;

    vkQueueSubmit(graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
    vkQueueWaitIdle(graphicsQueue);

    vkFreeCommandBuffers(device, commandPool, 1, &commandBuffer);
}

*/

