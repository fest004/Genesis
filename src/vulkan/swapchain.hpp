#pragma once

#include "../core.h"
#include "core/queues.hpp"
#include "resources/frameBuffers.hpp"


// In swapchain.hpp

void create_swap_chain(Gen_Devices& devices, Gen_Swapchain& swap_chain_info, Gen_Window& window_info);

SwapChainSupportDetails query_swapchain_support(VkPhysicalDevice& device, VkSurfaceKHR& surface);
VkSurfaceFormatKHR choose_swap_surface_format(const std::vector<VkSurfaceFormatKHR>& available_formats);
VkPresentModeKHR choose_swap_present_mode(const std::vector<VkPresentModeKHR>& available_present_modes);

VkExtent2D choose_swap_extent(const VkSurfaceCapabilitiesKHR& capabilities, GLFWwindow* window);

void create_image_views(VkDevice& device, Gen_Swapchain& swap_chain_info);
void recreate_swapchain(Gen_Devices& devices, Gen_Swapchain& swap_chain_info, Gen_Window& window_info, VkRenderPass& render_pass);
void cleanup_swap_chain(VkDevice& device, Gen_Swapchain& swap_info);


 
