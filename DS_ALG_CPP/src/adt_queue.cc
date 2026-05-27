/*
 * Ivan Sancho as Unreal Authorized Instructor (UAI), 2025.
 *
 * Queue source code (C++ port).
 * Algorithms and Data Structures.
 *
 * Student:
 *   Pau Alvarez Belenguer
 *
 */

#include <cstdio>
#include <new>

#include "common_def.h"
#include "adt_queue.h"

#include "EDK_MemoryManager/edk_memory_manager.h"

Queue::Queue(u16 capacity)
    : head_(0), tail_(0), length_(0), capacity_(capacity), storage_(nullptr) {
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

Queue::~Queue() {
  releaseStorage();
}

void Queue::releaseStorage() {
  if (nullptr != storage_) {
    for (u16 i = 0; i < capacity_; ++i) {
      storage_[i].~MemoryNode();
    }
    MM->free(storage_);
    storage_ = nullptr;
  }
  head_ = 0;
  tail_ = 0;
  length_ = 0;
  capacity_ = 0;
}

Queue::Queue(const Queue& other)
    : head_(0), tail_(0), length_(0), capacity_(0), storage_(nullptr) {
  if (0 == other.capacity_) return;
  storage_ = static_cast<MemoryNode*>(MM->malloc(sizeof(MemoryNode) * other.capacity_));
  if (nullptr == storage_) return;
  capacity_ = other.capacity_;
  for (u16 i = 0; i < capacity_; ++i) new (&storage_[i]) MemoryNode();
  for (u16 i = 0; i < other.length_; ++i) {
    u16 src_idx = (other.head_ + i) % other.capacity_;
    storage_[i].memCopy(other.storage_[src_idx].data(),
                        other.storage_[src_idx].size());
  }
  tail_ = (other.length_ == capacity_) ? 0 : other.length_;
  length_ = other.length_;
}

Queue& Queue::operator=(const Queue& other) {
  if (this == &other) return *this;
  releaseStorage();
  if (0 == other.capacity_) return *this;
  storage_ = static_cast<MemoryNode*>(MM->malloc(sizeof(MemoryNode) * other.capacity_));
  if (nullptr == storage_) return *this;
  capacity_ = other.capacity_;
  for (u16 i = 0; i < capacity_; ++i) new (&storage_[i]) MemoryNode();
  for (u16 i = 0; i < other.length_; ++i) {
    u16 src_idx = (other.head_ + i) % other.capacity_;
    storage_[i].memCopy(other.storage_[src_idx].data(),
                        other.storage_[src_idx].size());
  }
  tail_ = (other.length_ == capacity_) ? 0 : other.length_;
  length_ = other.length_;
  return *this;
}

Queue::Queue(Queue&& other) noexcept
    : head_(other.head_),
      tail_(other.tail_),
      length_(other.length_),
      capacity_(other.capacity_),
      storage_(other.storage_) {
  other.head_ = 0;
  other.tail_ = 0;
  other.length_ = 0;
  other.capacity_ = 0;
  other.storage_ = nullptr;
}

Queue& Queue::operator=(Queue&& other) noexcept {
  if (this == &other) return *this;
  releaseStorage();
  head_ = other.head_;
  tail_ = other.tail_;
  length_ = other.length_;
  capacity_ = other.capacity_;
  storage_ = other.storage_;
  other.head_ = 0;
  other.tail_ = 0;
  other.length_ = 0;
  other.capacity_ = 0;
  other.storage_ = nullptr;
  return *this;
}

s16 Queue::reset() {
  if (nullptr == storage_) return kErrorCode_NullData;
  for (u16 i = 0; i < length_; ++i) {
    u16 idx = (head_ + i) % capacity_;
    storage_[idx].reset();
  }
  head_ = 0;
  tail_ = 0;
  length_ = 0;
  return kErrorCode_Ok;
}

s16 Queue::softReset() {
  if (nullptr == storage_) return kErrorCode_NullData;
  for (u16 i = 0; i < length_; ++i) {
    u16 idx = (head_ + i) % capacity_;
    storage_[idx].softReset();
  }
  head_ = 0;
  tail_ = 0;
  length_ = 0;
  return kErrorCode_Ok;
}

s16 Queue::resize(u16 new_size) {
  if (0 == new_size) return kErrorCode_ZeroSize;
  if (new_size == capacity_) return kErrorCode_Ok;

  u16 keep_count = length_;
  if (new_size < length_) {
    for (u16 i = new_size; i < length_; ++i) {
      u16 idx = (head_ + i) % capacity_;
      storage_[idx].reset();
    }
    keep_count = new_size;
  }

  MemoryNode* fresh = static_cast<MemoryNode*>(MM->malloc(sizeof(MemoryNode) * new_size));
  if (nullptr == fresh) return kErrorCode_Memory;
  for (u16 i = 0; i < new_size; ++i) new (&fresh[i]) MemoryNode();
  for (u16 i = 0; i < keep_count; ++i) {
    u16 src_idx = (head_ + i) % capacity_;
    fresh[i].setData(storage_[src_idx].data(), storage_[src_idx].size());
    storage_[src_idx].softReset();
  }

  for (u16 i = 0; i < capacity_; ++i) storage_[i].~MemoryNode();
  MM->free(storage_);

  storage_ = fresh;
  capacity_ = new_size;
  head_ = 0;
  tail_ = (keep_count == new_size) ? 0 : keep_count;
  length_ = keep_count;
  return kErrorCode_Ok;
}

u16 Queue::capacity() const { return capacity_; }
u16 Queue::length() const { return length_; }
bool Queue::isEmpty() const { return 0 == length_; }
bool Queue::isFull() const { return length_ == capacity_; }

void* Queue::front() const {
  if (0 == length_) return nullptr;
  return storage_[head_].data();
}

void* Queue::back() const {
  if (0 == length_) return nullptr;
  u16 idx = (tail_ + capacity_ - 1) % capacity_;
  return storage_[idx].data();
}

s16 Queue::enqueue(void* data, u16 bytes) {
  if (nullptr == data) return kErrorCode_NullData;
  if (0 == bytes) return kErrorCode_ZeroSize;
  if (isFull()) return kErrorCode_IsFull;
  storage_[tail_].setData(data, bytes);
  tail_ = (tail_ + 1) % capacity_;
  length_++;
  return kErrorCode_Ok;
}

void* Queue::dequeue() {
  if (0 == length_) return nullptr;
  void* data = storage_[head_].data();
  storage_[head_].softReset();
  head_ = (head_ + 1) % capacity_;
  length_--;
  return data;
}

s16 Queue::concat(const Queue& src) {
  if (0 == src.length_) return kErrorCode_Ok;
  u16 total = length_ + src.length_;
  if (total > capacity_) {
    s16 grow = resize(capacity_ + src.capacity_);
    if (kErrorCode_Ok != grow) return grow;
  }
  for (u16 i = 0; i < src.length_; ++i) {
    u16 src_idx = (src.head_ + i) % src.capacity_;
    s16 cp = storage_[tail_].memCopy(src.storage_[src_idx].data(),
                                     src.storage_[src_idx].size());
    if (kErrorCode_Ok != cp) return cp;
    tail_ = (tail_ + 1) % capacity_;
    length_++;
  }
  return kErrorCode_Ok;
}

s16 Queue::traverse(void (*callback)(MemoryNode&)) {
  if (nullptr == callback) return kErrorCode_Null;
  for (u16 i = 0; i < length_; ++i) {
    u16 idx = (head_ + i) % capacity_;
    callback(storage_[idx]);
  }
  return kErrorCode_Ok;
}

void Queue::print() const {
  std::printf("[Queue Info] Address: %p\n", static_cast<const void*>(this));
  std::printf("[Queue Info] Head: %d\n", head_);
  std::printf("[Queue Info] Tail: %d\n", tail_);
  std::printf("[Queue Info] Length: %d\n", length_);
  std::printf("[Queue Info] Capacity: %d\n", capacity_);
  if (nullptr == storage_) {
    std::printf("[Queue Info] Storage Address: NULL\n");
    return;
  }
  std::printf("[Queue Info] Storage Address: %p\n", static_cast<const void*>(storage_));
  for (u16 i = 0; i < length_; ++i) {
    u16 idx = (head_ + i) % capacity_;
    std::printf("    [Queue Info] Logical #%d (raw #%d)\n", i, idx);
    storage_[idx].print();
  }
}
