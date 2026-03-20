// madronalib: a C++ framework for DSP applications.
// Copyright (c) 2020-2025 Madrona Labs LLC. http://www.madronalabs.com
// Distributed under the MIT license: http://madrona-labs.mit-license.org/

// MLOSCBuilder.h
// Fluent API for constructing OSC messages and bundles using madronalib types.

#pragma once

#include <vector>

#include "MLPath.h"
#include "MLValue.h"
#include "MLOSCSender.h"
#include "MLOSCTypeConverter.h"

#include "OscOutboundPacketStream.h"

namespace ml
{

class OSCSender;  // Forward declaration

// ============================================================================
// OSCMessageBuilder - Fluent API for message construction
// ============================================================================

class OSCMessageBuilder
{
 public:
  explicit OSCMessageBuilder(const Path& address) : address_(address) {}
  explicit OSCMessageBuilder(const char* address) : address_(runtimePath(address)) {}

  // Add arguments - returns *this for chaining
  OSCMessageBuilder& add(const Value& v)
  {
    args_.push_back(v);
    return *this;
  }

  OSCMessageBuilder& addFloat(float f) { return add(Value(f)); }
  OSCMessageBuilder& addInt(int i) { return add(Value(i)); }
  OSCMessageBuilder& addString(const char* s) { return add(Value(s)); }
  OSCMessageBuilder& addString(const TextFragment& s) { return add(Value(s)); }

  OSCMessageBuilder& addBlob(const uint8_t* data, size_t size)
  {
    return add(Value(data, size));
  }

  OSCMessageBuilder& addFloatArray(const std::vector<float>& arr)
  {
    return add(Value(arr));
  }

  // Variadic add for convenience
  template <typename T, typename... Args>
  OSCMessageBuilder& add(const T& first, const Args&... rest)
  {
    add(Value(first));
    return add(rest...);
  }

  // Send to destination
  bool sendTo(OSCSender& sender) const
  {
    return sender.send(address_, args_);
  }

  // Write to packet stream (for bundle building)
  void writeTo(osc::OutboundPacketStream& stream) const
  {
    TextFragment oscAddr = pathToOscAddress(address_);
    stream << osc::BeginMessage(oscAddr.getText());
    valuesToOsc(stream, args_);
    stream << osc::EndMessage;
  }

  // Accessors
  const Path& getAddress() const { return address_; }
  const std::vector<Value>& getArgs() const { return args_; }
  size_t getArgCount() const { return args_.size(); }

 private:
  Path address_;
  std::vector<Value> args_;
};

// ============================================================================
// OSCBundleBuilder - Fluent API for bundle construction
// ============================================================================

class OSCBundleBuilder
{
 public:
  // Time tag constants
  static constexpr uint64_t kImmediateTimeTag = 1;

  OSCBundleBuilder() : timeTag_(kImmediateTimeTag) {}

  // Set timetag
  OSCBundleBuilder& setTimeTag(uint64_t tag)
  {
    timeTag_ = tag;
    return *this;
  }

  OSCBundleBuilder& setImmediate()
  {
    timeTag_ = kImmediateTimeTag;
    return *this;
  }

  // Add messages
  OSCBundleBuilder& addMessage(const OSCMessageBuilder& msg)
  {
    messages_.push_back(msg);
    return *this;
  }

  OSCBundleBuilder& addMessage(const Path& address, const std::vector<Value>& args)
  {
    OSCMessageBuilder msg(address);
    for (const auto& arg : args)
    {
      msg.add(arg);
    }
    messages_.push_back(std::move(msg));
    return *this;
  }

  OSCBundleBuilder& addMessage(const Path& address, const Value& arg)
  {
    messages_.push_back(OSCMessageBuilder(address).add(arg));
    return *this;
  }

  OSCBundleBuilder& addMessage(const Path& address)
  {
    messages_.push_back(OSCMessageBuilder(address));
    return *this;
  }

  // Send entire bundle
  bool sendTo(OSCSender& sender) const
  {
    if (!sender.isOpen())
    {
      return false;
    }

    char* buffer = sender.getBuffer();
    size_t bufferSize = sender.getBufferSize();

    try
    {
      osc::OutboundPacketStream stream(buffer, bufferSize);

      stream << osc::BeginBundle(timeTag_);

      for (const auto& msg : messages_)
      {
        msg.writeTo(stream);
      }

      stream << osc::EndBundle;

      return sender.sendRaw(stream.Data(), stream.Size());
    }
    catch (const osc::Exception&)
    {
      return false;
    }
  }

  // Accessors
  uint64_t getTimeTag() const { return timeTag_; }
  size_t getMessageCount() const { return messages_.size(); }
  const std::vector<OSCMessageBuilder>& getMessages() const { return messages_; }

  // Clear for reuse
  void clear()
  {
    messages_.clear();
    timeTag_ = kImmediateTimeTag;
  }

 private:
  uint64_t timeTag_;
  std::vector<OSCMessageBuilder> messages_;
};

// ============================================================================
// Convenience functions
// ============================================================================

// Quick message send
inline bool oscSend(OSCSender& sender, const Path& address)
{
  return sender.send(address);
}

inline bool oscSend(OSCSender& sender, const Path& address, const Value& arg)
{
  return sender.send(address, arg);
}

inline bool oscSend(OSCSender& sender, const Path& address, const std::vector<Value>& args)
{
  return sender.send(address, args);
}

} // namespace ml
