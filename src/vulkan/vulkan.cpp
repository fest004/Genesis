#include "vulkan.hpp"
#include "swapchain.hpp"
#include "window.hpp"
#include <GLFW/glfw3.h>
#include <iostream>
#include <ostream>
#include <vulkan/vulkan_core.h>

int Vulkan::initVulkan()
{
  m_Window = createWindow();
  createInstance(m_VkInstance, m_ValidationLayers);
  setupDebugMessenger(m_VkInstance, &m_DebugMessenger);
  createSurface(m_VkInstance, m_Window, m_Surface);
  pickPhysicalDevice(m_VkInstance, m_Device, m_Surface, m_PhysicalDevice, m_DeviceExtensions);
  createLogicalDevice(m_PhysicalDevice, m_Device, m_GraphicsQueue, m_ValidationLayers, m_DeviceExtensions, m_Surface, m_PresentQueue);
  createSwapChain(m_Device, m_PhysicalDevice, m_Surface, m_SwapChain, m_Window, m_SwapChainImages, m_SwapChainImageFormat, m_SwapChainExtent);
  createImageViews(m_Device, m_SwapChainImages, m_SwapChainImageViews, m_SwapChainImageFormat);
  createRenderpass(m_Device, m_RenderPass, m_SwapChainImageFormat);
  createGraphicsPipelines(m_Device, m_GraphicsPipeline, m_PipelineLayout, m_SwapChainExtent, m_RenderPass);  
  createFrameBuffers(m_Device, m_SwapChainExtent, m_SwapChainFramebuffers, m_SwapChainImageViews, m_RenderPass);
  createCommandPool(m_Device, m_PhysicalDevice, m_Surface, m_CommandPool);
  createCommandBuffer(m_Device, m_CommandPool,m_CommandBuffer);
  createSyncObjects(m_Device, m_ImageAvailableSemaphore, m_RenderFinishedSemaphore, m_InFlightFence);

 

  return 1;
}

int Vulkan::update()
{
  if (!glfwWindowShouldClose(m_Window))
  {
    glfwPollEvents();
    drawFrame();

  }
  return 1;
}

void Vulkan::drawFrame()
{
  //Wait for previous frame to finish
  vkWaitForFences(m_Device, 1, &m_InFlightFence, VK_TRUE, UINT64_MAX); //Wait for fence  that tells us we are not rendering anything currently 
  vkResetFences(m_Device, 1, &m_InFlightFence); //Reset manually
  
  //Acquire new image from swapchain
  uint32_t imageIndex; 
  vkAcquireNextImageKHR(m_Device, m_SwapChain, UINT64_MAX, m_ImageAvailableSemaphore, VK_NULL_HANDLE, &imageIndex);

  //Record command buffer that draws scene to image
  vkResetCommandBuffer(m_CommandBuffer, 0);
  //recordCommandBuffer(pipeline, extent, swapChainFrameBuffers, renderpass, commandBuffer, imageIndex);
  recordCommandBuffer(m_GraphicsPipeline, m_SwapChainExtent, m_SwapChainFramebuffers, m_RenderPass, m_CommandBuffer, imageIndex);

  
  //Submit recorded command buffer
  VkSubmitInfo submitInfo{};
  submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

  VkSemaphore waitSemaphores[] = { m_ImageAvailableSemaphore };
  VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
  submitInfo.waitSemaphoreCount = 1;
  submitInfo.pWaitSemaphores = waitSemaphores;
  submitInfo.pWaitDstStageMask = waitStages;

  submitInfo.commandBufferCount = 1;
  submitInfo.pCommandBuffers = &m_CommandBuffer;

  VkSemaphore signalSemaphores[] = { m_RenderFinishedSemaphore }; 
  submitInfo.signalSemaphoreCount = 1;
  submitInfo.pSignalSemaphores = signalSemaphores;

    if (vkQueueSubmit(m_GraphicsQueue, 1, &submitInfo, m_InFlightFence) != VK_SUCCESS)
      GenLogCritical("Failed to submit draw command buffer! In window.cpp");

  //Present swap chain image

  VkPresentInfoKHR presentInfo{};
  presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
  presentInfo.waitSemaphoreCount = 1;
  presentInfo.pWaitSemaphores = signalSemaphores; 

  VkSwapchainKHR swapChains[] = { m_SwapChain };
  presentInfo.swapchainCount = 1;
  presentInfo.pSwapchains = swapChains;
  presentInfo.pImageIndices = &imageIndex;
  presentInfo.pResults = nullptr;

  vkQueuePresentKHR(m_PresentQueue, &presentInfo); //Oh my LOOORD
}


void Vulkan::cleanup()
{
  vkDestroySemaphore(m_Device, m_ImageAvailableSemaphore, nullptr);
  vkDestroySemaphore(m_Device, m_RenderFinishedSemaphore, nullptr);
  vkDestroyFence(m_Device, m_InFlightFence, nullptr);

  vkDestroyCommandPool(m_Device, m_CommandPool, nullptr);
  for (auto framebuffer : m_SwapChainFramebuffers)
    vkDestroyFramebuffer(m_Device, framebuffer, nullptr);

  vkDestroyPipelineLayout(m_Device, m_PipelineLayout, nullptr);
  vkDestroyRenderPass(m_Device, m_RenderPass, nullptr);

  for (auto imageView : m_SwapChainImageViews)
    vkDestroyImageView(m_Device, imageView, nullptr);


  vkDestroySwapchainKHR(m_Device, m_SwapChain, nullptr);
  vkDestroyDevice(m_Device, nullptr);

  if (DEBUG)
  {
    DestroyDebugUtilsMessengerEXT(m_VkInstance, m_DebugMessenger, nullptr);
  }

  vkDestroySurfaceKHR(m_VkInstance, m_Surface, nullptr);
  vkDestroyInstance(m_VkInstance, nullptr);

  glfwDestroyWindow(m_Window);
  glfwTerminate();
}

