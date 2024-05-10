#include "../core.h"
#include "../shaders/vertices.hpp"


void createFrameBuffers(const VkDevice& device, VkExtent2D& extent, 
                        std::vector<VkFramebuffer>& swapChainFramebuffers, 
                        std::vector<VkImageView>& swapChainImageViews, 
                        VkRenderPass& renderpass);



void createVertexBuffer(VkDevice& device, VkBuffer& vertexBuffer, const std::vector<Vertex>& vertices);
