// madronalib: a C++ framework for DSP applications.
// Copyright (c) 2026 Madrona Labs LLC. http://www.madronalabs.com
// Distributed under the MIT license: http://madrona-labs.mit-license.org/

// converters to/from binary and text formats for various objects.

#include <cmath>
#include <cstdlib>
#include <iostream>
#include <list>
#include <map>
#include <numeric>

#include "MLText.h"
#include "MLTextUtils.h"
#include "MLTree.h"
#include "MLValue.h"
#include "cJSON.h"
#include "MLSerialization.h"
#include "MLMemoryUtils.h"

namespace ml
{

struct BinaryGroupHeader
{
  size_t elements;
  size_t size;
};

bool operator==(const BinaryGroupHeader& a, const BinaryGroupHeader& b)
{
  return ((a.elements == b.elements) && (a.size == b.size));
}

// with 0 elements, size can never be > 0, so this header will not be in any previous binaries.
static constexpr BinaryGroupHeader kBinaryGroupHeaderV2{0, 1};

constexpr unsigned int kPathType{'P'};

struct BinaryChunkHeader
{
  unsigned int type : 8;
  unsigned int dataBytes : 24;
  BinaryChunkHeader() : type(0), dataBytes(0) {}
  BinaryChunkHeader(int t, size_t bytes)
  {
    type = t;
    dataBytes = static_cast<unsigned int>(bytes) & 0x00FFFFFF;
  }
};

static_assert(sizeof(BinaryChunkHeader) == 4);

// Values

struct ValueBinaryHeader
{
  static constexpr int kTypeBits{4};

  // type of value
  unsigned int type : kTypeBits;

  // size of data, not including header
  unsigned int size : Value::kMaxDataSizeBits;
};

// make sure all the values in our Type enum fit into the type field
static_assert((2 << ValueBinaryHeader::kTypeBits) >= Value::kNumTypes);

static_assert(sizeof(ValueBinaryHeader) == 4);

// A bounded view over bytes being deserialized.
//
// Binary reaching the readers below is untrusted: it arrives from host state,
// from preset files, and from the system clipboard. Every length in the format
// is stored in the data itself, so a reader that advances by a length it has
// not checked will walk off the end. This cursor makes that impossible by
// construction -- each read either lands inside the buffer or fails, and a
// failure is sticky, so callers can check once at the end instead of at every
// step.
class BinaryCursor
{
  const uint8_t* ptr_;
  const uint8_t* end_;
  bool ok_{true};

 public:
  BinaryCursor(const uint8_t* data, size_t sizeInBytes)
      : ptr_(data), end_(data ? data + sizeInBytes : nullptr)
  {
    if (!data) ok_ = false;
  }

  bool ok() const { return ok_; }
  void fail() { ok_ = false; }
  size_t remaining() const { return ok_ ? static_cast<size_t>(end_ - ptr_) : 0; }

  // Copy out a fixed-size header and advance past it.
  template <typename T>
  bool read(T& destination)
  {
    if (!ok_ || (remaining() < sizeof(T)))
    {
      ok_ = false;
      return false;
    }
    memcpy(&destination, ptr_, sizeof(T));
    ptr_ += sizeof(T);
    return true;
  }

  // Claim nBytes of payload and advance past it. Returns nullptr on overrun.
  const uint8_t* take(size_t nBytes)
  {
    if (!ok_ || (remaining() < nBytes))
    {
      ok_ = false;
      return nullptr;
    }
    const uint8_t* r = ptr_;
    ptr_ += nBytes;
    return r;
  }
};

size_t getBinarySize(const Value& v) { return sizeof(ValueBinaryHeader) + v.size(); }

std::vector<uint8_t> valueToBinary(Value v)
{
  // allocate vector and setup pointer
  std::vector<uint8_t> result;
  result.resize(getBinarySize(v));
  uint8_t* writePtr = result.data();

  // write header
  ValueBinaryHeader header{v.getType(), v.size()};
  memcpy(writePtr, &header, sizeof(ValueBinaryHeader));
  writePtr += sizeof(ValueBinaryHeader);

  // write data
  memcpy(writePtr, v.data(), v.size());
  return result;
}

void writeValueToBinary(Value v, uint8_t*& writePtr)
{
  // write header
  ValueBinaryHeader header{v.getType(), v.size()};
  memcpy(writePtr, &header, sizeof(ValueBinaryHeader));
  writePtr += sizeof(ValueBinaryHeader);

  // write data
  memcpy(writePtr, v.data(), v.size());
  writePtr += v.size();
}

// Is a payload of this many bytes possible for this type? Checking this is what
// makes a parsed Value meaningful and not merely in-bounds: a kFloat carrying
// three bytes would leave toFixedSizeType() reading uninitialized local storage.
Value makeValueFromBinaryData(unsigned int type, unsigned int sizeInBytes,
                              const uint8_t* dataPtr)
{
  return Value(type, sizeInBytes, dataPtr);
}

static bool valueSizeIsValidForType(unsigned int type, size_t size)
{
  switch (type)
  {
    case Value::kUndefined:
      return size == 0;
    case Value::kFloat:
      return size == sizeof(float);
    case Value::kInt:
      return size == sizeof(int32_t);
    case Value::kFloatArray:
      return (size % sizeof(float)) == 0;
    case Value::kText:
    case Value::kBlob:
      return true;
    default:
      // types outside the enum are encodable in the 4-bit field but not legal
      return false;
  }
}

static bool readValueFromBinary(BinaryCursor& cursor, Value& outValue)
{
  ValueBinaryHeader header{};
  if (!cursor.read(header)) return false;

  const size_t size = header.size;
  if (!valueSizeIsValidForType(header.type, size))
  {
    cursor.fail();
    return false;
  }

  const uint8_t* dataPtr = cursor.take(size);
  if (!dataPtr) return false;

  Value v = makeValueFromBinaryData(header.type, static_cast<unsigned int>(size), dataPtr);

  // text values from external binary data can carry invalid UTF-8, which the
  // code point iterators cannot handle safely (a truncated multi-byte tail
  // makes them run past the end of the buffer). Sanitize on the way in.
  if (v.getType() == Value::kText)
  {
    v = Value(sanitizeUTF8(v.getTextValue()));
  }

  outValue = v;
  return true;
}

// Unchecked. Only safe on bytes this process just wrote -- it trusts the length
// in the data and cannot see where the buffer ends. For anything from a file, a
// host, a network or the clipboard, use binaryToValue(data, size) instead.
Value readBinaryToValue(const uint8_t*& readPtr)
{
  // read header
  ValueBinaryHeader header;
  memcpy(&header, readPtr, sizeof(ValueBinaryHeader));
  readPtr += sizeof(ValueBinaryHeader);

  // copy readPtr at start of data, advance parameter ptr
  const uint8_t* dataPtr = readPtr;
  readPtr += header.size;

  Value v = makeValueFromBinaryData(header.type, header.size, dataPtr);

  if (v.getType() == Value::kText)
  {
    v = Value(sanitizeUTF8(v.getTextValue()));
  }

  return v;
}

Value binaryToValue(const uint8_t* data, size_t sizeInBytes)
{
  BinaryCursor cursor(data, sizeInBytes);
  Value v;
  if (!readValueFromBinary(cursor, v)) return Value();
  return v;
}

Value binaryToValue(const std::vector<uint8_t>& dataVec)
{
  return binaryToValue(dataVec.data(), dataVec.size());
}

// Paths

template <class K>
size_t getBinarySize(GenericPath<K> p)
{
  auto t = p.toText('/');
  auto headerSize = sizeof(BinaryChunkHeader);
  auto dataSize = t.lengthInBytes();
  return headerSize + dataSize;
}

static bool readPathFromBinary(BinaryCursor& cursor, Path& outPath)
{
  BinaryChunkHeader pathHeader{};
  if (!cursor.read(pathHeader)) return false;

  // A chunk that is not a path means the stream is not the shape it claims.
  // This used to fall through and advance the read pointer by an uninitialized
  // pathSize, which a single wrong byte in the input was enough to reach.
  if (pathHeader.type != kPathType)
  {
    cursor.fail();
    return false;
  }

  const size_t pathSize = pathHeader.dataBytes;
  const uint8_t* pChars = cursor.take(pathSize);
  if (!pChars) return false;

  // Sanitize for the same reason text Values are: runtimePath walks the text by
  // code point, and invalid UTF-8 makes that walk run past the end.
  outPath = runtimePath(
      sanitizeUTF8(TextFragment(reinterpret_cast<const char*>(pChars), pathSize)));
  return true;
}

// write the binary representation of the Path and increment the destination pointer.
template <class K>
void writeBinaryRepresentation(const GenericPath<K>& p, uint8_t*& writePtr)
{
  auto t = p.toText('/');
  auto headerSize = sizeof(BinaryChunkHeader);
  auto dataSize = t.lengthInBytes();

  // write header
  BinaryChunkHeader header{kPathType, (unsigned int)dataSize};
  memcpy(writePtr, &header, headerSize);
  writePtr += headerSize;

  // data
  const uint8_t* textData = (uint8_t*)t.getText();
  memcpy(writePtr, textData, dataSize);
  writePtr += dataSize;
}

// Tree< Value >

std::vector<unsigned char> valueTreeToBinary(const Tree<Value>& t)
{
  std::vector<uint8_t> returnVector;
  constexpr size_t headerSize = sizeof(BinaryGroupHeader);

  // calculate size
  size_t totalSize{sizeof(BinaryGroupHeader)};
  for (auto it = t.begin(); it != t.end(); ++it)
  {
    totalSize += getBinarySize(it.getCurrentPath());
    totalSize += getBinarySize(*it);
  }
  totalSize += headerSize * 2;
  returnVector.resize(totalSize);

  // advance past two headers, which we will fill in later
  uint8_t* writePtr = returnVector.data() + headerSize * 2;

  // use iterator to serialize tree
  size_t elements{0};
  for (auto it = t.begin(); it != t.end(); ++it)
  {
    // add path
    writeBinaryRepresentation(it.getCurrentPath(), writePtr);

    // add value
    writeValueToBinary((*it), writePtr);

    elements++;
  }

  // write version header
  writePtr = returnVector.data();
  BinaryGroupHeader* versionHeader{reinterpret_cast<BinaryGroupHeader*>(writePtr)};
  *versionHeader = kBinaryGroupHeaderV2;
  writePtr += headerSize;

  // write main header
  BinaryGroupHeader* mainHeader{reinterpret_cast<BinaryGroupHeader*>(writePtr)};
  mainHeader->elements = elements;
  mainHeader->size = returnVector.size();

  return returnVector;
}

// The largest element count this many bytes could honestly describe. Every
// element costs at least a path chunk header and a value header, so a count
// above this is a lie regardless of what the rest of the stream looks like.
// Deriving the bound from the input beats picking a constant: it needs no
// tuning and cannot drift away from the format.
static size_t maxPlausibleElements(size_t payloadBytes)
{
  constexpr size_t kMinBytesPerElement =
      sizeof(BinaryChunkHeader) + sizeof(ValueBinaryHeader);
  return payloadBytes / kMinBytesPerElement;
}

Tree<Value> binaryToValueTreeNew(const std::vector<uint8_t>& binaryData)
{
  Tree<Value> outputTree;
  const size_t inputSize = binaryData.size();
  constexpr size_t headerSize = sizeof(BinaryGroupHeader);

  if (inputSize < headerSize * 2) return outputTree;

  BinaryGroupHeader mainHeader{};
  memcpy(&mainHeader, binaryData.data() + headerSize, headerSize);

  const size_t elements = mainHeader.elements;
  const size_t totalSize = mainHeader.size;

  // totalSize is the extent the writer claims, so it is what we parse within --
  // but only once we know it is inside the buffer we actually have.
  if ((totalSize < headerSize * 2) || (totalSize > inputSize)) return outputTree;
  if (elements > maxPlausibleElements(totalSize - headerSize * 2)) return outputTree;

  BinaryCursor cursor(binaryData.data() + headerSize * 2, totalSize - headerSize * 2);

  Tree<Value> parsedTree;
  for (size_t i = 0; i < elements; ++i)
  {
    Path path;
    Value value;
    if (!readPathFromBinary(cursor, path)) return outputTree;
    if (!readValueFromBinary(cursor, value)) return outputTree;
    parsedTree[path] = value;
  }

  // Only publish a fully parsed tree. A half-applied parameter set is its own
  // hazard -- it leaves the plugin in a state that is neither the old patch nor
  // the new one, which is harder to diagnose than an outright failure to load.
  return parsedTree;
}

// deprecated code maintained for now to read older binaries of patches etc.

// Reads a path chunk in the pre-V2 layout. Bounded like its V2 counterpart --
// this format is still reached for every preset written before the V2 sentinel
// existed, so it gets the same treatment rather than being trusted.
static bool readPathFromBinaryOld(BinaryCursor& cursor, Path& outPath)
{
  BinaryChunkHeader pathHeader{};
  if (!cursor.read(pathHeader)) return false;

  const size_t pathSize = pathHeader.dataBytes;
  const uint8_t* pChars = cursor.take(pathSize);
  if (!pChars) return false;

  if (pathHeader.type != kPathType)
  {
    cursor.fail();
    return false;
  }

  outPath = runtimePath(
      sanitizeUTF8(TextFragment(reinterpret_cast<const char*>(pChars), pathSize)));
  return true;
}

// Legacy value chunk. Note the type tags here are ASCII characters, not the
// Value::Type enum used by the V2 format. 'F' and 'L' carry a fixed four bytes
// while the chunk still advances by dataBytes, so both have to be checked.
static bool readValueFromBinaryOld(BinaryCursor& cursor, Value& outValue)
{
  BinaryChunkHeader header{};
  if (!cursor.read(header)) return false;

  const size_t dataBytes = header.dataBytes;
  const unsigned int type = header.type;

  if (((type == 'F') || (type == 'L')) && (dataBytes < 4))
  {
    cursor.fail();
    return false;
  }

  const uint8_t* pData = cursor.take(dataBytes);
  if (!pData) return false;

  switch (type)
  {
    case 'U':  // undefined
    {
      outValue = Value();
      break;
    }
    case 'F':  // float
    {
      float f;
      memcpy(&f, pData, sizeof(float));
      outValue = Value(f);
      break;
    }
    case 'T':  // text
    {
      outValue = Value(sanitizeUTF8(
          TextFragment(reinterpret_cast<const char*>(pData), dataBytes)));
      break;
    }
    case 'L':  // long
    {
      uint32_t ul;
      memcpy(&ul, pData, sizeof(uint32_t));
      outValue = Value(static_cast<int>(ul));
      break;
    }
    case 'B':  // blob
    {
      outValue = Value(pData, dataBytes);
      break;
    }
    default:
    {
      cursor.fail();
      return false;
    }
  }
  return true;
}

Tree<Value> binaryToValueTreeOld(const std::vector<uint8_t>& binaryData)
{
  Tree<Value> outputTree;
  const size_t inputSize = binaryData.size();
  constexpr size_t headerSize = sizeof(BinaryGroupHeader);

  if (inputSize <= headerSize) return outputTree;

  BinaryGroupHeader groupHeader{};
  memcpy(&groupHeader, binaryData.data(), headerSize);

  const size_t elements = groupHeader.elements;
  const size_t size = groupHeader.size;

  if ((size < headerSize) || (size > inputSize)) return outputTree;
  if (elements > maxPlausibleElements(size - headerSize)) return outputTree;

  BinaryCursor cursor(binaryData.data() + headerSize, size - headerSize);

  Tree<Value> parsedTree;
  for (size_t i = 0; i < elements; ++i)
  {
    Path path;
    Value value;
    if (!readPathFromBinaryOld(cursor, path)) return outputTree;
    if (!readValueFromBinaryOld(cursor, value)) return outputTree;

    // An undefined value here means we have lost sync with the stream; older
    // writers never emitted one. Stop rather than filling the tree with junk.
    if (value.getType() == Value::kUndefined) return outputTree;

    parsedTree[path] = value;
  }

  return parsedTree;
}

Tree<Value> binaryToValueTree(const std::vector<uint8_t>& binaryData)
{
  Tree<Value> outputTree;
  const uint8_t* pData{binaryData.data()};
  size_t inputBytes = binaryData.size();

  // The legacy reader's first act is to read a 4-byte chunk header just past the
  // group header, so it needs that much to exist -- the old `> headerSize` test
  // let a 17-byte buffer through and over-read by three bytes before any of the
  // length fields came into it.
  if (inputBytes >= sizeof(BinaryGroupHeader) + sizeof(BinaryChunkHeader))
  {
    BinaryGroupHeader groupHeader{};
    memcpy(&groupHeader, pData, sizeof(BinaryGroupHeader));
    if (groupHeader == kBinaryGroupHeaderV2)
    {
      outputTree = binaryToValueTreeNew(binaryData);
    }
    else
    {
      outputTree = binaryToValueTreeOld(binaryData);
    }
  }
  return outputTree;
}

// JSONHolder

struct JSONHolder::Impl
{
  cJSON* data;
};

JSONHolder::JSONHolder()
{
  pImpl = new Impl;
  pImpl->data = (cJSON*)malloc(sizeof(cJSON));  // Use malloc for cJSON compatibility
  memset(pImpl->data, 0, sizeof(cJSON));
  pImpl->data->type = cJSON_Object;
}

JSONHolder::~JSONHolder()
{
  if (pImpl)
  {
    if (pImpl->data)
    {
      cJSON_Delete(pImpl->data);
    }
    delete pImpl;
    pImpl = nullptr;
  }
}

JSONHolder::JSONHolder(JSONHolder&& other) noexcept : pImpl(other.pImpl) { other.pImpl = nullptr; }

JSONHolder& JSONHolder::operator=(JSONHolder&& other) noexcept
{
  if (this != &other)
  {
    // Clean up our current data
    if (pImpl)
    {
      if (pImpl->data)
      {
        cJSON_Delete(pImpl->data);
      }
      delete pImpl;
    }

    // Transfer ownership
    pImpl = other.pImpl;
    other.pImpl = nullptr;
  }
  return *this;
}

void JSONHolder::addNumber(TextFragment key, double number)
{
  cJSON_AddNumberToObject(pImpl->data, key.getText(), number);
}

void JSONHolder::addString(TextFragment key, const char* str)
{
  cJSON_AddStringToObject(pImpl->data, key.getText(), str);
}

void JSONHolder::addFloatVector(TextFragment key, std::vector<float>& v)
{
  cJSON_AddItemToObject(pImpl->data, key.getText(),
                        cJSON_CreateFloatArray(v.data(), sizeToInt(v.size())));
}

void JSONHolder::addJSON(TextFragment key, JSONHolder& j)
{
  // Transfer the entire cJSON tree to this object
  cJSON_AddItemToObject(pImpl->data, key.getText(), j.pImpl->data);

  // j no longer owns its data - set to nullptr so destructor won't try to delete
  j.pImpl->data = nullptr;
}

cJSON* getData(const JSONHolder& cj) { return cj.pImpl->data; }

void setData(JSONHolder& cj, cJSON* pData) { cj.pImpl->data = pData; }

// return a JSON object representing the value tree. The caller is responsible
// for freeing the object.
//
// NOTE: this does not make the JSON tree, rather a flat structure with the
// path name for each object name! TODO fix
JSONHolder valueTreeToJSON(const Tree<Value>& t)
{
  JSONHolder root;

  for (auto it = t.begin(); it != t.end(); ++it)
  {
    auto p = it.getCurrentPath();
    TextFragment pathAsText(p.toText());
    Value v = (*it);

    const char* keyStr = pathAsText.getText();

    switch (v.getType())
    {
      case Value::kUndefined:
        break;
      case Value::kFloat:
        cJSON_AddNumberToObject(getData(root), keyStr, v.getFloatValue());
        break;
      case Value::kFloatArray:
      {
        auto a = cJSON_CreateFloatArray(v.getFloatArrayPtr(), sizeToInt(v.getFloatArraySize()));
        if (a)
        {
          cJSON_AddItemToObject(getData(root), keyStr, a);
        }
        break;
      }
      case Value::kText:
        cJSON_AddStringToObject(getData(root), keyStr, v.getTextValue().getText());
        break;
      case Value::kInt:
        cJSON_AddNumberToObject(getData(root), keyStr, v.getIntValue());
        break;
      case Value::kBlob:
      {
        std::vector<uint8_t> blobVec(v.data(), v.data() + v.size());
        TextFragment blobText(kBlobHeader, textUtils::base64Encode(blobVec));
        cJSON_AddStringToObject(getData(root), keyStr, blobText.getText());
        break;
      }
      default:
        // debug() << "MLAppState::saveStateToStateFile(): undefined param type! \n";
        break;
    }
  }
  return root;
}

void readJSONToValueTree(cJSON* obj, Tree<Value>& r, Path currentPath, int depth)
{
  int objIndex{0};

  while (obj)
  {
    Path newObjectPath(currentPath, runtimePath(obj->string));

    switch (obj->type & 255)
    {
      case cJSON_Number:
      {
        r.add(newObjectPath, (float)obj->valuedouble);
        break;
      }
      case cJSON_String:
      {
        TextFragment valueText(obj->valuestring);

        if (valueText.beginsWith(kBlobHeader))
        {
          // convert strings starting with the header into Blobs
          auto headerLen = kBlobHeader.lengthInCodePoints();
          auto textLen = valueText.lengthInCodePoints();
          auto body = textUtils::subText(valueText, headerLen, textLen);

          auto blobDataVec = textUtils::base64Decode(body.getText());
          r.add(newObjectPath, Value(blobDataVec.data(), blobDataVec.size()));
        }
        else
        {
          // convert ordinary strings into text
          r.add(newObjectPath, Value(valueText));
        }
        break;
      }
      case cJSON_Object:
      {
        // we only recurse for entire objects.
        readJSONToValueTree(obj->child, r, newObjectPath, depth);
        break;
      }
      case cJSON_Array:
      {
        std::vector<float> arrayElems;
        cJSON* c = obj->child;
        while (c)
        {
          arrayElems.push_back((float)c->valuedouble);
          c = c->next;
        }
        r.add(newObjectPath, Value(arrayElems));
        break;
      }
      default:
      {
        break;
      }
    }
    obj = obj->next;
    objIndex++;
  }
}

Tree<Value> JSONToValueTree(const JSONHolder& root)
{
  Tree<Value> r;
  if (getData(root))
  {
    cJSON* obj = getData(root)->child;
    readJSONToValueTree(obj, r, "", 0);
  }
  return r;
}

JSONHolder textToJSON(TextFragment t)
{
  JSONHolder root;
  cJSON* cjp = cJSON_Parse(t.getText());

  if (cjp)
  {
    cJSON_Delete(root.pImpl->data);
    root.pImpl->data = cjp;
  }

  return root;
}

TextFragment JSONToText(const JSONHolder& root)
{
  char* jsonString = cJSON_Print(getData(root));
  TextFragment result(jsonString);
  free(jsonString);  // Free the malloc'd string
  return result;
}

}  // namespace ml
