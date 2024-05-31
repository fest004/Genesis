#include "../../core.h"



void createDescriptorSets(VkDevice& device, std::vector<VkBuffer>& uniformBuffers, Gen_ImageTexture& imageInfo, Gen_DescriptorSet& descriptorInfo);
void createDescriptorSetLayout(VkDevice& device, VkDescriptorSetLayout& descriptorLayout, VkPipelineLayout& pipelineLayout);
void createDescriptorPool(VkDevice& device, VkDescriptorPool& descriptorPool);




