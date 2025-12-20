// madronalib: a C++ framework for DSP applications.
// Copyright (c) 2020-2025 Madrona Labs LLC. http://www.madronalabs.com
// Distributed under the MIT license: http://madrona-labs.mit-license.org/

// MLOSCReceiver.h
// Modern OSC receiver with callback-based API using madronalib types.
// Thread-safe callback management with cross-platform std::thread.

#pragma once

// MLPlatform.h must come first on Windows to fix std::thread/_beginthreadex issue
#include "MLPlatform.h"

#include <functional>
#include <memory>
#include <mutex>
#include <thread>
#include <vector>

#include "MLPath.h"
#include "MLValue.h"

#include "OscPacketListener.h"
#include "UdpSocket.h"

namespace ml
{

class OSCReceiver : public osc::OscPacketListener
{
 public:
  // Callback types using madronalib types
  using MessageCallback = std::function<void(Path address, std::vector<Value> args)>;
  using BundleCallback = std::function<void(uint64_t timeTag)>;

  OSCReceiver();
  explicit OSCReceiver(int port);
  ~OSCReceiver();

  // Non-copyable
  OSCReceiver(const OSCReceiver&) = delete;
  OSCReceiver& operator=(const OSCReceiver&) = delete;

  // Lifecycle
  bool open(int port);
  void close();
  bool isListening() const;
  int getPort() const { return port_; }

  // Set callbacks (thread-safe)
  void setMessageCallback(MessageCallback cb);
  void setBundleStartCallback(BundleCallback cb);
  void setBundleEndCallback(BundleCallback cb);

 protected:
  // Implements osc::OscPacketListener
  void ProcessMessage(const osc::ReceivedMessage& m,
                      const IpEndpointName& remoteEndpoint) override;
  void ProcessBundle(const osc::ReceivedBundle& b,
                     const IpEndpointName& remoteEndpoint) override;

 private:
  void listenerThreadFunc();
  void processReceivedBundle(const osc::ReceivedBundle& b,
                             const IpEndpointName& remoteEndpoint);

  std::unique_ptr<UdpListeningReceiveSocket> socket_;
  std::thread listenerThread_;
  int port_{0};

  mutable std::mutex callbackMutex_;
  MessageCallback messageCallback_;
  BundleCallback bundleStartCallback_;
  BundleCallback bundleEndCallback_;
};

} // namespace ml
