#include "../../core.h"

// TODO find general way of reducing the amounts of paramaters without making it
// overly object oriented


uint32_t findMemoryType(VkPhysicalDevice& physicalDevice, uint32_t typeFilter, VkMemoryPropertyFlags properties);

void createFrameBuffers(const VkDevice& device, Gen_Swapchain& swapChainInfo, VkRenderPass& renderpass);


void createVertexBuffer(Gen_Devices& devices, Gen_Buffers& buffers, VkQueue& graphicsQueue, VkCommandPool& commandPool, const std::vector<Vertex>& vertices);


void createBuffer(Gen_Devices& devices, VkBuffer& buffer, 
                  VkDeviceMemory& deviceMemory, VkDeviceSize size, 
                  VkBufferUsageFlags usage, 
                  VkMemoryPropertyFlags properties);



void copyBuffer(VkDevice device, VkCommandPool& commandPool, VkQueue& queue, VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
//void copyBuffer(VkDevice device, VkCommandPool& commandPool, VkQueue queue, VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
void createIndexBuffer(Gen_Devices& devices, Gen_Buffers& bufferInfo, VkQueue& graphicsQueue, VkCommandPool& commandPool, const std::vector<uint16_t>& indices, const std::vector<Vertex>& vertices);



VkCommandBuffer beginSingleTimeCommands(VkDevice& device, VkCommandPool& commandPool);
void endSingleTimeCommands(VkDevice& device, VkQueue& graphicsQueue, VkCommandPool& commandPool, VkCommandBuffer& commandBuffer);



