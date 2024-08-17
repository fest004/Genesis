#include "swapchain.hpp"
#include <GLFW/glfw3.h>
#include <vulkan/vulkan_core.h>


struct SwapChainSupportDetails;


// Function to create the swap chain
void create_swap_chain(Gen_Devices& devices, Gen_Swapchain& swap_chain_info, Gen_Window& window_info)
{
    SwapChainSupportDetails swap_chain_support = query_swapchain_support(devices.physical_device, window_info.surface);

    VkSurfaceFormatKHR surface_format = choose_swap_surface_format(swap_chain_support.formats);
    VkPresentModeKHR present_mode = choose_swap_present_mode(swap_chain_support.presentModes);
    VkExtent2D extent = choose_swap_extent(swap_chain_support.capabilities, window_info.window);

    uint32_t image_count = swap_chain_support.capabilities.minImageCount+ 1;
    if (swap_chain_support.capabilities.maxImageCount > 0 && image_count > swap_chain_support.capabilities.maxImageCount) {
        image_count = swap_chain_support.capabilities.maxImageCount;
    }

    VkSwapchainCreateInfoKHR create_info{};
    create_info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    create_info.surface = window_info.surface;
    create_info.minImageCount = image_count;
    create_info.imageFormat = surface_format.format;
    create_info.imageColorSpace = surface_format.colorSpace;
    create_info.imageExtent = extent;
    create_info.imageArrayLayers = 1;
    create_info.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    QueueFamilyIndices indices = find_queue_families(devices.physical_device, window_info.surface);
    if (indices.graphics_family != indices.present_family) {
        uint32_t queue_family_indices[] = {indices.graphics_family.value(), indices.present_family.value()};
        create_info.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        create_info.queueFamilyIndexCount = 2;
        create_info.pQueueFamilyIndices = queue_family_indices;
    } else {
        create_info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        create_info.queueFamilyIndexCount = 0;
        create_info.pQueueFamilyIndices = nullptr;
    }

    create_info.preTransform = swap_chain_support.capabilities.currentTransform;
    create_info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    create_info.presentMode = present_mode;
    create_info.clipped = VK_TRUE;
    create_info.oldSwapchain = VK_NULL_HANDLE;

    if (vkCreateSwapchainKHR(devices.logical_device, &create_info, nullptr, &swap_chain_info.swap_chain) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create swapchain!");
    }

    uint32_t image_count_vk;
    vkGetSwapchainImagesKHR(devices.logical_device, swap_chain_info.swap_chain, &image_count_vk, nullptr);
    swap_chain_info.swap_chain_images.resize(image_count_vk);
    vkGetSwapchainImagesKHR(devices.logical_device, swap_chain_info.swap_chain, &image_count_vk, swap_chain_info.swap_chain_images.data());

    swap_chain_info.swap_chain_image_format = surface_format.format;
    swap_chain_info.swap_chain_extent = extent;
}

// Function to query swap chain support details
SwapChainSupportDetails query_swapchain_support(VkPhysicalDevice& device, VkSurfaceKHR& surface)
{
    SwapChainSupportDetails details;
    
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.capabilities);

    uint32_t format_count;
    vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &format_count, nullptr);

    if (format_count != 0) {
        details.formats.resize(format_count);
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &format_count, details.formats.data());
    }

    uint32_t present_mode_count;
    vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &present_mode_count, nullptr);

    if (present_mode_count != 0) {
        details.presentModes.resize(present_mode_count);
        vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &present_mode_count, details.presentModes.data());
    }

    return details;
}

// Function to choose the surface format
VkSurfaceFormatKHR choose_swap_surface_format(const std::vector<VkSurfaceFormatKHR>& available_formats)
{
    for (const auto& format : available_formats) {
        if (format.format == VK_FORMAT_B8G8R8A8_SRGB && format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
            return format;
        }
    }
    return available_formats[0];
}

// Function to choose the present mode
VkPresentModeKHR choose_swap_present_mode(const std::vector<VkPresentModeKHR>& available_present_modes)
{
    for (const auto& mode : available_present_modes) {
        if (mode == VK_PRESENT_MODE_MAILBOX_KHR) {
            return mode;
        }
    }
    return VK_PRESENT_MODE_FIFO_KHR;
}

// Function to choose the swap extent
VkExtent2D choose_swap_extent(const VkSurfaceCapabilitiesKHR& capabilities, GLFWwindow* window)
{
    if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
        return capabilities.currentExtent;
    }

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);

    VkExtent2D extent = {
        static_cast<uint32_t>(width),
        static_cast<uint32_t>(height)
    };

    extent.width = glm::clamp(extent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
    extent.height = glm::clamp(extent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

    return extent;
}

// Function to create image views for the swap chain
void create_image_views(VkDevice& device, Gen_Swapchain& swap_chain_info)
{
    swap_chain_info.swap_chain_image_views.resize(swap_chain_info.swap_chain_images.size());

    for (size_t i = 0; i < swap_chain_info.swap_chain_images.size(); i++) {
        VkImageViewCreateInfo create_info{};
        create_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        create_info.image = swap_chain_info.swap_chain_images[i];
        create_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
        create_info.format = swap_chain_info.swap_chain_image_format;
        create_info.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
        create_info.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
        create_info.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
        create_info.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
        create_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        create_info.subresourceRange.baseMipLevel = 0;
        create_info.subresourceRange.levelCount = 1;
        create_info.subresourceRange.baseArrayLayer = 0;
        create_info.subresourceRange.layerCount = 1;

        if (vkCreateImageView(device, &create_info, nullptr, &swap_chain_info.swap_chain_image_views[i]) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create image view!");
        }
    }
}

// Function to recreate the swap chain
void recreate_swapchain(Gen_Devices& devices, Gen_Swapchain& swap_chain_info, Gen_Window& window_info, VkRenderPass& render_pass)
{
    int width, height;
    glfwGetFramebufferSize(window_info.window, &width, &height);

    while (width == 0 || height == 0) {
        glfwGetFramebufferSize(window_info.window, &width, &height);
        glfwWaitEvents();
    }

    vkDeviceWaitIdle(devices.logical_device);

    cleanup_swap_chain(devices.logical_device, swap_chain_info);

    create_swap_chain(devices, swap_chain_info, window_info);
    create_image_views(devices.logical_device, swap_chain_info);
    create_framebuffers(devices.logical_device, swap_chain_info, render_pass);
}

// Function to clean up the swap chain
void cleanup_swap_chain(VkDevice& device, Gen_Swapchain& swap_info)
{
    for (auto framebuffer : swap_info.swap_chain_framebuffers) {
        vkDestroyFramebuffer(device, framebuffer, nullptr);
    }

    for (auto image_view : swap_info.swap_chain_image_views) {
        vkDestroyImageView(device, image_view, nullptr);
    }

    vkDestroySwapchainKHR(device, swap_info.swap_chain, nullptr);
}

