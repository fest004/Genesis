#include "../core.h"







void updateUniformBuffer(std::vector<void*>& uniformBuffersMapped, const uint32_t& currentImage, const VkExtent2D& extent);
void createUniformBuffers(VkDevice& device, VkPhysicalDevice& physDevice, std::vector<VkBuffer>& buffers, std::vector<VkDeviceMemory>& deviceMemories, std::vector<void*>& uniformBuffersMapped);


