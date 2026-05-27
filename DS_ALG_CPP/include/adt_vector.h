/*
 *
 * adt_vector.h
 * Toni Barella, Ivan Sancho as Unreal Authorized Instructor (UAI).
 * Algorithms and Data Structures.
 * ESAT 2020-2021
 *
 * C++ port.
 *
 */

#ifndef __ADT_VECTOR_H__
#define __ADT_VECTOR_H__

#include "common_def.h"
#include "adt_memory_node.h"

/**
 * @brief Linear array based ADT with bounded capacity.
 * @details Elements are stored sequentially in a contiguous array of
 *          MemoryNode. Index 0 is always the head and @p tail_ points to
 *          the first free position.
 */
class Vector {
 public:
  /**
   * @brief Constructs a Vector with the given capacity.
   * @param capacity Maximum number of elements. Must be > 0.
   */
  explicit Vector(u16 capacity);

  /**
   * @brief Destructor. Releases the storage and every owned buffer.
   */
  ~Vector();

  /**
   * @brief Copy constructor. Deep copies every element of @p other.
   * @param other Source vector to copy from.
   */
  Vector(const Vector& other);

  /**
   * @brief Copy assignment. Deep copies every element of @p other.
   * @param other Source vector to copy from.
   * @return Reference to this vector.
   */
  Vector& operator=(const Vector& other);

  /**
   * @brief Move constructor. Transfers ownership from @p other.
   * @param other Source vector to move from. Left empty.
   */
  Vector(Vector&& other) noexcept;

  /**
   * @brief Move assignment. Transfers ownership from @p other.
   * @param other Source vector to move from. Left empty.
   * @return Reference to this vector.
   */
  Vector& operator=(Vector&& other) noexcept;

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

  s16 concat(const Vector& src);
  s16 traverse(void (*callback)(MemoryNode&));
  void print() const;

 private:
  u16 head_;
  u16 tail_;
  u16 capacity_;
  MemoryNode* storage_;

  void releaseStorage();
};

#endif  // __ADT_VECTOR_H__
