#include "glfw/window.hpp"

class Genesis
{
public:

  void run();
  void loop();
  int init();
  void cleanup();

private:
  Window m_Window;
  bool m_IsRunning = true;

};
