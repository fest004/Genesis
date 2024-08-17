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
  m_logger.init();
  m_vulkan.init_vulkan();

  GenLogTrace("Genesis Initialization succeeded!");

  return 1;
}


void Genesis::loop()
{
  while (m_is_running)
  {
    m_is_running = m_vulkan.update();
  }
}

void Genesis::cleanup()
{
  m_vulkan.cleanup();
  m_logger.shutdown();
}


