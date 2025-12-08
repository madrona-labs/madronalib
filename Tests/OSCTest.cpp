// madronalib: a C++ framework for DSP applications.
// Copyright (c) 2020-2025 Madrona Labs LLC. http://www.madronalabs.com
// Distributed under the MIT license: http://madrona-labs.mit-license.org/

// OSC unit tests using the Catch framework.

#include "catch.hpp"
#include "madronalib.h"
#include "MLOSCTypeConverter.h"
#include "MLOSCReceiver.h"
#include "MLOSCSender.h"

#include "OscOutboundPacketStream.h"
#include "OscReceivedElements.h"

#include <chrono>
#include <thread>
#include <atomic>

using namespace ml;

// ============================================================================
// Path Conversion Tests
// ============================================================================

TEST_CASE("madronalib/osc/path_conversion", "[osc]")
{
  SECTION("OSC address to Path")
  {
    Path p1 = oscAddressToPath("/synth/osc1/freq");
    REQUIRE(p1.getSize() == 3);
    REQUIRE(p1.toText() == TextFragment("synth/osc1/freq"));

    Path p2 = oscAddressToPath("/hello");
    REQUIRE(p2.getSize() == 1);
    REQUIRE(p2.toText() == TextFragment("hello"));

    Path p3 = oscAddressToPath("/a/b/c/d/e");
    REQUIRE(p3.getSize() == 5);

    // Empty and edge cases
    Path p4 = oscAddressToPath("/");
    REQUIRE(p4.getSize() == 0);

    Path p5 = oscAddressToPath("");
    REQUIRE(p5.getSize() == 0);

    Path p6 = oscAddressToPath(nullptr);
    REQUIRE(p6.getSize() == 0);
  }

  SECTION("Path to OSC address")
  {
    Path p1 = runtimePath("synth/osc1/freq");
    TextFragment addr1 = pathToOscAddress(p1);
    REQUIRE(addr1 == TextFragment("/synth/osc1/freq"));

    Path p2 = runtimePath("hello");
    TextFragment addr2 = pathToOscAddress(p2);
    REQUIRE(addr2 == TextFragment("/hello"));

    // Empty path
    Path p3;
    TextFragment addr3 = pathToOscAddress(p3);
    REQUIRE(addr3 == TextFragment("/"));
  }

  SECTION("Round-trip conversion")
  {
    const char* original = "/synth/filter/cutoff";
    Path p = oscAddressToPath(original);
    TextFragment converted = pathToOscAddress(p);
    REQUIRE(converted == TextFragment(original));
  }
}

// ============================================================================
// Type Conversion Tests
// ============================================================================

TEST_CASE("madronalib/osc/type_conversion", "[osc]")
{
  char buffer[1024];

  SECTION("Float value round-trip")
  {
    float testValue = 440.0f;
    Value original(testValue);

    // Create OSC message with float
    osc::OutboundPacketStream p(buffer, sizeof(buffer));
    p << osc::BeginMessage("/test") << testValue << osc::EndMessage;

    // Parse it back
    osc::ReceivedPacket packet(p.Data(), p.Size());
    osc::ReceivedMessage msg(packet);

    auto it = msg.ArgumentsBegin();
    Value converted = oscArgumentToValue(*it);

    REQUIRE(converted.getType() == Value::kFloat);
    REQUIRE(converted.getFloatValue() == testValue);
  }

  SECTION("Int value round-trip")
  {
    int testValue = 42;
    Value original(testValue);

    osc::OutboundPacketStream p(buffer, sizeof(buffer));
    p << osc::BeginMessage("/test") << (osc::int32)testValue << osc::EndMessage;

    osc::ReceivedPacket packet(p.Data(), p.Size());
    osc::ReceivedMessage msg(packet);

    auto it = msg.ArgumentsBegin();
    Value converted = oscArgumentToValue(*it);

    REQUIRE(converted.getType() == Value::kInt);
    REQUIRE(converted.getIntValue() == testValue);
  }

  SECTION("String value round-trip")
  {
    const char* testValue = "hello world";
    Value original(testValue);

    osc::OutboundPacketStream p(buffer, sizeof(buffer));
    p << osc::BeginMessage("/test") << testValue << osc::EndMessage;

    osc::ReceivedPacket packet(p.Data(), p.Size());
    osc::ReceivedMessage msg(packet);

    auto it = msg.ArgumentsBegin();
    Value converted = oscArgumentToValue(*it);

    REQUIRE(converted.getType() == Value::kText);
    REQUIRE(converted.getTextValue() == TextFragment(testValue));
  }

  SECTION("Blob value round-trip")
  {
    uint8_t testData[] = {0x01, 0x02, 0x03, 0x04, 0x05};
    Value original(testData, sizeof(testData));

    osc::OutboundPacketStream p(buffer, sizeof(buffer));
    p << osc::BeginMessage("/test")
      << osc::Blob(testData, sizeof(testData))
      << osc::EndMessage;

    osc::ReceivedPacket packet(p.Data(), p.Size());
    osc::ReceivedMessage msg(packet);

    auto it = msg.ArgumentsBegin();
    Value converted = oscArgumentToValue(*it);

    REQUIRE(converted.getType() == Value::kBlob);
    REQUIRE(converted.size() == sizeof(testData));

    const uint8_t* data = converted.data();
    for (size_t i = 0; i < sizeof(testData); ++i)
    {
      REQUIRE(data[i] == testData[i]);
    }
  }

  SECTION("Value to OSC stream")
  {
    osc::OutboundPacketStream p(buffer, sizeof(buffer));
    p << osc::BeginMessage("/test");

    Value floatVal(3.14f);
    Value intVal(42);
    Value textVal("hello");

    valueToOsc(p, floatVal);
    valueToOsc(p, intVal);
    valueToOsc(p, textVal);

    p << osc::EndMessage;

    // Parse and verify
    osc::ReceivedPacket packet(p.Data(), p.Size());
    osc::ReceivedMessage msg(packet);

    REQUIRE(msg.ArgumentCount() == 3);

    auto it = msg.ArgumentsBegin();
    REQUIRE(it->IsFloat());
    REQUIRE(it->AsFloat() == 3.14f);
    ++it;
    REQUIRE(it->IsInt32());
    REQUIRE(it->AsInt32() == 42);
    ++it;
    REQUIRE(it->IsString());
    REQUIRE(std::string(it->AsString()) == "hello");
  }

  SECTION("Float array value")
  {
    std::vector<float> testArray = {1.0f, 2.0f, 3.0f, 4.0f, 5.0f};
    Value arrayVal(testArray);

    osc::OutboundPacketStream p(buffer, sizeof(buffer));
    p << osc::BeginMessage("/test");
    valueToOsc(p, arrayVal);
    p << osc::EndMessage;

    // Parse and verify the array was written correctly
    osc::ReceivedPacket packet(p.Data(), p.Size());
    osc::ReceivedMessage msg(packet);

    std::vector<Value> values = oscMessageToValues(msg);
    REQUIRE(values.size() == 1);
    REQUIRE(values[0].getType() == Value::kFloatArray);
    REQUIRE(values[0].getFloatArraySize() == 5);
  }

  SECTION("Boolean values")
  {
    osc::OutboundPacketStream p(buffer, sizeof(buffer));
    p << osc::BeginMessage("/test") << true << false << osc::EndMessage;

    osc::ReceivedPacket packet(p.Data(), p.Size());
    osc::ReceivedMessage msg(packet);

    auto it = msg.ArgumentsBegin();
    Value trueVal = oscArgumentToValue(*it);
    ++it;
    Value falseVal = oscArgumentToValue(*it);

    REQUIRE(trueVal.getIntValue() == 1);
    REQUIRE(falseVal.getIntValue() == 0);
  }

  SECTION("Multiple arguments")
  {
    osc::OutboundPacketStream p(buffer, sizeof(buffer));
    p << osc::BeginMessage("/multi")
      << 1.0f << 2.0f << 3.0f << (osc::int32)100 << "text"
      << osc::EndMessage;

    osc::ReceivedPacket packet(p.Data(), p.Size());
    osc::ReceivedMessage msg(packet);

    std::vector<Value> values = oscMessageToValues(msg);
    REQUIRE(values.size() == 5);
    REQUIRE(values[0].getFloatValue() == 1.0f);
    REQUIRE(values[1].getFloatValue() == 2.0f);
    REQUIRE(values[2].getFloatValue() == 3.0f);
    REQUIRE(values[3].getIntValue() == 100);
    REQUIRE(values[4].getTextValue() == TextFragment("text"));
  }
}

// ============================================================================
// Type Tag Tests
// ============================================================================

TEST_CASE("madronalib/osc/type_tags", "[osc]")
{
  SECTION("Value to type tag")
  {
    REQUIRE(valueToOscTypeTag(Value(1.0f)) == osc::FLOAT_TYPE_TAG);
    REQUIRE(valueToOscTypeTag(Value(42)) == osc::INT32_TYPE_TAG);
    REQUIRE(valueToOscTypeTag(Value("hello")) == osc::STRING_TYPE_TAG);

    uint8_t data[4] = {0};
    REQUIRE(valueToOscTypeTag(Value(data, 4)) == osc::BLOB_TYPE_TAG);

    std::vector<float> arr = {1.0f, 2.0f};
    REQUIRE(valueToOscTypeTag(Value(arr)) == osc::ARRAY_BEGIN_TYPE_TAG);

    REQUIRE(valueToOscTypeTag(Value()) == osc::NIL_TYPE_TAG);
  }

  SECTION("Numeric type check")
  {
    REQUIRE(isNumericOscType(osc::FLOAT_TYPE_TAG));
    REQUIRE(isNumericOscType(osc::INT32_TYPE_TAG));
    REQUIRE(isNumericOscType(osc::DOUBLE_TYPE_TAG));
    REQUIRE(isNumericOscType(osc::INT64_TYPE_TAG));
    REQUIRE(!isNumericOscType(osc::STRING_TYPE_TAG));
    REQUIRE(!isNumericOscType(osc::BLOB_TYPE_TAG));
  }

  SECTION("String type check")
  {
    REQUIRE(isStringOscType(osc::STRING_TYPE_TAG));
    REQUIRE(isStringOscType(osc::SYMBOL_TYPE_TAG));
    REQUIRE(!isStringOscType(osc::FLOAT_TYPE_TAG));
    REQUIRE(!isStringOscType(osc::INT32_TYPE_TAG));
  }
}

// ============================================================================
// Sender/Receiver Integration Tests
// ============================================================================

TEST_CASE("madronalib/osc/sender_receiver", "[osc][integration]")
{
  SECTION("Sender creation")
  {
    OSCSender sender;
    REQUIRE(!sender.isOpen());

    // Opening to localhost should work
    bool opened = sender.open("127.0.0.1", 9999);
    REQUIRE(opened);
    REQUIRE(sender.isOpen());

    sender.close();
    REQUIRE(!sender.isOpen());
  }

  SECTION("Receiver creation")
  {
    OSCReceiver receiver;
    REQUIRE(!receiver.isListening());
    REQUIRE(receiver.getPort() == 0);
  }

  SECTION("Send and receive loopback")
  {
    const int testPort = 9876;
    std::atomic<bool> received{false};
    Path receivedPath;
    std::vector<Value> receivedArgs;

    // Set up receiver
    OSCReceiver receiver;
    receiver.setMessageCallback([&](Path addr, std::vector<Value> args) {
      receivedPath = addr;
      receivedArgs = std::move(args);
      received = true;
    });

    bool receiverOpened = receiver.open(testPort);
    if (!receiverOpened)
    {
      // Port might be in use, skip this test
      WARN("Could not open receiver on port " << testPort << ", skipping loopback test");
      return;
    }

    // Give the receiver time to start
    std::this_thread::sleep_for(std::chrono::milliseconds(50));

    // Set up sender
    OSCSender sender;
    bool senderOpened = sender.open("127.0.0.1", testPort);
    REQUIRE(senderOpened);

    // Send a message
    Path testPath = runtimePath("test/message");
    std::vector<Value> testArgs = {Value(42.0f), Value(100)};
    sender.send(testPath, testArgs);

    // Wait for message to arrive
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    // Verify
    REQUIRE(received);
    REQUIRE(receivedPath.toText() == testPath.toText());
    REQUIRE(receivedArgs.size() == 2);
    REQUIRE(receivedArgs[0].getFloatValue() == 42.0f);
    REQUIRE(receivedArgs[1].getIntValue() == 100);

    // Cleanup
    receiver.close();
    sender.close();
  }

  SECTION("Multiple messages")
  {
    const int testPort = 9877;
    std::atomic<int> messageCount{0};

    OSCReceiver receiver;
    receiver.setMessageCallback([&](Path, std::vector<Value>) {
      messageCount++;
    });

    bool receiverOpened = receiver.open(testPort);
    if (!receiverOpened)
    {
      WARN("Could not open receiver on port " << testPort);
      return;
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(50));

    OSCSender sender;
    sender.open("127.0.0.1", testPort);

    // Send multiple messages
    for (int i = 0; i < 5; ++i)
    {
      sender.send(runtimePath("test"), Value(i));
      std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    // Wait for messages
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    REQUIRE(messageCount >= 3); // Allow some dropped packets in test

    receiver.close();
    sender.close();
  }
}

// ============================================================================
// Error Handling Tests
// ============================================================================

TEST_CASE("madronalib/osc/error_handling", "[osc]")
{
  SECTION("Send without open")
  {
    OSCSender sender;
    bool result = sender.send(runtimePath("test"), Value(42.0f));
    REQUIRE(!result);
  }

  SECTION("Invalid host")
  {
    OSCSender sender;
    // This should not crash, but the behavior depends on the network stack
    // Some systems may accept any string, some may reject
    sender.open("invalid-host-name-that-does-not-exist", 9999);
    // Just verify it doesn't crash
  }
}
