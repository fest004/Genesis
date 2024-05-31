#include "../../core.h"
#include "frameBuffers.hpp"
#include <algorithm>
#include <cstddef>
#include <cstring>
#include <vulkan/vulkan_core.h>

uint32_t findMemoryType(VkPhysicalDevice& physicalDevice, uint32_t typeFilter, VkMemoryPropertyFlags properties);





void createFrameBuffers(const VkDevice& device, Gen_Swapchain& swapChainInfo, VkRenderPass& renderpass)
{
  swapChainInfo.swapChainFramebuffers.resize(swapChainInfo.swapChainImageViews.size());

  for (size_t i = 0; i < swapChainInfo.swapChainImageViews.size(); i++)
  {
    VkImageView attachments[] = 
    {
      swapChainInfo.swapChainImageViews[i]
    };

 VkFramebufferCreateInfo frameBufferInfo{};
    frameBufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    frameBufferInfo.renderPass = renderpass; 
    frameBufferInfo.attachmentCount = 1;
    frameBufferInfo.pAttachments = attachments;
    frameBufferInfo.width = swapChainInfo.swapChainExtent.width;
    frameBufferInfo.height = swapChainInfo.swapChainExtent.height;
    frameBufferInfo.layers = 1;


   if (vkCreateFramebuffer(device, &frameBufferInfo, nullptr, &swapChainInfo.swapChainFramebuffers[i]) != VK_SUCCESS)
      GenLogCritical("Failed to create framebuffer! In framebuffer.cpp");
 }
}


void createBuffer(Gen_Devices& devices, VkBuffer& buffer, VkDeviceMemory& deviceMemory, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties) 
{
    VkBufferCreateInfo bufferInfo{};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = size;
    bufferInfo.usage = usage;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    if (vkCreateBuffer(devices.logicalDevice, &bufferInfo, nullptr, &buffer) != VK_SUCCESS) {
        throw std::runtime_error("failed to create buffer!");
    }

    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(devices.logicalDevice, buffer, &memRequirements);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = findMemoryType(devices.physicalDevice, memRequirements.memoryTypeBits, properties);

  //TODO system that removes the need to allocate memory on every buffer, it is bad practice in larger
  //applications
    if (vkAllocateMemory(devices.logicalDevice, &allocInfo, nullptr, &deviceMemory) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate buffer memory!");
    }

    vkBindBufferMemory(devices.logicalDevice, buffer, deviceMemory, 0);
}


void createIndexBuffer(Gen_Devices& devices, Gen_Buffers& bufferInfo, VkQueue& graphicsQueue, VkCommandPool& commandPool, const std::vector<uint16_t>& indices, const std::vector<Vertex>& vertices)
{
  VkDeviceSize bufferSize = sizeof(vertices[0]) * vertices.size();

  VkBuffer stagingBuffer; 
  VkDeviceMemory stagingBufferMemory;

  createBuffer(devices, stagingBuffer, stagingBufferMemory, bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

  void* data;
  vkMapMemory(devices.logicalDevice, stagingBufferMemory, 0, bufferSize, 0, &data);
  memcpy(data, indices.data(), (size_t)bufferSize);
  vkUnmapMemory(devices.logicalDevice, stagingBufferMemory);

 
  createBuffer(devices, stagingBuffer, stagingBufferMemory, bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);


  copyBuffer(devices.logicalDevice, graphicsQueue, stagingBuffer, bufferInfo.indexBuffer, bufferSize, commandPool);

  vkDestroyBuffer(devices.logicalDevice, stagingBuffer, nullptr);
  vkFreeMemory(devices.logicalDevice, stagingBufferMemory, nullptr);
}


void createVertexBuffer(Gen_Devices& devices, Gen_Buffers& buffers, VkQueue& graphicsQueue, VkCommandPool& commandPool, const std::vector<Vertex>& vertices)
{
  VkDeviceSize bufferSize = sizeof(vertices[0]) * vertices.size();

  VkBuffer stagingBuffer; 
  VkDeviceMemory stagingBufferMemory;

  createBuffer(devices, stagingBuffer, stagingBufferMemory, bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

  void* data;
  vkMapMemory(devices.logicalDevice, stagingBufferMemory, 0, bufferSize, 0, &data);
  memcpy(data, vertices.data(), (size_t)bufferSize);
  vkUnmapMemory(devices.logicalDevice, stagingBufferMemory);


  createBuffer(devices, stagingBuffer, stagingBufferMemory, bufferSize,
               VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
               VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);


  copyBuffer(devices.logicalDevice, graphicsQueue, stagingBuffer, buffers.vertexBuffer, bufferSize, commandPool);

  vkDestroyBuffer(devices.logicalDevice, stagingBuffer, nullptr);
  vkFreeMemory(devices.logicalDevice, stagingBufferMemory, nullptr);

}


void copyBuffer(VkDevice& device, VkQueue& graphicsQueue, VkBuffer& srcBuffer, VkBuffer& dstBuffer, const VkDeviceSize& size, VkCommandPool& commandPool) 
{
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

  VkBufferCopy copy{};
  copy.srcOffset = 0;
  copy.dstOffset = 0;
  copy.size = size;
  vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copy);

  vkEndCommandBuffer(commandBuffer);
  
  VkSubmitInfo submitInfo{};
  submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
  submitInfo.commandBufferCount = 1;
  submitInfo.pCommandBuffers = &commandBuffer;
  vkQueueSubmit(graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
  vkQueueWaitIdle(graphicsQueue);
  vkFreeCommandBuffers(device , commandPool, 1, &commandBuffer);



}


uint32_t findMemoryType(VkPhysicalDevice& physicalDevice, uint32_t typeFilter, VkMemoryPropertyFlags properties)
{
  VkPhysicalDeviceMemoryProperties memProperties;
  vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProperties);

  for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++)
  {
    if (typeFilter & (1 << i) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties)
    {
      return i;
    }
  }

  GenLogCritical("Failed to find memory type!");
  return 69-420;

}


