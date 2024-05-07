#include "window.hpp"
#include <GLFW/glfw3.h>
#include <iostream>
#include <ostream>
#include <stdatomic.h>
#include <vulkan/vulkan_core.h>
#include "commandpool.hpp"


GLFWwindow* createWindow()
{
  glfwInit();
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
  GLFWwindow* window;
  window = glfwCreateWindow(WIDTH, HEIGHT, "Genesis", nullptr, nullptr);
  return window;
}


void createSurface(VkInstance& instance, GLFWwindow* window, VkSurfaceKHR& surface)
{
  if (glfwCreateWindowSurface(instance, window, nullptr, &surface) != VK_SUCCESS) 
  {
    GenLogCritical("Failed to create Window surface!");
  }
}


