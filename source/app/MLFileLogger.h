// madronalib: a C++ framework for DSP applications.
// Copyright (c) 2020-2025 Madrona Labs LLC. http://www.madronalabs.com
// Distributed under the MIT license: http://madrona-labs.mit-license.org/

// MLFileLogger.h
// Lightweight file-based logging for debug output.

#pragma once

#include <cstdio>
#include <fstream>
#include <mutex>

#include "MLPlatform.h"
#include "MLSharedResource.h"
#include "MLText.h"

namespace ml
{

// Log severity levels
enum class LogLevel
{
  kDebug = 0,
  kInfo = 1,
  kWarning = 2,
  kError = 3
};

class FileLogger
{
 public:
  FileLogger();
  ~FileLogger();

  // Non-copyable
  FileLogger(const FileLogger&) = delete;
  FileLogger& operator=(const FileLogger&) = delete;

  // Configuration
  void setPath(const char* path);
  void setMinLevel(LogLevel level) { minLevel_ = level; }
  LogLevel getMinLevel() const { return minLevel_; }

  // Enable/disable
  bool enable();
  void disable();
  bool isEnabled() const { return enabled_; }

  // Core logging - thread-safe
  void log(LogLevel level, const char* category, const char* message);

  // Convenience methods
  void debug(const char* category, const char* message);
  void info(const char* category, const char* message);
  void warning(const char* category, const char* message);
  void error(const char* category, const char* message);

  // Get current path (for diagnostics)
  TextFragment getPath() const { return path_; }

 private:
  TextFragment getDefaultPath();
  TextFragment formatTimestamp();
  const char* levelToString(LogLevel level);
  bool ensureDirectoryExists(const char* path);

  std::ofstream file_;
  std::mutex mutex_;
  TextFragment path_;
  LogLevel minLevel_{LogLevel::kDebug};
  bool enabled_{false};
};

// Global access via SharedResourcePointer (matches ActorLogger pattern)
inline FileLogger& getFileLogger()
{
  static SharedResourcePointer<FileLogger> logger;
  return *logger;
}

// Convenience macros for easy logging
// These check isEnabled() first to avoid formatting costs when disabled

#define LOG_DEBUG(category, ...)                                              \
  do                                                                          \
  {                                                                           \
    auto& _log = ml::getFileLogger();                                         \
    if (_log.isEnabled() && _log.getMinLevel() <= ml::LogLevel::kDebug)       \
    {                                                                         \
      char _buf[1024];                                                        \
      snprintf(_buf, sizeof(_buf), __VA_ARGS__);                              \
      _log.debug(category, _buf);                                             \
    }                                                                         \
  } while (0)

#define LOG_INFO(category, ...)                                               \
  do                                                                          \
  {                                                                           \
    auto& _log = ml::getFileLogger();                                         \
    if (_log.isEnabled() && _log.getMinLevel() <= ml::LogLevel::kInfo)        \
    {                                                                         \
      char _buf[1024];                                                        \
      snprintf(_buf, sizeof(_buf), __VA_ARGS__);                              \
      _log.info(category, _buf);                                              \
    }                                                                         \
  } while (0)

#define LOG_WARNING(category, ...)                                            \
  do                                                                          \
  {                                                                           \
    auto& _log = ml::getFileLogger();                                         \
    if (_log.isEnabled() && _log.getMinLevel() <= ml::LogLevel::kWarning)     \
    {                                                                         \
      char _buf[1024];                                                        \
      snprintf(_buf, sizeof(_buf), __VA_ARGS__);                              \
      _log.warning(category, _buf);                                           \
    }                                                                         \
  } while (0)

#define LOG_ERROR(category, ...)                                              \
  do                                                                          \
  {                                                                           \
    auto& _log = ml::getFileLogger();                                         \
    if (_log.isEnabled())                                                     \
    {                                                                         \
      char _buf[1024];                                                        \
      snprintf(_buf, sizeof(_buf), __VA_ARGS__);                              \
      _log.error(category, _buf);                                             \
    }                                                                         \
  } while (0)

}  // namespace ml
