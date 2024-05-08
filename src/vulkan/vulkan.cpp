#include "vulkan.hpp"
#include "swapchain.hpp"
#include "window.hpp"
#include <GLFW/glfw3.h>
#include <iostream>
#include <ostream>
#include <vulkan/vulkan_core.h>

int Vulkan::initVulkan()
{
  m_Window = createWindow(&m_FrameBufferResized);
  createInstance(m_VkInstance, m_ValidationLayers); //Instance of vulkan, set up validation layers
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
  createCommandBuffers(m_Device, m_CommandPool,m_CommandBuffers);
  createSyncObjects(m_Device, m_ImageAvailableSemaphores, m_RenderFinishedSemaphores, m_InFlightFences);

 

  return 1;
}

int Vulkan::update()
{
  if (!glfwWindowShouldClose(m_Window))
  {
    glfwPollEvents();
    drawFrame();

  }
  vkDeviceWaitIdle(m_Device);
  return 1;
}

void Vulkan::drawFrame()
{
  //Wait for previous frame to finish
  vkWaitForFences(m_Device, 1, &m_InFlightFences[m_CurrentFrame], VK_TRUE, UINT64_MAX); //Wait for fence  that tells us we are not rendering anything currently 
  
  //Acquire new image from swapchain
  uint32_t imageIndex; 
  VkResult result = vkAcquireNextImageKHR(m_Device, m_SwapChain, UINT64_MAX, m_ImageAvailableSemaphores[m_CurrentFrame], VK_NULL_HANDLE, &imageIndex);


  //Check if we need to recreate swapchain and if we do we recreate it lol
  if (result == VK_ERROR_OUT_OF_DATE_KHR)
  {
    recreateSwapchain(
      m_Device, m_PhysicalDevice, m_Surface, 
      m_SwapChain, m_Window, m_SwapChainImages, 
      m_SwapChainImageFormat, m_SwapChainExtent, 
      m_SwapChainImageViews, m_RenderPass, m_SwapChainFramebuffers
    );
  } 
  else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
  {
    GenLogCritical("Failed to acquire swap chain image! In vulkan.cpp:drawFrame()");

  }

  vkResetFences(m_Device, 1, &m_InFlightFences[m_CurrentFrame]); //Reset manually

  //Record command buffer that draws scene to image
  vkResetCommandBuffer(m_CommandBuffers[m_CurrentFrame], 0);
  //recordCommandBuffer(pipeline, extent, swapChainFrameBuffers, renderpass, commandBuffer, imageIndex);
  recordCommandBuffer(m_GraphicsPipeline, m_SwapChainExtent, m_SwapChainFramebuffers, m_RenderPass, m_CommandBuffers[m_CurrentFrame], imageIndex);

  
  //Submit recorded command buffer
  VkSubmitInfo submitInfo{};
  submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

  VkSemaphore waitSemaphores[] = { m_ImageAvailableSemaphores[m_CurrentFrame]};
  VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
  submitInfo.waitSemaphoreCount = 1;
  submitInfo.pWaitSemaphores = waitSemaphores;
  submitInfo.pWaitDstStageMask = waitStages;

  submitInfo.commandBufferCount = 1;
  submitInfo.pCommandBuffers = &m_CommandBuffers[m_CurrentFrame];

  VkSemaphore signalSemaphores[] = { m_RenderFinishedSemaphores[m_CurrentFrame] }; 
  submitInfo.signalSemaphoreCount = 1;
  submitInfo.pSignalSemaphores = signalSemaphores;

    if (vkQueueSubmit(m_GraphicsQueue, 1, &submitInfo, m_InFlightFences[m_CurrentFrame]) != VK_SUCCESS)
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

  result = vkQueuePresentKHR(m_PresentQueue, &presentInfo); //Oh my LOOORD

  if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || m_FrameBufferResized)
  {
    m_FrameBufferResized = false; //Reset the flag of resetting
    recreateSwapchain(
      m_Device, m_PhysicalDevice, m_Surface, 
      m_SwapChain, m_Window, m_SwapChainImages, 
      m_SwapChainImageFormat, m_SwapChainExtent, 
      m_SwapChainImageViews, m_RenderPass, m_SwapChainFramebuffers
    );
  } 
  else if (result != VK_SUCCESS) 
  {
    GenLogCritical("Failed to present swap chain image! In vulkan.cpp:drawFrame()");
  }
 

  m_CurrentFrame = (m_CurrentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
}


void Vulkan::cleanup()
{
  cleanupSwapChain(m_Device, m_SwapChain, m_SwapChainFramebuffers, m_SwapChainImageViews);


  for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
  {
    vkDestroySemaphore(m_Device, m_ImageAvailableSemaphores[i], nullptr);
    vkDestroySemaphore(m_Device, m_RenderFinishedSemaphores[i], nullptr);
    vkDestroyFence(m_Device, m_InFlightFences[i], nullptr);
  }

  vkDestroyCommandPool(m_Device, m_CommandPool, nullptr);
  vkDestroyPipelineLayout(m_Device, m_PipelineLayout, nullptr);
  vkDestroyRenderPass(m_Device, m_RenderPass, nullptr);
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

