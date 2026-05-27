/*
 * Ivan Sancho as Unreal Authorized Instructor (UAI), 2025.
 *
 * Queue source code.
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
#include "adt_queue.h"

#include "EDK_MemoryManager/edk_memory_manager.h"

//Queue forward declarations

static s16 QUEUE_destroy(Queue* queue);
static s16 QUEUE_softReset(Queue* queue);
static s16 QUEUE_reset(Queue* queue);
static s16 QUEUE_resize(Queue* queue, u16 new_size);

static u16 QUEUE_capacity(Queue* queue);
static u16 QUEUE_length(Queue* queue);
static boolean QUEUE_isEmpty(Queue* queue);
static boolean QUEUE_isFull(Queue* queue);

static void* QUEUE_front(Queue* queue);
static void* QUEUE_back(Queue* queue);

static s16 QUEUE_enqueue(Queue* queue, void* data, u16 bytes);
static void* QUEUE_dequeue(Queue* queue);

static s16 QUEUE_concat(Queue* queue, Queue* queue_src);
static s16 QUEUE_traverse(Queue* queue, void (*callback)(MemoryNode*));

static void QUEUE_print(Queue* queue);


struct queue_ops_s queue_ops = {	.destroy = QUEUE_destroy,
									.softReset = QUEUE_softReset,
									.reset = QUEUE_reset,
									.resize = QUEUE_resize,
									.capacity = QUEUE_capacity,
									.length = QUEUE_length,
									.isEmpty = QUEUE_isEmpty,
									.isFull = QUEUE_isFull,
									.front = QUEUE_front,
									.back = QUEUE_back,
									.enqueue = QUEUE_enqueue,
									.dequeue = QUEUE_dequeue,
									.concat = QUEUE_concat,
									.traverse = QUEUE_traverse,
									.print = QUEUE_print,
								};


Queue* QUEUE_create(u16 capacity){
	if (0 == capacity) {
		return NULL;
	}

	Queue* new_queue = (Queue*)MM->malloc(sizeof(Queue));
	if (NULL == new_queue) {
		return NULL;
	}

	new_queue->capacity_ = capacity;
	new_queue->head_ = 0;
	new_queue->tail_ = 0;
	new_queue->length_ = 0;
	new_queue->ops_ = &queue_ops;

	new_queue->storage_ = (MemoryNode*)MM->malloc(sizeof(MemoryNode) * capacity);
	if (NULL == new_queue->storage_) {
		MM->free(new_queue);
		return NULL;
	}

	for (u16 idx = 0; idx < capacity; idx++) {
		MEMNODE_createLite(&new_queue->storage_[idx]);
	}

	return new_queue;
}

s16 QUEUE_destroy(Queue* queue) {
	if (NULL == queue) {
		return kErrorCode_Null;
	}

	if (NULL != queue->storage_) {
		for (u16 i = 0; i < queue->length_; ++i) {
			u16 idx = (queue->head_ + i) % queue->capacity_;
			queue->storage_[idx].ops_->reset(&queue->storage_[idx]);
		}
		MM->free(queue->storage_);
	}

	queue->storage_ = NULL;
	queue->ops_ = NULL;
	queue->head_ = 0;
	queue->tail_ = 0;
	queue->length_ = 0;
	queue->capacity_ = 0;

	MM->free(queue);
	return kErrorCode_Ok;
}

s16 QUEUE_softReset(Queue* queue){
	if (NULL == queue) {
		return kErrorCode_Null;
	}
	if (NULL == queue->storage_) {
		return kErrorCode_NullData;
	}

	for (u16 i = 0; i < queue->length_; ++i) {
		u16 idx = (queue->head_ + i) % queue->capacity_;
		queue->storage_[idx].ops_->softReset(&queue->storage_[idx]);
	}

	queue->head_ = 0;
	queue->tail_ = 0;
	queue->length_ = 0;
	return kErrorCode_Ok;
}

s16 QUEUE_reset(Queue* queue){
	if (NULL == queue) {
		return kErrorCode_Null;
	}
	if (NULL == queue->storage_) {
		return kErrorCode_NullData;
	}

	for (u16 i = 0; i < queue->length_; ++i) {
		u16 idx = (queue->head_ + i) % queue->capacity_;
		queue->storage_[idx].ops_->reset(&queue->storage_[idx]);
	}

	queue->head_ = 0;
	queue->tail_ = 0;
	queue->length_ = 0;
	return kErrorCode_Ok;
}

s16 QUEUE_resize(Queue* queue, u16 new_size){
	if (NULL == queue) {
		return kErrorCode_Null;
	}
	if (0 == new_size) {
		return kErrorCode_ZeroSize;
	}
	if (new_size == queue->capacity_) {
		return kErrorCode_Ok;
	}

	u16 keep_count = queue->length_;
	if (new_size < queue->length_) {
		for (u16 i = new_size; i < queue->length_; ++i) {
			u16 idx = (queue->head_ + i) % queue->capacity_;
			queue->storage_[idx].ops_->reset(&queue->storage_[idx]);
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
		u16 src_idx = (queue->head_ + i) % queue->capacity_;
		fresh_storage[i].data_ = queue->storage_[src_idx].data_;
		fresh_storage[i].size_ = queue->storage_[src_idx].size_;
	}

	MM->free(queue->storage_);
	queue->storage_ = fresh_storage;
	queue->capacity_ = new_size;
	queue->head_ = 0;
	queue->tail_ = (keep_count == new_size) ? 0 : keep_count;
	queue->length_ = keep_count;
	return kErrorCode_Ok;
}


u16 QUEUE_capacity(Queue* queue){
	return (NULL != queue) ? queue->capacity_ : 0;
}

u16 QUEUE_length(Queue* queue){
	return (NULL != queue) ? queue->length_ : 0;
}

boolean QUEUE_isEmpty(Queue* queue){
	if (NULL == queue) {
		return False;
	}
	return (0 == queue->length_) ? True : False;
}

boolean QUEUE_isFull(Queue* queue){
	if (NULL == queue) {
		return False;
	}
	return (queue->length_ == queue->capacity_) ? True : False;
}


void* QUEUE_front(Queue* queue){
	if (NULL == queue) {
		return NULL;
	}
	if (0 == queue->length_) {
		return NULL;
	}

	return queue->storage_[queue->head_].data_;
}

void* QUEUE_back(Queue* queue){
	if (NULL == queue) {
		return NULL;
	}
	if (0 == queue->length_) {
		return NULL;
	}

	u16 back_idx = (queue->tail_ + queue->capacity_ - 1) % queue->capacity_;
	return queue->storage_[back_idx].data_;
}


s16 QUEUE_enqueue(Queue* queue, void* data, u16 bytes){
	if (NULL == queue) {
		return kErrorCode_Null;
	}
	if (NULL == data) {
		return kErrorCode_NullData;
	}
	if (0 == bytes) {
		return kErrorCode_ZeroSize;
	}
	if (QUEUE_isFull(queue)) {
		return kErrorCode_IsFull;
	}

	queue->storage_[queue->tail_].data_ = data;
	queue->storage_[queue->tail_].size_ = bytes;
	queue->tail_ = (queue->tail_ + 1) % queue->capacity_;
	queue->length_++;
	return kErrorCode_Ok;
}

void* QUEUE_dequeue(Queue* queue){
	if (NULL == queue) {
		return NULL;
	}
	if (0 == queue->length_) {
		return NULL;
	}

	MemoryNode* front_node = &queue->storage_[queue->head_];
	void* extracted_data = front_node->data_;

	front_node->data_ = NULL;
	front_node->size_ = 0;
	queue->head_ = (queue->head_ + 1) % queue->capacity_;
	queue->length_--;
	return extracted_data;
}


s16 QUEUE_concat(Queue* queue, Queue* queue_src){
	if (NULL == queue || NULL == queue_src) {
		return kErrorCode_Null;
	}

	u16 source_count = queue_src->length_;
	if (0 == source_count) {
		return kErrorCode_Ok;
	}

	u16 total_after = queue->length_ + source_count;
	if (total_after > queue->capacity_) {
		s16 expansion = QUEUE_resize(queue, queue->capacity_ + queue_src->capacity_);
		if (kErrorCode_Ok != expansion) {
			return expansion;
		}
	}

	for (u16 i = 0; i < source_count; ++i) {
		u16 src_idx = (queue_src->head_ + i) % queue_src->capacity_;
		MemoryNode* src_node = &queue_src->storage_[src_idx];
		MemoryNode* dst_node = &queue->storage_[queue->tail_];

		s16 copy_result = dst_node->ops_->memCopy(dst_node,
			src_node->data_,
			src_node->size_);
		if (kErrorCode_Ok != copy_result) {
			return copy_result;
		}

		queue->tail_ = (queue->tail_ + 1) % queue->capacity_;
		queue->length_++;
	}

	return kErrorCode_Ok;
}

s16 QUEUE_traverse(Queue* queue, void (*callback)(MemoryNode*)){
	if (NULL == queue) {
		return kErrorCode_Null;
	}
	if (NULL == callback) {
		return kErrorCode_Null;
	}

	for (u16 i = 0; i < queue->length_; ++i) {
		u16 idx = (queue->head_ + i) % queue->capacity_;
		callback(&queue->storage_[idx]);
	}
	return kErrorCode_Ok;
}


void QUEUE_print(Queue* queue){
	if (NULL == queue) {
		printf("[Queue Info] Null Queue\n");
		return;
	}

	printf("[Queue Info] Address: %p\n", (void*)queue);
	printf("[Queue Info] Head: %d\n", queue->head_);
	printf("[Queue Info] Tail: %d\n", queue->tail_);
	printf("[Queue Info] Length: %d\n", queue->length_);
	printf("[Queue Info] Capacity: %d\n", queue->capacity_);

	if (NULL == queue->storage_) {
		printf("[Queue Info] Storage Address: NULL\n");
		return;
	}

	printf("[Queue Info] Storage Address: %p\n", (void*)queue->storage_);

	for (u16 i = 0; i < queue->length_; ++i) {
		u16 idx = (queue->head_ + i) % queue->capacity_;
		printf("    [Queue Info] Logical #%d (raw #%d)\n", i, idx);
		printf("        [Node Info] Size: %d\n", queue->storage_[idx].size_);

		if (NULL == queue->storage_[idx].data_) {
			printf("        [Node Info] Data: NULL\n");
		}
		else {
			printf("        [Node Info] Data Content: ");
			u8* byte_data = (u8*)queue->storage_[idx].data_;
			for (u16 j = 0; j < queue->storage_[idx].size_; j++) {
				printf("%c", byte_data[j]);
			}
			printf("\n");
		}
	}
}
