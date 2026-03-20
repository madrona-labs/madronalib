// madronalib: a C++ framework for DSP applications.
// Copyright (c) 2020-2025 Madrona Labs LLC. http://www.madronalabs.com
// Distributed under the MIT license: http://madrona-labs.mit-license.org/

#include "MLFileLogger.h"

#include <chrono>
#include <cstring>
#include <ctime>
#include <iomanip>
#include <sstream>

#include "MLTextUtils.h"

// Platform-specific includes for directory creation and path resolution
#if ML_WINDOWS
#include <direct.h>
#include <shlobj.h>
#elif ML_MAC || ML_LINUX
#include <errno.h>
#include <pwd.h>
#include <sys/stat.h>
#include <unistd.h>
#endif

namespace ml
{

FileLogger::FileLogger() { path_ = getDefaultPath(); }

FileLogger::~FileLogger() { disable(); }

TextFragment FileLogger::getDefaultPath()
{
#if ML_WINDOWS
  // %APPDATA%/madronalib/debug.log
  char appDataPath[MAX_PATH];
  if (SUCCEEDED(SHGetFolderPathA(NULL, CSIDL_APPDATA, NULL, 0, appDataPath)))
  {
    std::string fullPath = std::string(appDataPath) + "\\madronalib\\debug.log";
    return TextFragment(fullPath.c_str());
  }
  return TextFragment("C:\\madronalib\\debug.log");

#elif ML_MAC
  // ~/Library/Logs/madronalib/debug.log
  const char* home = getenv("HOME");
  if (!home)
  {
    struct passwd* pw = getpwuid(getuid());
    if (pw) home = pw->pw_dir;
  }
  if (home)
  {
    std::string fullPath = std::string(home) + "/Library/Logs/madronalib/debug.log";
    return TextFragment(fullPath.c_str());
  }
  return TextFragment("/tmp/madronalib/debug.log");

#elif ML_LINUX
  // Follow XDG spec: $XDG_DATA_HOME/madronalib/debug.log
  // or ~/.local/share/madronalib/debug.log
  const char* xdgDataHome = getenv("XDG_DATA_HOME");
  if (xdgDataHome && xdgDataHome[0] != '\0')
  {
    std::string fullPath = std::string(xdgDataHome) + "/madronalib/debug.log";
    return TextFragment(fullPath.c_str());
  }
  const char* home = getenv("HOME");
  if (!home)
  {
    struct passwd* pw = getpwuid(getuid());
    if (pw) home = pw->pw_dir;
  }
  if (home)
  {
    std::string fullPath = std::string(home) + "/.local/share/madronalib/debug.log";
    return TextFragment(fullPath.c_str());
  }
  return TextFragment("/tmp/madronalib/debug.log");

#else
  return TextFragment("./debug.log");
#endif
}

void FileLogger::setPath(const char* path)
{
  std::lock_guard<std::mutex> lock(mutex_);
  if (enabled_)
  {
    // If already enabled, close and reopen at new path
    file_.close();
    path_ = TextFragment(path);
    file_.open(path_.getText(), std::ios::app);
  }
  else
  {
    path_ = TextFragment(path);
  }
}

bool FileLogger::ensureDirectoryExists(const char* filePath)
{
  // Extract directory path from file path
  TextFragment dirPath = textUtils::getPath(TextFragment(filePath));
  if (dirPath.lengthInBytes() == 0)
  {
    return true;  // Current directory, always exists
  }

#if ML_WINDOWS
  // Windows: recursively create directories
  std::string pathStr(dirPath.getText());
  for (size_t i = 0; i < pathStr.size(); ++i)
  {
    if (pathStr[i] == '/' || pathStr[i] == '\\')
    {
      std::string subPath = pathStr.substr(0, i);
      if (!subPath.empty()) _mkdir(subPath.c_str());
    }
  }
  _mkdir(pathStr.c_str());
  return true;
#else
  // POSIX: recursively create directories
  std::string pathStr(dirPath.getText());

  // Build path incrementally, creating each level
  std::string currentPath;
  for (size_t i = 0; i < pathStr.size(); ++i)
  {
    currentPath += pathStr[i];
    if (pathStr[i] == '/')
    {
      if (!currentPath.empty() && currentPath != "/")
      {
        mkdir(currentPath.c_str(), 0755);
        // Ignore errors - directory might already exist
      }
    }
  }
  // Create final directory
  if (!currentPath.empty() && currentPath.back() != '/')
  {
    mkdir(currentPath.c_str(), 0755);
  }

  // Check if directory now exists
  struct stat st;
  return (stat(dirPath.getText(), &st) == 0 && S_ISDIR(st.st_mode));
#endif
}

bool FileLogger::enable()
{
  std::lock_guard<std::mutex> lock(mutex_);
  if (enabled_) return true;

  // Ensure parent directory exists
  if (!ensureDirectoryExists(path_.getText()))
  {
    return false;
  }

  file_.open(path_.getText(), std::ios::app);
  if (file_.is_open())
  {
    enabled_ = true;
    // Write session start marker
    auto now = std::chrono::system_clock::now();
    auto time_t_now = std::chrono::system_clock::to_time_t(now);
    file_ << "\n=== madronalib log session started at "
          << std::put_time(std::localtime(&time_t_now), "%Y-%m-%d %H:%M:%S") << " ===\n";
    file_.flush();
    return true;
  }
  return false;
}

void FileLogger::disable()
{
  std::lock_guard<std::mutex> lock(mutex_);
  if (enabled_)
  {
    file_.close();
    enabled_ = false;
  }
}

void FileLogger::log(LogLevel level, const char* category, const char* message)
{
  if (level < minLevel_) return;

  std::lock_guard<std::mutex> lock(mutex_);
  if (!enabled_) return;

  // Format: [TIMESTAMP] [LEVEL] [CATEGORY] message
  file_ << formatTimestamp().getText() << " [" << levelToString(level) << "]"
        << " [" << category << "] " << message << "\n";
  file_.flush();  // Immediate flush for crash debugging
}

void FileLogger::debug(const char* category, const char* message)
{
  log(LogLevel::kDebug, category, message);
}

void FileLogger::info(const char* category, const char* message)
{
  log(LogLevel::kInfo, category, message);
}

void FileLogger::warning(const char* category, const char* message)
{
  log(LogLevel::kWarning, category, message);
}

void FileLogger::error(const char* category, const char* message)
{
  log(LogLevel::kError, category, message);
}

TextFragment FileLogger::formatTimestamp()
{
  auto now = std::chrono::system_clock::now();
  auto time_t_now = std::chrono::system_clock::to_time_t(now);
  auto ms =
      std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;

  std::ostringstream oss;
  oss << std::put_time(std::localtime(&time_t_now), "%H:%M:%S") << '.' << std::setfill('0')
      << std::setw(3) << ms.count();
  return TextFragment(oss.str().c_str());
}

const char* FileLogger::levelToString(LogLevel level)
{
  switch (level)
  {
    case LogLevel::kDebug:
      return "DEBUG";
    case LogLevel::kInfo:
      return "INFO";
    case LogLevel::kWarning:
      return "WARN";
    case LogLevel::kError:
      return "ERROR";
    default:
      return "?????";
  }
}

}  // namespace ml
