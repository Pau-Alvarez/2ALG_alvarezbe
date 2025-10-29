/*
 *
 * Ivan Sancho as Unreal Authorized Instructor (UAI), 2025.
 *
 * Memory Node source code.
 * Algorithms and Data Structures.
 * 
 * Students:
 *   TODO
 *   TODO
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common_def.h"
#include "adt_memory_node.h"

#include "EDK_MemoryManager/edk_memory_manager.h"


//****** Memory Node forward declarations:

//Inits a MN with no checks:
static s16 MEMNODE_initWithoutCheck(MemoryNode* node);

static void* MEMNODE_data(MemoryNode* node);
static u16 MEMNODE_size(MemoryNode* node);


// Memory Node's API Definitions
struct memory_node_ops_s memory_node_ops = { .data = MEMNODE_data,
                                             .size = MEMNODE_size,
};

// Memory Node Definitions
MemoryNode* MEMNODE_create() {
  MemoryNode* node = (MemoryNode*) MM->malloc(sizeof(MemoryNode));
  if (NULL == node) {
#ifdef VERBOSE_
    printf("Error: [%s] not enough memory available\n", __FUNCTION__);
#endif
    return NULL;
  }
  MEMNODE_initWithoutCheck(node);
  return node;
}

s16 MEMNODE_createFromRef(MemoryNode** node) {
  *node = MEMNODE_create();
  if (NULL == *node) {
#ifdef VERBOSE_
    printf("Error: [%s] not enough memory available\n", __FUNCTION__);
#endif
    return kErrorCode_Memory;
  }
  return kErrorCode_Ok;
}

s16 MEMNODE_createLite(MemoryNode* node) {
  //TODO
}

s16 MEMNODE_initWithoutCheck(MemoryNode* node) {
  node->data_ = NULL;
  node->size_ = 0;
  node->next_ = NULL;
  node->prev_ = NULL;
  node->ops_ = &memory_node_ops;
  return kErrorCode_Ok;
}

void* MEMNODE_data(MemoryNode* node) { // returns a reference to data_
  return node->data_;
}

u16	MEMNODE_size(MemoryNode* node) { // returns data size
  return node->size_;
}
