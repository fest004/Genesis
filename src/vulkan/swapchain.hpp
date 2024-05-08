#pragma once

#include "../core.h"
#include "queues.hpp"
#include "frameBuffers.hpp"


// In swapchain.hpp
struct SwapChainSupportDetails {
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
};


void createSwapChain(VkDevice& device, VkPhysicalDevice& physicalDevice, 
    VkSurfaceKHR& surface, VkSwapchainKHR& swapChain, GLFWwindow* window, 
    std::vector<VkImage>& swapChainImages, VkFormat& swapChainImageFormat, VkExtent2D& swapChainExtent);

SwapChainSupportDetails querySwapchainSupport(VkPhysicalDevice& device, VkSurfaceKHR& surface);
VkSurfaceFormatKHR chooseSwapSurfaceFormat(std::vector<VkSurfaceFormatKHR> availableFormats);
VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR> availablePresentModes);
VkExtent2D chooseSwapExtent(VkSurfaceCapabilitiesKHR& capabilities, GLFWwindow* window);
void createImageViews(VkDevice& device,std::vector<VkImage>& swapChainImages, std::vector<VkImageView>& swapChainImageViews, VkFormat& swapChainImageFormat);


void recreateSwapchain(
  VkDevice& device, VkPhysicalDevice& physicalDevice, 
    VkSurfaceKHR& surface, VkSwapchainKHR& swapChain, GLFWwindow* window, 
    std::vector<VkImage>& swapChainImages, VkFormat& swapChainImageFormat, VkExtent2D& swapChainExtent,
    std::vector<VkImageView>& swapChainImageViews, VkRenderPass& renderPass, 
    std::vector<VkFramebuffer>& swapChainFramebuffers 
                       );


void cleanupSwapChain(VkDevice& device, VkSwapchainKHR& swapchain, std::vector<VkFramebuffer>& framebuffers, std::vector<VkImageView>& imageViews);

 
