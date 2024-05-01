#include "../core.h"
#include "../utils/readfile.cpp"


void createGraphicsPipelines(VkDevice& device, VkPipelineLayout& pipelineLayout, const VkExtent2D& extent);
VkShaderModule createShaderModule(VkDevice device, const std::vector<char>& code);
