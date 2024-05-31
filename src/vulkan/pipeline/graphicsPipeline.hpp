#include "../../core.h"
#include "../../utils/readfile.cpp"


void createGraphicsPipelines(VkDevice& device, Gen_Graphics& graphicsInfo, const VkExtent2D& extent, VkDescriptorSetLayout& descripterLayout);
VkShaderModule createShaderModule(VkDevice device, const std::vector<char>& code);
