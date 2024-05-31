
#include "../../core.h"
#include <vulkan/vulkan_core.h>


void createSyncObjects(VkDevice& device, Gen_SyncObjects& syncObjects)
{
  syncObjects.imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
  syncObjects.renderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
  syncObjects.inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);


  VkSemaphoreCreateInfo semaphoreInfo{};
  semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO; 

  VkFenceCreateInfo fenceInfo{};
  fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
  fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;



  for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
  {

  if (
      vkCreateSemaphore(device, &semaphoreInfo, nullptr, &syncObjects.imageAvailableSemaphores[i]) != VK_SUCCESS ||
      vkCreateSemaphore(device, &semaphoreInfo, nullptr, &syncObjects.renderFinishedSemaphores[i])   != VK_SUCCESS || 
      vkCreateFence(device, &fenceInfo, nullptr, &syncObjects.inFlightFences[i]) != VK_SUCCESS
     )
    {
      GenLogCritical("Failed to create sync objects! In sync.cpp");
    }
  }

}

