#include "uniformbuffers.hpp"
#include <chrono>
#include <cstdio>
#include <vulkan/vulkan_core.h>
#include "frameBuffers.hpp"





void update_uniform_buffer(std::vector<void*>& uniform_buffers_mapped, const uint32_t& current_image, const VkExtent2D& extent, Gen_Camera camera)
{
  static auto start_time = std::chrono::high_resolution_clock::now();

  auto current_time = std::chrono::high_resolution_clock::now();
  float time = std::chrono::duration<float, std::chrono::seconds::period>(current_time - start_time).count();
  
  UniformBufferObject ubo{};


  GenLogTrace("Camera Position: x={}, y={}, z={}", camera.cam_pos.x, camera.cam_pos.y, camera.cam_pos.z);

  //Rotating the current image by a factor scaling with time passed
  ubo.model = glm::rotate(glm::mat4(1.0f), time * glm::radians(180.0f * 1), glm::vec3(0.0f, 0.0f, 1.0f));

  /* Camera stuff */
  ubo.view = glm::lookAt(camera.cam_pos, camera.cam_pos + camera.cam_front, camera.cam_up);

  ubo.proj = glm::perspective(glm::radians(60.0f), extent.width / (float)extent.height, 0.1f, 10.0f);


  ubo.proj[1][1] *= -1;

  memcpy(uniform_buffers_mapped[current_image], &ubo, sizeof(ubo));
}


void create_uniform_buffers(Gen_Devices& devices, Gen_Buffers& buffer_info)
{
  VkDeviceSize size = sizeof(UniformBufferObject);

  buffer_info.uniform_buffers.resize(MAX_FRAMES_IN_FLIGHT);
  buffer_info.uniform_buffers_memory.resize(MAX_FRAMES_IN_FLIGHT);
  buffer_info.uniform_buffers_mapped.resize(MAX_FRAMES_IN_FLIGHT);

  for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
  {
    create_buffer(devices, buffer_info.uniform_buffers[i], buffer_info.uniform_buffers_memory[i], size, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
    vkMapMemory(devices.logical_device, buffer_info.uniform_buffers_memory[i], 0, size, 0,  &buffer_info.uniform_buffers_mapped[i]);
  }

}
