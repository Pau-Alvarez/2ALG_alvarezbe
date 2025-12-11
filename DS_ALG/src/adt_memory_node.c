/*
 * Ivan Sancho as Unreal Authorized Instructor (UAI), 2025.
 *
 * Memory Node source code.
 * Algorithms and Data Structures.
 *
 * Student:
 *   Pau Álvarez Belenguer
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
static s16 MEMNODE_setData(MemoryNode* node, void* src, u16 bytes);
static u16 MEMNODE_size(MemoryNode* node);
static s16 MEMNODE_reset(MemoryNode* node);
static s16 MEMNODE_softReset(MemoryNode* node);
static s16 MEMNODE_free(MemoryNode* node);
static s16 MEMNODE_softFree(MemoryNode* node);

static s16 MEMNODE_memSet(MemoryNode* node, u8 value);
static s16 MEMNODE_memCopy(MemoryNode* node, void* src, u16 bytes);
static s16 MEMNODE_memConcat(MemoryNode* node, void* src, u16 bytes);
static s16 MEMNODE_memMask(MemoryNode* node, u8 value);
static void MEMNODE_print (MemoryNode* node);

// Memory Node's API Definitions
struct memory_node_ops_s memory_node_ops = { .data = MEMNODE_data,
                                             .setData = MEMNODE_setData,
                                             .size = MEMNODE_size,
                                             .reset = MEMNODE_reset,
                                             .softReset = MEMNODE_softReset,
                                             .free = MEMNODE_free,
                                             .softFree = MEMNODE_softFree,
                                             .memSet = MEMNODE_memSet,
                                             .memCopy = MEMNODE_memCopy,
                                             .memConcat = MEMNODE_memConcat,
                                             .memMask = MEMNODE_memMask,
                                             .print = MEMNODE_print,
      
};

// Memory Node Definitions
MemoryNode* MEMNODE_create() {
    MemoryNode* node = (MemoryNode*)MM->malloc(sizeof(MemoryNode));
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
    if (NULL == node) return kErrorCode_Null;
    if (NULL != *node) return kErrorCode_AlreadyInitialized;
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
    if (!node) return kErrorCode_Null;
    return MEMNODE_initWithoutCheck(node);
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
    if (!node) return NULL;
    return node->data_;
}

static s16 MEMNODE_setData(MemoryNode* node, void* src, u16 bytes) {
    if (!node) return kErrorCode_Null;
    if (!src) return kErrorCode_Null;
    if (0 == bytes) return kErrorCode_ZeroSize;
    node->data_ = src;
    node->size_ = bytes;
    return kErrorCode_Ok;
}

u16	MEMNODE_size(MemoryNode* node) { // returns data size
    if (!node) return NULL;
    return node->size_;
}

s16 MEMNODE_reset(MemoryNode* node) {
    if (!node) return kErrorCode_Null;
    if (node->data_) MM->free(node->data_);
    node->data_ = NULL;
    node->size_ = 0;
    return kErrorCode_Ok;
}

s16 MEMNODE_softReset(MemoryNode* node) {
    if (!node) return kErrorCode_Null;
    node->size_ = 0;
    node->data_ = NULL;
    node->next_ = NULL;
    node->prev_ = NULL;
    node = NULL;
    return kErrorCode_Ok;
}

s16 MEMNODE_free(MemoryNode* node) {
    if (!node) return kErrorCode_Null;
    if (node->data_)MM->free(node->data_);
    MM->free(node);
    return kErrorCode_Ok;
}

s16 MEMNODE_softFree(MemoryNode* node) {
    if (!node) return kErrorCode_Null;
    MM->free(node);
    return kErrorCode_Ok;
}


s16 MEMNODE_memSet(MemoryNode* node, u8 value) {
    if (!node) return kErrorCode_Null;
    if (!node->data_) return kErrorCode_NullData;
    u8* tmp = (u8*)node->data_;
    int cnt = 0;
    do {
        tmp[cnt] = value;
        cnt++;
    } while (NULL != tmp[cnt]);
    return kErrorCode_Ok;
}

s16 MEMNODE_memCopy(MemoryNode* node, void* src, u16 bytes) {
    if (!node) return kErrorCode_Null;
    if (!src) return kErrorCode_Null;
    if (0 == bytes) return kErrorCode_ZeroSize;
    if (!node->data_) node->data_ = MM->malloc(bytes);
    u8* tmp1 = (u8*)node->data_;
    u8* tmp2 = (u8*)src;
    for (int i = 0; i < bytes;i++) {
        tmp1[i] = tmp2[i];
    }
    return kErrorCode_Ok;
}

s16 MEMNODE_memConcat(MemoryNode* node, void* src, u16 bytes) {
    if (!node) return kErrorCode_Null;
    if (!src) return kErrorCode_Null;
    if (0 == bytes) return kErrorCode_ZeroSize;
    u16 original_size = node->size_;
    void* original_data = node->data_;
    u32 total_size = (u32)original_size + (u32)bytes;

    void* combined_buffer = MM->malloc(total_size);
    if (NULL == combined_buffer) {
        return kErrorCode_Memory;
    }
    if (NULL != original_data && original_size > 0) {
        memcpy(combined_buffer, original_data, original_size);
    }
    memcpy((u8*)combined_buffer + original_size, src, bytes);

    if (NULL != original_data) {
        MM->free(original_data);
    }

    node->data_ = combined_buffer;
    node->size_ = (u16)total_size;

    return kErrorCode_Ok;

}
    

s16 MEMNODE_memMask(MemoryNode* node, u8 value){
    if (!node) return kErrorCode_Null;
    if (!node->data_) return kErrorCode_NullData;
    u8* tmp = (u8*)node->data_;
    int cnt = 0;
    do {
        tmp[cnt] &= value;
        cnt++;
    } while (NULL != tmp[cnt]);
    return kErrorCode_Ok;
}

static void MEMNODE_print(MemoryNode* node) {
    if (NULL == node) {
        printf("[Node Info] Null Node\n");
        return;
    }
    if (NULL == node->data_) {
        printf("[Node Info] Null Data\n");
        return;
    }

    printf("[Node Info] Address: %p\n", (void*)node);
    printf("[Node Info] Size: %d\n", node->size_);
    printf("[Node Info] Data Address: %p\n", (void*)node->data_);
    printf("[Node Info] Data Content: ");
    u8* byte_data = (u8*)node->data_;
    for (u16 i = 0; i < node->size_; i++) {
        printf("%c", byte_data[i]);
    }
    printf("\n");

    if (NULL == node->next_) {
        printf("[Node Info] Next address: NULL\n");
    }
    else {
        printf("[Node Info] Next Node: %p\n", (void*)node->next_);
    }
    if (NULL == node->prev_) {
        printf("[Node Info] Prev address: NULL\n");
    }
    else {
        printf("[Node Info] Prev Node: %p\n", (void*)node->prev_);
    }

}


static MemoryNode* MEMNODE_next(MemoryNode* node) {
    return (NULL != node) ? node->next_ : NULL;
}

static s16 MEMNODE_setNext(MemoryNode* node, MemoryNode* next) {
    if (NULL == node) {
        return kErrorCode_Null;
    }

    node->next_ = next;
    return kErrorCode_Ok;
}

static MemoryNode* MEMNODE_prev(MemoryNode* node) {
    return (NULL != node) ? node->prev_ : NULL;
}

static s16 MEMNODE_setPrev(MemoryNode* node, MemoryNode* prev) {
    if (NULL == node) {
        return kErrorCode_Null;
    }

    node->prev_ = prev;
    return kErrorCode_Ok;
}
