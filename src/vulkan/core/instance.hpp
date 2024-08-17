#pragma once

#include "vulkan/vulkan.h"
#include <GLFW/glfw3.h>
#include <cstdio>
#include "../../core.h"

int create_instance(VkInstance& instance, const std::vector<const char*>& validation_layers);
int check_external_extensions(const char** glfw_extensions, int arr_size, std::vector<VkExtensionProperties>& extensions);
std::vector<const char*> get_required_extensions();

