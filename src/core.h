#pragma once

/*

Core libraries and datatypes for the project

 */

#include <vulkan/vulkan_core.h>
#define GLM_FORCE_DEFAULT_ALIGNED_GENTYPES
#include <glm/glm.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>




#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vulkan/vulkan.h>
#include "genlog/logger.hpp"
#include "genlog/log.hpp"
#include "math.h"


#define MAX_FRAMES_IN_FLIGHT 2 //Frames to be rendered at the same time
#define WIDTH 640
#define HEIGHT 480



struct SwapChainSupportDetails 
{
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
};


struct alignas(16) UniformBufferObject 
{
  glm::mat4 model;
  glm::mat4 view;
  glm::mat4 proj;
};

struct Gen_Devices
{
  VkDevice logical_device;
  VkPhysicalDevice physical_device = VK_NULL_HANDLE;
};

struct Gen_Graphics
{
  VkPipeline graphics_pipeline;
  VkPipelineLayout pipeline_layout;
  VkRenderPass render_pass;
};

struct Gen_Camera
{
  glm::vec3 cam_pos =   { 0.0f, 0.0f, 3.0f  };
  glm::vec3 cam_front = { 0.0f, 0.0f, -1.0f };
  glm::vec3 cam_up =    { 1.0f, 0.0f, 0.0f  };
};

struct Gen_Swapchain
{
  SwapChainSupportDetails swap_chain_support_details;
  VkSwapchainKHR swap_chain;
  std::vector<VkImage> swap_chain_images;
  VkFormat swap_chain_image_format;
  VkExtent2D swap_chain_extent;
  std::vector<VkImageView> swap_chain_image_views;
  std::vector<VkFramebuffer> swap_chain_framebuffers;
};

struct Gen_Buffers
{
  VkBuffer vertex_buffer;
  VkDeviceMemory vertex_buffer_memory;
  std::vector<VkBuffer> uniform_buffers;
  std::vector<VkDeviceMemory> uniform_buffers_memory;
  VkBuffer index_buffer;
  VkDeviceMemory index_buffer_memory;
  std::vector<void*> uniform_buffers_mapped;
  std::vector<VkCommandBuffer> command_buffers;
};

struct Gen_SyncObjects
{
  std::vector<VkSemaphore> image_available_semaphores; //Ready for render signal
  std::vector<VkSemaphore> render_finished_semaphores; //Rendering has finished signal
  std::vector<VkFence> in_flight_fences; //Only one frame rendering at a time
};

struct Gen_ImageTexture
{
  VkImage image;
  VkDeviceMemory texture_image_memory;
  VkImageView texture_image_view;
  VkSampler texture_sampler;
};

struct Gen_DescriptorSet
{
  VkDescriptorSetLayout descriptor_set_layout;
  VkDescriptorPool descriptor_pool;
  std::vector<VkDescriptorSet> descriptor_sets;
};

struct Gen_Window 
{
  GLFWwindow* window;
  VkSurfaceKHR surface;
  bool frame_buffer_resized;
};


struct Vertex 
  {
    glm::vec2 pos;
    glm::vec3 color;
    glm::vec2 tex_coord;

    static VkVertexInputBindingDescription get_binding_description() 
    {
        VkVertexInputBindingDescription binding_description{};
        binding_description.binding = 0;
        binding_description.stride = sizeof(Vertex);
        binding_description.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

        return binding_description;
    }

    static std::array<VkVertexInputAttributeDescription, 2> get_attribute_descriptions() 
    {
        std::array<VkVertexInputAttributeDescription, 2> attribute_descriptions{};

        attribute_descriptions[0].binding = 0;
        attribute_descriptions[0].location = 0;
        attribute_descriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
        attribute_descriptions[0].offset = offsetof(Vertex, pos);

        attribute_descriptions[1].binding = 0;
        attribute_descriptions[1].location = 1;
        attribute_descriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
        attribute_descriptions[1].offset = offsetof(Vertex, color);

    /*
        attribute_descriptions[2].binding = 0;
        attribute_descriptions[2].location = 2;
        attribute_descriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
        attribute_descriptions[2].offset = offsetof(Vertex, tex_coord);
    */

        return attribute_descriptions;
    }
};


#ifndef NDEBUG
//Things that happen with DEBUG mode on
  const bool DEBUG = true;
#else
//Things that happen with DEBUG mode off
  const bool VALIDATIONLAYER = false;
#endif



