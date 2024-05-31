#pragma once

#include "../core.h"
#include "core/queues.hpp"
#include "resources/frameBuffers.hpp"


// In swapchain.hpp

void createSwapChain(Gen_Devices& devices, Gen_Swapchain& swapChainInfo, Gen_Window& windowInfo);

SwapChainSupportDetails querySwapchainSupport(VkPhysicalDevice& device, VkSurfaceKHR& surface);
VkSurfaceFormatKHR chooseSwapSurfaceFormat(std::vector<VkSurfaceFormatKHR> availableFormats);
VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR> availablePresentModes);
VkExtent2D chooseSwapExtent(VkSurfaceCapabilitiesKHR& capabilities, GLFWwindow* window);
void createImageViews(VkDevice& device, Gen_Swapchain& swapChainInfo);


void recreateSwapchain(
  VkDevice& device, VkPhysicalDevice& physicalDevice, 
    VkSurfaceKHR& surface, VkSwapchainKHR& swapChain, GLFWwindow* window, 
    std::vector<VkImage>& swapChainImages, VkFormat& swapChainImageFormat, VkExtent2D& swapChainExtent,
    std::vector<VkImageView>& swapChainImageViews, VkRenderPass& renderPass, 
    std::vector<VkFramebuffer>& swapChainFramebuffers 
                       );


void cleanupSwapChain(VkDevice& device, VkSwapchainKHR& swapchain, std::vector<VkFramebuffer>& framebuffers, std::vector<VkImageView>& imageViews);

 
