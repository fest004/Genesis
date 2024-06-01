#include "uniformbuffers.hpp"
#include <chrono>
#include <vulkan/vulkan_core.h>
#include "frameBuffers.hpp"





void updateUniformBuffer(std::vector<void*>& uniformBuffersMapped, const uint32_t& currentImage, const VkExtent2D& extent)
{
  static auto startTime = std::chrono::high_resolution_clock::now();

  auto currentTime = std::chrono::high_resolution_clock::now();
  float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();
  
  UniformBufferObject ubo{};

  //Rotating the current image by a factor scaling with time passed
  ubo.model = glm::rotate(glm::mat4(1.0f), time * glm::radians(180.0f * 1), glm::vec3(0.0f, 0.0f, 1.0f));

  //Look at above 45 deg angle
  ubo.view = glm::lookAt(glm::vec3(2.0f, 2.0f, 3.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));

  ubo.proj = glm::perspective(glm::radians(45.0f), extent.width / (float)extent.height, 0.1f, 10.0f);


  ubo.proj[1][1] *= -1;

  memcpy(uniformBuffersMapped[currentImage], &ubo, sizeof(ubo));
}


void createUniformBuffers(Gen_Devices& devices, Gen_Buffers& bufferInfo)
{
  VkDeviceSize size = sizeof(UniformBufferObject);

  bufferInfo.uniformBuffers.resize(MAX_FRAMES_IN_FLIGHT);
  bufferInfo.uniformBuffersMemory.resize(MAX_FRAMES_IN_FLIGHT);
  bufferInfo.uniformBuffersMapped.resize(MAX_FRAMES_IN_FLIGHT);

  for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
  {
    createBuffer(devices, bufferInfo.uniformBuffers[i], bufferInfo.uniformBuffersMemory[i], size, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
    vkMapMemory(devices.logicalDevice, bufferInfo.uniformBuffersMemory[i], 0, size, 0,  &bufferInfo.uniformBuffersMapped[i]);
  }

}
