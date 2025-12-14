// adt_memory_node.h
// Toni Barella, Ivan Sancho
// Algoritmos & Inteligencia Artificial
// ESAT 2020-2021
//

#ifndef __ADT_MEMORY_NODE_H__
#define __ADT_MEMORY_NODE_H__

#include "EDK_MemoryManager/edk_platform_types.h"

// Memory Node type
typedef struct memory_node_s {
  void *data_;
  u16 size_;
  struct memory_node_s *next_;
  struct memory_node_s *prev_;
  struct memory_node_ops_s *ops_;
} MemoryNode;

// Memory Node's API Declarations
struct memory_node_ops_s {
  /**
 * @brief Returns a reference to the data of a given node
 * @param node The node from which to retrieve the data
 * @return Pointer to the data of the specified node
 */
  void*(*data) (MemoryNode *node); 
  /**
 * @brief Stores source data into a given node
 * @param node The memory node where the data will be stored
 * @param src Pointer to the source data to copy
 * @param bytes Number of bytes to copy from the source
 * @return Status code indicating success or failure (s16)
 */
  s16(*setData) (MemoryNode *node, void *src, u16 bytes);
  /**
 * @brief Returns the size of a given node
 * @param node The memory node to query
 * @return Size of the node in bytes (u16)
 */
  u16(*size) (MemoryNode *node);

  /**
 * @brief 
 * @param
 * @return 
 */
  s16(*reset) (MemoryNode *node);

  /**
* @brief Resets the content of a given node
 * @param node The memory node to reset
 * @return Status code indicating success or failure (s16)
  */
  s16(*softReset) (MemoryNode *node);	
 /**
 * @brief Frees the memory container of a given node
 * @param node The memory node whose container will be freed
 * @return Status code indicating success or failure (s16)
 */
s16(*free)(MemoryNode *node);

/**
 * @brief Frees only the node structure without freeing its data content
 * @param node The memory node to free
 * @return Status code indicating success or failure (s16)
 */
s16(*softFree)(MemoryNode *node);

/**
 * @brief Sets all bytes in the node to a given value
 * @param node The memory node to modify
 * @param value The byte value to set throughout the node
 * @return Status code indicating success or failure (s16)
 */
s16(*memSet)(MemoryNode *node, u8 value);

/**
 * @brief Resizes the node and copies source data into it
 * @param node The memory node to resize and copy into
 * @param src Pointer to the source data to copy
 * @param bytes Number of bytes to copy from the source
 * @return Status code indicating success or failure (s16)
 */
s16(*memCopy)(MemoryNode *node, void *src, u16 bytes);

/**
 * @brief Reserves new node size and concatenates source data to existing data
 * @param node The memory node to expand
 * @param src Pointer to the source data to concatenate
 * @param bytes Number of bytes to concatenate from the source
 * @return Status code indicating success or failure (s16)
 */
s16(*memConcat)(MemoryNode *node, void *src, u16 bytes);

/**
 * @brief Applies an AND mask to the node data to keep only bits set to 1
 * @param node The memory node to mask
 * @param mask The bitmask to apply
 * @return Status code indicating success or failure (s16)
 */
s16(*memMask)(MemoryNode *node, u8 mask);

/**
 * @brief Prints the content of a node
 * @param node The memory node to print
 */
void(*print)(MemoryNode *node);

/**
 * @brief Returns the next node in the linked structure
 * @param node The current memory node
 * @return Pointer to the next memory node
 */
MemoryNode* (*next)(MemoryNode* node);

/**
 * @brief Sets the next node in the linked structure
 * @param node The current memory node
 * @param next The node to set as next
 * @return Status code indicating success or failure (s16)
 */
s16 (*setNext)(MemoryNode* node, MemoryNode* next);

/**
 * @brief Returns the previous node in the linked structure
 * @param node The current memory node
 * @return Pointer to the previous memory node
 */
MemoryNode* (*prev)(MemoryNode* node);

/**
 * @brief Sets the previous node in the linked structure
 * @param node The current memory node
 * @param prev The node to set as previous
 * @return Status code indicating success or failure (s16)
 */
s16 (*setPrev)(MemoryNode* node, MemoryNode* prev);
};

/**
 * @brief Creates a new memory node
 * @return Pointer to the newly created memory node
 */
MemoryNode* MEMNODE_create();

/**
 * @brief Creates a new memory node from a reference pointer
 * @param node Double pointer to store the created memory node
 * @return Status code indicating success or failure (s16)
 */
s16 MEMNODE_createFromRef(MemoryNode **node);

/**
 * @brief Initializes a memory node without validation checks
 * @param node The memory node to initialize (must not be NULL)
 * @return Status code indicating success or failure (s16)
 */
s16 MEMNODE_createLite(MemoryNode *node);

#endif // __ADT_MEMORY_NODE_H__
