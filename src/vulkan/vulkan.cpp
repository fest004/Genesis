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
    drawFrame(m_Device, m_SwapChain, m_CommandBuffer, m_InFlightFence, m_ImageAvailableSemaphore, 
              m_RenderFinishedSemaphore, m_GraphicsPipeline, m_SwapChainExtent, m_SwapChainFramebuffers, m_RenderPass, m_GraphicsQueue, m_PresentQueue);

  }
  return 1;
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







