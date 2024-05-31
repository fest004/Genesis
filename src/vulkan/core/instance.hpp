#pragma once

#include "vulkan/vulkan.h"
#include <GLFW/glfw3.h>
#include <cstdio>
#include "../../core.h"

int createInstance(VkInstance& instance, const std::vector<const char*>& validationLayers);
int checkExternalExtensions(const char** glfwExtensions, int arrSize, std::vector<VkExtensionProperties>& extensions);
std::vector<const char*> getRequiredExtensions();

