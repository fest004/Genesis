#include "swapchain.hpp"
#include <GLFW/glfw3.h>
#include <vulkan/vulkan_core.h>


struct SwapChainSupportDetails;


void createSwapChain(Gen_Devices& devices, Gen_Swapchain& swapChainInfo, Gen_Window& windowInfo)
{
  SwapChainSupportDetails swapChainSupport = querySwapchainSupport(devices.physicalDevice, windowInfo.surface);

  VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
  VkPresentModeKHR presentMode = chooseSwapPresentMode(swapChainSupport.presentModes);
  VkExtent2D extent = chooseSwapExtent(swapChainSupport.capabilities, windowInfo.window);

  uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
  if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount) {
      imageCount = swapChainSupport.capabilities.maxImageCount;
  }

  VkSwapchainCreateInfoKHR createInfo{};
  createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
  createInfo.surface = windowInfo.surface;

  createInfo.minImageCount = imageCount;
  createInfo.imageFormat = surfaceFormat.format;
  createInfo.imageColorSpace = surfaceFormat.colorSpace;
  createInfo.imageExtent = extent;
  createInfo.imageArrayLayers = 1;
  createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

  QueueFamilyIndices indices = findQueueFamilies(devices.physicalDevice, windowInfo.surface);
  uint32_t queueFamilyIndices[] = {indices.graphicsFamily.value(), indices.presentFamily.value()};

  if (indices.graphicsFamily != indices.presentFamily) 
  {
      createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
      createInfo.queueFamilyIndexCount = 2;
      createInfo.pQueueFamilyIndices = queueFamilyIndices;
  } 
  else 
  {
      createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
  }

  createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
  createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
  createInfo.presentMode = presentMode;
  createInfo.clipped = VK_TRUE;

  createInfo.oldSwapchain = VK_NULL_HANDLE;

  if (vkCreateSwapchainKHR(devices.logicalDevice, &createInfo, nullptr, &swapChainInfo.swapChain) != VK_SUCCESS) {
    GenLogCritical("Failed to create swapchain! In swapchain.cpp:vkCreateSwapChain()");
  }
  vkGetSwapchainImagesKHR(devices.logicalDevice, swapChainInfo.swapChain, &imageCount, nullptr);

  swapChainInfo.swapChainImages.resize(imageCount);
  vkGetSwapchainImagesKHR(devices.logicalDevice, swapChainInfo.swapChain, &imageCount, swapChainInfo.swapChainImages.data());

  swapChainInfo.swapChainImageFormat = surfaceFormat.format;
  swapChainInfo.swapChainExtent = extent;
}

SwapChainSupportDetails querySwapchainSupport(VkPhysicalDevice& device, VkSurfaceKHR& surface)
{
  SwapChainSupportDetails details;
  
  vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.capabilities);

  uint32_t formatCount;
  vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);

  if (formatCount != 0)
  {
    details.formats.resize(formatCount);
    vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, details.formats.data());
  }

  uint32_t presentModeCount;
  vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr);

  if (presentModeCount != 0)
  {
    details.presentModes.resize(presentModeCount);
    vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, details.presentModes.data());
  }
  return details;
}

//Surface Format
//Appriopiate Color format (SRBG is more accurate to percieved colors :D)
VkSurfaceFormatKHR chooseSwapSurfaceFormat(std::vector<VkSurfaceFormatKHR> availableFormats)
{
  for (const auto& availableFormat : availableFormats)
  {

    if (availableFormat.format     == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
    {
        return availableFormat;
    }
  }
  return availableFormats[0];
}

//Presentation Mode
//Find best option between the following: 
//VK_PRESENT_MODE_IMMEDIATE_KHR - Images are submitted right away
//VK_PRESENT_MODE_FIFO_KHR - Swap chain is queue(hence FIFO) of images. Displayed First and insert last again
//VK_PRESENT_MODE_FIFO_RELAXED_KHR Only differs from previous where in the queue is empty
//MAILBOX - Images queued are replaced by newer ones
VkPresentModeKHR chooseSwapPresentMode(std::vector<VkPresentModeKHR> availablePresentModes)
{
  for (const auto& availablePresentMode : availablePresentModes)
  {
    if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR)
    {
      return availablePresentMode;
    }
  }

  return VK_PRESENT_MODE_FIFO_KHR;
}


//Swap Extent
//Resolution of swap chain images. Often same as resolution of window
VkExtent2D chooseSwapExtent(VkSurfaceCapabilitiesKHR& capabilities, GLFWwindow* window)
{
  if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
  {
    return capabilities.currentExtent;
  }

  int width, height;
  glfwGetFramebufferSize(window, &width, &height);

  VkExtent2D actualExtent = 
  {
    static_cast<uint32_t>(width),
    static_cast<uint32_t>(height)
  };

  actualExtent.width = glm::clamp(actualExtent.width, 
                                  capabilities.minImageExtent.width, 
                                  capabilities.maxImageExtent.width);

  actualExtent.height= glm::clamp(actualExtent.height, 
                                  capabilities.minImageExtent.height, 
                                  capabilities.maxImageExtent.height);

  return actualExtent;
}

//Image view is... A view into an image! Like if they should be 2D with no depth or whatever
void createImageViews(VkDevice& device, Gen_Swapchain& swapChainInfo)
{
  swapChainInfo.swapChainImageViews.resize(swapChainInfo.swapChainImages.size());

  if (DEBUG)
    GenLogTrace("swapChainImages size: {}", swapChainInfo.swapChainImages.size());

  for (size_t i = 0; i < swapChainInfo.swapChainImages.size(); i++)
  {
    VkImageViewCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    createInfo.image = swapChainInfo.swapChainImages[i];

    //2D at the moment
    createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
    createInfo.format = swapChainInfo.swapChainImageFormat;

    //Identity colors
    createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
    createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
    createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
    createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;

    //Which part of the image should be accessed: Color targets with no mipmapping or multiple layers
    //Mipmapping are precaulculated optimized images of lower resolution, to lower render times and create higher LOD
    createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    createInfo.subresourceRange.baseMipLevel = 0; //TODO experiment with this
    createInfo.subresourceRange.levelCount = 1;
    createInfo.subresourceRange.baseArrayLayer = 0;
    createInfo.subresourceRange.layerCount = 1;

    //Lastly create image view
    if (vkCreateImageView(device, &createInfo, nullptr, &swapChainInfo.swapChainImageViews[i]) != VK_SUCCESS)
      GenLogCritical("Failed to create image view! File is: swapchain.cpp");
  }


}

void recreateSwapchain(Gen_Devices& devices, Gen_Swapchain& swapChainInfo, Gen_Window& windowInfo, VkRenderPass& renderPass)
{
  if (DEBUG)
    GenLogTrace("Recreate swapchain! In swapchain.cpp:recreateSwapchain()");

  int width, height = 0;
  glfwGetFramebufferSize(windowInfo.window, &width, &height);

  while (width == 0 || height == 0)
  {
    glfwGetFramebufferSize(windowInfo.window, &width, &width);
    glfwWaitEvents();
  }

  vkDeviceWaitIdle(devices.logicalDevice);

  createSwapChain(devices, swapChainInfo, windowInfo);

  createImageViews(devices.logicalDevice, swapChainInfo);



  createFrameBuffers(devices.logicalDevice, swapChainInfo, renderPass);
}

void cleanupSwapChain(VkDevice& device, Gen_Swapchain& swapInfo)
{
  for (auto framebuffer : swapInfo.swapChainFramebuffers)
  {
    vkDestroyFramebuffer(device, framebuffer,nullptr); 
  }

  for (auto imageView : swapInfo.swapChainImageViews)
  {
    vkDestroyImageView(device, imageView, nullptr);
  }

  vkDestroySwapchainKHR(device, swapInfo.swapChain, nullptr);
}


