#include "graphicsPipeline.hpp"



void createGraphicsPipelines(VkDevice& device)
{
  auto vertexShader = readFile("../src/shaders/vert.spv");
  auto fragShader =   readFile("../src/shaders/frag.spv");

  VkShaderModule vertexModule = createShaderModule(device, vertexShader);
  VkShaderModule fragModule = createShaderModule(device, fragShader);

  //Vertex createInfo
  VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
  vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
  vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
  vertShaderStageInfo.module = vertexModule;
  vertShaderStageInfo.pName = "main";


  //Fragment createInfo
  VkPipelineShaderStageCreateInfo fragShaderStageInfo{};
  fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
  fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
  fragShaderStageInfo.module = fragModule;
  fragShaderStageInfo.pName = "main";

  //CLEANUP
  vkDestroyShaderModule(device, vertexModule, nullptr);
  vkDestroyShaderModule(device, fragModule, nullptr);
}

VkShaderModule createShaderModule(VkDevice device, const std::vector<char>& code)
{
  VkShaderModuleCreateInfo createInfo{};
  createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
  createInfo.codeSize = code.size();
  createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

  VkShaderModule shaderModule;
  if (vkCreateShaderModule(device, &createInfo, nullptr, &shaderModule) != VK_SUCCESS)
    GenLogCritical("Failed to create Shader Module! In file graphicsPipelines.cpp")

  
  return shaderModule;
}
