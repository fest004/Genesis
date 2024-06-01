#include "vulkan/vulkan.hpp"
#include "core.h"

class Genesis
{
public:

  void run();
  void loop();
  void cleanup();

private:

  int init();


  Vulkan m_Vulkan;
  bool m_IsRunning = true;
  Log m_Logger;

};
