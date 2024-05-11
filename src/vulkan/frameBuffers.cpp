#include "frameBuffers.hpp"
#include <algorithm>
#include <cstring>
#include <vulkan/vulkan_core.h>
#include "../shaders/vertices.hpp"


uint32_t findMemoryType(VkPhysicalDevice& physicalDevice, uint32_t typeFilter, VkMemoryPropertyFlags properties);

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


void createVertexBuffer(VkDevice& device, VkPhysicalDevice& physDevice, VkDeviceMemory& vertexBufferMemory, VkBuffer& vertexBuffer, const std::vector<Vertex>& vertices)
{
  VkBufferCreateInfo vertexInfo{};
  vertexInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
  vertexInfo.size = sizeof(vertices[0]) * vertices.size();
  vertexInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
  vertexInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

  if (vkCreateBuffer(device, &vertexInfo, nullptr, &vertexBuffer) != VK_SUCCESS)
    GenLogCritical(("Failed to create Vertex Buffer! in frameBuffers.cpp:createVertexBuffer"));

  VkMemoryRequirements memRequireMents;
  vkGetBufferMemoryRequirements(device, vertexBuffer, &memRequireMents);

  VkMemoryAllocateInfo allocInfo{};
  allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
  allocInfo.allocationSize = memRequireMents.size;
  allocInfo.memoryTypeIndex = findMemoryType(physDevice, memRequireMents.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                                                                                        VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
  if (vkAllocateMemory(device, &allocInfo, nullptr, &vertexBufferMemory) != VK_SUCCESS) 
    GenLogCritical("Failed to allocate memory!");

  vkBindBufferMemory(device, vertexBuffer, vertexBufferMemory, 0);

  void* data;
  vkMapMemory(device, vertexBufferMemory, 0, vertexInfo.size, 0, &data);
  memcpy(data, vertices.data(), (size_t)vertexInfo.size);

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


