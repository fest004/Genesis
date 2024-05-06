#include "window.hpp"
#include <GLFW/glfw3.h>
#include <iostream>
#include <ostream>
#include <stdatomic.h>
#include <vulkan/vulkan_core.h>
#include "commandpool.hpp"


GLFWwindow* createWindow()
{
  glfwInit();
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
  GLFWwindow* window;
  window = glfwCreateWindow(WIDTH, HEIGHT, "Genesis", nullptr, nullptr);
  return window;
}


void createSurface(VkInstance& instance, GLFWwindow* window, VkSurfaceKHR& surface)
{
  if (glfwCreateWindowSurface(instance, window, nullptr, &surface) != VK_SUCCESS) 
  {
    GenLogCritical("Failed to create Window surface!");
  }
}

void drawFrame(VkDevice& device, VkSwapchainKHR& swapchain, VkCommandBuffer& commandBuffer, 
               VkFence& fence, VkSemaphore& imgAvailable, VkSemaphore& renderFinished,
                VkPipeline& pipeline, VkExtent2D& extent, std::vector<VkFramebuffer> swapChainFrameBuffers, VkRenderPass& renderpass, VkQueue& graphicsQueue, VkQueue& presentQueue)
{
  //Wait for previous frame to finish
  vkWaitForFences(device, 1, &fence, VK_TRUE, UINT64_MAX); //Wait for fence  that tells us we are not rendering anything currently 
  vkResetFences(device, 1, &fence); //Reset manually
  
  //Acquire new image from swapchain
  uint32_t imageIndex; 
  vkAcquireNextImageKHR(device, swapchain, UINT64_MAX, imgAvailable, VK_NULL_HANDLE, &imageIndex);

  //Record command buffer that draws scene to image
  vkResetCommandBuffer(commandBuffer, 0);
  //recordCommandBuffer(pipeline, extent, swapChainFrameBuffers, renderpass, commandBuffer, imageIndex);
  recordCommandBuffer(pipeline, extent, swapChainFrameBuffers, 
                      renderpass, commandBuffer, imageIndex);

  
  //Submit recorded command buffer
  VkSubmitInfo submitInfo{};
  submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

  VkSemaphore waitSemaphores[] = { imgAvailable };
  VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
  submitInfo.waitSemaphoreCount = 1;
  submitInfo.pWaitSemaphores = waitSemaphores;
  submitInfo.pWaitDstStageMask = waitStages;

  submitInfo.commandBufferCount = 1;
  submitInfo.pCommandBuffers = &commandBuffer;

  VkSemaphore signalSemaphores[] = { renderFinished }; 
  submitInfo.signalSemaphoreCount = 1;
  submitInfo.pSignalSemaphores = signalSemaphores;

    if (vkQueueSubmit(graphicsQueue, 1, &submitInfo, fence) != VK_SUCCESS)
      GenLogCritical("Failed to submit draw command buffer! In window.cpp");

  //Present swap chain image

  VkPresentInfoKHR presentInfo{};
  presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
  presentInfo.waitSemaphoreCount = 1;
  presentInfo.pWaitSemaphores = signalSemaphores; 

  VkSwapchainKHR swapChains[] = { swapchain };
  presentInfo.swapchainCount = 1;
  presentInfo.pSwapchains = swapChains;
  presentInfo.pImageIndices = &imageIndex;
  presentInfo.pResults = nullptr;

  vkQueuePresentKHR(presentQueue, &presentInfo); //Oh my LOOORD

  
  
}


int windowUpdate(GLFWwindow* window)
{
  if (glfwWindowShouldClose(window)) 
    return 0;

  glfwPollEvents();

  return 1;
}


