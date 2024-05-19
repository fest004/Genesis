#pragma once

/*

Core libraries and datatypes for the project

 */

#define GLM_FORCE_DEFAULT_ALIGNED_GENTYPES
#include <glm/glm.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>




#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vulkan/vulkan.h>
#include "genlog/logger.hpp"
#include "genlog/log.hpp"
#include "math.h"


#define MAX_FRAMES_IN_FLIGHT 2 //Frames to be rendered at the same time
#define WIDTH 640
#define HEIGHT 480


struct alignas(16) UniformBufferObject 
{
  glm::mat4 model;
  glm::mat4 view;
  glm::mat4 proj;
};




#ifndef NDEBUG
//Things that happen with DEBUG mode on
  const bool DEBUG = true;
#else
//Things that happen with DEBUG mode off
  const bool VALIDATIONLAYER = false;
#endif



