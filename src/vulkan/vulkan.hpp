#pragma once

#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>
#include "core/instance.hpp"
#include "debug/validationLayers.hpp"
#include "core/physicalDevice.hpp"
#include "core/logicalDevice.hpp"
#include "window/window.hpp"
#include "swapchain.hpp"
#include "pipeline/graphicsPipeline.hpp"
#include "pipeline/renderpass.hpp"
#include "resources/frameBuffers.hpp"
#include "commandpool.hpp"
#include "sync/sync.hpp"
#include "resources/descriptorsetlayout.hpp"
#include "resources/uniformbuffers.hpp"
#include "resources/image.hpp"



class Vulkan
{
  public:
    int initVulkan(); //Initializes all relevant Vulkan modules
    void cleanup();
    int update();
    void drawFrame();
    uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);


  private:

  //Runtime
  VkInstance m_VkInstance;
  VkDebugUtilsMessengerEXT m_DebugMessenger;
  const std::vector<const char*> m_ValidationLayers = { "VK_LAYER_KHRONOS_validation" };

  Gen_Devices m_Devices;
  Gen_Swapchain m_SwapchainInfo;
  Gen_Graphics m_GraphicsInfo;
  Gen_Window m_WindowInfo;
  Gen_Buffers m_BufferInfo;
  Gen_ImageTexture m_ImageTextureInfo;
  Gen_DescriptorSet m_DescriptorSetInfo;
  Gen_SyncObjects m_SyncInfo;

  //Queues
  VkQueue m_GraphicsQueue;
  VkQueue m_PresentQueue;

  VkCommandPool m_CommandPool;

  uint32_t m_CurrentFrame = 0;

  UniformBufferObject m_UBO;

  const std::vector<Vertex> m_Vertices = 
  {
    {{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
    {{0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
    {{0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},
    {{-0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}}
  };


  const std::vector<uint16_t> m_Indices
  {
    {0, 1, 2, 2, 3, 0}
  };


  const std::vector<const char*> m_DeviceExtensions = 
  {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME
  };
};
