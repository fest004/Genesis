#include "../../core.h"
#include <cstring>
#include <vulkan/vulkan_core.h>
#include "frameBuffers.hpp"
#include "layouttransition.hpp"


void createImage(VkDevice& device, VkPhysicalDevice& physDevice, VkDeviceMemory& textureImageMem, uint32_t width, uint32_t height, VkFormat format, 
                 VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);


void createImageTexture(Gen_Devices& devices, Gen_ImageTexture& imageInfo, VkQueue& graphicsQueue, VkCommandPool commandPool,  const char* filepath);




void createImage(Gen_Devices& devices, VkDeviceMemory& textureImageMem, uint32_t width, uint32_t height, VkFormat format, 
                 VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);



void copyBufferToImage(VkDevice& device, VkQueue& graphicsQueue, VkCommandPool& commandPool, VkBuffer& buffer, VkImage& image, uint32_t width, uint32_t height);

void createTextureImageView(VkDevice& device, Gen_ImageTexture& imageInfo);


void createTextureSampler(Gen_Devices& devices, VkSampler& sampler);
