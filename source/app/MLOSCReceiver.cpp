// madronalib: a C++ framework for DSP applications.
// Copyright (c) 2020-2025 Madrona Labs LLC. http://www.madronalabs.com
// Distributed under the MIT license: http://madrona-labs.mit-license.org/

#include "MLOSCReceiver.h"
#include "MLOSCTypeConverter.h"

#include "OscReceivedElements.h"

#include <iostream>

namespace ml
{

OSCReceiver::OSCReceiver() = default;

OSCReceiver::OSCReceiver(int port)
{
  open(port);
}

OSCReceiver::~OSCReceiver()
{
  close();
}

bool OSCReceiver::open(int port)
{
  // Close any existing connection
  close();

  try
  {
    port_ = port;
    socket_ = std::make_unique<UdpListeningReceiveSocket>(
        IpEndpointName(IpEndpointName::ANY_ADDRESS, port),
        this);

    // Start listener thread
    listenerThread_ = std::thread(&OSCReceiver::listenerThreadFunc, this);

    return true;
  }
  catch (const std::exception& e)
  {
    std::cerr << "OSCReceiver::open failed on port " << port << ": " << e.what() << std::endl;
    socket_.reset();
    port_ = 0;
    return false;
  }
}

void OSCReceiver::close()
{
  if (socket_)
  {
    socket_->AsynchronousBreak();

    if (listenerThread_.joinable())
    {
      listenerThread_.join();
    }

    socket_.reset();
    port_ = 0;
  }
}

bool OSCReceiver::isListening() const
{
  return socket_ != nullptr;
}

void OSCReceiver::setMessageCallback(MessageCallback cb)
{
  std::lock_guard<std::mutex> lock(callbackMutex_);
  messageCallback_ = std::move(cb);
}

void OSCReceiver::setBundleStartCallback(BundleCallback cb)
{
  std::lock_guard<std::mutex> lock(callbackMutex_);
  bundleStartCallback_ = std::move(cb);
}

void OSCReceiver::setBundleEndCallback(BundleCallback cb)
{
  std::lock_guard<std::mutex> lock(callbackMutex_);
  bundleEndCallback_ = std::move(cb);
}

void OSCReceiver::listenerThreadFunc()
{
  if (socket_)
  {
    socket_->Run();
  }
}

void OSCReceiver::ProcessMessage(const osc::ReceivedMessage& m,
                                  const IpEndpointName& /*remoteEndpoint*/)
{
  MessageCallback cb;
  {
    std::lock_guard<std::mutex> lock(callbackMutex_);
    cb = messageCallback_;
  }

  if (cb)
  {
    try
    {
      // Convert OSC address to Path
      Path address = oscAddressToPath(m.AddressPattern());

      // Convert OSC arguments to Values
      std::vector<Value> args = oscMessageToValues(m);

      // Invoke callback
      cb(address, std::move(args));
    }
    catch (const osc::Exception& e)
    {
      std::cerr << "OSCReceiver: error processing message: " << e.what() << std::endl;
    }
  }
}

void OSCReceiver::ProcessBundle(const osc::ReceivedBundle& b,
                                 const IpEndpointName& remoteEndpoint)
{
  // Call bundle start callback
  {
    BundleCallback cb;
    {
      std::lock_guard<std::mutex> lock(callbackMutex_);
      cb = bundleStartCallback_;
    }
    if (cb)
    {
      cb(b.TimeTag());
    }
  }

  // Process bundle elements
  processReceivedBundle(b, remoteEndpoint);

  // Call bundle end callback
  {
    BundleCallback cb;
    {
      std::lock_guard<std::mutex> lock(callbackMutex_);
      cb = bundleEndCallback_;
    }
    if (cb)
    {
      cb(b.TimeTag());
    }
  }
}

void OSCReceiver::processReceivedBundle(const osc::ReceivedBundle& b,
                                         const IpEndpointName& remoteEndpoint)
{
  // Manually iterate over bundle elements
  for (auto it = b.ElementsBegin(); it != b.ElementsEnd(); ++it)
  {
    if (it->IsBundle())
    {
      // Recursively process nested bundle
      osc::ReceivedBundle nestedBundle(*it);
      ProcessBundle(nestedBundle, remoteEndpoint);
    }
    else
    {
      // Process message
      osc::ReceivedMessage msg(*it);
      ProcessMessage(msg, remoteEndpoint);
    }
  }
}

} // namespace ml
