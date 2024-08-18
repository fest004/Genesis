
#include "../../core.h"
#include <vulkan/vulkan_core.h>


void create_sync_objects(VkDevice& device, Gen_SyncObjects& sync_objects)
{
  sync_objects.image_available_semaphores.resize(MAX_FRAMES_IN_FLIGHT);
  sync_objects.render_finished_semaphores.resize(MAX_FRAMES_IN_FLIGHT);
  sync_objects.in_flight_fences.resize(MAX_FRAMES_IN_FLIGHT);


  VkSemaphoreCreateInfo semaphore_info{};
  semaphore_info.sType= VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO; 

  VkFenceCreateInfo fence_info{};
  fence_info.sType= VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
  fence_info.flags = VK_FENCE_CREATE_SIGNALED_BIT;



  for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
  {

  if (
      vkCreateSemaphore(device, &semaphore_info, nullptr, &sync_objects.image_available_semaphores[i]) != VK_SUCCESS ||
      vkCreateSemaphore(device, &semaphore_info, nullptr, &sync_objects.render_finished_semaphores[i])   != VK_SUCCESS || 
      vkCreateFence(device, &fence_info, nullptr, &sync_objects.in_flight_fences[i]) != VK_SUCCESS
     )
    {
     GenLogCritical("Failed to create sync objects! In sync.cpp");
    }
  }

}

