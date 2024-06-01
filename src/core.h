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
  VkDevice logicalDevice;
  VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
};

struct Gen_Graphics
{
  VkPipeline graphicsPipeline;
  VkPipelineLayout pipelineLayout;
  VkRenderPass renderPass;
};

struct Gen_Swapchain
{
  SwapChainSupportDetails swapChainSupportDetails;
  VkSwapchainKHR swapChain;
  std::vector<VkImage> swapChainImages;
  VkFormat swapChainImageFormat;
  VkExtent2D swapChainExtent;
  std::vector<VkImageView> swapChainImageViews;
  std::vector<VkFramebuffer> swapChainFramebuffers;
};

struct Gen_Buffers
{
  VkBuffer vertexBuffer;
  VkDeviceMemory vertexBufferMemory;
  std::vector<VkBuffer> uniformBuffers;
  std::vector<VkDeviceMemory> uniformBuffersMemory;
  VkBuffer indexBuffer;
  VkDeviceMemory indexBufferMemory;
  std::vector<void*> uniformBuffersMapped;
  std::vector<VkCommandBuffer> commandBuffers;
};

struct Gen_SyncObjects
{
  std::vector<VkSemaphore> imageAvailableSemaphores; //Ready for render signal
  std::vector<VkSemaphore> renderFinishedSemaphores; //Rendering has finished signal
  std::vector<VkFence> inFlightFences; //Only one frame rendering at a time
};

struct Gen_ImageTexture
{
  VkImage image;
  VkDeviceMemory textureImageMemory;
  VkImageView textureImageView;
  VkSampler textureSampler;
};

struct Gen_DescriptorSet
{
  VkDescriptorSetLayout descriptorSetLayout;
  VkDescriptorPool descriptorPool;
  std::vector<VkDescriptorSet> descriptorsSets;
};

struct Gen_Window 
{
  GLFWwindow* window;
  VkSurfaceKHR surface;
  bool frameBufferResized;
};


struct Vertex 
  {
    glm::vec2 pos;
    glm::vec3 color;
    glm::vec2 texCoord;

    static VkVertexInputBindingDescription getBindingDescription() 
    {
        VkVertexInputBindingDescription bindingDescription{};
        bindingDescription.binding = 0;
        bindingDescription.stride = sizeof(Vertex);
        bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

        return bindingDescription;
    }

    static std::array<VkVertexInputAttributeDescription, 3> getAttributeDescriptions() 
    {
        std::array<VkVertexInputAttributeDescription, 3> attributeDescriptions{};

        attributeDescriptions[0].binding = 0;
        attributeDescriptions[0].location = 0;
        attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
        attributeDescriptions[0].offset = offsetof(Vertex, pos);

        attributeDescriptions[1].binding = 0;
        attributeDescriptions[1].location = 1;
        attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[1].offset = offsetof(Vertex, color);

        attributeDescriptions[2].binding = 0;
        attributeDescriptions[2].location = 2;
        attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
        attributeDescriptions[2].offset = offsetof(Vertex, texCoord);

        return attributeDescriptions;
    }
};


#ifndef NDEBUG
//Things that happen with DEBUG mode on
  const bool DEBUG = true;
#else
//Things that happen with DEBUG mode off
  const bool VALIDATIONLAYER = false;
#endif



