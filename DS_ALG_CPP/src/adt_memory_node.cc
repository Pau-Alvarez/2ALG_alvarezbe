/*
 * Ivan Sancho as Unreal Authorized Instructor (UAI), 2025.
 *
 * Memory Node source code (C++ port).
 * Algorithms and Data Structures.
 *
 * Student:
 *   Pau Alvarez Belenguer
 *
 */

#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "common_def.h"
#include "adt_memory_node.h"

#include "EDK_MemoryManager/edk_memory_manager.h"

MemoryNode::MemoryNode()
    : data_(nullptr), size_(0), next_(nullptr), prev_(nullptr) {}

MemoryNode::~MemoryNode() {
  if (nullptr != data_) {
    MM->free(data_);
    data_ = nullptr;
  }
  size_ = 0;
  next_ = nullptr;
  prev_ = nullptr;
}

MemoryNode::MemoryNode(const MemoryNode& other)
    : data_(nullptr), size_(0), next_(nullptr), prev_(nullptr) {
  if (nullptr != other.data_ && other.size_ > 0) {
    data_ = MM->malloc(other.size_);
    if (nullptr != data_) {
      std::memcpy(data_, other.data_, other.size_);
      size_ = other.size_;
    }
  }
}

MemoryNode& MemoryNode::operator=(const MemoryNode& other) {
  if (this == &other) {
    return *this;
  }
  if (nullptr != data_) {
    MM->free(data_);
    data_ = nullptr;
  }
  size_ = 0;
  next_ = nullptr;
  prev_ = nullptr;

  if (nullptr != other.data_ && other.size_ > 0) {
    data_ = MM->malloc(other.size_);
    if (nullptr != data_) {
      std::memcpy(data_, other.data_, other.size_);
      size_ = other.size_;
    }
  }
  return *this;
}

MemoryNode::MemoryNode(MemoryNode&& other) noexcept
    : data_(other.data_),
      size_(other.size_),
      next_(other.next_),
      prev_(other.prev_) {
  other.data_ = nullptr;
  other.size_ = 0;
  other.next_ = nullptr;
  other.prev_ = nullptr;
}

MemoryNode& MemoryNode::operator=(MemoryNode&& other) noexcept {
  if (this == &other) {
    return *this;
  }
  if (nullptr != data_) {
    MM->free(data_);
  }
  data_ = other.data_;
  size_ = other.size_;
  next_ = other.next_;
  prev_ = other.prev_;

  other.data_ = nullptr;
  other.size_ = 0;
  other.next_ = nullptr;
  other.prev_ = nullptr;
  return *this;
}

MemoryNode* MemoryNode::create() {
  MemoryNode* node = new MemoryNode();
  return node;
}

s16 MemoryNode::createFromRef(MemoryNode** node) {
  if (nullptr == node) return kErrorCode_Null;
  if (nullptr != *node) return kErrorCode_AlreadyInitialized;
  *node = new MemoryNode();
  if (nullptr == *node) {
#ifdef VERBOSE_
    std::printf("Error: [%s] not enough memory available\n", __FUNCTION__);
#endif
    return kErrorCode_Memory;
  }
  return kErrorCode_Ok;
}

void* MemoryNode::data() const {
  return data_;
}

s16 MemoryNode::setData(void* src, u16 bytes) {
  if (nullptr == src) return kErrorCode_Null;
  if (0 == bytes) return kErrorCode_ZeroSize;
  data_ = src;
  size_ = bytes;
  return kErrorCode_Ok;
}

u16 MemoryNode::size() const {
  return size_;
}

s16 MemoryNode::reset() {
  if (nullptr != data_) {
    MM->free(data_);
    data_ = nullptr;
  }
  size_ = 0;
  next_ = nullptr;
  prev_ = nullptr;
  return kErrorCode_Ok;
}

s16 MemoryNode::softReset() {
  data_ = nullptr;
  size_ = 0;
  next_ = nullptr;
  prev_ = nullptr;
  return kErrorCode_Ok;
}

s16 MemoryNode::memSet(u8 value) {
  if (nullptr == data_) return kErrorCode_NullData;
  u8* tmp = static_cast<u8*>(data_);
  for (u16 i = 0; i < size_; ++i) {
    tmp[i] = value;
  }
  return kErrorCode_Ok;
}

s16 MemoryNode::memCopy(void* src, u16 bytes) {
  if (nullptr == src) return kErrorCode_Null;
  if (0 == bytes) return kErrorCode_ZeroSize;

  void* new_buffer = MM->malloc(bytes);
  if (nullptr == new_buffer) return kErrorCode_Memory;

  std::memcpy(new_buffer, src, bytes);

  if (nullptr != data_) {
    MM->free(data_);
  }
  data_ = new_buffer;
  size_ = bytes;
  return kErrorCode_Ok;
}

s16 MemoryNode::memConcat(void* src, u16 bytes) {
  if (nullptr == src) return kErrorCode_Null;
  if (0 == bytes) return kErrorCode_ZeroSize;

  u32 total_size = static_cast<u32>(size_) + static_cast<u32>(bytes);

  void* combined_buffer = MM->malloc(total_size);
  if (nullptr == combined_buffer) {
    return kErrorCode_Memory;
  }
  if (nullptr != data_ && size_ > 0) {
    std::memcpy(combined_buffer, data_, size_);
  }
  std::memcpy(static_cast<u8*>(combined_buffer) + size_, src, bytes);

  if (nullptr != data_) {
    MM->free(data_);
  }

  data_ = combined_buffer;
  size_ = static_cast<u16>(total_size);
  return kErrorCode_Ok;
}

s16 MemoryNode::memMask(u8 mask) {
  if (nullptr == data_) return kErrorCode_NullData;
  u8* tmp = static_cast<u8*>(data_);
  for (u16 i = 0; i < size_; ++i) {
    tmp[i] &= mask;
  }
  return kErrorCode_Ok;
}

void MemoryNode::print() const {
  std::printf("[Node Info] Address: %p\n", static_cast<const void*>(this));
  std::printf("[Node Info] Size: %d\n", size_);
  if (nullptr == data_) {
    std::printf("[Node Info] Data Address: NULL\n");
    return;
  }
  std::printf("[Node Info] Data Address: %p\n", data_);
  std::printf("[Node Info] Data Content: ");
  u8* byte_data = static_cast<u8*>(data_);
  for (u16 i = 0; i < size_; ++i) {
    std::printf("%c", byte_data[i]);
  }
  std::printf("\n");
}
