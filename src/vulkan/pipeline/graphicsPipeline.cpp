#include "graphicsPipeline.hpp"
#include <string>
#include <vulkan/vulkan_core.h>




VkShaderModule create_shader_module(VkDevice device, std::string filepath)
{
  auto shader = read_file(filepath);
  VkShaderModule shader_module = create_shader_module(device, shader);
  return shader_module;
}

VkPipelineShaderStageCreateInfo create_fragment_shader_info(VkShaderModule fragm_module)
{
  //Fragment create_info
  VkPipelineShaderStageCreateInfo frag_shader_stage_info{};
  frag_shader_stage_info.sType= VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
  frag_shader_stage_info.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
  frag_shader_stage_info.module = fragm_module;
  frag_shader_stage_info.pName= "main";

  return frag_shader_stage_info;
}

VkPipelineShaderStageCreateInfo create_vertex_shader_info(VkShaderModule vert_module)
{
  //Vertex create_info
  VkPipelineShaderStageCreateInfo frag_shader_stage_info{};
  frag_shader_stage_info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
  frag_shader_stage_info.stage = VK_SHADER_STAGE_VERTEX_BIT;
  frag_shader_stage_info.module = vert_module;
  frag_shader_stage_info.pName = "main";

  return frag_shader_stage_info;
}


void create_graphics_pipelines(VkDevice& device, Gen_Graphics& graphics_info, VkExtent2D& extent, VkDescriptorSetLayout& descripter_layout)
{
  auto frag_module   = create_shader_module(device, "../src/shaders/frag.spv");
  auto vertex_module = create_shader_module(device, "../src/shaders/vert.spv");
  auto vertex_info   = create_vertex_shader_info(vertex_module);
  auto frag_info     = create_fragment_shader_info(frag_module);

  VkPipelineShaderStageCreateInfo shader_stages[] = { vertex_info, frag_info };

  //Dynamic states can be changed without recreating the whole pipeline 
  //Viewport,
  //Scissor,
  //Line Width
  //Blend constants
  std::vector<VkDynamicState> dynamic_states = 
  {
    VK_DYNAMIC_STATE_VIEWPORT,
    VK_DYNAMIC_STATE_SCISSOR

  };

  VkPipelineDynamicStateCreateInfo dynamic_state{};
  dynamic_state.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
  dynamic_state.dynamicStateCount= static_cast<uint32_t>(dynamic_states.size());
  dynamic_state.pDynamicStates= dynamic_states.data();


  auto binding_description = Vertex::get_binding_description();
  auto attribute_description = Vertex::get_attribute_descriptions();

  VkPipelineVertexInputStateCreateInfo vertex_input_info{};
  vertex_input_info.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;

  vertex_input_info.vertexBindingDescriptionCount= 1;
  vertex_input_info.vertexAttributeDescriptionCount = static_cast<uint32_t>(attribute_description.size());
  vertex_input_info.pVertexBindingDescriptions = &binding_description;
  vertex_input_info.pVertexAttributeDescriptions = attribute_description.data();

  VkPipelineInputAssemblyStateCreateInfo input_assembly_info{};
  input_assembly_info.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
  input_assembly_info.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
  input_assembly_info.primitiveRestartEnable = VK_FALSE;

  VkViewport viewport{};
  viewport.x = 0.0f;
  viewport.y = 0.0f;
  viewport.width = extent.width; 
  viewport.width = extent.height;
  viewport.minDepth= 0.0f;
  viewport.maxDepth= 1.0f;

  VkRect2D scissor{};
  scissor.offset = { 0,0 };
  scissor.extent = extent; //TODO mby extent needs to not be const? 

  VkPipelineViewportStateCreateInfo viewport_state_info{};
  viewport_state_info.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
  viewport_state_info.viewportCount= 1;
  viewport_state_info.scissorCount= 1;

  VkPipelineViewportStateCreateInfo viewport_state{};
  viewport_state.sType= VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
  viewport_state.viewportCount= 1;
  viewport_state.pViewports = &viewport;
  viewport_state.scissorCount = 1;
  viewport_state.pScissors = &scissor;

  VkPipelineRasterizationStateCreateInfo rasterizer{};
  rasterizer.sType= VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
  rasterizer.depthClampEnable = VK_FALSE;
  rasterizer.rasterizerDiscardEnable = VK_FALSE;
  rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
  rasterizer.lineWidth = 1.0f;
  rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
  rasterizer.frontFace= VK_FRONT_FACE_COUNTER_CLOCKWISE;
  rasterizer.depthBiasEnable = VK_FALSE;
  rasterizer.depthBiasSlopeFactor = 0.0f;
  rasterizer.depthBiasClamp = 0.0f;
  rasterizer.depthBiasSlopeFactor = 0.0f;

  //Disable multisampling
  VkPipelineMultisampleStateCreateInfo multisampling{};
  multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
  multisampling.sampleShadingEnable = VK_FALSE;
  multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
  multisampling.minSampleShading = 1.0f;
  multisampling.pSampleMask = nullptr;
  multisampling.alphaToCoverageEnable = VK_FALSE;
  multisampling.alphaToOneEnable = VK_FALSE;

  //Color blending
  VkPipelineColorBlendAttachmentState color_blend_attachment{};
  color_blend_attachment.colorWriteMask = 
    VK_COLOR_COMPONENT_R_BIT | 
    VK_COLOR_COMPONENT_G_BIT | 
    VK_COLOR_COMPONENT_B_BIT | 
    VK_COLOR_COMPONENT_A_BIT ;
  color_blend_attachment.blendEnable = VK_FALSE;
  color_blend_attachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;
  color_blend_attachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;
  color_blend_attachment.colorBlendOp = VK_BLEND_OP_ADD; 
  color_blend_attachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE; 
  color_blend_attachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO; 

  VkPipelineColorBlendStateCreateInfo color_blending{};
  color_blending.sType= VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
  color_blending.logicOpEnable = VK_FALSE;
  color_blending.logicOp = VK_LOGIC_OP_COPY; 
  color_blending.attachmentCount= 1;
  color_blending.pAttachments = &color_blend_attachment;
  color_blending.blendConstants[0] = 0.0f; 
  color_blending.blendConstants[1] = 0.0f; 
  color_blending.blendConstants[2] = 0.0f; 
  color_blending.blendConstants[3] = 0.0f; 


  //Pipeline layout
  VkPipelineLayoutCreateInfo pipeline_layout_info{};
  pipeline_layout_info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
  pipeline_layout_info.setLayoutCount = 1;
  pipeline_layout_info.pSetLayouts = &descripter_layout;

  if (vkCreatePipelineLayout(device, &pipeline_layout_info, nullptr, &graphics_info.pipeline_layout) != VK_SUCCESS)
    GenLogCritical("Failed to create Graphics Pipeline Layout! In graphics_pipeline.cpp");


  VkGraphicsPipelineCreateInfo pipeline_info{};
  pipeline_info.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
  pipeline_info.stageCount = 2;
  pipeline_info.pStages = shader_stages;
  pipeline_info.pVertexInputState = &vertex_input_info;
  pipeline_info.pInputAssemblyState = &input_assembly_info;
  pipeline_info.pViewportState= &viewport_state_info;
  pipeline_info.pRasterizationState = &rasterizer;
  pipeline_info.pMultisampleState = &multisampling;
  pipeline_info.pDepthStencilState = nullptr;
  pipeline_info.pColorBlendState = &color_blending;
  pipeline_info.pDynamicState = &dynamic_state;

  pipeline_info.layout = graphics_info.pipeline_layout;
  pipeline_info.renderPass = graphics_info.render_pass;
  pipeline_info.subpass = 0;

  pipeline_info.basePipelineHandle = VK_NULL_HANDLE;
  pipeline_info.basePipelineIndex = -1;

  if (vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &pipeline_info, nullptr, &graphics_info.graphics_pipeline) != VK_SUCCESS)
    GenLogCritical("Failed to create graphics pipeline! In file graphics_pipeline.cpp");

  //CLEANUP
  vkDestroyShaderModule(device, vertex_module, nullptr);
  vkDestroyShaderModule(device, frag_module, nullptr);
}

VkShaderModule create_shader_module(VkDevice device, const std::vector<char>& code)
{
  VkShaderModuleCreateInfo create_info{};
  create_info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
  create_info.codeSize = code.size();
  create_info.pCode = reinterpret_cast<const uint32_t*>(code.data());

  VkShaderModule shader_module;
  if (vkCreateShaderModule(device, &create_info, nullptr, &shader_module) != VK_SUCCESS)
    GenLogCritical("Failed to create Shader Module! In file graphics_pipelines.cpp");

  
  return shader_module;
}
