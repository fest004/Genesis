#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

class Window
{

public:
  int createWindow();
  int createSurface(VkInstance& instance);
  int update();
  int closeWindow(VkInstance& instance);


private:
  GLFWwindow* m_GLFWWindow;
  VkSurfaceKHR m_Surface;

};



