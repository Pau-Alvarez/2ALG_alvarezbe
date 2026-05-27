/*
 *
 * adt_dllist.h
 * Toni Barella, Ivan Sancho as Unreal Authorized Instructor (UAI).
 * Algorithms and Data Structures.
 * ESAT 2020-2021
 *
 * C++ port.
 *
 */

#ifndef __ADT_DLLIST_H__
#define __ADT_DLLIST_H__

#include "common_def.h"
#include "adt_memory_node.h"

/**
 * @brief Doubly linked list ADT with bounded capacity.
 * @details Each node is owned by a MemoryNode chained through @p next_ /
 *          @p prev_, so traversals are bidirectional and tail removal is
 *          O(1).
 */
class DLList {
 public:
  explicit DLList(u16 capacity);
  ~DLList();

  DLList(const DLList& other);
  DLList& operator=(const DLList& other);
  DLList(DLList&& other) noexcept;
  DLList& operator=(DLList&& other) noexcept;

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

  s16 concat(const DLList& src);
  s16 traverse(void (*callback)(MemoryNode&));
  void print() const;

 private:
  MemoryNode* head_;
  MemoryNode* tail_;
  u16 length_;
  u16 capacity_;

  void releaseChain(bool release_data);
};

#endif  // __ADT_DLLIST_H__
