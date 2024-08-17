#include "../../core.h"

// TODO find general way of reducing the amounts of paramaters without making it
// overly object oriented


uint32_t find_memory_type(VkPhysicalDevice& physical_device, uint32_t type_filter, VkMemoryPropertyFlags properties);

void create_framebuffers(const VkDevice& device, Gen_Swapchain& swap_chain_info, VkRenderPass& renderpass);

void create_vertex_buffer(Gen_Devices& devices, Gen_Buffers& buffers, VkQueue& graphics_queue, VkCommandPool& command_pool, const std::vector<Vertex>& vertices);


void create_buffer(Gen_Devices& devices, VkBuffer& buffer, 
                  VkDeviceMemory& device_memory, VkDeviceSize size, 
                  VkBufferUsageFlags usage, 
                  VkMemoryPropertyFlags properties);



void copy_buffer(VkDevice device, VkCommandPool& command_pool, VkQueue& queue, VkBuffer src_buffer, VkBuffer dst_buffer, VkDeviceSize size);
//void copy_buffer(VkDevice device, VkCommandPool& command_pool, VkQueue queue, VkBuffer src_buffer, VkBuffer dst_buffer, VkDeviceSize size);

void create_index_buffer(Gen_Devices& devices, Gen_Buffers& buffer_info, VkQueue& graphics_queue, VkCommandPool& command_pool, const std::vector<uint16_t>& indices);


VkCommandBuffer begin_single_time_commands(VkDevice& device, VkCommandPool& command_pool);
void end_single_time_commands(VkDevice& device, VkQueue& graphics_queue, VkCommandPool& command_pool, VkCommandBuffer& command_buffer);



