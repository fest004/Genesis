#include "window.hpp"
#include <GLFW/glfw3.h>
#include <vulkan/vulkan_core.h>


#define WIDTH 640
#define HEIGHT 480


int Window::createWindow()
{
  glfwInit();
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
  m_GLFWWindow = glfwCreateWindow(WIDTH, HEIGHT, "Genesis", NULL, NULL);

  return 1;
}


int Window::createSurface(VkInstance& instance)
{
  VkResult res = glfwCreateWindowSurface(instance, m_GLFWWindow, NULL, &m_Surface);

  return res;
}


int Window::update()
{
  if (glfwWindowShouldClose(m_GLFWWindow)) 
    return 0;

  glfwPollEvents();

  return 1;
}

int Window::closeWindow(VkInstance& instance)
{
  glfwDestroyWindow(m_GLFWWindow);
  glfwTerminate();
  return 1;
}

