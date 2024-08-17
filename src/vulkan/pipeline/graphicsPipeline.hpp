#include "../../core.h"
#include "../../utils/readfile.cpp"


void create_graphics_pipelines(VkDevice& device, Gen_Graphics& graphics_info, VkExtent2D& extent, VkDescriptorSetLayout& descripter_layout);
VkShaderModule create_shader_module(VkDevice device, const std::vector<char>& code);
