#pragma once

/*

Core libraries and datatypes for the project

 */
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vulkan/vulkan.h>
#include "genlog/logger.hpp"
#include "genlog/log.hpp"
#include "math.h"


#ifndef NDEBUG
//Things that happen with DEBUG mode on
  const bool DEBUG = true;
#else
//Things that happen with DEBUG mode off
  const bool VALIDATIONLAYER = false;
#endif



