#include "../core.h"



void createFrameBuffers(const VkDevice& device, VkExtent2D extent, 
                        std::vector<VkFramebuffer>& swapChainFramebuffers, 
                        std::vector<VkImageView>& swapChainImageViews, 
                        VkRenderPass& renderpass);
