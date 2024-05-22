#include "../core.h"
#include <cstring>
#include <vulkan/vulkan_core.h>
#include "frameBuffers.hpp"
#include "layouttransition.hpp"


void createImage(VkDevice& device, VkPhysicalDevice& physDevice, VkDeviceMemory& textureImageMem, uint32_t width, uint32_t height, VkFormat format, 
                 VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);



void createImageTexture(VkDevice& device, VkCommandPool& commandPool, VkPhysicalDevice& physDevice, VkImage& image, VkDeviceMemory& textureImageMem, VkQueue& graphicsQueue, const char* filepath);





void createImage(VkDevice& device, VkPhysicalDevice& physDevice, VkDeviceMemory& textureImageMem, uint32_t width, uint32_t height, VkFormat format, 
                 VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);


void copyBufferToImage(VkDevice& device, VkQueue& graphicsQueue, VkCommandPool& commandPool, VkBuffer& buffer, VkImage& image, uint32_t width, uint32_t height);


void createTextureImageView(VkDevice& device, VkImage& image, VkImageView& textureImageView);


void createTextureSampler(VkDevice& device, VkPhysicalDevice& physDevice, VkSampler& sampler);
