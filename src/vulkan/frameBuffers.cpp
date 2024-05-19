#include "../core.h"
#include "frameBuffers.hpp"
#include <algorithm>
#include <cstddef>
#include <cstring>
#include <vulkan/vulkan_core.h>
#include "../shaders/vertices.hpp"

uint32_t findMemoryType(VkPhysicalDevice& physicalDevice, uint32_t typeFilter, VkMemoryPropertyFlags properties);





void createFrameBuffers(const VkDevice& device, VkExtent2D& extent, std::vector<VkFramebuffer>& swapChainFramebuffers, std::vector<VkImageView>& swapChainImageViews, VkRenderPass& renderpass)
{
  swapChainFramebuffers.resize(swapChainImageViews.size());

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
}



void createBuffer(VkDevice& device, VkPhysicalDevice& physDevice, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory) 
{
    VkBufferCreateInfo bufferInfo{};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = size;
    bufferInfo.usage = usage;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    if (vkCreateBuffer(device, &bufferInfo, nullptr, &buffer) != VK_SUCCESS) {
        throw std::runtime_error("failed to create buffer!");
    }

    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(device, buffer, &memRequirements);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = findMemoryType(physDevice, memRequirements.memoryTypeBits, properties);

  //TODO system that removes the need to allocate memory on every buffer, it is bad practice in larger
  //applications
    if (vkAllocateMemory(device, &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate buffer memory!");
    }

    vkBindBufferMemory(device, buffer, bufferMemory, 0);
}


void createIndexBuffer(VkDevice& device, VkPhysicalDevice& physDevice, VkQueue& graphicsQueue, VkCommandPool& commandPool, VkDeviceMemory& indexBufferMemory, VkBuffer& indexBuffer, const std::vector<uint16_t>& indices)
{
  VkDeviceSize bufferSize = sizeof(vertices[0]) * vertices.size();

  VkBuffer stagingBuffer; 
  VkDeviceMemory stagingBufferMemory;
  createBuffer(device, physDevice, bufferSize, 
  VK_BUFFER_USAGE_TRANSFER_SRC_BIT, 
  VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, 
  stagingBuffer, stagingBufferMemory);

  void* data;
  vkMapMemory(device, stagingBufferMemory, 0, bufferSize, 0, &data);
  memcpy(data, indices.data(), (size_t)bufferSize);
  vkUnmapMemory(device, stagingBufferMemory);

 
  createBuffer(device, physDevice, bufferSize, 
  VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, 
  VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, 
  indexBuffer, indexBufferMemory);
  copyBuffer(device, physDevice, graphicsQueue, stagingBuffer, indexBuffer, bufferSize, commandPool);

  vkDestroyBuffer(device, stagingBuffer, nullptr);
  vkFreeMemory(device, stagingBufferMemory, nullptr);
}

void createVertexBuffer(VkDevice& device, VkPhysicalDevice& physDevice, VkQueue& graphicsQueue, VkCommandPool& commandPool, VkDeviceMemory& vertexBufferMemory, VkBuffer& vertexBuffer, const std::vector<Vertex>& vertices)
{
  VkDeviceSize bufferSize = sizeof(vertices[0]) * vertices.size();

  VkBuffer stagingBuffer; 
  VkDeviceMemory stagingBufferMemory;
  createBuffer(device, physDevice, bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, 
               VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, 
               stagingBuffer, 
               stagingBufferMemory);



  void* data;
  vkMapMemory(device, stagingBufferMemory, 0, bufferSize, 0, &data);
  memcpy(data, vertices.data(), (size_t)bufferSize);
  vkUnmapMemory(device, stagingBufferMemory);


  createBuffer(device, physDevice, bufferSize, 
               VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
               VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, vertexBuffer, vertexBufferMemory);

  copyBuffer(device, physDevice, graphicsQueue, stagingBuffer, vertexBuffer, bufferSize, commandPool);

  vkDestroyBuffer(device, stagingBuffer, nullptr);
  vkFreeMemory(device, stagingBufferMemory, nullptr);

}


void copyBuffer(VkDevice& device, VkPhysicalDevice& physDevice, VkQueue& graphicsQueue, VkBuffer& srcBuffer, VkBuffer& dstBuffer, const VkDeviceSize& size, VkCommandPool& commandPool) 
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
  vkFreeCommandBuffers(device, commandPool, 1, &commandBuffer);



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


