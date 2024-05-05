#include "../core.h"
#include "../utils/readfile.cpp"



void createGraphicsPipelines(VkDevice& device, VkPipeline& graphicsPipeline, VkPipelineLayout& pipelineLayout, const VkExtent2D& extent, VkRenderPass& renderpass);
VkShaderModule createShaderModule(VkDevice device, const std::vector<char>& code);
