#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include "../core.h"



GLFWwindow* createWindow(bool* windowResized);
void createSurface(VkInstance& instance, GLFWwindow* window, VkSurfaceKHR& surface);

