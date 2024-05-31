#pragma once

#include <cstdint>
#include <optional>
#include "../../core.h"

struct QueueFamilyIndices
{
  //Optional wrapper to indicate a non initialized value is not a valid queue family
  std::optional<uint32_t> graphicsFamily;
  std::optional<uint32_t> presentFamily;

  bool isComplete() 
  {
    return graphicsFamily.has_value() && presentFamily.has_value();
  }
};

QueueFamilyIndices findQueueFamilies(VkPhysicalDevice& device, VkSurfaceKHR& surface);

