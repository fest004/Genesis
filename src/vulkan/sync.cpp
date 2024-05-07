
#include "../core.h"
#include <vulkan/vulkan_core.h>


void createSyncObjects(VkDevice& device, std::vector<VkSemaphore>& availableSems, std::vector<VkSemaphore>& fRenderSems, std::vector<VkFence>& fences)
{
  availableSems.resize(MAX_FRAMES_IN_FLIGHT);
  fRenderSems.resize(MAX_FRAMES_IN_FLIGHT);
  fences.resize(MAX_FRAMES_IN_FLIGHT);


  VkSemaphoreCreateInfo semaphoreInfo{};
  semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO; 

  VkFenceCreateInfo fenceInfo{};
  fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
  fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;



  for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
  {

  if (
      vkCreateSemaphore(device, &semaphoreInfo, nullptr, &availableSems[i]) != VK_SUCCESS ||
      vkCreateSemaphore(device, &semaphoreInfo, nullptr, &fRenderSems[i])   != VK_SUCCESS || 
      vkCreateFence(device, &fenceInfo, nullptr, &fences[i]) != VK_SUCCESS
     )
    {
      GenLogCritical("Failed to create sync objects! In sync.cpp");
    }
  }

}

