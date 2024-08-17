#include <spdlog/sinks/stdout_color_sinks-inl.h>
#include <spdlog/spdlog.h>

#ifndef LOG_H
#define LOG_H

// Header for spdlog, great, high speed logging library

class Log 
{
public:
  Log() = default;
  ~Log() = default;

  void init();
  void shutdown();
};

#endif
