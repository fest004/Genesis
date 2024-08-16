
#include "vulkan.hpp"
#include "commandpool.hpp"
#include "core/physicalDevice.hpp"
#include "swapchain.hpp"
#include <GLFW/glfw3.h>
#include <cstdint>
#include <iostream>
#include <ostream>
#include <vulkan/vulkan_core.h>

int Vulkan::initVulkan()
{
  m_WindowInfo.window = createWindow(&m_WindowInfo.frameBufferResized);
  createInstance(m_VkInstance, m_ValidationLayers); //Instance of vulkan, set up validation layers
  setupDebugMessenger(m_VkInstance, &m_DebugMessenger);
  createSurface(m_VkInstance, m_WindowInfo);
  pickPhysicalDevice(m_VkInstance, m_Devices, m_WindowInfo.surface, m_DeviceExtensions);
  createLogicalDevice(m_Devices, m_GraphicsQueue, m_PresentQueue, m_ValidationLayers, m_DeviceExtensions, m_WindowInfo.surface);
  createSwapChain(m_Devices, m_SwapchainInfo, m_WindowInfo);
  createImageViews(m_Devices.logicalDevice, m_SwapchainInfo);
  createRenderpass(m_Devices.logicalDevice, m_GraphicsInfo.renderPass, m_SwapchainInfo.swapChainImageFormat);
  createDescriptorSetLayout(m_Devices.logicalDevice, m_DescriptorSetInfo.descriptorSetLayout, m_GraphicsInfo.pipelineLayout);
  createGraphicsPipelines(m_Devices.logicalDevice, m_GraphicsInfo, m_SwapchainInfo.swapChainExtent, m_DescriptorSetInfo.descriptorSetLayout);
  createFrameBuffers(m_Devices.logicalDevice, m_SwapchainInfo, m_GraphicsInfo.renderPass);
  createCommandPool(m_Devices, m_WindowInfo.surface, m_CommandPool);
  createImageTexture(m_Devices, m_ImageTextureInfo, m_GraphicsQueue, m_CommandPool, "../images/sanic.png");
  createTextureImageView(m_Devices.logicalDevice, m_ImageTextureInfo);
  createTextureSampler(m_Devices, m_ImageTextureInfo.textureSampler);
  createVertexBuffer(m_Devices, m_BufferInfo, m_GraphicsQueue, m_CommandPool, m_Vertices);
  createIndexBuffer(m_Devices, m_BufferInfo, m_GraphicsQueue, m_CommandPool, m_Indices, m_Vertices);
  createUniformBuffers(m_Devices, m_BufferInfo);
  createDescriptorPool(m_Devices.logicalDevice, m_DescriptorSetInfo.descriptorPool);
  createDescriptorSets(m_Devices.logicalDevice, m_BufferInfo.uniformBuffers, m_ImageTextureInfo, m_DescriptorSetInfo);
  createCommandBuffers(m_Devices.logicalDevice, m_CommandPool, m_BufferInfo.commandBuffers);
  createSyncObjects(m_Devices.logicalDevice, m_SyncInfo);
  return 1;
}

int Vulkan::update()
{
  if (!glfwWindowShouldClose(m_WindowInfo.window))
  {
    glfwPollEvents();
    drawFrame();
  }
  vkDeviceWaitIdle(m_Devices.logicalDevice);

  return !glfwWindowShouldClose(m_WindowInfo.window);
}

void Vulkan::drawFrame()
{
  //Wait for previous frame to finish
  vkWaitForFences(m_Devices.logicalDevice, 1, &m_SyncInfo.inFlightFences[m_CurrentFrame], VK_TRUE, UINT64_MAX); //Wait for fence  that tells us we are not rendering anything currently 
  
  //Acquire new image from swapchain
  uint32_t imageIndex; 
  VkResult result = vkAcquireNextImageKHR(m_Devices.logicalDevice, m_SwapchainInfo.swapChain, 
                                          UINT64_MAX, m_SyncInfo.imageAvailableSemaphores[m_CurrentFrame], 
                                          VK_NULL_HANDLE, &imageIndex);


  //Check if we need to recreate swapchain and if we do we recreate it lol
  if (result == VK_ERROR_OUT_OF_DATE_KHR)
  {
    recreateSwapchain(m_Devices, m_SwapchainInfo, m_WindowInfo, m_GraphicsInfo.renderPass);
  } 
  else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
  {
    GenLogCritical("Failed to acquire swap chain image! In vulkan.cpp:drawFrame()");

  }


  updateUniformBuffer(m_BufferInfo.uniformBuffersMapped, m_CurrentFrame, m_SwapchainInfo.swapChainExtent);
  vkResetFences(m_Devices.logicalDevice, 1, &m_SyncInfo.inFlightFences[m_CurrentFrame]); //Reset manually

  //Record command buffer that draws scene to image
  vkResetCommandBuffer(m_BufferInfo.commandBuffers[m_CurrentFrame], 0);



 
  recordCommandBuffer(m_GraphicsInfo, m_SwapchainInfo, m_BufferInfo, m_DescriptorSetInfo, m_Indices, m_CurrentFrame, imageIndex);
  
  //Submit recorded command buffer
  VkSubmitInfo submitInfo{};
  submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

  VkSemaphore waitSemaphores[] = { m_SyncInfo.imageAvailableSemaphores[m_CurrentFrame]};
  VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
  submitInfo.waitSemaphoreCount = 1;
  submitInfo.pWaitSemaphores = waitSemaphores;
  submitInfo.pWaitDstStageMask = waitStages;

  submitInfo.commandBufferCount = 1;
  submitInfo.pCommandBuffers = &m_BufferInfo.commandBuffers[m_CurrentFrame];

  VkSemaphore signalSemaphores[] = { m_SyncInfo.renderFinishedSemaphores[m_CurrentFrame] }; 
  submitInfo.signalSemaphoreCount = 1;
  submitInfo.pSignalSemaphores = signalSemaphores;

  if (vkQueueSubmit(m_GraphicsQueue, 1, &submitInfo, m_SyncInfo.inFlightFences[m_CurrentFrame]) != VK_SUCCESS)
    GenLogCritical("Failed to submit draw command buffer! In window.cpp");

  //Present swap chain image

  VkPresentInfoKHR presentInfo{};
  presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
  presentInfo.waitSemaphoreCount = 1;
  presentInfo.pWaitSemaphores = signalSemaphores; 

  VkSwapchainKHR swapChains[] = { m_SwapchainInfo.swapChain };
  presentInfo.swapchainCount = 1;
  presentInfo.pSwapchains = swapChains;
  presentInfo.pImageIndices = &imageIndex;
  presentInfo.pResults = nullptr;

  result = vkQueuePresentKHR(m_PresentQueue, &presentInfo); //Oh my LOOORD

  if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || m_WindowInfo.frameBufferResized)
  {
    m_WindowInfo.frameBufferResized = false; //Reset the flag of resetting
    recreateSwapchain(m_Devices, m_SwapchainInfo, m_WindowInfo, m_GraphicsInfo.renderPass);
  } 
  else if (result != VK_SUCCESS) 
  {
    GenLogCritical("Failed to present swap chain image! In vulkan.cpp:drawFrame()");
  }
 

  m_CurrentFrame = (m_CurrentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
}


  

void Vulkan::cleanup()
{
  cleanupSwapChain(m_Devices.logicalDevice, m_SwapchainInfo);

  vkDestroyPipeline(m_Devices.logicalDevice, m_GraphicsInfo.graphicsPipeline, nullptr);
  vkDestroyPipelineLayout(m_Devices.logicalDevice, m_GraphicsInfo.pipelineLayout, nullptr);
  vkDestroyRenderPass(m_Devices.logicalDevice, m_GraphicsInfo.renderPass, nullptr);

  for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
  {
    vkDestroyBuffer(m_Devices.logicalDevice, m_BufferInfo.uniformBuffers[i], nullptr);
    vkFreeMemory(m_Devices.logicalDevice, m_BufferInfo.uniformBuffersMemory[i], nullptr);
    std::cout << "Uniformbuffermemory: " << i << " = " << &m_BufferInfo.uniformBuffersMemory[i] << "\n"; 
  }
  std::cout << "VertexBufferMemory: " << " = " << &m_BufferInfo.vertexBufferMemory << "\n"; 
  std::cout << "IndexBufferMemory: " << " = " << &m_BufferInfo.vertexBufferMemory << "\n"; 

  vkDestroyDescriptorPool(m_Devices.logicalDevice, m_DescriptorSetInfo.descriptorPool, nullptr);
  vkDestroySampler(m_Devices.logicalDevice, m_ImageTextureInfo.textureSampler, nullptr);
  vkDestroyImageView(m_Devices.logicalDevice, m_ImageTextureInfo.textureImageView, nullptr);
  vkDestroyImage(m_Devices.logicalDevice, m_ImageTextureInfo.image, nullptr);
  vkFreeMemory(m_Devices.logicalDevice, m_ImageTextureInfo.textureImageMemory, nullptr);
  vkDestroyDescriptorSetLayout(m_Devices.logicalDevice, m_DescriptorSetInfo.descriptorSetLayout, nullptr);
  
  vkDestroyBuffer(m_Devices.logicalDevice, m_BufferInfo.indexBuffer, nullptr);
  vkFreeMemory(m_Devices.logicalDevice, m_BufferInfo.indexBufferMemory, nullptr);

  vkDestroyBuffer(m_Devices.logicalDevice, m_BufferInfo.vertexBuffer, nullptr);
  vkFreeMemory(m_Devices.logicalDevice, m_BufferInfo.vertexBufferMemory, nullptr);

  for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
  {
    vkDestroySemaphore(m_Devices.logicalDevice, m_SyncInfo.imageAvailableSemaphores[i], nullptr);
    vkDestroySemaphore(m_Devices.logicalDevice, m_SyncInfo.renderFinishedSemaphores[i], nullptr);
    vkDestroyFence(m_Devices.logicalDevice, m_SyncInfo.inFlightFences[i], nullptr);
  }
  

  vkDestroyCommandPool(m_Devices.logicalDevice, m_CommandPool, nullptr);
  vkDeviceWaitIdle(m_Devices.logicalDevice);
  vkDestroyDevice(m_Devices.logicalDevice, nullptr);

  if (DEBUG)
    DestroyDebugUtilsMessengerEXT(m_VkInstance, m_DebugMessenger, nullptr);

  vkDestroySurfaceKHR(m_VkInstance, m_WindowInfo.surface, nullptr);
  vkDestroyInstance(m_VkInstance, nullptr);

  glfwDestroyWindow(m_WindowInfo.window);
  glfwTerminate();
}

