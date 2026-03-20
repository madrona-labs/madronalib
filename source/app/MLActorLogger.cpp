// madronalib: a C++ framework for DSP applications.
// Copyright (c) 2020-2025 Madrona Labs LLC. http://www.madronalabs.com
// Distributed under the MIT license: http://madrona-labs.mit-license.org/

#include "MLActorLogger.h"

#include <sstream>

#include "MLOSCBuilder.h"

namespace ml
{

ActorLogger::~ActorLogger()
{
  disable();
}

bool ActorLogger::enable(const char* host, int port)
{
  if (enabled_)
  {
    return true;  // Already enabled
  }

  if (!sender_.open(host, port))
  {
    return false;
  }

  // Register callback with Actor class
  Actor::setLogCallback([this](Path actorName, const Message& m, bool isEnqueue) {
    handleLogCallback(actorName, m, isEnqueue);
  });

  enabled_ = true;
  return true;
}

void ActorLogger::disable()
{
  if (enabled_)
  {
    Actor::clearLogCallback();
    sender_.close();
    enabled_ = false;
  }
}

void ActorLogger::enableFileOutput(bool enable)
{
  fileOutputEnabled_ = enable;
  if (enable)
  {
    // Ensure the file logger is enabled
    getFileLogger().enable();

    // If OSC logging isn't enabled, we still need to set up the callback
    if (!enabled_)
    {
      Actor::setLogCallback([this](Path actorName, const Message& m, bool isEnqueue) {
        handleLogCallback(actorName, m, isEnqueue);
      });
    }
  }
}

void ActorLogger::handleLogCallback(Path actorName, const Message& m, bool isEnqueue)
{
  // Skip anonymous (unregistered) actors
  if (!actorName)
  {
    return;
  }

  // Check enqueue/dispatch filters
  if (isEnqueue && !logEnqueue_)
  {
    return;
  }
  if (!isEnqueue && !logDispatch_)
  {
    return;
  }

  // Check actor path filter
  if (filterPath_ && actorName != filterPath_)
  {
    return;
  }

  // OSC output (when OSC logging is enabled)
  if (enabled_)
  {
    // Build OSC address: /log/<enqueue|dispatch>/<actor_name>
    TextFragment prefix = isEnqueue ? TextFragment("log/enqueue/") : TextFragment("log/dispatch/");
    TextFragment actorText = actorName.toText();
    TextFragment addressText(prefix, actorText);
    Path logAddress = runtimePath(addressText);

    // Send OSC message with:
    // 1. Message address (string)
    // 2. Message value (varies)
    // 3. Message flags (int)
    OSCMessageBuilder msg(logAddress);
    msg.addString(m.address.toText());
    msg.add(m.value);
    msg.addInt(static_cast<int>(m.flags));
    msg.sendTo(sender_);
  }

  // File output (when file logging is enabled)
  if (fileOutputEnabled_)
  {
    std::ostringstream oss;
    oss << (isEnqueue ? "enqueue" : "dispatch") << " " << actorName.toText().getText() << " -> "
        << m.address.toText().getText() << " = " << m.value;
    if (m.flags != 0)
    {
      oss << " (flags: " << static_cast<int>(m.flags) << ")";
    }
    getFileLogger().debug("Actor", oss.str().c_str());
  }
}

}  // namespace ml
