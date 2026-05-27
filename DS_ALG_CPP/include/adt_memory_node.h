/*
 *
 * adt_memory_node.h
 * Toni Barella, Ivan Sancho as Unreal Authorized Instructor (UAI).
 * Algorithms and Data Structures.
 * ESAT 2020-2021
 *
 * C++ port.
 *
 */

#ifndef __ADT_MEMORY_NODE_H__
#define __ADT_MEMORY_NODE_H__

#include "common_def.h"
#include "EDK_MemoryManager/edk_platform_types.h"

class List;
class DLList;

/**
 * @brief Generic memory node used by every ADT in the library.
 * @details A MemoryNode owns a raw byte buffer (@p data_) and the size of
 *          that buffer (@p size_). It can also be chained to its siblings
 *          through @p next_ and @p prev_ when the higher level container
 *          is a linked structure. Inheritance and polymorphism are not
 *          used: the node is a final concrete class.
 */
class MemoryNode {
 public:
  /**
   * @brief Default constructor. Initialises all attributes to zero/null.
   */
  MemoryNode();

  /**
   * @brief Destructor. Releases the data buffer if it owns one.
   */
  ~MemoryNode();

  /**
   * @brief Copy constructor. Deep copies the data buffer of @p other.
   * @param other Source node to copy from.
   */
  MemoryNode(const MemoryNode& other);

  /**
   * @brief Copy assignment. Deep copies the data buffer of @p other.
   * @param other Source node to copy from.
   * @return Reference to this node.
   */
  MemoryNode& operator=(const MemoryNode& other);

  /**
   * @brief Move constructor. Transfers ownership from @p other.
   * @param other Source node to move from. Its buffer is detached.
   */
  MemoryNode(MemoryNode&& other) noexcept;

  /**
   * @brief Move assignment. Transfers ownership from @p other.
   * @param other Source node to move from. Its buffer is detached.
   * @return Reference to this node.
   */
  MemoryNode& operator=(MemoryNode&& other) noexcept;

  /**
   * @brief Factory that allocates and default-constructs a MemoryNode.
   * @return Pointer to the new node, or @c nullptr on allocation failure.
   */
  static MemoryNode* create();

  /**
   * @brief Factory behind a double pointer reference.
   * @param node Address of the pointer that will receive the new node.
   * @return kErrorCode_Null if @p node is null,
   *         kErrorCode_AlreadyInitialized if @p *node is not null,
   *         kErrorCode_Memory if allocation fails,
   *         kErrorCode_Ok otherwise.
   */
  static s16 createFromRef(MemoryNode** node);

  /**
   * @brief Returns a reference to the owned data buffer.
   * @return Pointer to the data, or @c nullptr when empty.
   */
  void* data() const;

  /**
   * @brief Stores an external pointer inside the node without copying.
   * @param src Pointer to the source bytes.
   * @param bytes Size in bytes of @p src.
   * @return kErrorCode_Null / NullData / ZeroSize on bad input,
   *         kErrorCode_Ok otherwise.
   */
  s16 setData(void* src, u16 bytes);

  /**
   * @brief Returns the size in bytes of the data buffer.
   * @return Size in bytes, or 0 when empty.
   */
  u16 size() const;

  /**
   * @brief Frees the data buffer and clears every attribute.
   * @return kErrorCode_Ok.
   */
  s16 reset();

  /**
   * @brief Clears every attribute without freeing the data buffer.
   * @return kErrorCode_Ok.
   */
  s16 softReset();

  /**
   * @brief Sets every byte of the buffer to @p value.
   * @param value Byte value used as filler.
   * @return kErrorCode_NullData if the buffer is null,
   *         kErrorCode_Ok otherwise.
   */
  s16 memSet(u8 value);

  /**
   * @brief Allocates a new buffer and copies @p bytes bytes from @p src.
   * @param src Pointer to the source bytes.
   * @param bytes Size in bytes of @p src.
   * @return kErrorCode_Null / ZeroSize / Memory on failure,
   *         kErrorCode_Ok otherwise.
   */
  s16 memCopy(void* src, u16 bytes);

  /**
   * @brief Appends @p bytes bytes of @p src to the buffer (deep).
   * @param src Pointer to the bytes to append.
   * @param bytes Size in bytes of @p src.
   * @return kErrorCode_Null / ZeroSize / Memory on failure,
   *         kErrorCode_Ok otherwise.
   */
  s16 memConcat(void* src, u16 bytes);

  /**
   * @brief Applies a bitwise AND mask to every byte of the buffer.
   * @param mask The mask to apply.
   * @return kErrorCode_NullData if the buffer is null,
   *         kErrorCode_Ok otherwise.
   */
  s16 memMask(u8 mask);

  /**
   * @brief Prints the content of the node to standard output.
   */
  void print() const;

 private:
  void* data_;
  u16 size_;
  MemoryNode* next_;
  MemoryNode* prev_;

  friend class List;
  friend class DLList;
};

#endif  // __ADT_MEMORY_NODE_H__
