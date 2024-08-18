#include "window.hpp"
#include <GLFW/glfw3.h>
#include <iostream>
#include <ostream>
#include <vulkan/vulkan_core.h>


#include "../vulkan.hpp"



static void frame_buffer_resized_callback(GLFWwindow* window, int width, int height);
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

glm::vec3 camera_position(0.0f, 0.0f, 3.0f);

GLFWwindow* create_window(bool* window_resized)
{
  glfwInit();
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
  GLFWwindow* window;

  window = glfwCreateWindow(WIDTH, HEIGHT, "Genesis", nullptr, nullptr);
  glfwSetWindowUserPointer(window, window_resized);
  glfwSetFramebufferSizeCallback(window, frame_buffer_resized_callback);
  return window;
}

static void frame_buffer_resized_callback(GLFWwindow* window, int width, int height)
{
  auto flag = reinterpret_cast<bool*>(glfwGetWindowUserPointer(window));
  *flag = true;
  if (DEBUG)
    GenLogTrace("Window resized!");
}


void create_surface(VkInstance& instance, Gen_Window& window_info)
{
  if (glfwCreateWindowSurface(instance, window_info.window, nullptr, &window_info.surface) != VK_SUCCESS) 
  {
    GenLogCritical("Failed to create Window surface!");
  }
}


