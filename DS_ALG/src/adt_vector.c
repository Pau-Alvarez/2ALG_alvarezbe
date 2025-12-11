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
#include "adt_vector.h"

#include "EDK_MemoryManager/edk_memory_manager.h"

//Vector forward declarations

static s16 VECTOR_destroy(Vector* vector);
static s16 VECTOR_softReset(Vector* vector);
static s16 VECTOR_reset(Vector* vector);
static s16 VECTOR_resize(Vector* vector, u16 new_size);

static u16 VECTOR_capacity(Vector* vector);
static u16 VECTOR_length(Vector* vector);
static boolean VECTOR_isEmpty(Vector* vector);
static boolean VECTOR_isFull(Vector* vector);

static void* VECTOR_first(Vector* vector, u16* size);
static void* VECTOR_last(Vector* vector, u16* size);
static void* VECTOR_at(Vector* vector, u16* size, u16 position);

static s16 VECTOR_insertFirst(Vector* vector, void *data, u16 bytes);
static s16 VECTOR_insertLast(Vector* vector, void* data, u16 bytes);
static s16 VECTOR_insertAt(Vector* vector, void* data, u16 bytes, u16 position);

static void* VECTOR_extractFirst(Vector* vector, u16* size);
static void* VECTOR_extractLast(Vector* vector, u16* size);
static void* VECTOR_extractAt(Vector* vector, u16* size, u16 position);

static s16 VECTOR_concat(Vector* vector, Vector *vector_src);
static s16 VECTOR_traverse(Vector* vector, void (*callback)(MemoryNode*));

static void VECTOR_print(Vector* vector);

Vector* VECTOR_create(u16 capacity);

struct vector_ops_s vector_ops = {	.destroy = VECTOR_destroy,
									.softReset = VECTOR_softReset,
									.reset = VECTOR_reset,
									.resize = VECTOR_resize,
									.capacity = VECTOR_capacity,
									.length = VECTOR_length,
									.isEmpty = VECTOR_isEmpty,
									.isFull = VECTOR_isFull,
									.first = VECTOR_first,
									.last = VECTOR_last,
									.at = VECTOR_at,
									.insertFirst = VECTOR_insertFirst,
									.insertLast = VECTOR_insertLast,
									.insertAt = VECTOR_insertAt,
									.extractFirst = VECTOR_extractFirst,
									.extractLast = VECTOR_extractLast,
									.extractAt = VECTOR_extractAt,
									.concat = VECTOR_concat,
									.traverse = VECTOR_traverse,
									.print = VECTOR_print,
								};


Vector* VECTOR_create(u16 capacity){
	if (0 == capacity) {
		return NULL;
	}

	Vector* new_vector = (Vector*)MM->malloc(sizeof(Vector));
	if (NULL == new_vector) {
		return NULL;
	}

	new_vector->capacity_ = capacity;
	new_vector->head_ = 0;
	new_vector->tail_ = 0;
	new_vector->ops_ = &vector_ops;

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

s16 VECTOR_destroy(Vector* vector) {
	if (NULL == vector) {
		return kErrorCode_Null;
	}

	if (NULL != vector->storage_) {
		for (u16 i = 0; i < vector->tail_; ++i) {
			vector->storage_[i].ops_->reset(&vector->storage_[i]);
		}
		MM->free(vector->storage_);
	}

	vector->storage_ = NULL;

	vector->ops_ = NULL;

	vector->head_ = 0;

	vector->tail_ = 0;

	vector->capacity_ = 0;

	MM->free(vector);

	return kErrorCode_Ok;
}

s16 VECTOR_softReset(Vector* vector){
	if (NULL == vector) {
		return kErrorCode_Null;
	}

	if (NULL == vector->storage_) {
		return kErrorCode_NullData;
	}

	for (u16 i = 0; i < vector->tail_; ++i) {
		vector->storage_[i].ops_->softReset(&vector->storage_[i]);
	}

	vector->tail_ = 0;
	vector->head_ = 0;

	return kErrorCode_Ok;
}

s16 VECTOR_reset(Vector* vector){
	if (NULL == vector) {
		return kErrorCode_Null;
	}
	if (NULL == vector->storage_) {
		return kErrorCode_NullData;
	}

	for (u16 i = 0; i < vector->tail_; ++i) {
		vector->storage_[i].ops_->reset(&vector->storage_[i]);
	}

	vector->tail_ = 0;
	vector->head_ = 0;

	return kErrorCode_Ok;
}

s16 VECTOR_resize(Vector* vector, u16 new_size){
	if (NULL == vector) {
		return kErrorCode_Null;
	}
	if (0 == new_size) {
		return kErrorCode_ZeroSize;
	}
	if (new_size == vector->capacity_) {
		return kErrorCode_Ok;
	}

	u16 previous_elem = vector->tail_;
	boolean smaller = (new_size < vector->capacity_);

	if (smaller && new_size < vector->tail_) {
		u16 first_element_to_remove = new_size;
		u16 last_element_to_remove = vector->tail_;

		for (u16 i = first_element_to_remove; i < last_element_to_remove; ++i) {
			vector->storage_[i].ops_->reset(&vector->storage_[i]);
		}

		previous_elem = new_size;
	}


	MemoryNode* fresh_storage = (MemoryNode*)MM->malloc(sizeof(MemoryNode) * new_size);
	if (NULL == fresh_storage) {
		return kErrorCode_Memory;
	}

	for (u16 position = 0; position < new_size; ++position) {
		MEMNODE_createLite(&fresh_storage[position]);
	}


	MemoryNode* source_storage = vector->storage_;
	for (u16 i = 0; i < previous_elem; ++i) {
		fresh_storage[i].data_ = source_storage[i].data_;
		fresh_storage[i].size_ = source_storage[i].size_;
		fresh_storage[i].ops_ = source_storage[i].ops_;
		fresh_storage[i].next_ = source_storage[i].next_;
		fresh_storage[i].prev_ = source_storage[i].prev_;
	}

	MM->free(source_storage);

	vector->storage_ = fresh_storage;
	vector->capacity_ = new_size;
	vector->tail_ = previous_elem;
	vector->head_ = 0;

	return kErrorCode_Ok;
}


u16 VECTOR_capacity(Vector* vector){
	return (NULL != vector) ? vector->capacity_ : 0;
}

u16 VECTOR_length(Vector* vector){
	return (NULL != vector) ? vector->tail_ : 0;
}

boolean VECTOR_isEmpty(Vector* vector){
	if (NULL == vector) {
		return False;
	}
	return (vector->head_ == vector->tail_) ? True : False;
}

boolean VECTOR_isFull(Vector* vector){
	if (NULL == vector) {
		return False;
	}
	return (vector->tail_ == vector->capacity_) ? True : False;
}


void* VECTOR_first(Vector* vector, u16* size){
	if (NULL == vector || NULL == size) {
		return NULL;
	}
	if (0 == vector->tail_) {
		return NULL;
	}

	*size = vector->storage_[0].size_;
	return vector->storage_[0].data_;
}

void* VECTOR_last(Vector* vector, u16* size){
	if (NULL == vector || NULL == size) {
		return NULL;
	}
	if (0 == vector->tail_) {
		return NULL;
	}

	MemoryNode* last_node = &vector->storage_[vector->tail_ - 1];
	*size = last_node->size_;
	return last_node->data_;
}

void* VECTOR_at(Vector* vector, u16* size, u16 position){
	if (NULL == vector || NULL == size) {
		return NULL;
	}
	if (position >= vector->tail_) {
		return NULL;
	}

	MemoryNode* target_node = &vector->storage_[position];
	*size = target_node->size_;
	return target_node->data_;
}


s16 VECTOR_insertFirst(Vector* vector, void* data, u16 bytes){
	if (NULL == vector) {
		return kErrorCode_Null;
	}
	if (NULL == data) {
		return kErrorCode_NullData;
	}
	if (0 == bytes) {
		return kErrorCode_ZeroSize;
	}
	if (VECTOR_isFull(vector)) {
		return kErrorCode_IsFull;
	}

	for (u16 i = vector->tail_; i > 0; --i) {
		vector->storage_[i] = vector->storage_[i - 1];
	}

	vector->storage_[0].ops_->setData(&vector->storage_[0], data, bytes);
	vector->tail_++;

	return kErrorCode_Ok;
}

s16 VECTOR_insertLast(Vector* vector, void* data, u16 bytes){
	if (NULL == vector) {
		return kErrorCode_Null;
	}
	if (NULL == data) {
		return kErrorCode_NullData;
	}
	if (0 == bytes) {
		return kErrorCode_ZeroSize;
	}
	if (VECTOR_isFull(vector)) {
		return kErrorCode_IsFull;
	}

	vector->storage_[vector->tail_].ops_->setData(
		&vector->storage_[vector->tail_], data, bytes);
	vector->tail_++;

	return kErrorCode_Ok;
}

s16 VECTOR_insertAt(Vector* vector, void* data, u16 bytes, u16 position){
	if (NULL == vector) {
		return kErrorCode_Null;
	}
	if (NULL == data) {
		return kErrorCode_NullData;
	}
	if (0 == bytes) {
		return kErrorCode_ZeroSize;
	}
	if (VECTOR_isFull(vector)) {
		return kErrorCode_IsFull;
	}

	if (position > vector->tail_) {
		position = vector->tail_;
	}

	if (position == vector->tail_) {
		return VECTOR_insertLast(vector, data, bytes);
	}

	for (u16 i = vector->tail_; i > position; --i) {
		vector->storage_[i] = vector->storage_[i - 1];
	}

	vector->storage_[position].ops_->setData(
		&vector->storage_[position], data, bytes);
	vector->tail_++;

	return kErrorCode_Ok;
}


void* VECTOR_extractFirst(Vector* vector, u16* size){
	if (NULL == vector || NULL == size) {
		return NULL;
	}
	if (VECTOR_isEmpty(vector)) {
		return NULL;
	}

	MemoryNode* first_node = &vector->storage_[0];
	void* extracted_data = first_node->data_;
	*size = first_node->size_;

	for (u16 i = 0; i < vector->tail_ - 1; i++) {
		vector->storage_[i] = vector->storage_[i + 1];
	}

	vector->tail_--;

	return extracted_data;
}

void* VECTOR_extractLast(Vector* vector, u16* size){
	if (NULL == vector || NULL == size) {
		return NULL;
	}
	if (VECTOR_isEmpty(vector)) {
		return NULL;
	}

	MemoryNode* last_node = &vector->storage_[vector->tail_ - 1];
	void* extracted_data = last_node->data_;
	*size = last_node->size_;

	vector->tail_--;

	vector->storage_[vector->tail_].data_ = NULL;

	vector->storage_[vector->tail_].size_ = 0;

	return extracted_data;
}

void* VECTOR_extractAt(Vector* vector, u16* size, u16 position){
	if (NULL == vector || NULL == size) {
		return NULL;
	}
	if (VECTOR_isEmpty(vector)) {
		return NULL;
	}
	if (position >= vector->tail_) {
		return NULL;
	}

	if (0 == position) {
		return VECTOR_extractFirst(vector, size);
	}

	if (position == vector->tail_ - 1) {
		return VECTOR_extractLast(vector, size);
	}

	MemoryNode* target_node = &vector->storage_[position];
	void* extracted_data = target_node->data_;
	*size = target_node->size_;

	for (u16 i = position; i < vector->tail_ - 1; ++i) {
		vector->storage_[i] = vector->storage_[i + 1];
	}

	vector->tail_--;

	vector->storage_[vector->tail_].data_ = NULL;
	vector->storage_[vector->tail_].size_ = 0;

	return extracted_data;
}


s16 VECTOR_concat(Vector* vector, Vector* vector_src){
	if (NULL == vector || NULL == vector_src) {
		return kErrorCode_Null;
	}

	u16 source_element_count = vector_src->tail_;
	if (0 == source_element_count) {
		return kErrorCode_Ok;
	}

	u16 current_element_count = vector->tail_;
	u16 total_elements_after_concat = current_element_count + source_element_count;
	u16 available_space = vector->capacity_;
	boolean needs_expansion = (total_elements_after_concat > available_space);

	if (needs_expansion) {
		u16 expanded_capacity = vector->capacity_ + vector_src->capacity_;
		s16 expansion_result = VECTOR_resize(vector, expanded_capacity);

		if (kErrorCode_Ok != expansion_result) {
			return expansion_result;
		}
	}

	u16 insertion_point = current_element_count;

	for (u16 source_idx = 0; source_idx < source_element_count; source_idx++) {
		MemoryNode* node_to_copy = &vector_src->storage_[source_idx];
		u16 destination_idx = insertion_point + source_idx;
		MemoryNode* destination_node = &vector->storage_[destination_idx];

		s16 copy_result = destination_node->ops_->memCopy(
			destination_node,
			node_to_copy->data_,
			node_to_copy->size_
		);

		if (kErrorCode_Ok != copy_result) {
			return copy_result;
		}
	}

	vector->tail_ = total_elements_after_concat;

	return kErrorCode_Ok;
}

s16 VECTOR_traverse(Vector* vector, void (*callback)(MemoryNode*)){
	if (NULL == vector) {
		return kErrorCode_Null;
	}
	if (NULL == callback) {
		return kErrorCode_Null;
	}
	u16 first_element = vector->head_;
	u16 last_element = vector->tail_;
	u16 total_iterations = last_element - first_element;

	if (0 == total_iterations) {
		return kErrorCode_Ok;
	}

	for (u16 current_position = first_element; current_position < last_element; ++current_position) {
		MemoryNode* current_node = &vector->storage_[current_position];

		callback(current_node);

		printf("\n");
	}

	return kErrorCode_Ok;
}


void VECTOR_print(Vector* vector){
	if (NULL == vector) {
		printf("[Vector Info] Null Vector\n");
		return;
	}

	printf("[Vector Info] Address: %p\n", (void*)vector);
	printf("[Vector Info] Head: %d\n", vector->head_);
	printf("[Vector Info] Tail: %d\n", vector->tail_);
	printf("[Vector Info] Length: %d\n", (vector->tail_ - vector->head_));
	printf("[Vector Info] Capacity: %d\n", vector->capacity_);

	if (NULL == vector->storage_) {
		printf("[Vector Info] Storage Address: NULL\n");
	}
	else {
		printf("[Vector Info] Storage Address: %p\n", (void*)vector->storage_);
	}

	// MEJORA: Imprimir cada nodo del storage usado
	for (u16 idx = 0; idx < vector->tail_; idx++) {
		printf("    [Vector Info] Storage #%d\n", idx);
		printf("        [Node Info] Address: %p\n", (void*)&vector->storage_[idx]);
		printf("        [Node Info] Size: %d\n", vector->storage_[idx].size_);

		if (NULL == vector->storage_[idx].data_) {
			printf("        [Node Info] Data Address: NULL\n");
		}
		else {
			printf("        [Node Info] Data Address: %p\n",
				(void*)vector->storage_[idx].data_);
		}

		if (NULL != vector->storage_[idx].data_ && vector->storage_[idx].size_ > 0) {
			printf("        [Node Info] Data Content: ");
			u8* byte_data = (u8*)vector->storage_[idx].data_;
			for (u16 j = 0; j < vector->storage_[idx].size_; j++) {
				printf("%c", byte_data[j]);
			}
			printf("\n");
		}

		if (NULL == vector->storage_[idx].next_) {
			printf("        [Node Info] Next Address: NULL\n");
		}
		else {
			printf("        [Node Info] Next Address: %p\n",
				(void*)vector->storage_[idx].next_);
		}

		if (NULL == vector->storage_[idx].prev_) {
			printf("        [Node Info] Prev Address: NULL\n");
		}
		else {
			printf("        [Node Info] Prev Address: %p\n",
				(void*)vector->storage_[idx].prev_);
		}
	}
}


