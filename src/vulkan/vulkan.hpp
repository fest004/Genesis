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
    int init_vulkan(); //Initializes all relevant Vulkan modules
    void cleanup();
    int update();
    void draw_frame();
    uint32_t find_memory_type(uint32_t type_filter, VkMemoryPropertyFlags properties);


  private:

  //Runtime
  VkInstance m_vk_instance;
  VkDebugUtilsMessengerEXT m_debug_messenger;
  const std::vector<const char*> m_validation_layers = { "VK_LAYER_KHRONOS_validation" };

  Gen_Devices m_devices;
  Gen_Swapchain m_swapchain_info;
  Gen_Graphics m_graphics_info;
  Gen_Window m_window_info;
  Gen_Buffers m_buffer_info;
  Gen_ImageTexture m_image_texture_info;
  Gen_DescriptorSet m_descriptor_set_info;
  Gen_SyncObjects m_sync_info;

  //Queues
  VkQueue m_graphics_queue;
  VkQueue m_present_queue;

  VkCommandPool m_command_pool;

  uint32_t m_current_frame = 0;

  UniformBufferObject m_UBO;

  const std::vector<Vertex> m_vertices = 
  {
    {{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
    {{0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
    {{0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},
    {{-0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}}
  };


  const std::vector<uint16_t> m_indices
  {
    {0, 1, 2, 2, 3, 0}
  };


  const std::vector<const char*> m_device_extensions = 
  {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME
  };
};
