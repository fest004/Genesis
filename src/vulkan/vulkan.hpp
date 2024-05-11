#pragma once

#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>
#include "instance.hpp"
#include "validationLayers.hpp"
#include "physicalDevice.hpp"
#include "logicalDevice.hpp"
#include "window.hpp"
#include "swapchain.hpp"
#include "graphicsPipeline.hpp"
#include "renderpass.hpp"
#include "frameBuffers.hpp"
#include "commandpool.hpp"
#include "sync.hpp"
#include "../shaders/vertices.hpp"



class Vulkan
{
  public:
    int initVulkan(); //Initializes all relevant Vulkan modules
    void cleanup();
    int update();
    void drawFrame();
    uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);



    //Getters and setters
    VkInstance getInstance() { return m_VkInstance; }; 
    const std::vector<const char*> getValidationLayers() { return m_ValidationLayers; }


  private:

    //Runtime
    VkInstance m_VkInstance;
    VkDebugUtilsMessengerEXT m_DebugMessenger;
    const std::vector<const char*> m_ValidationLayers = { "VK_LAYER_KHRONOS_validation" };

    //Devices
    VkPhysicalDevice m_PhysicalDevice = VK_NULL_HANDLE;
    VkDevice m_Device;

    //Queues
    VkQueue m_GraphicsQueue;
    VkQueue m_PresentQueue;


    //Swapchain 
    SwapChainSupportDetails m_SwapChainSupportDetails;
    VkSwapchainKHR m_SwapChain;
    std::vector<VkImage> m_SwapChainImages;
    VkFormat m_SwapChainImageFormat;
    VkExtent2D m_SwapChainExtent;
    std::vector<VkImageView> m_SwapChainImageViews;
    std::vector<VkFramebuffer> m_SwapChainFramebuffers;

    //Graphics
    VkPipeline m_GraphicsPipeline;
    VkPipelineLayout m_PipelineLayout;
    VkRenderPass m_RenderPass;

    //Window
    GLFWwindow* m_Window;
    VkSurfaceKHR m_Surface;
    bool m_FrameBufferResized = false;


    //Commands
    VkCommandPool m_CommandPool;
    std::vector<VkCommandBuffer> m_CommandBuffers;

    VkBuffer m_VertexBuffer;
    VkDeviceMemory m_VertexBufferMemory;

    //Sync control
    std::vector<VkSemaphore> m_ImageAvailableSemaphores; //Ready for render signal
    std::vector<VkSemaphore> m_RenderFinishedSemaphores; //Rendering has finished signal
    std::vector<VkFence> m_InFlightFences; //Only one frame rendering at a time
    uint32_t m_CurrentFrame = 0;


    const std::vector<Vertex> m_Vertices =
    {
        {{0.0f, -0.5f}, {1.0f, 1.0f, 1.0f}},
        {{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
        {{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}
    };


    const std::vector<const char*> m_DeviceExtensions = 
    {
      VK_KHR_SWAPCHAIN_EXTENSION_NAME
    };


    

};
