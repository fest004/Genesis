#include "commandpool.hpp"
#include "core/queues.hpp"
#include <array>
#include <iostream>
#include <vulkan/vulkan_core.h>


void record_command_buffer(Gen_Graphics& graphics_info, Gen_Swapchain& swapchain, 
                         Gen_Buffers& buffer_info, 
                         Gen_DescriptorSet& desc, const std::vector<uint16_t> indices,
                         uint32_t& current_frame, uint32_t& index)
{
    VkCommandBufferBeginInfo begin_info{};
    begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    begin_info.flags = 0;
    begin_info.pInheritanceInfo = nullptr;

    VkCommandBuffer com_buffer = buffer_info.command_buffers[current_frame];

    if (vkBeginCommandBuffer(com_buffer, &begin_info) != VK_SUCCESS)
        GenLogCritical(("Failed to begin command buffer! In commandpool.cpp"));

    VkRenderPassBeginInfo render_pass_info{};
    render_pass_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    render_pass_info.renderPass = graphics_info.render_pass;

    render_pass_info.framebuffer = swapchain.swap_chain_framebuffers[index];
    render_pass_info.renderArea.offset = {0, 0};
    render_pass_info.renderArea.extent = swapchain.swap_chain_extent;
   
    VkClearValue clear_color = 
    {
        {{0.2f, 0.2f, 0.2f, 1.0f}} // TODO: Experiment with this color
    };
    render_pass_info.clearValueCount = 1;
    render_pass_info.pClearValues = &clear_color;

    vkCmdBeginRenderPass(buffer_info.command_buffers[current_frame], &render_pass_info, VK_SUBPASS_CONTENTS_INLINE);
    vkCmdBindPipeline(com_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphics_info.graphics_pipeline);

    VkViewport viewport{};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = static_cast<float>(swapchain.swap_chain_extent.width);
    viewport.height = static_cast<float>(swapchain.swap_chain_extent.height);
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    vkCmdSetViewport(com_buffer, 0, 1, &viewport);

    VkRect2D scissor{};
    scissor.offset = {0, 0};
    scissor.extent = swapchain.swap_chain_extent;
    vkCmdSetScissor(com_buffer, 0, 1, &scissor);

    VkBuffer vertex_buffers[] = {buffer_info.vertex_buffer};
    VkDeviceSize offsets[] = {0};

    vkCmdBindVertexBuffers(com_buffer, 0, 1, vertex_buffers, offsets);
    vkCmdBindIndexBuffer(com_buffer, buffer_info.index_buffer, 0, VK_INDEX_TYPE_UINT16);
    vkCmdBindDescriptorSets(com_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphics_info.pipeline_layout, 0, 1, &desc.descriptor_sets[current_frame], 0, nullptr);

    vkCmdDrawIndexed(com_buffer, static_cast<uint32_t>(indices.size()), 1, 0, 0, 0);

    vkCmdEndRenderPass(com_buffer);

    if (vkEndCommandBuffer(com_buffer) != VK_SUCCESS)
        GenLogCritical("Failed to end command buffer! In commandpool.cpp");
}

void create_command_buffers(VkDevice& device, VkCommandPool& command_pool, std::vector<VkCommandBuffer>& command_buffers)
{
    command_buffers.resize(MAX_FRAMES_IN_FLIGHT);

    VkCommandBufferAllocateInfo create_info{};
    create_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    create_info.commandPool = command_pool;
    create_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    create_info.commandBufferCount = static_cast<uint32_t>(command_buffers.size());

    if (vkAllocateCommandBuffers(device, &create_info, command_buffers.data()) != VK_SUCCESS)
        GenLogCritical("Failed to allocate command buffers! In commandpool.cpp");
}

void create_command_pool(Gen_Devices& devices, VkSurfaceKHR& surface, VkCommandPool& command_pool)
{
    QueueFamilyIndices family_indices = find_queue_families(devices.physical_device, surface);

    VkCommandPoolCreateInfo create_info{};
    create_info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    create_info.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT; // Record command buffer every frame
    create_info.queueFamilyIndex = family_indices.graphics_family.value();

    if (vkCreateCommandPool(devices.logical_device, &create_info, nullptr, &command_pool) != VK_SUCCESS)
        GenLogCritical("Failed to create Command Pool! In commandpool.cpp");
}

