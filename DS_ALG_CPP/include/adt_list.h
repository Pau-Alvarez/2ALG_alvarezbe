/*
 *
 * adt_list.h
 * Toni Barella, Ivan Sancho as Unreal Authorized Instructor (UAI).
 * Algorithms and Data Structures.
 * ESAT 2020-2021
 *
 * C++ port.
 *
 */

#ifndef __ADT_LIST_H__
#define __ADT_LIST_H__

#include "common_def.h"
#include "adt_memory_node.h"

/**
 * @brief Singly linked list ADT with bounded capacity.
 * @details Each element is owned by a MemoryNode linked through @p next_.
 *          The list keeps direct references to head and tail.
 */
class List {
 public:
  explicit List(u16 capacity);
  ~List();

  List(const List& other);
  List& operator=(const List& other);
  List(List&& other) noexcept;
  List& operator=(List&& other) noexcept;

  s16 reset();
  s16 softReset();
  s16 resize(u16 new_size);

  u16 capacity() const;
  u16 length() const;
  bool isEmpty() const;
  bool isFull() const;

  void* first(u16* size) const;
  void* last(u16* size) const;
  void* at(u16* size, u16 position) const;

  s16 insertFirst(void* data, u16 bytes);
  s16 insertLast(void* data, u16 bytes);
  s16 insertAt(void* data, u16 bytes, u16 position);

  void* extractFirst(u16* size);
  void* extractLast(u16* size);
  void* extractAt(u16* size, u16 position);

  s16 concat(const List& src);
  s16 traverse(void (*callback)(MemoryNode&));
  void print() const;

 private:
  MemoryNode* head_;
  MemoryNode* tail_;
  u16 length_;
  u16 capacity_;

  void releaseChain(bool release_data);
};

#endif  // __ADT_LIST_H__
