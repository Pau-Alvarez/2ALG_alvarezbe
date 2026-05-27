/*
 * Ivan Sancho as Unreal Authorized Instructor (UAI), 2025.
 *
 * List source code (C++ port).
 * Algorithms and Data Structures.
 *
 * Student:
 *   Pau Alvarez Belenguer
 *
 */

#include <cstdio>

#include "common_def.h"
#include "adt_list.h"

#include "EDK_MemoryManager/edk_memory_manager.h"

List::List(u16 capacity)
    : head_(nullptr), tail_(nullptr), length_(0), capacity_(capacity) {}

List::~List() {
  releaseChain(true);
}

void List::releaseChain(bool release_data) {
  MemoryNode* current = head_;
  while (nullptr != current) {
    MemoryNode* next = current->next_;
    if (release_data) {
      delete current;
    }
    else {
      current->softReset();
      delete current;
    }
    current = next;
  }
  head_ = nullptr;
  tail_ = nullptr;
  length_ = 0;
}

List::List(const List& other)
    : head_(nullptr), tail_(nullptr), length_(0), capacity_(other.capacity_) {
  MemoryNode* runner = other.head_;
  while (nullptr != runner) {
    MemoryNode* node = new MemoryNode();
    node->memCopy(runner->data(), runner->size());
    if (nullptr == tail_) {
      head_ = node;
      tail_ = node;
    } else {
      tail_->next_ = node;
      tail_ = node;
    }
    length_++;
    runner = runner->next_;
  }
}

List& List::operator=(const List& other) {
  if (this == &other) {
    return *this;
  }
  releaseChain(true);
  capacity_ = other.capacity_;
  MemoryNode* runner = other.head_;
  while (nullptr != runner) {
    MemoryNode* node = new MemoryNode();
    node->memCopy(runner->data(), runner->size());
    if (nullptr == tail_) {
      head_ = node;
      tail_ = node;
    } else {
      tail_->next_ = node;
      tail_ = node;
    }
    length_++;
    runner = runner->next_;
  }
  return *this;
}

List::List(List&& other) noexcept
    : head_(other.head_),
      tail_(other.tail_),
      length_(other.length_),
      capacity_(other.capacity_) {
  other.head_ = nullptr;
  other.tail_ = nullptr;
  other.length_ = 0;
  other.capacity_ = 0;
}

List& List::operator=(List&& other) noexcept {
  if (this == &other) {
    return *this;
  }
  releaseChain(true);
  head_ = other.head_;
  tail_ = other.tail_;
  length_ = other.length_;
  capacity_ = other.capacity_;
  other.head_ = nullptr;
  other.tail_ = nullptr;
  other.length_ = 0;
  other.capacity_ = 0;
  return *this;
}

s16 List::reset() {
  releaseChain(true);
  return kErrorCode_Ok;
}

s16 List::softReset() {
  releaseChain(false);
  return kErrorCode_Ok;
}

s16 List::resize(u16 new_size) {
  if (0 == new_size) return kErrorCode_ZeroSize;
  if (new_size < length_) {
    MemoryNode* current = head_;
    for (u16 i = 0; i < new_size - 1 && nullptr != current; ++i) {
      current = current->next_;
    }
    MemoryNode* discard = (nullptr != current) ? current->next_ : nullptr;
    if (nullptr != current) {
      current->next_ = nullptr;
      tail_ = current;
    }
    while (nullptr != discard) {
      MemoryNode* next = discard->next_;
      delete discard;
      discard = next;
    }
    length_ = new_size;
  }
  capacity_ = new_size;
  return kErrorCode_Ok;
}

u16 List::capacity() const { return capacity_; }
u16 List::length() const { return length_; }
bool List::isEmpty() const { return 0 == length_; }
bool List::isFull() const { return length_ == capacity_; }

void* List::first(u16* size) const {
  if (nullptr == size) return nullptr;
  if (nullptr == head_) return nullptr;
  *size = head_->size();
  return head_->data();
}

void* List::last(u16* size) const {
  if (nullptr == size) return nullptr;
  if (nullptr == tail_) return nullptr;
  *size = tail_->size();
  return tail_->data();
}

void* List::at(u16* size, u16 position) const {
  if (nullptr == size) return nullptr;
  if (position >= length_) return nullptr;
  MemoryNode* current = head_;
  for (u16 i = 0; i < position; ++i) {
    current = current->next_;
  }
  *size = current->size();
  return current->data();
}

s16 List::insertFirst(void* data, u16 bytes) {
  if (nullptr == data) return kErrorCode_NullData;
  if (0 == bytes) return kErrorCode_ZeroSize;
  if (isFull()) return kErrorCode_IsFull;

  MemoryNode* node = new MemoryNode();
  if (nullptr == node) return kErrorCode_Memory;
  node->setData(data, bytes);
  if (nullptr == head_) {
    head_ = node;
    tail_ = node;
  } else {
    node->next_ = head_;
    head_ = node;
  }
  length_++;
  return kErrorCode_Ok;
}

s16 List::insertLast(void* data, u16 bytes) {
  if (nullptr == data) return kErrorCode_NullData;
  if (0 == bytes) return kErrorCode_ZeroSize;
  if (isFull()) return kErrorCode_IsFull;

  MemoryNode* node = new MemoryNode();
  if (nullptr == node) return kErrorCode_Memory;
  node->setData(data, bytes);
  if (nullptr == tail_) {
    head_ = node;
    tail_ = node;
  } else {
    tail_->next_ = node;
    tail_ = node;
  }
  length_++;
  return kErrorCode_Ok;
}

s16 List::insertAt(void* data, u16 bytes, u16 position) {
  if (nullptr == data) return kErrorCode_NullData;
  if (0 == bytes) return kErrorCode_ZeroSize;
  if (isFull()) return kErrorCode_IsFull;

  if (0 == position) return insertFirst(data, bytes);
  if (position >= length_) return insertLast(data, bytes);

  MemoryNode* prev = head_;
  for (u16 i = 0; i < position - 1; ++i) {
    prev = prev->next_;
  }
  MemoryNode* node = new MemoryNode();
  if (nullptr == node) return kErrorCode_Memory;
  node->setData(data, bytes);
  node->next_ = prev->next_;
  prev->next_ = node;
  length_++;
  return kErrorCode_Ok;
}

void* List::extractFirst(u16* size) {
  if (nullptr == size) return nullptr;
  if (nullptr == head_) return nullptr;
  MemoryNode* old_head = head_;
  void* data = old_head->data();
  *size = old_head->size();
  head_ = old_head->next_;
  if (nullptr == head_) tail_ = nullptr;
  length_--;
  old_head->softReset();
  delete old_head;
  return data;
}

void* List::extractLast(u16* size) {
  if (nullptr == size) return nullptr;
  if (nullptr == tail_) return nullptr;
  MemoryNode* old_tail = tail_;
  void* data = old_tail->data();
  *size = old_tail->size();
  if (head_ == tail_) {
    head_ = nullptr;
    tail_ = nullptr;
  } else {
    MemoryNode* runner = head_;
    while (runner->next_ != old_tail) {
      runner = runner->next_;
    }
    runner->next_ = nullptr;
    tail_ = runner;
  }
  length_--;
  old_tail->softReset();
  delete old_tail;
  return data;
}

void* List::extractAt(u16* size, u16 position) {
  if (nullptr == size) return nullptr;
  if (position >= length_) return nullptr;
  if (0 == position) return extractFirst(size);
  if (position == length_ - 1) return extractLast(size);

  MemoryNode* prev = head_;
  for (u16 i = 0; i < position - 1; ++i) {
    prev = prev->next_;
  }
  MemoryNode* target = prev->next_;
  void* data = target->data();
  *size = target->size();
  prev->next_ = target->next_;
  length_--;
  target->softReset();
  delete target;
  return data;
}

s16 List::concat(const List& src) {
  u16 total = length_ + src.length_;
  if (total > capacity_) {
    s16 grow = resize(total);
    if (kErrorCode_Ok != grow) return grow;
  }
  MemoryNode* runner = src.head_;
  while (nullptr != runner) {
    MemoryNode* node = new MemoryNode();
    if (nullptr == node) return kErrorCode_Memory;
    s16 cp = node->memCopy(runner->data(), runner->size());
    if (kErrorCode_Ok != cp) {
      node->softReset();
      delete node;
      return cp;
    }
    if (nullptr == tail_) {
      head_ = node;
      tail_ = node;
    } else {
      tail_->next_ = node;
      tail_ = node;
    }
    length_++;
    runner = runner->next_;
  }
  return kErrorCode_Ok;
}

s16 List::traverse(void (*callback)(MemoryNode&)) {
  if (nullptr == callback) return kErrorCode_Null;
  MemoryNode* current = head_;
  while (nullptr != current) {
    callback(*current);
    current = current->next_;
  }
  return kErrorCode_Ok;
}

void List::print() const {
  std::printf("[List Info] Address: %p\n", static_cast<const void*>(this));
  std::printf("[List Info] Length: %d\n", length_);
  std::printf("[List Info] Capacity: %d\n", capacity_);
  std::printf("[List Info] Head: %p\n", static_cast<const void*>(head_));
  std::printf("[List Info] Tail: %p\n", static_cast<const void*>(tail_));
  MemoryNode* current = head_;
  u16 idx = 0;
  while (nullptr != current) {
    std::printf("    [List Info] Node #%d\n", idx);
    current->print();
    current = current->next_;
    idx++;
  }
}
