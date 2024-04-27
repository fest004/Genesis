#include <spdlog/spdlog.h>
#define GENLOG_DEFAULT_LOGGER_NAME "GenLog"

#ifndef GENLOG_CONFIG_RELEASE

// Different levels of logging, from general info to critical erros

#define GenLogTrace(...)                                                        \
  if (spdlog::get(GENLOG_DEFAULT_LOGGER_NAME) != nullptr) {                     \
    spdlog::get(GENLOG_DEFAULT_LOGGER_NAME)->trace(__VA_ARGS__);                \
  }
#define GenLogDebug(...)                                                        \
  if (spdlog::get(GENLOG_DEFAULT_LOGGER_NAME) != nullptr) {                     \
    spdlog::get(GENLOG_DEFAULT_LOGGER_NAME)->debug(__VA_ARGS__);                \
  }
#define GenLogInfo(...)                                                         \
  if (spdlog::get(GENLOG_DEFAULT_LOGGER_NAME) != nullptr) {                     \
    spdlog::get(GENLOG_DEFAULT_LOGGER_NAME)->info(__VA_ARGS__);                 \
  }
#define GenLogWarn(...)                                                         \
  if (spdlog::get(GENLOG_DEFAULT_LOGGER_NAME) != nullptr) {                     \
    spdlog::get(GENLOG_DEFAULT_LOGGER_NAME)->warn(__VA_ARGS__);                 \
  }
#define GenLogError(...)                                                        \
  if (spdlog::get(GENLOG_DEFAULT_LOGGER_NAME) != nullptr) {                     \
    spdlog::get(GENLOG_DEFAULT_LOGGER_NAME)->error(__VA_ARGS__);                \
  }
#define GenLogCritical(...)                                                     \
  if (spdlog::get(GENLOG_DEFAULT_LOGGER_NAME) != nullptr) {                     \
    spdlog::get(GENLOG_DEFAULT_LOGGER_NAME)->critical(__VA_ARGS__);             \
    throw std::runtime_error("");                                               \
  }
#else
// Disable logging for release builds
#define GenLogTrace(...) (void)0
#define GenLogDebug(...) (void)0
#define GenLogInfo(...) (void)0
#define GenLogWarn(...) (void)0
#define GenLogError(...) (void)0
#define GenLogCritical(...) (void)0
#endif
