#include "../../core.h"


void create_descriptor_sets(VkDevice& device, std::vector<VkBuffer>& uniform_buffers, Gen_ImageTexture& gen_image_info, Gen_DescriptorSet& descriptor_info);

void create_descriptor_set_layout(VkDevice& device, VkDescriptorSetLayout& descriptor_layout, VkPipelineLayout& pipeline_layout);
void create_descriptor_pool(VkDevice& device, VkDescriptorPool& descriptor_pool);




