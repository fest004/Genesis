#include "genesis.hpp"
#include "core.h"

void Genesis::run()
{
  init();
  loop();
  cleanup();

}

int Genesis::init()
{
  m_Logger.Init();
  m_Window.createWindow();
  m_Vulkan.initVulkan();

  GenLogTrace("Genesis Initialization succeeded!");

  return 1;
}


void Genesis::loop()
{
  int counter = 0;
  while (m_IsRunning)
  {
    m_IsRunning = m_Window.update();

  }
}

void Genesis::cleanup()
{
  m_Vulkan.cleanup();
  m_Logger.Shutdown();
}


