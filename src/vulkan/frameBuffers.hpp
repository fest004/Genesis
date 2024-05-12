#include "../core.h"
#include "../shaders/vertices.hpp"

// TODO find general way of reducing the amounts of paramaters without making it
// overly object oriented

void createFrameBuffers(const VkDevice &device, VkExtent2D &extent,
                        std::vector<VkFramebuffer> &swapChainFramebuffers,
                        std::vector<VkImageView> &swapChainImageViews,
                        VkRenderPass &renderpass);

void createVertexBuffer(VkDevice &device, VkPhysicalDevice &physDevice,
                        VkQueue &graphicsQueue, VkCommandPool &commandPool,
                        VkDeviceMemory &vertexBufferMemory,
                        VkBuffer &vertexBuffer,
                        const std::vector<Vertex> &vertices);

void createBuffer(VkDevice &device, VkPhysicalDevice &physDevice,
                  VkDeviceSize size, VkBufferUsageFlags usage,
                  VkMemoryPropertyFlags properties, VkBuffer &buffer,
                  VkDeviceMemory &bufferMemory);

void copyBuffer(VkDevice &device, VkPhysicalDevice &physDevice,
                VkQueue &graphicsQueue, VkBuffer &srcBuffer,
                VkBuffer &dstBuffer, const VkDeviceSize &size,
                VkCommandPool &commandPool);

void createIndexBuffer(VkDevice &device, VkPhysicalDevice &physDevice,
                       VkQueue &graphicsQueue, VkCommandPool &commandPool,
                       VkDeviceMemory &indexBufferMemory, VkBuffer &indexBuffer,
                       const std::vector<uint16_t> &indices);
