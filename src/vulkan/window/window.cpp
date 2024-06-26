#include "window.hpp"
#include <GLFW/glfw3.h>
#include <iostream>
#include <ostream>
#include <vulkan/vulkan_core.h>



static void frameBufferResizedCallback(GLFWwindow* window, int width, int height);

GLFWwindow* createWindow(bool* windowResized)
{
  glfwInit();
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
  GLFWwindow* window;

  window = glfwCreateWindow(WIDTH, HEIGHT, "Genesis", nullptr, nullptr);
  glfwSetWindowUserPointer(window, windowResized);
  glfwSetFramebufferSizeCallback(window, frameBufferResizedCallback);
  return window;
}

static void frameBufferResizedCallback(GLFWwindow* window, int width, int height)
{
  auto flag = reinterpret_cast<bool*>(glfwGetWindowUserPointer(window));
  *flag = true;
  if (DEBUG)
    GenLogTrace("Window resized!");
}


void createSurface(VkInstance& instance, Gen_Window& windowInfo)
{
  if (glfwCreateWindowSurface(instance, windowInfo.window, nullptr, &windowInfo.surface) != VK_SUCCESS) 
  {
    GenLogCritical("Failed to create Window surface!");
  }
}


