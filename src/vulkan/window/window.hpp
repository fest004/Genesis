#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include "../../core.h"


GLFWwindow* create_window(bool* window_resized);
void create_surface(VkInstance& instance, Gen_Window& window_info);

