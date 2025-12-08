// madronalib: a C++ framework for DSP applications.
// Copyright (c) 2020-2025 Madrona Labs LLC. http://www.madronalabs.com
// Distributed under the MIT license: http://madrona-labs.mit-license.org/

// MLOSCParameterSync.h
// Bidirectional OSC <-> ParameterTree synchronization via Actor/Message system.

#pragma once

#include "MLActor.h"
#include "MLParameters.h"
#include "MLOSCReceiver.h"
#include "MLOSCSender.h"
#include "MLOSCBuilder.h"

namespace ml
{

class OSCParameterSync : public Actor
{
 public:
  explicit OSCParameterSync(ParameterTree& params);
  ~OSCParameterSync() override;

  // Non-copyable
  OSCParameterSync(const OSCParameterSync&) = delete;
  OSCParameterSync& operator=(const OSCParameterSync&) = delete;

  // Network I/O
  bool startListening(int port);
  void stopListening();
  void setRemoteEndpoint(const char* host, int port);
  void disconnect();

  // Configuration
  void setAutoSync(bool enable) { autoSync_ = enable; }
  bool getAutoSync() const { return autoSync_; }

  // Control whether to send/receive normalized or real values
  void setUseNormalizedValues(bool normalized) { useNormalizedValues_ = normalized; }
  bool getUseNormalizedValues() const { return useNormalizedValues_; }

  // Manual operations
  void sendParameter(const Path& paramName);
  void sendAllParameters();

  // Status
  bool isListening() const { return receiver_.isListening(); }
  bool isConnected() const { return sender_.isOpen(); }

  // Actor interface - receives Messages from the system
  void onMessage(Message m) override;

 private:
  // OSC network -> Parameter system
  void handleOSCMessage(Path address, std::vector<Value> args);

  // Check if a path corresponds to a known parameter
  bool isParameterPath(const Path& path) const;

  // Get parameter value for sending
  Value getParameterValue(const Path& paramName) const;

  ParameterTree& params_;
  OSCReceiver receiver_;
  OSCSender sender_;

  bool autoSync_{false};
  bool useNormalizedValues_{false};

  // Path prefix for parameter messages (e.g., "param/")
  Path paramPrefix_;
};

} // namespace ml
