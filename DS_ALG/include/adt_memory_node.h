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
 * @brief
 * @param 
 * @return 
 */
//Returns a refernce for a given node data
  void*(*data) (MemoryNode *node); 
  /**
 * @brief 
 * @param 
 * @param 
 * @return 
 */
 //stores src into a given node taking up the bytes param of space
  s16(*setData) (MemoryNode *node, void *src, u16 bytes);
  /**
 * @brief 
 * @param 
 * @return 
 */
 //return node size
  u16(*size) (MemoryNode *node);

  /**
 * @brief 
 * @param
 * @return 
 */
 //resets the content of a given node
  s16(*reset) (MemoryNode *node);

  /**
  * @brief  
  * @param 
  * @return 
  */
  //resets both the container and content of a given node
  s16(*softReset) (MemoryNode *node);	
  /**
 * @brief  
 * @param 
 * @return 
 */
 //resets both the container of a given node
  s16(*free) (MemoryNode *node);
  /**
 * @brief 
 * @param 
 * @return 
 */
 /**/
//resets and frees the whole node and it's content
  s16(*softFree) (MemoryNode *node);	// free only the node (its memory, not its data)

  /**
 * @brief 
 * @param 
 * @param 
 * @return 
 */
 //stores given value into the given node
  s16(*memSet) (MemoryNode *node, u8 value);
  /**
 * @brief 
 * @param 
 * @param 
 * @param 
 * @return 
 */
 //resizes node with bytes and copies src into existing node data
  s16(*memCopy) (MemoryNode *node, void *src, u16 bytes);
  /**
 * @brief 
 * @param 
 * @param 
 * @param 
 * @return 
 */
 //reserves new node size with existing data and bytes
 //and concats existing data with src
  s16(*memConcat) (MemoryNode *node, void *src, u16 bytes);
  /**
 * @brief 
 * @param 
 * @param 
 * @return 
 */
 //AND masks a node to only keep bits set as 1
  s16(*memMask) (MemoryNode *node, u8 mask);
/**
 * @brief 
 * @param 
 * @return 
 */
 //prints node content
  void(*print) (MemoryNode *node);
/**
 * @brief 
 * @param 
 * @return 
 */
 //returns node->data next node
  MemoryNode* (*next)(MemoryNode* node);
  /**
 * @brief 
 * @param 
 * @param 
 * @return 
 */
 //sets node->data next node
  s16 (*setNext)(MemoryNode* node, MemoryNode* next);
  /**
 * @brief 
 * @param 
 * @return 
 */
 //returns node->data prev node
  MemoryNode* (*prev)(MemoryNode* node);
  /**
 * @brief 
 * @param 
 * @param 
 * @return 
 */
 //sets node->data prev node
  s16 (*setPrev)(MemoryNode* node, MemoryNode* prev);
};

MemoryNode* MEMNODE_create();
s16 MEMNODE_createFromRef(MemoryNode **node);
/**
 * @brief 
 * @param 
 * @return 
 */
 //calls memnode_initwithoutcheck and returns an error if node == NULL
s16 MEMNODE_createLite(MemoryNode *node);

#endif // __ADT_MEMORY_NODE_H__
