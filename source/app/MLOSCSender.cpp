// madronalib: a C++ framework for DSP applications.
// Copyright (c) 2020-2025 Madrona Labs LLC. http://www.madronalabs.com
// Distributed under the MIT license: http://madrona-labs.mit-license.org/

#include "MLOSCSender.h"
#include "MLOSCTypeConverter.h"

#include "OscOutboundPacketStream.h"
#include "IpEndpointName.h"

#include <iostream>
#include <cstring>

namespace ml
{

OSCSender::OSCSender()
{
  std::memset(buffer_, 0, kBufferSize);
}

OSCSender::OSCSender(const char* host, int port)
{
  std::memset(buffer_, 0, kBufferSize);
  open(host, port);
}

OSCSender::~OSCSender()
{
  close();
}

bool OSCSender::open(const char* host, int port)
{
  close();

  try
  {
    socket_ = std::make_unique<UdpTransmitSocket>(
        IpEndpointName(host, port));
    return true;
  }
  catch (const std::exception& e)
  {
    std::cerr << "OSCSender::open failed: " << e.what() << std::endl;
    socket_.reset();
    return false;
  }
}

void OSCSender::close()
{
  socket_.reset();
}

bool OSCSender::isOpen() const
{
  return socket_ != nullptr;
}

bool OSCSender::send(const Path& address, const std::vector<Value>& args)
{
  if (!socket_)
  {
    return false;
  }

  try
  {
    osc::OutboundPacketStream p(buffer_, kBufferSize);

    // Convert Path to OSC address
    TextFragment oscAddr = pathToOscAddress(address);

    p << osc::BeginMessage(oscAddr.getText());

    // Add all arguments
    for (const auto& arg : args)
    {
      valueToOsc(p, arg);
    }

    p << osc::EndMessage;

    socket_->Send(p.Data(), p.Size());
    return true;
  }
  catch (const osc::Exception& e)
  {
    std::cerr << "OSCSender::send error: " << e.what() << std::endl;
    return false;
  }
}

bool OSCSender::send(const Path& address, const Value& arg)
{
  return send(address, std::vector<Value>{arg});
}

bool OSCSender::send(const Path& address)
{
  return send(address, std::vector<Value>{});
}

bool OSCSender::sendRaw(const char* data, size_t size)
{
  if (!socket_ || !data || size == 0)
  {
    return false;
  }

  try
  {
    socket_->Send(data, size);
    return true;
  }
  catch (const std::exception& e)
  {
    std::cerr << "OSCSender::sendRaw error: " << e.what() << std::endl;
    return false;
  }
}

} // namespace ml
