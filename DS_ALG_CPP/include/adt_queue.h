/*
 *
 * adt_queue.h
 * Toni Barella, Ivan Sancho as Unreal Authorized Instructor (UAI).
 * Algorithms and Data Structures.
 * ESAT 2020-2021
 *
 * C++ port.
 *
 */

#ifndef __ADT_QUEUE_H__
#define __ADT_QUEUE_H__

#include "common_def.h"
#include "adt_memory_node.h"

/**
 * @brief Queue ADT (FIFO) backed by a circular MemoryNode array.
 */
class Queue {
 public:
  explicit Queue(u16 capacity);
  ~Queue();

  Queue(const Queue& other);
  Queue& operator=(const Queue& other);
  Queue(Queue&& other) noexcept;
  Queue& operator=(Queue&& other) noexcept;

  s16 reset();
  s16 softReset();
  s16 resize(u16 new_size);

  u16 capacity() const;
  u16 length() const;
  bool isEmpty() const;
  bool isFull() const;

  void* front() const;
  void* back() const;
  s16 enqueue(void* data, u16 bytes);
  void* dequeue();

  s16 concat(const Queue& src);
  s16 traverse(void (*callback)(MemoryNode&));
  void print() const;

 private:
  u16 head_;
  u16 tail_;
  u16 length_;
  u16 capacity_;
  MemoryNode* storage_;

  void releaseStorage();
};

#endif  // __ADT_QUEUE_H__
