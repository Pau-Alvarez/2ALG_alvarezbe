/*
 *
 * adt_stack.h
 * Toni Barella, Ivan Sancho as Unreal Authorized Instructor (UAI).
 * Algorithms and Data Structures.
 * ESAT 2020-2021
 *
 * C++ port.
 *
 */

#ifndef __ADT_STACK_H__
#define __ADT_STACK_H__

#include "common_def.h"
#include "adt_memory_node.h"

/**
 * @brief Stack ADT (LIFO) backed by a contiguous MemoryNode array.
 */
class Stack {
 public:
  explicit Stack(u16 capacity);
  ~Stack();

  Stack(const Stack& other);
  Stack& operator=(const Stack& other);
  Stack(Stack&& other) noexcept;
  Stack& operator=(Stack&& other) noexcept;

  s16 reset();
  s16 softReset();
  s16 resize(u16 new_size);

  u16 capacity() const;
  u16 length() const;
  bool isEmpty() const;
  bool isFull() const;

  void* top() const;
  s16 push(void* data, u16 bytes);
  void* pop(u16* size);

  s16 concat(const Stack& src);
  s16 traverse(void (*callback)(MemoryNode&));
  void print() const;

 private:
  u16 head_;
  u16 tail_;
  u16 capacity_;
  MemoryNode* storage_;

  void releaseStorage();
};

#endif  // __ADT_STACK_H__
