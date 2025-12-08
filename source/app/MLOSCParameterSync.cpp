// madronalib: a C++ framework for DSP applications.
// Copyright (c) 2020-2025 Madrona Labs LLC. http://www.madronalabs.com
// Distributed under the MIT license: http://madrona-labs.mit-license.org/

#include "MLOSCParameterSync.h"
#include "MLOSCTypeConverter.h"

namespace ml
{

OSCParameterSync::OSCParameterSync(ParameterTree& params)
    : params_(params)
{
  // Set up OSC receiver callback
  receiver_.setMessageCallback([this](Path addr, std::vector<Value> args) {
    handleOSCMessage(addr, std::move(args));
  });
}

OSCParameterSync::~OSCParameterSync()
{
  stopListening();
  disconnect();
  stop();  // Stop the Actor timer
}

bool OSCParameterSync::startListening(int port)
{
  bool result = receiver_.open(port);
  if (result)
  {
    // Start the Actor's message processing timer
    start();
  }
  return result;
}

void OSCParameterSync::stopListening()
{
  receiver_.close();
}

void OSCParameterSync::setRemoteEndpoint(const char* host, int port)
{
  sender_.open(host, port);
}

void OSCParameterSync::disconnect()
{
  sender_.close();
}

void OSCParameterSync::handleOSCMessage(Path address, std::vector<Value> args)
{
  if (args.empty())
  {
    return;
  }

  // Check if this is a known parameter
  if (isParameterPath(address))
  {
    // Create a Message to send into the system
    // The message includes the kMsgFromOSC flag to prevent echo
    Message msg(address, args[0], kMsgFromOSC);

    // Enqueue the message for processing by the Actor
    enqueueMessage(msg);
  }
}

void OSCParameterSync::onMessage(Message m)
{
  // Handle messages from the system
  if (!m.address)
  {
    return;
  }

  // If this message came from OSC, update the parameter tree directly
  if (m.flags & kMsgFromOSC)
  {
    Path paramPath = m.address;

    // Update the parameter tree
    if (useNormalizedValues_)
    {
      params_.setFromNormalizedValue(paramPath, m.value);
    }
    else
    {
      params_.setFromRealValue(paramPath, m.value);
    }
  }
  else if (autoSync_ && sender_.isOpen())
  {
    // Message from UI/Controller - forward to OSC network
    // Check if this is a parameter message
    if (isParameterPath(m.address))
    {
      // Send to OSC network
      Value valueToSend = getParameterValue(m.address);
      sender_.send(m.address, valueToSend);
    }
  }
}

bool OSCParameterSync::isParameterPath(const Path& path) const
{
  // Check if the path exists in the parameter descriptions
  for (auto it = params_.descriptions.begin(); it != params_.descriptions.end(); ++it)
  {
    const auto& paramDesc = *it;
    if (paramDesc)
    {
      Path paramPath = runtimePath(paramDesc->getTextProperty("name"));
      if (paramPath == path)
      {
        return true;
      }
    }
  }
  return false;
}

Value OSCParameterSync::getParameterValue(const Path& paramName) const
{
  if (useNormalizedValues_)
  {
    return params_.getNormalizedValueAtPath(paramName);
  }
  else
  {
    return params_.getRealValueAtPath(paramName);
  }
}

void OSCParameterSync::sendParameter(const Path& paramName)
{
  if (!sender_.isOpen())
  {
    return;
  }

  Value value = getParameterValue(paramName);
  sender_.send(paramName, value);
}

void OSCParameterSync::sendAllParameters()
{
  if (!sender_.isOpen())
  {
    return;
  }

  // Build a bundle with all parameters
  OSCBundleBuilder bundle;
  bundle.setImmediate();

  for (auto it = params_.descriptions.begin(); it != params_.descriptions.end(); ++it)
  {
    const auto& paramDesc = *it;
    if (paramDesc)
    {
      Path paramPath = runtimePath(paramDesc->getTextProperty("name"));
      Value value = getParameterValue(paramPath);
      bundle.addMessage(paramPath, value);
    }
  }

  bundle.sendTo(sender_);
}

} // namespace ml
