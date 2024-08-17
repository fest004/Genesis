#pragma once

#include <cstdint>
#include <optional>
#include "../../core.h"

struct QueueFamilyIndices
{
  //Optional wrapper to indicate a non initialized value is not a valid queue family
  std::optional<uint32_t> graphics_family;
  std::optional<uint32_t> present_family;

  bool is_complete() 
  {
    return graphics_family.has_value() && present_family.has_value();
  }
};

QueueFamilyIndices find_queue_families(VkPhysicalDevice& device, VkSurfaceKHR& surface);

