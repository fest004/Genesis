#include "../../core.h"

// TODO find general way of reducing the amounts of paramaters without making it
// overly object oriented


void createFrameBuffers(const VkDevice& device, Gen_Swapchain& swapChainInfo, VkRenderPass& renderpass);


void createVertexBuffer(Gen_Devices& devices, Gen_Buffers& buffers, VkQueue& graphicsQueue, VkCommandPool& commandPool, const std::vector<Vertex>& vertices);


void createBuffer(Gen_Devices& devices, VkBuffer& buffer, 
                  VkDeviceMemory& deviceMemory, VkDeviceSize size, 
                  VkBufferUsageFlags usage, 
                  VkMemoryPropertyFlags properties);



void copyBuffer(VkDevice& device, VkQueue& graphicsQueue, 
                VkBuffer& srcBuffer, VkBuffer& dstBuffer, 
                const VkDeviceSize& size, VkCommandPool& commandPool);

void createIndexBuffer(Gen_Devices& devices, Gen_Buffers& bufferInfo, VkQueue& graphicsQueue, VkCommandPool& commandPool, const std::vector<uint16_t>& indices, const std::vector<Vertex>& vertices);


uint32_t findMemoryType(VkPhysicalDevice& physicalDevice, uint32_t typeFilter, VkMemoryPropertyFlags properties);
