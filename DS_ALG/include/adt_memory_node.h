/*
 *
 * adt_memory_node.h
 * Toni Barella, Ivan Sancho as Unreal Authorized Instructor (UAI).
 * Algorithms and Data Structures.
 * ESAT 2020-2021
 *
 */

#ifndef __ADT_MEMORY_NODE_H__
#define __ADT_MEMORY_NODE_H__

#include "EDK_MemoryManager/edk_platform_types.h"

/**
 * @brief Generic memory node used by every ADT in the library.
 * @details A MemoryNode owns a void pointer to a raw data buffer and the
 *          size of that buffer. It can be linked with its siblings through
 *          @p next_ and @p prev_ to build lists, queues, stacks, etc.
 *          The @p ops_ pointer exposes the public API for the node.
 */
typedef struct memory_node_s {
  void *data_;                       /**< Pointer to the raw data buffer. */
  u16 size_;                         /**< Size in bytes of @p data_. */
  struct memory_node_s *next_;       /**< Next node in the chain (NULL if none). */
  struct memory_node_s *prev_;       /**< Previous node in the chain (NULL if none). */
  struct memory_node_ops_s *ops_;    /**< Pointer to the Memory Node API. */
} MemoryNode;

/**
 * @brief Public API for the MemoryNode ADT.
 */
struct memory_node_ops_s {
  /**
   * @brief Returns a reference to the data stored inside the node.
   * @param node The node to query.
   * @return Pointer to the data buffer, or NULL when @p node is NULL.
   */
  void* (*data) (MemoryNode *node);

  /**
   * @brief Stores a reference to an external data buffer inside the node.
   * @param node The node that will own the reference.
   * @param src Pointer to the data buffer to store.
   * @param bytes Size in bytes of @p src.
   * @return kErrorCode_Null if @p node or @p src are NULL,
   *         kErrorCode_ZeroSize if @p bytes is 0,
   *         kErrorCode_Ok otherwise.
   */
  s16 (*setData) (MemoryNode *node, void *src, u16 bytes);

  /**
   * @brief Returns the size in bytes of the data owned by the node.
   * @param node The node to query.
   * @return Size of the data buffer, or 0 when @p node is NULL.
   */
  u16 (*size) (MemoryNode *node);

  /**
   * @brief Frees the data buffer owned by the node and clears its fields.
   * @details The node structure itself is preserved and remains usable.
   * @param node The node to reset.
   * @return kErrorCode_Null if @p node is NULL,
   *         kErrorCode_Ok otherwise.
   */
  s16 (*reset) (MemoryNode *node);

  /**
   * @brief Clears the fields of the node without freeing the data buffer.
   * @details The buffer pointed by @p data_ becomes orphan: ownership is
   *          assumed to be transferred to the caller.
   * @param node The node to soft reset.
   * @return kErrorCode_Null if @p node is NULL,
   *         kErrorCode_Ok otherwise.
   */
  s16 (*softReset) (MemoryNode *node);

  /**
   * @brief Frees the data buffer and then frees the node itself.
   * @param node The node to release.
   * @return kErrorCode_Null if @p node is NULL,
   *         kErrorCode_Ok otherwise.
   */
  s16 (*free) (MemoryNode *node);

  /**
   * @brief Frees the node structure but keeps the data buffer alive.
   * @param node The node to release.
   * @return kErrorCode_Null if @p node is NULL,
   *         kErrorCode_Ok otherwise.
   */
  s16 (*softFree) (MemoryNode *node);

  /**
   * @brief Sets every byte of the data buffer to a given value.
   * @param node The node whose buffer will be filled.
   * @param value The byte value to write across the buffer.
   * @return kErrorCode_Null if @p node is NULL,
   *         kErrorCode_NullData if the buffer is NULL,
   *         kErrorCode_Ok otherwise.
   */
  s16 (*memSet) (MemoryNode *node, u8 value);

  /**
   * @brief Allocates a buffer of @p bytes bytes and copies @p src into it.
   * @param node The destination node.
   * @param src Pointer to the source bytes to copy.
   * @param bytes Number of bytes to copy.
   * @return kErrorCode_Null if @p node or @p src are NULL,
   *         kErrorCode_ZeroSize if @p bytes is 0,
   *         kErrorCode_Memory if allocation fails,
   *         kErrorCode_Ok otherwise.
   */
  s16 (*memCopy) (MemoryNode *node, void *src, u16 bytes);

  /**
   * @brief Appends @p src to the current buffer growing the node.
   * @details A new buffer is allocated, the previous data is copied first
   *          and the new bytes are appended at the end. The previous buffer
   *          is freed.
   * @param node The node to expand.
   * @param src Pointer to the bytes to append.
   * @param bytes Number of bytes to append.
   * @return kErrorCode_Null if @p node or @p src are NULL,
   *         kErrorCode_ZeroSize if @p bytes is 0,
   *         kErrorCode_Memory if allocation fails,
   *         kErrorCode_Ok otherwise.
   */
  s16 (*memConcat) (MemoryNode *node, void *src, u16 bytes);

  /**
   * @brief Applies a bitwise AND mask to every byte of the buffer.
   * @param node The node whose buffer will be masked.
   * @param mask The bitmask to apply.
   * @return kErrorCode_Null if @p node is NULL,
   *         kErrorCode_NullData if the buffer is NULL,
   *         kErrorCode_Ok otherwise.
   */
  s16 (*memMask) (MemoryNode *node, u8 mask);

  /**
   * @brief Prints the content of the node to standard output.
   * @param node The node to print.
   */
  void (*print) (MemoryNode *node);
};

/**
 * @brief Allocates and initialises a new MemoryNode on the heap.
 * @return Pointer to the new node, or NULL on allocation failure.
 */
MemoryNode* MEMNODE_create();

/**
 * @brief Allocates a node behind a pointer-to-pointer reference.
 * @param node Address of the pointer that will receive the new node.
 * @return kErrorCode_Null if @p node is NULL,
 *         kErrorCode_AlreadyInitialized if @p *node is not NULL,
 *         kErrorCode_Memory if the allocation fails,
 *         kErrorCode_Ok otherwise.
 */
s16 MEMNODE_createFromRef(MemoryNode **node);

/**
 * @brief Initialises a pre-allocated MemoryNode without sanity checks.
 * @param node The node to initialise.
 * @return kErrorCode_Null if @p node is NULL,
 *         kErrorCode_Ok otherwise.
 */
s16 MEMNODE_createLite(MemoryNode *node);

#endif // __ADT_MEMORY_NODE_H__
