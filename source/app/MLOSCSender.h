// madronalib: a C++ framework for DSP applications.
// Copyright (c) 2020-2025 Madrona Labs LLC. http://www.madronalabs.com
// Distributed under the MIT license: http://madrona-labs.mit-license.org/

// MLOSCSender.h
// Modern OSC sender with clean API using madronalib types.

#pragma once

#include <memory>
#include <vector>

#include "MLPath.h"
#include "MLValue.h"

#include "UdpSocket.h"

namespace ml
{

class OSCSender
{
 public:
  // Buffer size for outgoing packets
  static constexpr size_t kBufferSize = 4096;

  OSCSender();
  OSCSender(const char* host, int port);
  ~OSCSender();

  // Non-copyable
  OSCSender(const OSCSender&) = delete;
  OSCSender& operator=(const OSCSender&) = delete;

  // Lifecycle
  bool open(const char* host, int port);
  void close();
  bool isOpen() const;

  // Send a message with the given address and arguments
  bool send(const Path& address, const std::vector<Value>& args);
  bool send(const Path& address, const Value& arg);
  bool send(const Path& address);  // No arguments

  // Low-level access for bundle building
  char* getBuffer() { return buffer_; }
  size_t getBufferSize() const { return kBufferSize; }
  bool sendRaw(const char* data, size_t size);

 private:
  std::unique_ptr<UdpTransmitSocket> socket_;
  char buffer_[kBufferSize];
};

} // namespace ml
