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


  Vulkan m_vulkan;
  bool m_is_running = true;
  Log m_logger;

};
