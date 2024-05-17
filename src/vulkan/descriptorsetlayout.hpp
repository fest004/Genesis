#include "../core.h"



void createDescriptorSets(VkDevice& device, std::vector<VkBuffer>& uniformBuffers, VkDescriptorPool& descriptorPool, VkDescriptorSetLayout& descriptorLayout, std::vector<VkDescriptorSet>& descriptorSets);


void createDescriptorSetLayout(VkDevice& device, VkDescriptorSetLayout& descriptorLayout, VkPipelineLayout& pipelineLayout);
void createDescriptorPool(VkDevice& device, VkDescriptorPool& descriptorPool);



