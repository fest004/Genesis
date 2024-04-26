#include "genesis.hpp"


void Genesis::run()
{
  init();

  while (m_IsRunning)
  {
    loop();
  }

  cleanup();

}

int Genesis::init()
{
  
  m_Window.createWindow();
  // m_Window.createSurface(VkInstance &instance) //TODO maybe create instance first and pass it to createsurface as constructor arg to make it a reference

  return 1;
}


void Genesis::loop()
{
  m_Window.update();

}

void Genesis::cleanup()
{

}


