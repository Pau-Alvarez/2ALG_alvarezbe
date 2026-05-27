/*
 * Ivan Sancho as Unreal Authorized Instructor (UAI), 2025.
 *
 * Stack source code (C++ port).
 * Algorithms and Data Structures.
 *
 * Student:
 *   Pau Alvarez Belenguer
 *
 */

#include <cstdio>
#include <new>

#include "common_def.h"
#include "adt_stack.h"

#include "EDK_MemoryManager/edk_memory_manager.h"

Stack::Stack(u16 capacity)
    : head_(0), tail_(0), capacity_(capacity), storage_(nullptr) {
  if (0 == capacity) return;
  storage_ = static_cast<MemoryNode*>(MM->malloc(sizeof(MemoryNode) * capacity));
  if (nullptr == storage_) {
    capacity_ = 0;
    return;
  }
  for (u16 i = 0; i < capacity; ++i) {
    new (&storage_[i]) MemoryNode();
  }
}

Stack::~Stack() {
  releaseStorage();
}

void Stack::releaseStorage() {
  if (nullptr != storage_) {
    for (u16 i = 0; i < capacity_; ++i) {
      storage_[i].~MemoryNode();
    }
    MM->free(storage_);
    storage_ = nullptr;
  }
  head_ = 0;
  tail_ = 0;
  capacity_ = 0;
}

Stack::Stack(const Stack& other)
    : head_(0), tail_(0), capacity_(0), storage_(nullptr) {
  if (0 == other.capacity_) return;
  storage_ = static_cast<MemoryNode*>(MM->malloc(sizeof(MemoryNode) * other.capacity_));
  if (nullptr == storage_) return;
  capacity_ = other.capacity_;
  for (u16 i = 0; i < capacity_; ++i) new (&storage_[i]) MemoryNode();
  for (u16 i = 0; i < other.tail_; ++i) {
    storage_[i].memCopy(other.storage_[i].data(), other.storage_[i].size());
  }
  tail_ = other.tail_;
}

Stack& Stack::operator=(const Stack& other) {
  if (this == &other) return *this;
  releaseStorage();
  if (0 == other.capacity_) return *this;
  storage_ = static_cast<MemoryNode*>(MM->malloc(sizeof(MemoryNode) * other.capacity_));
  if (nullptr == storage_) return *this;
  capacity_ = other.capacity_;
  for (u16 i = 0; i < capacity_; ++i) new (&storage_[i]) MemoryNode();
  for (u16 i = 0; i < other.tail_; ++i) {
    storage_[i].memCopy(other.storage_[i].data(), other.storage_[i].size());
  }
  tail_ = other.tail_;
  return *this;
}

Stack::Stack(Stack&& other) noexcept
    : head_(other.head_),
      tail_(other.tail_),
      capacity_(other.capacity_),
      storage_(other.storage_) {
  other.head_ = 0;
  other.tail_ = 0;
  other.capacity_ = 0;
  other.storage_ = nullptr;
}

Stack& Stack::operator=(Stack&& other) noexcept {
  if (this == &other) return *this;
  releaseStorage();
  head_ = other.head_;
  tail_ = other.tail_;
  capacity_ = other.capacity_;
  storage_ = other.storage_;
  other.head_ = 0;
  other.tail_ = 0;
  other.capacity_ = 0;
  other.storage_ = nullptr;
  return *this;
}

s16 Stack::reset() {
  if (nullptr == storage_) return kErrorCode_NullData;
  for (u16 i = 0; i < tail_; ++i) storage_[i].reset();
  head_ = 0;
  tail_ = 0;
  return kErrorCode_Ok;
}

s16 Stack::softReset() {
  if (nullptr == storage_) return kErrorCode_NullData;
  for (u16 i = 0; i < tail_; ++i) storage_[i].softReset();
  head_ = 0;
  tail_ = 0;
  return kErrorCode_Ok;
}

s16 Stack::resize(u16 new_size) {
  if (0 == new_size) return kErrorCode_ZeroSize;
  if (new_size == capacity_) return kErrorCode_Ok;

  u16 keep_count = tail_;
  if (new_size < tail_) {
    for (u16 i = new_size; i < tail_; ++i) storage_[i].reset();
    keep_count = new_size;
  }

  MemoryNode* fresh = static_cast<MemoryNode*>(MM->malloc(sizeof(MemoryNode) * new_size));
  if (nullptr == fresh) return kErrorCode_Memory;
  for (u16 i = 0; i < new_size; ++i) new (&fresh[i]) MemoryNode();
  for (u16 i = 0; i < keep_count; ++i) {
    fresh[i].setData(storage_[i].data(), storage_[i].size());
    storage_[i].softReset();
  }
  for (u16 i = 0; i < capacity_; ++i) storage_[i].~MemoryNode();
  MM->free(storage_);
  storage_ = fresh;
  capacity_ = new_size;
  tail_ = keep_count;
  head_ = 0;
  return kErrorCode_Ok;
}

u16 Stack::capacity() const { return capacity_; }
u16 Stack::length() const { return tail_; }
bool Stack::isEmpty() const { return 0 == tail_; }
bool Stack::isFull() const { return tail_ == capacity_; }

void* Stack::top() const {
  if (0 == tail_) return nullptr;
  return storage_[tail_ - 1].data();
}

s16 Stack::push(void* data, u16 bytes) {
  if (nullptr == data) return kErrorCode_NullData;
  if (0 == bytes) return kErrorCode_ZeroSize;
  if (isFull()) return kErrorCode_IsFull;
  storage_[tail_].setData(data, bytes);
  tail_++;
  return kErrorCode_Ok;
}

void* Stack::pop(u16* size) {
  if (0 == tail_) return nullptr;
  void* data = storage_[tail_ - 1].data();
  if (nullptr != size) *size = storage_[tail_ - 1].size();
  tail_--;
  storage_[tail_].softReset();
  return data;
}

s16 Stack::concat(const Stack& src) {
  if (0 == src.tail_) return kErrorCode_Ok;
  u16 total = tail_ + src.tail_;
  if (total > capacity_) {
    s16 grow = resize(capacity_ + src.capacity_);
    if (kErrorCode_Ok != grow) return grow;
  }
  for (u16 i = 0; i < src.tail_; ++i) {
    s16 cp = storage_[tail_ + i].memCopy(src.storage_[i].data(), src.storage_[i].size());
    if (kErrorCode_Ok != cp) return cp;
  }
  tail_ = total;
  return kErrorCode_Ok;
}

s16 Stack::traverse(void (*callback)(MemoryNode&)) {
  if (nullptr == callback) return kErrorCode_Null;
  for (u16 i = head_; i < tail_; ++i) callback(storage_[i]);
  return kErrorCode_Ok;
}

void Stack::print() const {
  std::printf("[Stack Info] Address: %p\n", static_cast<const void*>(this));
  std::printf("[Stack Info] Length: %d\n", tail_);
  std::printf("[Stack Info] Capacity: %d\n", capacity_);
  if (nullptr == storage_) {
    std::printf("[Stack Info] Storage Address: NULL\n");
    return;
  }
  std::printf("[Stack Info] Storage Address: %p\n", static_cast<const void*>(storage_));
  for (u16 i = 0; i < tail_; ++i) {
    std::printf("    [Stack Info] Storage #%d\n", i);
    storage_[i].print();
  }
}
