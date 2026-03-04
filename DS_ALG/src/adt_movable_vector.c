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
#include "adt_movable_vector.h"
#include "EDK_MemoryManager/edk_memory_manager.h"

#define MOVABLE_VECTOR_SCALE 2

static s16 MOVVECTOR_initWithoutCheck(MHVector* vec, u16 capacity);
static u16 MOVVECTOR_capacity(MHVector* vector);
static s16 MOVVECTOR_length(MHVector* vector);
static boolean MOVVECTOR_isEmpty(MHVector* vector);
static boolean MOVVECTOR_isFull(MHVector* vector);
static s16 MOVVECTOR_insertFirst(MHVector* vector, void* data, u16 bytes);
static s16 MOVVECTOR_insertLast(MHVector* vector, void* data, u16 bytes);
static s16 MOVVECTOR_insertAt(MHVector* vector, void* data, u16 bytes, u16 position);
static void MOVVECTOR_print(MHVector* vector);
static void* MOVVECTOR_first(MHVector* vector, u16* size);
static void* MOVVECTOR_last(MHVector* vector, u16* size);
static void* MOVVECTOR_at(MHVector* vector, u16* size, u16 position);
static void* MOVVECTOR_extractFirst(MHVector* vector, u16* size);
static void* MOVVECTOR_extractLast(MHVector* vector, u16* size);
static void* MOVVECTOR_extractAt(MHVector* vector, u16* size, u16 position);
static s16 MOVVECTOR_traverse(MHVector* vector, void (*callback)(MemoryNode*));
static s16 MOVVECTOR_concat(MHVector* vector, MHVector* vector_src);
static s16 MOVVECTOR_resize(MHVector* vector, u16 new_size);
static s16 MOVVECTOR_reset(MHVector* vector);
static s16 MOVVECTOR_softReset(MHVector* vector);
static s16 MOVVECTOR_destroy(MHVector* vector);

struct movable_vector_ops_s mov_vector_ops = {
    .length = MOVVECTOR_length,
    .isFull = MOVVECTOR_isFull,
    .isEmpty = MOVVECTOR_isEmpty,
    .capacity = MOVVECTOR_capacity,
    .insertFirst = MOVVECTOR_insertFirst,
    .insertLast = MOVVECTOR_insertLast,
    .insertAt = MOVVECTOR_insertAt,
    .print = MOVVECTOR_print,
    .first = MOVVECTOR_first,
    .last = MOVVECTOR_last,
    .at = MOVVECTOR_at,
    .extractFirst = MOVVECTOR_extractFirst,
    .extractLast = MOVVECTOR_extractLast,
    .extractAt = MOVVECTOR_extractAt,
    .traverse = MOVVECTOR_traverse,
    .concat = MOVVECTOR_concat,
    .resize = MOVVECTOR_resize,
    .reset = MOVVECTOR_reset,
    .softReset = MOVVECTOR_softReset,
    .destroy = MOVVECTOR_destroy
};

MHVector* MHVECTOR_create(u16 capacity) {
  if (0 == capacity) return NULL;
  MHVector* vec = (MHVector*)MM->malloc(sizeof(MHVector));
  if (NULL == vec) {
#ifdef VERBOSE_
    printf("Error: [%s] not enough memory\n", __FUNCTION__);
#endif
    return NULL;
  }
  MOVVECTOR_initWithoutCheck(vec, capacity);
  return vec;
}

s16 MOVVECTOR_initWithoutCheck(MHVector* vec, u16 capacity) {
  vec->head_ = 0;
  vec->tail_ = 0;
  vec->capacity_ = capacity * MOVABLE_VECTOR_SCALE;
  vec->storage_ = MM->malloc(sizeof(MemoryNode) * vec->capacity_);
  if (NULL == vec->storage_) return kErrorCode_Memory;
  for (u16 i = 0; i < vec->capacity_; ++i) {
    MEMNODE_createLite(vec->storage_ + i);
  }
  vec->ops_ = &mov_vector_ops;
  return kErrorCode_Ok;
}

u16 MOVVECTOR_capacity(MHVector* vector) {
  if (NULL == vector) return 0;
  return vector->capacity_ / MOVABLE_VECTOR_SCALE;
}

s16 MOVVECTOR_length(MHVector* vector) {
  if (NULL == vector) return 0;
  return vector->tail_;
}

boolean MOVVECTOR_isEmpty(MHVector* vector) {
  if (NULL == vector) return False;
  return (0 == vector->tail_);
}

boolean MOVVECTOR_isFull(MHVector* vector) {
  if (NULL == vector) return False;
  return (vector->tail_ == MOVVECTOR_capacity(vector));
}

s16 MOVVECTOR_insertFirst(MHVector* vector, void* data, u16 bytes) {
  if (NULL == vector || NULL == vector->storage_) return kErrorCode_Null;
  if (NULL == data) return kErrorCode_NullData;
  if (0 == bytes) return kErrorCode_ZeroSize;
  if (vector->ops_->isFull(vector)) return kErrorCode_IsFull;

  vector->head_ = (vector->head_ - 1 + vector->capacity_) % vector->capacity_;
  vector->storage_[vector->head_].ops_->setData(&vector->storage_[vector->head_], data, bytes);
  vector->tail_++;
  return kErrorCode_Ok;
}

s16 MOVVECTOR_insertLast(MHVector* vector, void* data, u16 bytes) {
  if (NULL == vector || NULL == vector->storage_) return kErrorCode_Null;
  if (NULL == data) return kErrorCode_NullData;
  if (0 == bytes) return kErrorCode_ZeroSize;
  if (vector->ops_->isFull(vector)) return kErrorCode_IsFull;

  u16 index = (vector->head_ + vector->tail_) % vector->capacity_;
  vector->storage_[index].ops_->setData(&vector->storage_[index], data, bytes);
  vector->tail_++;
  return kErrorCode_Ok;
}

s16 MOVVECTOR_insertAt(MHVector* vector, void* data, u16 bytes, u16 position) {
  if (NULL == vector || NULL == vector->storage_) return kErrorCode_Null;
  if (NULL == data) return kErrorCode_NullData;
  if (0 == bytes) return kErrorCode_ZeroSize;
  if (vector->ops_->isFull(vector)) return kErrorCode_IsFull;

  if (position >= vector->tail_) {
    return MOVVECTOR_insertLast(vector, data, bytes);
  }
  if (0 == position) {
    return MOVVECTOR_insertFirst(vector, data, bytes);
  }

  u16 dest = (vector->head_ + vector->tail_) % vector->capacity_;
  for (s16 i = vector->tail_ - 1; i >= (s16)position; --i) {
    u16 src = (vector->head_ + i) % vector->capacity_;
    vector->storage_[dest].ops_->setData(&vector->storage_[dest],
      vector->storage_[src].data_,
      vector->storage_[src].size_);
    vector->storage_[src].ops_->softReset(&vector->storage_[src]);
    dest = src;
  }
  vector->storage_[dest].ops_->setData(&vector->storage_[dest], data, bytes);
  vector->tail_++;
  return kErrorCode_Ok;
}

void* MOVVECTOR_first(MHVector* vector, u16* size) {
  if (NULL == vector || NULL == size) return NULL;
  if (0 == vector->tail_) return NULL;
  *size = vector->storage_[vector->head_].size_;
  return vector->storage_[vector->head_].data_;
}

void* MOVVECTOR_last(MHVector* vector, u16* size) {
  if (NULL == vector || NULL == size) return NULL;
  if (0 == vector->tail_) return NULL;
  u16 last = (vector->head_ + vector->tail_ - 1) % vector->capacity_;
  *size = vector->storage_[last].size_;
  return vector->storage_[last].data_;
}

void* MOVVECTOR_at(MHVector* vector, u16* size, u16 position) {
  if (NULL == vector || NULL == size) return NULL;
  if (position >= vector->tail_) return NULL;
  u16 index = (vector->head_ + position) % vector->capacity_;
  *size = vector->storage_[index].size_;
  return vector->storage_[index].data_;
}

void* MOVVECTOR_extractFirst(MHVector* vector, u16* size) {
  if (NULL == vector || NULL == size) return NULL;
  if (0 == vector->tail_) return NULL;

  void* data = vector->storage_[vector->head_].data_;
  *size = vector->storage_[vector->head_].size_;
  vector->storage_[vector->head_].ops_->softReset(&vector->storage_[vector->head_]);
  vector->head_ = (vector->head_ + 1) % vector->capacity_;
  vector->tail_--;
  return data;
}

void* MOVVECTOR_extractLast(MHVector* vector, u16* size) {
  if (NULL == vector || NULL == size) return NULL;
  if (0 == vector->tail_) return NULL;

  u16 last = (vector->head_ + vector->tail_ - 1) % vector->capacity_;
  void* data = vector->storage_[last].data_;
  *size = vector->storage_[last].size_;
  vector->storage_[last].ops_->softReset(&vector->storage_[last]);
  vector->tail_--;
  return data;
}

void* MOVVECTOR_extractAt(MHVector* vector, u16* size, u16 position) {
  if (NULL == vector || NULL == size) return NULL;
  if (0 == vector->tail_) return NULL;
  if (position >= vector->tail_) return NULL;

  if (0 == position) return MOVVECTOR_extractFirst(vector, size);
  if (position == vector->tail_ - 1) return MOVVECTOR_extractLast(vector, size);

  u16 idx = (vector->head_ + position) % vector->capacity_;
  void* data = vector->storage_[idx].data_;
  *size = vector->storage_[idx].size_;

  for (u16 i = position; i < vector->tail_ - 1; ++i) {
    u16 cur = (vector->head_ + i) % vector->capacity_;
    u16 nxt = (vector->head_ + i + 1) % vector->capacity_;
    vector->storage_[cur].ops_->setData(&vector->storage_[cur],
      vector->storage_[nxt].data_,
      vector->storage_[nxt].size_);
  }
  u16 last = (vector->head_ + vector->tail_ - 1) % vector->capacity_;
  vector->storage_[last].ops_->softReset(&vector->storage_[last]);
  vector->tail_--;
  return data;
}

void MOVVECTOR_print(MHVector* vector) {
  if (NULL == vector) {
    printf("    [Vector info] Address: NULL\n");
    return;
  }
  printf("    [Vector info] Address: %p\n", (void*)vector);
  printf("    [Vector info] Head: %d\n", vector->head_);
  printf("    [Vector info] Tail (length): %d\n", vector->tail_);
  printf("    [Vector info] Logical Capacity: %d\n", MOVVECTOR_capacity(vector));
  printf("    [Vector info] Allocated Capacity: %d\n", vector->capacity_);
  printf("    [Vector info] Storage Address: %p\n", (void*)vector->storage_);
  for (u16 i = 0; i < vector->tail_; ++i) {
    u16 idx = (vector->head_ + i) % vector->capacity_;
    printf("    [Vector Info] Storage #%d (logical %d):\n", idx, i);
    vector->storage_[idx].ops_->print(&vector->storage_[idx]);
  }
}

s16 MOVVECTOR_traverse(MHVector* vector, void (*callback)(MemoryNode*)) {
  if (NULL == vector) return kErrorCode_Null;
  if (NULL == callback) return kErrorCode_Null;
  for (u16 i = 0; i < vector->tail_; ++i) {
    u16 idx = (vector->head_ + i) % vector->capacity_;
    callback(&vector->storage_[idx]);
  }
  return kErrorCode_Ok;
}

s16 MOVVECTOR_concat(MHVector* vector, MHVector* vector_src) {
  if (NULL == vector || NULL == vector_src) return kErrorCode_Null;
  if (NULL == vector->storage_ || NULL == vector_src->storage_) return kErrorCode_NullData;

  u16 new_logical_cap = MOVVECTOR_capacity(vector) + MOVVECTOR_capacity(vector_src);
  u16 new_alloc_cap = new_logical_cap * MOVABLE_VECTOR_SCALE;
  MemoryNode* new_storage = MM->malloc(sizeof(MemoryNode) * new_alloc_cap);
  if (NULL == new_storage) return kErrorCode_Memory;

  for (u16 i = 0; i < new_alloc_cap; ++i) {
    MEMNODE_createLite(new_storage + i);
  }

  for (u16 i = 0; i < vector->tail_; ++i) {
    u16 src_idx = (vector->head_ + i) % vector->capacity_;
    new_storage[i].data_ = vector->storage_[src_idx].data_;
    new_storage[i].size_ = vector->storage_[src_idx].size_;
    vector->storage_[src_idx].ops_->softReset(&vector->storage_[src_idx]);
  }
  for (u16 i = 0; i < vector_src->tail_; ++i) {
    u16 src_idx = (vector_src->head_ + i) % vector_src->capacity_;
    void* src_data = vector_src->storage_[src_idx].data_;
    u16 src_size = vector_src->storage_[src_idx].size_;
    void* new_data = MM->malloc(src_size);
    if (NULL == new_data) {
      for (u16 j = 0; j < vector->tail_ + i; ++j) {
        if (NULL != new_storage[j].data_) {
          MM->free(new_storage[j].data_);
        }
      }
      MM->free(new_storage);
      return kErrorCode_Memory;
    }
    memcpy(new_data, src_data, src_size);
    new_storage[vector->tail_ + i].ops_->setData(&new_storage[vector->tail_ + i], new_data, src_size);
  }

  MM->free(vector->storage_);
  vector->storage_ = new_storage;
  vector->capacity_ = new_alloc_cap;
  vector->head_ = 0;
  vector->tail_ = vector->tail_ + vector_src->tail_;
  return kErrorCode_Ok;
}

s16 MOVVECTOR_resize(MHVector* vector, u16 new_size) {
  if (NULL == vector) return kErrorCode_Null;
  if (0 == new_size) return kErrorCode_ZeroSize;
  if (new_size == MOVVECTOR_capacity(vector)) return kErrorCode_Ok;
  if (NULL == vector->storage_) return kErrorCode_NullData;

  u16 new_alloc_cap = new_size * MOVABLE_VECTOR_SCALE;
  MemoryNode* new_storage = MM->malloc(sizeof(MemoryNode) * new_alloc_cap);
  if (NULL == new_storage) return kErrorCode_Memory;

  for (u16 i = 0; i < new_alloc_cap; ++i) {
    MEMNODE_createLite(new_storage + i);
  }

  u16 copy_len = (new_size < vector->tail_) ? new_size : vector->tail_;
  for (u16 i = 0; i < copy_len; ++i) {
    u16 src_idx = (vector->head_ + i) % vector->capacity_;
    new_storage[i].ops_->setData(&new_storage[i],
      vector->storage_[src_idx].data_,
      vector->storage_[src_idx].size_);
  }

  if (new_size < vector->tail_) {
    for (u16 i = new_size; i < vector->tail_; ++i) {
      u16 src_idx = (vector->head_ + i) % vector->capacity_;
      if (NULL != vector->storage_[src_idx].data_) {
        MM->free(vector->storage_[src_idx].data_);
      }
    }
  }

  for (u16 i = 0; i < vector->capacity_; ++i) {
  }
  MM->free(vector->storage_);

  vector->storage_ = new_storage;
  vector->capacity_ = new_alloc_cap;
  vector->head_ = 0;
  vector->tail_ = copy_len;
  return kErrorCode_Ok;
}

s16 MOVVECTOR_reset(MHVector* vector) {
  if (NULL == vector) return kErrorCode_Null;
  if (NULL == vector->storage_) return kErrorCode_Null;

  for (u16 i = 0; i < vector->tail_; ++i) {
    u16 idx = (vector->head_ + i) % vector->capacity_;
    if (NULL != vector->storage_[idx].data_) {
      MM->free(vector->storage_[idx].data_);
    }
    vector->storage_[idx].ops_->softReset(&vector->storage_[idx]);
  }
  vector->head_ = 0;
  vector->tail_ = 0;
  return kErrorCode_Ok;
}

s16 MOVVECTOR_softReset(MHVector* vector) {
  if (NULL == vector) return kErrorCode_Null;
  if (NULL == vector->storage_) return kErrorCode_Null;

  for (u16 i = 0; i < vector->tail_; ++i) {
    u16 idx = (vector->head_ + i) % vector->capacity_;
    vector->storage_[idx].ops_->softReset(&vector->storage_[idx]);
  }
  vector->head_ = 0;
  vector->tail_ = 0;
  return kErrorCode_Ok;
}

s16 MOVVECTOR_destroy(MHVector* vector) {
  if (NULL == vector) return kErrorCode_Null;
  if (NULL == vector->storage_) return kErrorCode_Null;

  for (u16 i = 0; i < vector->tail_; ++i) {
    u16 idx = (vector->head_ + i) % vector->capacity_;
    if (NULL != vector->storage_[idx].data_) {
      MM->free(vector->storage_[idx].data_);
    }
  }
  MM->free(vector->storage_);
  MM->free(vector);
  return kErrorCode_Ok;
}