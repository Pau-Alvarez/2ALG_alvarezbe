/*
 * Ivan Sancho as Unreal Authorized Instructor (UAI), 2025.
 *
 * Circular Vector source code.
 * Algorithms and Data Structures.
 *
 * Student:
 *   Pau Alvarez Belenguer
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common_def.h"
#include "adt_circular_vector.h"

#include "EDK_MemoryManager/edk_memory_manager.h"

//Circular Vector forward declarations

static s16 CIRCULARVECTOR_destroy(CircularVector* vector);
static s16 CIRCULARVECTOR_softReset(CircularVector* vector);
static s16 CIRCULARVECTOR_reset(CircularVector* vector);
static s16 CIRCULARVECTOR_resize(CircularVector* vector, u16 new_size);

static u16 CIRCULARVECTOR_capacity(CircularVector* vector);
static u16 CIRCULARVECTOR_length(CircularVector* vector);
static boolean CIRCULARVECTOR_isEmpty(CircularVector* vector);
static boolean CIRCULARVECTOR_isFull(CircularVector* vector);

static void* CIRCULARVECTOR_first(CircularVector* vector, u16* size);
static void* CIRCULARVECTOR_last(CircularVector* vector, u16* size);
static void* CIRCULARVECTOR_at(CircularVector* vector, u16* size, u16 position);

static s16 CIRCULARVECTOR_insertFirst(CircularVector* vector, void *data, u16 bytes);
static s16 CIRCULARVECTOR_insertLast(CircularVector* vector, void* data, u16 bytes);
static s16 CIRCULARVECTOR_insertAt(CircularVector* vector, void* data, u16 bytes, u16 position);

static void* CIRCULARVECTOR_extractFirst(CircularVector* vector, u16* size);
static void* CIRCULARVECTOR_extractLast(CircularVector* vector, u16* size);
static void* CIRCULARVECTOR_extractAt(CircularVector* vector, u16* size, u16 position);

static s16 CIRCULARVECTOR_concat(CircularVector* vector, CircularVector* vector_src);
static s16 CIRCULARVECTOR_traverse(CircularVector* vector, void (*callback)(MemoryNode*));

static void CIRCULARVECTOR_print(CircularVector* vector);


struct adt_circ_vector_s circular_vector_ops = {	.destroy = CIRCULARVECTOR_destroy,
													.softReset = CIRCULARVECTOR_softReset,
													.reset = CIRCULARVECTOR_reset,
													.resize = CIRCULARVECTOR_resize,
													.capacity = CIRCULARVECTOR_capacity,
													.length = CIRCULARVECTOR_length,
													.isEmpty = CIRCULARVECTOR_isEmpty,
													.isFull = CIRCULARVECTOR_isFull,
													.first = CIRCULARVECTOR_first,
													.last = CIRCULARVECTOR_last,
													.at = CIRCULARVECTOR_at,
													.insertFirst = CIRCULARVECTOR_insertFirst,
													.insertLast = CIRCULARVECTOR_insertLast,
													.insertAt = CIRCULARVECTOR_insertAt,
													.extractFirst = CIRCULARVECTOR_extractFirst,
													.extractLast = CIRCULARVECTOR_extractLast,
													.extractAt = CIRCULARVECTOR_extractAt,
													.concat = CIRCULARVECTOR_concat,
													.traverse = CIRCULARVECTOR_traverse,
													.print = CIRCULARVECTOR_print,
												};


CircularVector* CIRCULARVECTOR_create(u16 capacity){
	if (0 == capacity) {
		return NULL;
	}

	CircularVector* new_vector = (CircularVector*)MM->malloc(sizeof(CircularVector));
	if (NULL == new_vector) {
		return NULL;
	}

	new_vector->capacity_ = capacity;
	new_vector->head_ = 0;
	new_vector->tail_ = 0;
	new_vector->length_ = 0;
	new_vector->ops_ = &circular_vector_ops;

	new_vector->storage_ = (MemoryNode*)MM->malloc(sizeof(MemoryNode) * capacity);
	if (NULL == new_vector->storage_) {
		MM->free(new_vector);
		return NULL;
	}

	for (u16 idx = 0; idx < capacity; idx++) {
		MEMNODE_createLite(&new_vector->storage_[idx]);
	}

	return new_vector;
}

s16 CIRCULARVECTOR_destroy(CircularVector* vector) {
	if (NULL == vector) {
		return kErrorCode_Null;
	}

	if (NULL != vector->storage_) {
		for (u16 i = 0; i < vector->length_; ++i) {
			u16 idx = (vector->head_ + i) % vector->capacity_;
			vector->storage_[idx].ops_->reset(&vector->storage_[idx]);
		}
		MM->free(vector->storage_);
	}

	vector->storage_ = NULL;
	vector->ops_ = NULL;
	vector->head_ = 0;
	vector->tail_ = 0;
	vector->length_ = 0;
	vector->capacity_ = 0;

	MM->free(vector);
	return kErrorCode_Ok;
}

s16 CIRCULARVECTOR_softReset(CircularVector* vector){
	if (NULL == vector) {
		return kErrorCode_Null;
	}
	if (NULL == vector->storage_) {
		return kErrorCode_NullData;
	}

	for (u16 i = 0; i < vector->length_; ++i) {
		u16 idx = (vector->head_ + i) % vector->capacity_;
		vector->storage_[idx].ops_->softReset(&vector->storage_[idx]);
	}

	vector->head_ = 0;
	vector->tail_ = 0;
	vector->length_ = 0;
	return kErrorCode_Ok;
}

s16 CIRCULARVECTOR_reset(CircularVector* vector){
	if (NULL == vector) {
		return kErrorCode_Null;
	}
	if (NULL == vector->storage_) {
		return kErrorCode_NullData;
	}

	for (u16 i = 0; i < vector->length_; ++i) {
		u16 idx = (vector->head_ + i) % vector->capacity_;
		vector->storage_[idx].ops_->reset(&vector->storage_[idx]);
	}

	vector->head_ = 0;
	vector->tail_ = 0;
	vector->length_ = 0;
	return kErrorCode_Ok;
}

s16 CIRCULARVECTOR_resize(CircularVector* vector, u16 new_size){
	if (NULL == vector) {
		return kErrorCode_Null;
	}
	if (0 == new_size) {
		return kErrorCode_ZeroSize;
	}
	if (new_size == vector->capacity_) {
		return kErrorCode_Ok;
	}

	u16 keep_count = vector->length_;
	if (new_size < vector->length_) {
		for (u16 i = new_size; i < vector->length_; ++i) {
			u16 idx = (vector->head_ + i) % vector->capacity_;
			vector->storage_[idx].ops_->reset(&vector->storage_[idx]);
		}
		keep_count = new_size;
	}

	MemoryNode* fresh_storage = (MemoryNode*)MM->malloc(sizeof(MemoryNode) * new_size);
	if (NULL == fresh_storage) {
		return kErrorCode_Memory;
	}

	for (u16 position = 0; position < new_size; ++position) {
		MEMNODE_createLite(&fresh_storage[position]);
	}

	for (u16 i = 0; i < keep_count; ++i) {
		u16 src_idx = (vector->head_ + i) % vector->capacity_;
		fresh_storage[i].data_ = vector->storage_[src_idx].data_;
		fresh_storage[i].size_ = vector->storage_[src_idx].size_;
	}

	MM->free(vector->storage_);
	vector->storage_ = fresh_storage;
	vector->capacity_ = new_size;
	vector->head_ = 0;
	vector->tail_ = (keep_count == new_size) ? 0 : keep_count;
	vector->length_ = keep_count;
	return kErrorCode_Ok;
}


u16 CIRCULARVECTOR_capacity(CircularVector* vector){
	return (NULL != vector) ? vector->capacity_ : 0;
}

u16 CIRCULARVECTOR_length(CircularVector* vector){
	return (NULL != vector) ? vector->length_ : 0;
}

boolean CIRCULARVECTOR_isEmpty(CircularVector* vector){
	if (NULL == vector) {
		return False;
	}
	return (0 == vector->length_) ? True : False;
}

boolean CIRCULARVECTOR_isFull(CircularVector* vector){
	if (NULL == vector) {
		return False;
	}
	return (vector->length_ == vector->capacity_) ? True : False;
}


void* CIRCULARVECTOR_first(CircularVector* vector, u16* size){
	if (NULL == vector || NULL == size) {
		return NULL;
	}
	if (0 == vector->length_) {
		return NULL;
	}

	*size = vector->storage_[vector->head_].size_;
	return vector->storage_[vector->head_].data_;
}

void* CIRCULARVECTOR_last(CircularVector* vector, u16* size){
	if (NULL == vector || NULL == size) {
		return NULL;
	}
	if (0 == vector->length_) {
		return NULL;
	}

	u16 last_idx = (vector->tail_ + vector->capacity_ - 1) % vector->capacity_;
	*size = vector->storage_[last_idx].size_;
	return vector->storage_[last_idx].data_;
}

void* CIRCULARVECTOR_at(CircularVector* vector, u16* size, u16 position){
	if (NULL == vector || NULL == size) {
		return NULL;
	}
	if (position >= vector->length_) {
		return NULL;
	}

	u16 idx = (vector->head_ + position) % vector->capacity_;
	*size = vector->storage_[idx].size_;
	return vector->storage_[idx].data_;
}


s16 CIRCULARVECTOR_insertFirst(CircularVector* vector, void* data, u16 bytes){
	if (NULL == vector) {
		return kErrorCode_Null;
	}
	if (NULL == data) {
		return kErrorCode_NullData;
	}
	if (0 == bytes) {
		return kErrorCode_ZeroSize;
	}
	if (CIRCULARVECTOR_isFull(vector)) {
		return kErrorCode_IsFull;
	}

	vector->head_ = (vector->head_ + vector->capacity_ - 1) % vector->capacity_;
	vector->storage_[vector->head_].data_ = data;
	vector->storage_[vector->head_].size_ = bytes;
	vector->length_++;
	return kErrorCode_Ok;
}

s16 CIRCULARVECTOR_insertLast(CircularVector* vector, void* data, u16 bytes){
	if (NULL == vector) {
		return kErrorCode_Null;
	}
	if (NULL == data) {
		return kErrorCode_NullData;
	}
	if (0 == bytes) {
		return kErrorCode_ZeroSize;
	}
	if (CIRCULARVECTOR_isFull(vector)) {
		return kErrorCode_IsFull;
	}

	vector->storage_[vector->tail_].data_ = data;
	vector->storage_[vector->tail_].size_ = bytes;
	vector->tail_ = (vector->tail_ + 1) % vector->capacity_;
	vector->length_++;
	return kErrorCode_Ok;
}

s16 CIRCULARVECTOR_insertAt(CircularVector* vector, void* data, u16 bytes, u16 position){
	if (NULL == vector) {
		return kErrorCode_Null;
	}
	if (NULL == data) {
		return kErrorCode_NullData;
	}
	if (0 == bytes) {
		return kErrorCode_ZeroSize;
	}
	if (CIRCULARVECTOR_isFull(vector)) {
		return kErrorCode_IsFull;
	}

	if (0 == position) {
		return CIRCULARVECTOR_insertFirst(vector, data, bytes);
	}
	if (position >= vector->length_) {
		return CIRCULARVECTOR_insertLast(vector, data, bytes);
	}

	for (u16 i = vector->length_; i > position; --i) {
		u16 dst = (vector->head_ + i) % vector->capacity_;
		u16 src = (vector->head_ + i - 1) % vector->capacity_;
		vector->storage_[dst].data_ = vector->storage_[src].data_;
		vector->storage_[dst].size_ = vector->storage_[src].size_;
	}

	u16 insert_idx = (vector->head_ + position) % vector->capacity_;
	vector->storage_[insert_idx].data_ = data;
	vector->storage_[insert_idx].size_ = bytes;
	vector->tail_ = (vector->tail_ + 1) % vector->capacity_;
	vector->length_++;
	return kErrorCode_Ok;
}


void* CIRCULARVECTOR_extractFirst(CircularVector* vector, u16* size){
	if (NULL == vector || NULL == size) {
		return NULL;
	}
	if (0 == vector->length_) {
		return NULL;
	}

	MemoryNode* front_node = &vector->storage_[vector->head_];
	void* extracted_data = front_node->data_;
	*size = front_node->size_;

	front_node->data_ = NULL;
	front_node->size_ = 0;
	vector->head_ = (vector->head_ + 1) % vector->capacity_;
	vector->length_--;
	return extracted_data;
}

void* CIRCULARVECTOR_extractLast(CircularVector* vector, u16* size){
	if (NULL == vector || NULL == size) {
		return NULL;
	}
	if (0 == vector->length_) {
		return NULL;
	}

	u16 last_idx = (vector->tail_ + vector->capacity_ - 1) % vector->capacity_;
	MemoryNode* back_node = &vector->storage_[last_idx];
	void* extracted_data = back_node->data_;
	*size = back_node->size_;

	back_node->data_ = NULL;
	back_node->size_ = 0;
	vector->tail_ = last_idx;
	vector->length_--;
	return extracted_data;
}

void* CIRCULARVECTOR_extractAt(CircularVector* vector, u16* size, u16 position){
	if (NULL == vector || NULL == size) {
		return NULL;
	}
	if (position >= vector->length_) {
		return NULL;
	}

	if (0 == position) {
		return CIRCULARVECTOR_extractFirst(vector, size);
	}
	if (position == vector->length_ - 1) {
		return CIRCULARVECTOR_extractLast(vector, size);
	}

	u16 target_idx = (vector->head_ + position) % vector->capacity_;
	void* extracted_data = vector->storage_[target_idx].data_;
	*size = vector->storage_[target_idx].size_;

	for (u16 i = position; i < vector->length_ - 1; ++i) {
		u16 dst = (vector->head_ + i) % vector->capacity_;
		u16 src = (vector->head_ + i + 1) % vector->capacity_;
		vector->storage_[dst].data_ = vector->storage_[src].data_;
		vector->storage_[dst].size_ = vector->storage_[src].size_;
	}

	u16 cleared = (vector->head_ + vector->length_ - 1) % vector->capacity_;
	vector->storage_[cleared].data_ = NULL;
	vector->storage_[cleared].size_ = 0;
	vector->tail_ = cleared;
	vector->length_--;
	return extracted_data;
}


s16 CIRCULARVECTOR_concat(CircularVector* vector, CircularVector* vector_src){
	if (NULL == vector || NULL == vector_src) {
		return kErrorCode_Null;
	}

	u16 source_count = vector_src->length_;
	if (0 == source_count) {
		return kErrorCode_Ok;
	}

	u16 total_after = vector->length_ + source_count;
	if (total_after > vector->capacity_) {
		s16 expansion = CIRCULARVECTOR_resize(vector,
			vector->capacity_ + vector_src->capacity_);
		if (kErrorCode_Ok != expansion) {
			return expansion;
		}
	}

	for (u16 i = 0; i < source_count; ++i) {
		u16 src_idx = (vector_src->head_ + i) % vector_src->capacity_;
		MemoryNode* src_node = &vector_src->storage_[src_idx];
		MemoryNode* dst_node = &vector->storage_[vector->tail_];

		s16 copy_result = dst_node->ops_->memCopy(dst_node,
			src_node->data_,
			src_node->size_);
		if (kErrorCode_Ok != copy_result) {
			return copy_result;
		}

		vector->tail_ = (vector->tail_ + 1) % vector->capacity_;
		vector->length_++;
	}

	return kErrorCode_Ok;
}

s16 CIRCULARVECTOR_traverse(CircularVector* vector, void (*callback)(MemoryNode*)){
	if (NULL == vector) {
		return kErrorCode_Null;
	}
	if (NULL == callback) {
		return kErrorCode_Null;
	}

	for (u16 i = 0; i < vector->length_; ++i) {
		u16 idx = (vector->head_ + i) % vector->capacity_;
		callback(&vector->storage_[idx]);
	}
	return kErrorCode_Ok;
}


void CIRCULARVECTOR_print(CircularVector* vector){
	if (NULL == vector) {
		printf("[CircularVector Info] Null Vector\n");
		return;
	}

	printf("[CircularVector Info] Address: %p\n", (void*)vector);
	printf("[CircularVector Info] Head: %d\n", vector->head_);
	printf("[CircularVector Info] Tail: %d\n", vector->tail_);
	printf("[CircularVector Info] Length: %d\n", vector->length_);
	printf("[CircularVector Info] Capacity: %d\n", vector->capacity_);

	if (NULL == vector->storage_) {
		printf("[CircularVector Info] Storage Address: NULL\n");
		return;
	}

	printf("[CircularVector Info] Storage Address: %p\n", (void*)vector->storage_);

	for (u16 i = 0; i < vector->length_; ++i) {
		u16 idx = (vector->head_ + i) % vector->capacity_;
		printf("    [CircularVector Info] Logical #%d (raw #%d)\n", i, idx);
		printf("        [Node Info] Size: %d\n", vector->storage_[idx].size_);

		if (NULL == vector->storage_[idx].data_) {
			printf("        [Node Info] Data: NULL\n");
		}
		else {
			printf("        [Node Info] Data Content: ");
			u8* byte_data = (u8*)vector->storage_[idx].data_;
			for (u16 j = 0; j < vector->storage_[idx].size_; j++) {
				printf("%c", byte_data[j]);
			}
			printf("\n");
		}
	}
}
