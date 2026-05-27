/*
 * Ivan Sancho as Unreal Authorized Instructor (UAI), 2025.
 *
 * Vector source code (C++ port).
 * Algorithms and Data Structures.
 *
 * Student:
 *   Pau Alvarez Belenguer
 *
 */

#include <cstdio>
#include <cstring>
#include <new>

#include "common_def.h"
#include "adt_vector.h"

#include "EDK_MemoryManager/edk_memory_manager.h"

Vector::Vector(u16 capacity)
    : head_(0), tail_(0), capacity_(capacity), storage_(nullptr) {
  if (0 == capacity) {
    return;
  }
  storage_ = static_cast<MemoryNode*>(MM->malloc(sizeof(MemoryNode) * capacity));
  if (nullptr == storage_) {
    capacity_ = 0;
    return;
  }
  for (u16 i = 0; i < capacity; ++i) {
    new (&storage_[i]) MemoryNode();
  }
}

Vector::~Vector() {
  releaseStorage();
}

void Vector::releaseStorage() {
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

Vector::Vector(const Vector& other)
    : head_(0), tail_(0), capacity_(0), storage_(nullptr) {
  if (other.capacity_ == 0) {
    return;
  }
  storage_ = static_cast<MemoryNode*>(MM->malloc(sizeof(MemoryNode) * other.capacity_));
  if (nullptr == storage_) {
    return;
  }
  capacity_ = other.capacity_;
  for (u16 i = 0; i < capacity_; ++i) {
    new (&storage_[i]) MemoryNode();
  }
  for (u16 i = 0; i < other.tail_; ++i) {
    storage_[i].memCopy(other.storage_[i].data_, other.storage_[i].size_);
  }
  head_ = 0;
  tail_ = other.tail_;
}

Vector& Vector::operator=(const Vector& other) {
  if (this == &other) {
    return *this;
  }
  releaseStorage();
  if (other.capacity_ == 0) {
    return *this;
  }
  storage_ = static_cast<MemoryNode*>(MM->malloc(sizeof(MemoryNode) * other.capacity_));
  if (nullptr == storage_) {
    return *this;
  }
  capacity_ = other.capacity_;
  for (u16 i = 0; i < capacity_; ++i) {
    new (&storage_[i]) MemoryNode();
  }
  for (u16 i = 0; i < other.tail_; ++i) {
    storage_[i].memCopy(other.storage_[i].data_, other.storage_[i].size_);
  }
  head_ = 0;
  tail_ = other.tail_;
  return *this;
}

Vector::Vector(Vector&& other) noexcept
    : head_(other.head_),
      tail_(other.tail_),
      capacity_(other.capacity_),
      storage_(other.storage_) {
  other.head_ = 0;
  other.tail_ = 0;
  other.capacity_ = 0;
  other.storage_ = nullptr;
}

Vector& Vector::operator=(Vector&& other) noexcept {
  if (this == &other) {
    return *this;
  }
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

s16 Vector::reset() {
  if (nullptr == storage_) return kErrorCode_NullData;
  for (u16 i = 0; i < tail_; ++i) {
    storage_[i].reset();
  }
  head_ = 0;
  tail_ = 0;
  return kErrorCode_Ok;
}

s16 Vector::softReset() {
  if (nullptr == storage_) return kErrorCode_NullData;
  for (u16 i = 0; i < tail_; ++i) {
    storage_[i].softReset();
  }
  head_ = 0;
  tail_ = 0;
  return kErrorCode_Ok;
}

s16 Vector::resize(u16 new_size) {
  if (0 == new_size) return kErrorCode_ZeroSize;
  if (new_size == capacity_) return kErrorCode_Ok;

  u16 keep_count = tail_;
  if (new_size < tail_) {
    for (u16 i = new_size; i < tail_; ++i) {
      storage_[i].reset();
    }
    keep_count = new_size;
  }

  MemoryNode* fresh_storage = static_cast<MemoryNode*>(MM->malloc(sizeof(MemoryNode) * new_size));
  if (nullptr == fresh_storage) {
    return kErrorCode_Memory;
  }
  for (u16 i = 0; i < new_size; ++i) {
    new (&fresh_storage[i]) MemoryNode();
  }
  for (u16 i = 0; i < keep_count; ++i) {
    fresh_storage[i].data_ = storage_[i].data_;
    fresh_storage[i].size_ = storage_[i].size_;
    storage_[i].data_ = nullptr;
    storage_[i].size_ = 0;
  }

  for (u16 i = 0; i < capacity_; ++i) {
    storage_[i].~MemoryNode();
  }
  MM->free(storage_);

  storage_ = fresh_storage;
  capacity_ = new_size;
  tail_ = keep_count;
  head_ = 0;
  return kErrorCode_Ok;
}

u16 Vector::capacity() const { return capacity_; }
u16 Vector::length() const { return tail_; }
bool Vector::isEmpty() const { return head_ == tail_; }
bool Vector::isFull() const { return tail_ == capacity_; }

void* Vector::first(u16* size) const {
  if (nullptr == size) return nullptr;
  if (0 == tail_) return nullptr;
  *size = storage_[0].size_;
  return storage_[0].data_;
}

void* Vector::last(u16* size) const {
  if (nullptr == size) return nullptr;
  if (0 == tail_) return nullptr;
  *size = storage_[tail_ - 1].size_;
  return storage_[tail_ - 1].data_;
}

void* Vector::at(u16* size, u16 position) const {
  if (nullptr == size) return nullptr;
  if (position >= tail_) return nullptr;
  *size = storage_[position].size_;
  return storage_[position].data_;
}

s16 Vector::insertFirst(void* data, u16 bytes) {
  if (nullptr == data) return kErrorCode_NullData;
  if (0 == bytes) return kErrorCode_ZeroSize;
  if (isFull()) return kErrorCode_IsFull;

  for (u16 i = tail_; i > 0; --i) {
    storage_[i].data_ = storage_[i - 1].data_;
    storage_[i].size_ = storage_[i - 1].size_;
  }
  storage_[0].data_ = data;
  storage_[0].size_ = bytes;
  tail_++;
  return kErrorCode_Ok;
}

s16 Vector::insertLast(void* data, u16 bytes) {
  if (nullptr == data) return kErrorCode_NullData;
  if (0 == bytes) return kErrorCode_ZeroSize;
  if (isFull()) return kErrorCode_IsFull;

  storage_[tail_].data_ = data;
  storage_[tail_].size_ = bytes;
  tail_++;
  return kErrorCode_Ok;
}

s16 Vector::insertAt(void* data, u16 bytes, u16 position) {
  if (nullptr == data) return kErrorCode_NullData;
  if (0 == bytes) return kErrorCode_ZeroSize;
  if (isFull()) return kErrorCode_IsFull;

  if (position >= tail_) {
    return insertLast(data, bytes);
  }
  if (0 == position) {
    return insertFirst(data, bytes);
  }

  for (u16 i = tail_; i > position; --i) {
    storage_[i].data_ = storage_[i - 1].data_;
    storage_[i].size_ = storage_[i - 1].size_;
  }
  storage_[position].data_ = data;
  storage_[position].size_ = bytes;
  tail_++;
  return kErrorCode_Ok;
}

void* Vector::extractFirst(u16* size) {
  if (nullptr == size) return nullptr;
  if (isEmpty()) return nullptr;

  void* extracted = storage_[0].data_;
  *size = storage_[0].size_;
  for (u16 i = 0; i < tail_ - 1; ++i) {
    storage_[i].data_ = storage_[i + 1].data_;
    storage_[i].size_ = storage_[i + 1].size_;
  }
  tail_--;
  storage_[tail_].data_ = nullptr;
  storage_[tail_].size_ = 0;
  return extracted;
}

void* Vector::extractLast(u16* size) {
  if (nullptr == size) return nullptr;
  if (isEmpty()) return nullptr;

  void* extracted = storage_[tail_ - 1].data_;
  *size = storage_[tail_ - 1].size_;
  tail_--;
  storage_[tail_].data_ = nullptr;
  storage_[tail_].size_ = 0;
  return extracted;
}

void* Vector::extractAt(u16* size, u16 position) {
  if (nullptr == size) return nullptr;
  if (isEmpty()) return nullptr;
  if (position >= tail_) return nullptr;

  if (0 == position) return extractFirst(size);
  if (position == tail_ - 1) return extractLast(size);

  void* extracted = storage_[position].data_;
  *size = storage_[position].size_;
  for (u16 i = position; i < tail_ - 1; ++i) {
    storage_[i].data_ = storage_[i + 1].data_;
    storage_[i].size_ = storage_[i + 1].size_;
  }
  tail_--;
  storage_[tail_].data_ = nullptr;
  storage_[tail_].size_ = 0;
  return extracted;
}

s16 Vector::concat(const Vector& src) {
  if (0 == src.tail_) {
    return kErrorCode_Ok;
  }
  u16 total = tail_ + src.tail_;
  if (total > capacity_) {
    s16 grow = resize(capacity_ + src.capacity_);
    if (kErrorCode_Ok != grow) return grow;
  }
  for (u16 i = 0; i < src.tail_; ++i) {
    s16 cp = storage_[tail_ + i].memCopy(src.storage_[i].data_, src.storage_[i].size_);
    if (kErrorCode_Ok != cp) return cp;
  }
  tail_ = total;
  return kErrorCode_Ok;
}

s16 Vector::traverse(void (*callback)(MemoryNode&)) {
  if (nullptr == callback) return kErrorCode_Null;
  for (u16 i = head_; i < tail_; ++i) {
    callback(storage_[i]);
  }
  return kErrorCode_Ok;
}

void Vector::print() const {
  std::printf("[Vector Info] Address: %p\n", static_cast<const void*>(this));
  std::printf("[Vector Info] Head: %d\n", head_);
  std::printf("[Vector Info] Tail: %d\n", tail_);
  std::printf("[Vector Info] Length: %d\n", (tail_ - head_));
  std::printf("[Vector Info] Capacity: %d\n", capacity_);
  if (nullptr == storage_) {
    std::printf("[Vector Info] Storage Address: NULL\n");
    return;
  }
  std::printf("[Vector Info] Storage Address: %p\n", static_cast<const void*>(storage_));
  for (u16 i = 0; i < tail_; ++i) {
    std::printf("    [Vector Info] Storage #%d\n", i);
    storage_[i].print();
  }
}
