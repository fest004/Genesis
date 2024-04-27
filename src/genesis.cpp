#include "genesis.hpp"

void Genesis::run()
{
  if (DEBUG && init()) { GenLogInfo("Initializition succeeded!"); }
  loop();
  cleanup();

}

int Genesis::init()
{
  m_Logger.Init();
  m_Window.createWindow();
  m_Vulkan.initVulkan();

  return 1;
}


void Genesis::loop()
{
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


