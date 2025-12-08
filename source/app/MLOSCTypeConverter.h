// madronalib: a C++ framework for DSP applications.
// Copyright (c) 2020-2025 Madrona Labs LLC. http://www.madronalabs.com
// Distributed under the MIT license: http://madrona-labs.mit-license.org/

// MLOSCTypeConverter.h
// Header-only utilities for converting between OSC types and madronalib types.
// Provides bidirectional conversion between:
// - OSC addresses and madronalib Paths
// - OSC argument types and madronalib Values

#pragma once

#include <vector>
#include <cstring>

#include "MLPath.h"
#include "MLValue.h"
#include "MLText.h"

#include "OscTypes.h"
#include "OscReceivedElements.h"
#include "OscOutboundPacketStream.h"

namespace ml
{

// ============================================================================
// Path <-> OSC Address Conversion
// ============================================================================

// Convert OSC address pattern to madronalib Path
// OSC: "/synth/osc1/freq" -> Path: "synth/osc1/freq"
inline Path oscAddressToPath(const char* oscAddress)
{
  if (!oscAddress || oscAddress[0] == '\0')
  {
    return Path();
  }

  // Skip leading '/' if present
  const char* start = oscAddress;
  if (start[0] == '/')
  {
    start++;
  }

  return runtimePath(start);
}

// Convert madronalib Path to OSC address pattern
// Path: "synth/osc1/freq" -> OSC: "/synth/osc1/freq"
inline TextFragment pathToOscAddress(const Path& p)
{
  if (!p)
  {
    return TextFragment("/");
  }

  return TextFragment("/", p.toText());
}

// ============================================================================
// OSC Argument -> Value Conversion
// ============================================================================

// Convert a single OSC argument to a madronalib Value
inline Value oscArgumentToValue(const osc::ReceivedMessageArgument& arg)
{
  switch (arg.TypeTag())
  {
    case osc::FLOAT_TYPE_TAG:
      return Value(arg.AsFloatUnchecked());

    case osc::INT32_TYPE_TAG:
      return Value(arg.AsInt32Unchecked());

    case osc::STRING_TYPE_TAG:
      return Value(arg.AsStringUnchecked());

    case osc::SYMBOL_TYPE_TAG:
      return Value(arg.AsSymbolUnchecked());

    case osc::BLOB_TYPE_TAG:
    {
      const void* data = nullptr;
      osc::osc_bundle_element_size_t size = 0;
      arg.AsBlobUnchecked(data, size);
      return Value(static_cast<const uint8_t*>(data), static_cast<size_t>(size));
    }

    case osc::DOUBLE_TYPE_TAG:
      // Convert double to float (Value doesn't have double type)
      return Value(static_cast<float>(arg.AsDoubleUnchecked()));

    case osc::INT64_TYPE_TAG:
      // Convert int64 to int (Value uses 32-bit int)
      return Value(static_cast<int>(arg.AsInt64Unchecked()));

    case osc::TRUE_TYPE_TAG:
      return Value(1);

    case osc::FALSE_TYPE_TAG:
      return Value(0);

    case osc::NIL_TYPE_TAG:
    case osc::INFINITUM_TYPE_TAG:
    default:
      return Value();
  }
}

// Convert all arguments in an OSC message to a vector of Values
inline std::vector<Value> oscMessageToValues(const osc::ReceivedMessage& msg)
{
  std::vector<Value> values;

  for (auto it = msg.ArgumentsBegin(); it != msg.ArgumentsEnd(); ++it)
  {
    // Handle array types
    if (it->IsArrayBegin())
    {
      // Count array elements and collect floats
      std::vector<float> floats;
      ++it; // skip array begin marker

      while (it != msg.ArgumentsEnd() && !it->IsArrayEnd())
      {
        if (it->IsFloat())
        {
          floats.push_back(it->AsFloatUnchecked());
        }
        else if (it->IsInt32())
        {
          floats.push_back(static_cast<float>(it->AsInt32Unchecked()));
        }
        else if (it->IsDouble())
        {
          floats.push_back(static_cast<float>(it->AsDoubleUnchecked()));
        }
        ++it;
      }

      if (!floats.empty())
      {
        values.push_back(Value(floats));
      }
    }
    else if (!it->IsArrayEnd())
    {
      values.push_back(oscArgumentToValue(*it));
    }
  }

  return values;
}

// ============================================================================
// Value -> OSC Argument Conversion
// ============================================================================

// Append a madronalib Value to an OSC packet stream
inline void valueToOsc(osc::OutboundPacketStream& stream, const Value& v)
{
  switch (v.getType())
  {
    case Value::kFloat:
      stream << v.getFloatValue();
      break;

    case Value::kInt:
      stream << static_cast<osc::int32>(v.getIntValue());
      break;

    case Value::kText:
    {
      TextFragment text = v.getTextValue();
      stream << text.getText();
      break;
    }

    case Value::kBlob:
    {
      osc::Blob blob(v.data(), static_cast<osc::osc_bundle_element_size_t>(v.size()));
      stream << blob;
      break;
    }

    case Value::kFloatArray:
    {
      stream << osc::BeginArray;
      const float* ptr = v.getFloatArrayPtr();
      size_t count = v.getFloatArraySize();
      for (size_t i = 0; i < count; ++i)
      {
        stream << ptr[i];
      }
      stream << osc::EndArray;
      break;
    }

    case Value::kUndefined:
    default:
      stream << osc::OscNil;
      break;
  }
}

// Append multiple Values to an OSC packet stream
inline void valuesToOsc(osc::OutboundPacketStream& stream, const std::vector<Value>& values)
{
  for (const auto& v : values)
  {
    valueToOsc(stream, v);
  }
}

// ============================================================================
// Type Tag Utilities
// ============================================================================

// Get the OSC type tag character for a Value
inline char valueToOscTypeTag(const Value& v)
{
  switch (v.getType())
  {
    case Value::kFloat:
      return osc::FLOAT_TYPE_TAG;
    case Value::kInt:
      return osc::INT32_TYPE_TAG;
    case Value::kText:
      return osc::STRING_TYPE_TAG;
    case Value::kBlob:
      return osc::BLOB_TYPE_TAG;
    case Value::kFloatArray:
      return osc::ARRAY_BEGIN_TYPE_TAG; // Arrays start with '['
    case Value::kUndefined:
    default:
      return osc::NIL_TYPE_TAG;
  }
}

// Check if an OSC type tag represents a numeric type
inline bool isNumericOscType(char typeTag)
{
  return typeTag == osc::FLOAT_TYPE_TAG ||
         typeTag == osc::INT32_TYPE_TAG ||
         typeTag == osc::DOUBLE_TYPE_TAG ||
         typeTag == osc::INT64_TYPE_TAG;
}

// Check if an OSC type tag represents a string type
inline bool isStringOscType(char typeTag)
{
  return typeTag == osc::STRING_TYPE_TAG ||
         typeTag == osc::SYMBOL_TYPE_TAG;
}

} // namespace ml
