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


#define MAX_FRAMES_IN_FLIGHT 2 //Frames to be rendered at the same time
#define WIDTH 640
#define HEIGHT 480




#ifndef NDEBUG
//Things that happen with DEBUG mode on
  const bool DEBUG = true;
#else
//Things that happen with DEBUG mode off
  const bool VALIDATIONLAYER = false;
#endif



