// madronalib: a C++ framework for DSP applications.
// Copyright (c) 2020-2025 Madrona Labs LLC. http://www.madronalabs.com
// Distributed under the MIT license: http://madrona-labs.mit-license.org/

// FileLogger unit tests using the Catch framework.

#include "catch.hpp"
#include "madronalib.h"
#include "MLFileLogger.h"

#include <cstdio>
#include <fstream>
#include <string>
#include <thread>
#include <vector>

#if ML_MAC || ML_LINUX
#include <unistd.h>
#endif

using namespace ml;

// Helper to read file contents
static std::string readFileContents(const char* path)
{
  std::ifstream file(path);
  if (!file.is_open()) return "";
  return std::string((std::istreambuf_iterator<char>(file)),
                     std::istreambuf_iterator<char>());
}

// ============================================================================
// Basic Logging Tests
// ============================================================================

TEST_CASE("madronalib/app/filelogger/basic", "[filelogger]")
{
  const char* testPath = "/tmp/madronalib_test_basic.log";
  std::remove(testPath);

  auto& logger = getFileLogger();
  logger.setPath(testPath);
  logger.setMinLevel(LogLevel::kDebug);

  REQUIRE(logger.enable());
  REQUIRE(logger.isEnabled());

  SECTION("Log all levels")
  {
    logger.debug("TEST", "Debug message");
    logger.info("TEST", "Info message");
    logger.warning("TEST", "Warning message");
    logger.error("TEST", "Error message");

    logger.disable();
    REQUIRE(!logger.isEnabled());

    std::string content = readFileContents(testPath);
    REQUIRE(content.find("[DEBUG]") != std::string::npos);
    REQUIRE(content.find("[INFO]") != std::string::npos);
    REQUIRE(content.find("[WARN]") != std::string::npos);
    REQUIRE(content.find("[ERROR]") != std::string::npos);
    REQUIRE(content.find("[TEST]") != std::string::npos);
    REQUIRE(content.find("Debug message") != std::string::npos);
    REQUIRE(content.find("Info message") != std::string::npos);
    REQUIRE(content.find("Warning message") != std::string::npos);
    REQUIRE(content.find("Error message") != std::string::npos);
  }

  std::remove(testPath);
}

// ============================================================================
// Level Filtering Tests
// ============================================================================

TEST_CASE("madronalib/app/filelogger/level-filter", "[filelogger]")
{
  const char* testPath = "/tmp/madronalib_test_levels.log";
  std::remove(testPath);

  auto& logger = getFileLogger();
  logger.setPath(testPath);
  logger.setMinLevel(LogLevel::kWarning);  // Only warnings and errors
  logger.enable();

  logger.debug("TEST", "Should not appear - debug");
  logger.info("TEST", "Should not appear - info");
  logger.warning("TEST", "Should appear - warning");
  logger.error("TEST", "Should appear - error");

  logger.disable();

  std::string content = readFileContents(testPath);
  REQUIRE(content.find("[DEBUG]") == std::string::npos);
  REQUIRE(content.find("[INFO]") == std::string::npos);
  REQUIRE(content.find("[WARN]") != std::string::npos);
  REQUIRE(content.find("[ERROR]") != std::string::npos);
  REQUIRE(content.find("Should appear - warning") != std::string::npos);
  REQUIRE(content.find("Should appear - error") != std::string::npos);

  // Reset for other tests
  logger.setMinLevel(LogLevel::kDebug);
  std::remove(testPath);
}

// ============================================================================
// Convenience Macro Tests
// ============================================================================

TEST_CASE("madronalib/app/filelogger/macros", "[filelogger]")
{
  const char* testPath = "/tmp/madronalib_test_macros.log";
  std::remove(testPath);

  auto& logger = getFileLogger();
  logger.setPath(testPath);
  logger.setMinLevel(LogLevel::kDebug);
  logger.enable();

  int width = 800;
  int height = 600;
  int errorCode = 1234;

  LOG_DEBUG("GUI", "Initializing window");
  LOG_INFO("GUI", "Window size: %dx%d", width, height);
  LOG_WARNING("GL", "Deprecated function used");
  LOG_ERROR("GL", "OpenGL error code: %d", errorCode);

  logger.disable();

  std::string content = readFileContents(testPath);
  REQUIRE(content.find("Initializing window") != std::string::npos);
  REQUIRE(content.find("Window size: 800x600") != std::string::npos);
  REQUIRE(content.find("Deprecated function used") != std::string::npos);
  REQUIRE(content.find("OpenGL error code: 1234") != std::string::npos);

  std::remove(testPath);
}

// ============================================================================
// Default Path Tests
// ============================================================================

TEST_CASE("madronalib/app/filelogger/default-path", "[filelogger]")
{
  // Create a fresh logger to get default path
  FileLogger freshLogger;
  TextFragment defaultPath = freshLogger.getPath();

  const char* pathStr = defaultPath.getText();
  REQUIRE(pathStr != nullptr);

#if ML_LINUX
  // Should contain .local/share/madronalib or XDG_DATA_HOME path
  bool hasExpectedPath = (strstr(pathStr, ".local/share/madronalib") != nullptr ||
                          strstr(pathStr, "madronalib/debug.log") != nullptr);
  REQUIRE(hasExpectedPath);
#elif ML_MAC
  REQUIRE(strstr(pathStr, "Library/Logs/madronalib") != nullptr);
#elif ML_WINDOWS
  REQUIRE(strstr(pathStr, "madronalib") != nullptr);
#endif
}

// ============================================================================
// Session Marker Tests
// ============================================================================

TEST_CASE("madronalib/app/filelogger/session-marker", "[filelogger]")
{
  const char* testPath = "/tmp/madronalib_test_session.log";
  std::remove(testPath);

  auto& logger = getFileLogger();
  logger.setPath(testPath);
  logger.setMinLevel(LogLevel::kDebug);
  logger.enable();
  logger.info("TEST", "First session message");
  logger.disable();

  std::string content = readFileContents(testPath);
  REQUIRE(content.find("=== madronalib log session started") != std::string::npos);

  std::remove(testPath);
}

// ============================================================================
// Timestamp Format Tests
// ============================================================================

TEST_CASE("madronalib/app/filelogger/timestamp", "[filelogger]")
{
  const char* testPath = "/tmp/madronalib_test_timestamp.log";
  std::remove(testPath);

  auto& logger = getFileLogger();
  logger.setPath(testPath);
  logger.setMinLevel(LogLevel::kDebug);
  logger.enable();
  logger.info("TEST", "Timestamp test");
  logger.disable();

  std::string content = readFileContents(testPath);

  // Check for timestamp format HH:MM:SS.mmm (e.g., "14:30:45.123")
  // Look for pattern with colons and dot
  bool hasTimestamp = false;
  size_t pos = content.find("[INFO]");
  if (pos != std::string::npos && pos >= 12)
  {
    // Check if there's a timestamp before the log level
    // Format should be like "14:30:45.123 [INFO]"
    std::string prefix = content.substr(pos - 13, 12);
    // Should contain two colons and one dot
    int colonCount = 0, dotCount = 0;
    for (char c : prefix)
    {
      if (c == ':') colonCount++;
      if (c == '.') dotCount++;
    }
    hasTimestamp = (colonCount == 2 && dotCount == 1);
  }
  REQUIRE(hasTimestamp);

  std::remove(testPath);
}

// ============================================================================
// Directory Creation Tests
// ============================================================================

TEST_CASE("madronalib/app/filelogger/directory-creation", "[filelogger]")
{
#if ML_MAC || ML_LINUX
  const char* testPath = "/tmp/madronalib_test_subdir/nested/debug.log";

  // Remove directory if it exists
  std::remove(testPath);
  rmdir("/tmp/madronalib_test_subdir/nested");
  rmdir("/tmp/madronalib_test_subdir");

  auto& logger = getFileLogger();
  logger.setPath(testPath);
  logger.setMinLevel(LogLevel::kDebug);

  bool enabled = logger.enable();
  REQUIRE(enabled);

  logger.info("TEST", "Nested directory test");
  logger.disable();

  std::string content = readFileContents(testPath);
  REQUIRE(content.find("Nested directory test") != std::string::npos);

  // Cleanup
  std::remove(testPath);
  rmdir("/tmp/madronalib_test_subdir/nested");
  rmdir("/tmp/madronalib_test_subdir");
#else
  // Skip on Windows for now
  WARN("Directory creation test skipped on Windows");
#endif
}

// ============================================================================
// Thread Safety Tests
// ============================================================================

TEST_CASE("madronalib/app/filelogger/thread-safety", "[filelogger]")
{
  const char* testPath = "/tmp/madronalib_test_threads.log";
  std::remove(testPath);

  auto& logger = getFileLogger();
  logger.setPath(testPath);
  logger.setMinLevel(LogLevel::kDebug);
  logger.enable();

  // Log from multiple threads
  std::vector<std::thread> threads;
  const int numThreads = 4;
  const int messagesPerThread = 50;

  for (int t = 0; t < numThreads; ++t)
  {
    threads.emplace_back([t, messagesPerThread]() {
      for (int i = 0; i < messagesPerThread; ++i)
      {
        char msg[64];
        snprintf(msg, sizeof(msg), "Thread %d message %d", t, i);
        LOG_INFO("THREAD", "%s", msg);
      }
    });
  }

  for (auto& t : threads)
  {
    t.join();
  }

  logger.disable();

  std::string content = readFileContents(testPath);

  // Count lines (each message should be on its own line)
  int lineCount = 0;
  for (char c : content)
  {
    if (c == '\n') lineCount++;
  }

  // Should have session marker (1 line) + all messages
  // Allow some tolerance for the session marker lines
  REQUIRE(lineCount >= numThreads * messagesPerThread);

  std::remove(testPath);
}
