// madronalib: a C++ framework for DSP applications.
// Copyright (c) 2020-2025 Madrona Labs LLC. http://www.madronalabs.com
// Distributed under the MIT license: http://madrona-labs.mit-license.org/

// MLActorLogger.h
// Optional OSC-based logging for Actor message traffic.
// Broadcasts all enqueue and dispatch events to an OSC destination for debugging.

#pragma once

#include "MLActor.h"
#include "MLOSCSender.h"
#include "MLSharedResource.h"

namespace ml
{

class ActorLogger
{
 public:
  ActorLogger() = default;
  ~ActorLogger();

  // Non-copyable
  ActorLogger(const ActorLogger&) = delete;
  ActorLogger& operator=(const ActorLogger&) = delete;

  // Enable logging to OSC destination
  bool enable(const char* host, int port);
  void disable();

  bool isEnabled() const { return enabled_; }

  // Configure what to log
  void setLogEnqueue(bool log) { logEnqueue_ = log; }
  void setLogDispatch(bool log) { logDispatch_ = log; }
  bool getLogEnqueue() const { return logEnqueue_; }
  bool getLogDispatch() const { return logDispatch_; }

  // Filter by actor path (empty = log all registered actors)
  void setFilter(Path actorPath) { filterPath_ = actorPath; }
  void clearFilter() { filterPath_ = Path(); }
  Path getFilter() const { return filterPath_; }

 private:
  void handleLogCallback(Path actorName, const Message& m, bool isEnqueue);

  OSCSender sender_;
  bool enabled_{false};
  bool logEnqueue_{true};
  bool logDispatch_{true};
  Path filterPath_;
};

// Global access via SharedResourcePointer
inline ActorLogger& getActorLogger()
{
  static SharedResourcePointer<ActorLogger> logger;
  return *logger;
}

}  // namespace ml
