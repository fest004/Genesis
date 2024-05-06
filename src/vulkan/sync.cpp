
#include "../core.h"
#include <vulkan/vulkan_core.h>


void createSyncObjects(VkDevice& device, VkSemaphore& available, VkSemaphore& fRender, VkFence& fence)
{
  VkSemaphoreCreateInfo semaphoreInfo{};
  semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO; 

  VkFenceCreateInfo fenceInfo{};
  fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
  fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;



  if (
      vkCreateSemaphore(device, &semaphoreInfo, nullptr, &available) != VK_SUCCESS ||
      vkCreateSemaphore(device, &semaphoreInfo, nullptr, &fRender)   != VK_SUCCESS || 
      vkCreateFence(device, &fenceInfo, nullptr, &fence) != VK_SUCCESS
     )
  {
    GenLogCritical("Failed to create sync objects! In sync.cpp");
  }

}

