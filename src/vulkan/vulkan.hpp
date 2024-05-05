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



class Vulkan
{
  public:
    int initVulkan(); //Initializes all relevant Vulkan modules
    void cleanup();
    int update();


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

    //Graphics
    VkPipeline m_GraphicsPipeline;
    VkPipelineLayout m_PipelineLayout;
    VkRenderPass m_RenderPass;

    GLFWwindow* m_Window;
    VkSurfaceKHR m_Surface;





    const std::vector<const char*> m_DeviceExtensions = 
    {
      VK_KHR_SWAPCHAIN_EXTENSION_NAME
    };


    

};
