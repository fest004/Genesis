#include "../core.h"
#include "../utils/readfile.cpp"

void createGraphicsPipelines(VkDevice& device);
VkShaderModule createShaderModule(VkDevice device, const std::vector<char>& code);
