/*
 * Ivan Sancho as Unreal Authorized Instructor (UAI), 2025.
 *
 * Stack source code.
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
#include "adt_stack.h"

#include "EDK_MemoryManager/edk_memory_manager.h"

//Stack forward declarations

static s16 STACK_destroy(Stack* stack);
static s16 STACK_softReset(Stack* stack);
static s16 STACK_reset(Stack* stack);
static s16 STACK_resize(Stack* stack, u16 new_size);

static u16 STACK_capacity(Stack* stack);
static u16 STACK_length(Stack* stack);
static boolean STACK_isEmpty(Stack* stack);
static boolean STACK_isFull(Stack* stack);

static void* STACK_top(Stack* stack);
static s16 STACK_push(Stack* stack, void* data, u16 bytes);
static void* STACK_pop(Stack* stack, u16* size);

static s16 STACK_concat(Stack* stack, Stack* stack_src);
static s16 STACK_traverse(Stack* stack, void (*callback)(MemoryNode*));

static void STACK_print(Stack* stack);


struct stack_ops_s stack_ops = {	.destroy = STACK_destroy,
									.softReset = STACK_softReset,
									.reset = STACK_reset,
									.resize = STACK_resize,
									.capacity = STACK_capacity,
									.length = STACK_length,
									.isEmpty = STACK_isEmpty,
									.isFull = STACK_isFull,
									.top = STACK_top,
									.push = STACK_push,
									.pop = STACK_pop,
									.concat = STACK_concat,
									.traverse = STACK_traverse,
									.print = STACK_print,
								};


Stack* STACK_create(u16 capacity){
	if (0 == capacity) {
		return NULL;
	}

	Stack* new_stack = (Stack*)MM->malloc(sizeof(Stack));
	if (NULL == new_stack) {
		return NULL;
	}

	new_stack->capacity_ = capacity;
	new_stack->head_ = 0;
	new_stack->tail_ = 0;
	new_stack->ops_ = &stack_ops;

	new_stack->storage_ = (MemoryNode*)MM->malloc(sizeof(MemoryNode) * capacity);
	if (NULL == new_stack->storage_) {
		MM->free(new_stack);
		return NULL;
	}

	for (u16 idx = 0; idx < capacity; idx++) {
		MEMNODE_createLite(&new_stack->storage_[idx]);
	}

	return new_stack;
}

s16 STACK_destroy(Stack* stack) {
	if (NULL == stack) {
		return kErrorCode_Null;
	}

	if (NULL != stack->storage_) {
		for (u16 i = 0; i < stack->tail_; ++i) {
			stack->storage_[i].ops_->reset(&stack->storage_[i]);
		}
		MM->free(stack->storage_);
	}

	stack->storage_ = NULL;
	stack->ops_ = NULL;
	stack->head_ = 0;
	stack->tail_ = 0;
	stack->capacity_ = 0;

	MM->free(stack);
	return kErrorCode_Ok;
}

s16 STACK_softReset(Stack* stack){
	if (NULL == stack) {
		return kErrorCode_Null;
	}
	if (NULL == stack->storage_) {
		return kErrorCode_NullData;
	}

	for (u16 i = 0; i < stack->tail_; ++i) {
		stack->storage_[i].ops_->softReset(&stack->storage_[i]);
	}

	stack->head_ = 0;
	stack->tail_ = 0;
	return kErrorCode_Ok;
}

s16 STACK_reset(Stack* stack){
	if (NULL == stack) {
		return kErrorCode_Null;
	}
	if (NULL == stack->storage_) {
		return kErrorCode_NullData;
	}

	for (u16 i = 0; i < stack->tail_; ++i) {
		stack->storage_[i].ops_->reset(&stack->storage_[i]);
	}

	stack->head_ = 0;
	stack->tail_ = 0;
	return kErrorCode_Ok;
}

s16 STACK_resize(Stack* stack, u16 new_size){
	if (NULL == stack) {
		return kErrorCode_Null;
	}
	if (0 == new_size) {
		return kErrorCode_ZeroSize;
	}
	if (new_size == stack->capacity_) {
		return kErrorCode_Ok;
	}

	u16 previous_elem = stack->tail_;
	if (new_size < stack->tail_) {
		for (u16 i = new_size; i < stack->tail_; ++i) {
			stack->storage_[i].ops_->reset(&stack->storage_[i]);
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

	for (u16 i = 0; i < previous_elem; ++i) {
		fresh_storage[i].data_ = stack->storage_[i].data_;
		fresh_storage[i].size_ = stack->storage_[i].size_;
	}

	MM->free(stack->storage_);
	stack->storage_ = fresh_storage;
	stack->capacity_ = new_size;
	stack->tail_ = previous_elem;
	stack->head_ = 0;
	return kErrorCode_Ok;
}


u16 STACK_capacity(Stack* stack){
	return (NULL != stack) ? stack->capacity_ : 0;
}

u16 STACK_length(Stack* stack){
	return (NULL != stack) ? stack->tail_ : 0;
}

boolean STACK_isEmpty(Stack* stack){
	if (NULL == stack) {
		return False;
	}
	return (0 == stack->tail_) ? True : False;
}

boolean STACK_isFull(Stack* stack){
	if (NULL == stack) {
		return False;
	}
	return (stack->tail_ == stack->capacity_) ? True : False;
}


void* STACK_top(Stack* stack){
	if (NULL == stack) {
		return NULL;
	}
	if (0 == stack->tail_) {
		return NULL;
	}

	return stack->storage_[stack->tail_ - 1].data_;
}


s16 STACK_push(Stack* stack, void* data, u16 bytes){
	if (NULL == stack) {
		return kErrorCode_Null;
	}
	if (NULL == data) {
		return kErrorCode_NullData;
	}
	if (0 == bytes) {
		return kErrorCode_ZeroSize;
	}
	if (STACK_isFull(stack)) {
		return kErrorCode_IsFull;
	}

	stack->storage_[stack->tail_].data_ = data;
	stack->storage_[stack->tail_].size_ = bytes;
	stack->tail_++;
	return kErrorCode_Ok;
}

void* STACK_pop(Stack* stack, u16* size){
	if (NULL == stack) {
		return NULL;
	}
	if (0 == stack->tail_) {
		return NULL;
	}

	MemoryNode* top_node = &stack->storage_[stack->tail_ - 1];
	void* extracted_data = top_node->data_;
	if (NULL != size) {
		*size = top_node->size_;
	}

	stack->tail_--;
	stack->storage_[stack->tail_].data_ = NULL;
	stack->storage_[stack->tail_].size_ = 0;

	return extracted_data;
}


s16 STACK_concat(Stack* stack, Stack* stack_src){
	if (NULL == stack || NULL == stack_src) {
		return kErrorCode_Null;
	}

	u16 source_count = stack_src->tail_;
	if (0 == source_count) {
		return kErrorCode_Ok;
	}

	u16 total_after = stack->tail_ + source_count;
	if (total_after > stack->capacity_) {
		s16 expansion = STACK_resize(stack, stack->capacity_ + stack_src->capacity_);
		if (kErrorCode_Ok != expansion) {
			return expansion;
		}
	}

	u16 insertion_point = stack->tail_;
	for (u16 i = 0; i < source_count; ++i) {
		MemoryNode* src_node = &stack_src->storage_[i];
		MemoryNode* dst_node = &stack->storage_[insertion_point + i];

		s16 copy_result = dst_node->ops_->memCopy(dst_node,
			src_node->data_,
			src_node->size_);
		if (kErrorCode_Ok != copy_result) {
			return copy_result;
		}
	}

	stack->tail_ = total_after;
	return kErrorCode_Ok;
}

s16 STACK_traverse(Stack* stack, void (*callback)(MemoryNode*)){
	if (NULL == stack) {
		return kErrorCode_Null;
	}
	if (NULL == callback) {
		return kErrorCode_Null;
	}

	for (u16 i = stack->head_; i < stack->tail_; ++i) {
		callback(&stack->storage_[i]);
	}
	return kErrorCode_Ok;
}


void STACK_print(Stack* stack){
	if (NULL == stack) {
		printf("[Stack Info] Null Stack\n");
		return;
	}

	printf("[Stack Info] Address: %p\n", (void*)stack);
	printf("[Stack Info] Length: %d\n", stack->tail_);
	printf("[Stack Info] Capacity: %d\n", stack->capacity_);

	if (NULL == stack->storage_) {
		printf("[Stack Info] Storage Address: NULL\n");
		return;
	}

	printf("[Stack Info] Storage Address: %p\n", (void*)stack->storage_);

	for (u16 idx = 0; idx < stack->tail_; idx++) {
		printf("    [Stack Info] Storage #%d\n", idx);
		printf("        [Node Info] Size: %d\n", stack->storage_[idx].size_);

		if (NULL == stack->storage_[idx].data_) {
			printf("        [Node Info] Data: NULL\n");
		}
		else {
			printf("        [Node Info] Data Content: ");
			u8* byte_data = (u8*)stack->storage_[idx].data_;
			for (u16 j = 0; j < stack->storage_[idx].size_; j++) {
				printf("%c", byte_data[j]);
			}
			printf("\n");
		}
	}
}
