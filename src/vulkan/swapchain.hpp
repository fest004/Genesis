#pragma once

#include "../core.h"
#include "queues.hpp"


// In swapchain.hpp
struct SwapChainSupportDetails {
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
};


void createSwapChain(VkDevice& device, VkPhysicalDevice& physicalDevice, 
    VkSurfaceKHR& surface, VkSwapchainKHR& swapChain, GLFWwindow* window, 
    std::vector<VkImage> swapChainImages, VkFormat swapChainImageFormat, VkExtent2D swapChainExtent);
SwapChainSupportDetails querySwapchainSupport(VkPhysicalDevice& device, VkSurfaceKHR& surface);
VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR> availableFormats);
VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR> availablePresentModes);
VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities, GLFWwindow* window);

