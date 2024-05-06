#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include "../core.h"



GLFWwindow* createWindow();
void createSurface(VkInstance& instance, GLFWwindow* window, VkSurfaceKHR& surface);
int windowUpdate(GLFWwindow* window);

void drawFrame(VkDevice& device, VkSwapchainKHR& swapchain, VkCommandBuffer& commandBuffer, 
               VkFence& fence, VkSemaphore& imgAvailable, VkSemaphore& renderFinished,
                VkPipeline& pipeline, VkExtent2D& extent, std::vector<VkFramebuffer> swapChainFrameBuffers, VkRenderPass& renderpass, VkQueue& graphicsQueue, VkQueue& presentQueue);





