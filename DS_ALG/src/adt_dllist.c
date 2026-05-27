/*
 * Ivan Sancho as Unreal Authorized Instructor (UAI), 2025.
 *
 * Double Linked List source code.
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
#include "adt_dllist.h"

#include "EDK_MemoryManager/edk_memory_manager.h"

//DLList forward declarations

static s16 DLLIST_destroy(DLList* list);
static s16 DLLIST_softReset(DLList* list);
static s16 DLLIST_reset(DLList* list);
static s16 DLLIST_resize(DLList* list, u16 new_size);

static u16 DLLIST_capacity(DLList* list);
static u16 DLLIST_length(DLList* list);
static boolean DLLIST_isEmpty(DLList* list);
static boolean DLLIST_isFull(DLList* list);

static void* DLLIST_first(DLList* list, u16* size);
static void* DLLIST_last(DLList* list, u16* size);
static void* DLLIST_at(DLList* list, u16* size, u16 position);

static s16 DLLIST_insertFirst(DLList* list, void* data, u16 bytes);
static s16 DLLIST_insertLast(DLList* list, void* data, u16 bytes);
static s16 DLLIST_insertAt(DLList* list, void* data, u16 bytes, u16 position);

static void* DLLIST_extractFirst(DLList* list, u16* size);
static void* DLLIST_extractLast(DLList* list, u16* size);
static void* DLLIST_extractAt(DLList* list, u16* size, u16 position);

static s16 DLLIST_concat(DLList* list, DLList* list_src);
static s16 DLLIST_traverse(DLList* list, void (*callback)(MemoryNode*));

static void DLLIST_print(DLList* list);


struct dllist_ops_s dllist_ops = {	.destroy = DLLIST_destroy,
									.softReset = DLLIST_softReset,
									.reset = DLLIST_reset,
									.resize = DLLIST_resize,
									.capacity = DLLIST_capacity,
									.length = DLLIST_length,
									.isEmpty = DLLIST_isEmpty,
									.isFull = DLLIST_isFull,
									.first = DLLIST_first,
									.last = DLLIST_last,
									.at = DLLIST_at,
									.insertFirst = DLLIST_insertFirst,
									.insertLast = DLLIST_insertLast,
									.insertAt = DLLIST_insertAt,
									.extractFirst = DLLIST_extractFirst,
									.extractLast = DLLIST_extractLast,
									.extractAt = DLLIST_extractAt,
									.concat = DLLIST_concat,
									.traverse = DLLIST_traverse,
									.print = DLLIST_print,
								};


DLList* DLLIST_create(u16 capacity){
	if (0 == capacity) {
		return NULL;
	}

	DLList* new_list = (DLList*)MM->malloc(sizeof(DLList));
	if (NULL == new_list) {
		return NULL;
	}

	new_list->head_ = NULL;
	new_list->tail_ = NULL;
	new_list->length_ = 0;
	new_list->capacity_ = capacity;
	new_list->ops_ = &dllist_ops;

	return new_list;
}

s16 DLLIST_destroy(DLList* list) {
	if (NULL == list) {
		return kErrorCode_Null;
	}

	MemoryNode* current = list->head_;
	while (NULL != current) {
		MemoryNode* next = current->next_;
		current->ops_->free(current);
		current = next;
	}

	list->head_ = NULL;
	list->tail_ = NULL;
	list->length_ = 0;
	list->capacity_ = 0;
	list->ops_ = NULL;

	MM->free(list);
	return kErrorCode_Ok;
}

s16 DLLIST_softReset(DLList* list){
	if (NULL == list) {
		return kErrorCode_Null;
	}

	MemoryNode* current = list->head_;
	while (NULL != current) {
		MemoryNode* next = current->next_;
		current->ops_->softFree(current);
		current = next;
	}

	list->head_ = NULL;
	list->tail_ = NULL;
	list->length_ = 0;

	return kErrorCode_Ok;
}

s16 DLLIST_reset(DLList* list){
	if (NULL == list) {
		return kErrorCode_Null;
	}

	MemoryNode* current = list->head_;
	while (NULL != current) {
		MemoryNode* next = current->next_;
		current->ops_->free(current);
		current = next;
	}

	list->head_ = NULL;
	list->tail_ = NULL;
	list->length_ = 0;

	return kErrorCode_Ok;
}

s16 DLLIST_resize(DLList* list, u16 new_size){
	if (NULL == list) {
		return kErrorCode_Null;
	}
	if (0 == new_size) {
		return kErrorCode_ZeroSize;
	}

	if (new_size < list->length_) {
		MemoryNode* current = list->head_;
		for (u16 i = 0; i < new_size - 1 && NULL != current; ++i) {
			current = current->next_;
		}

		MemoryNode* discard = (NULL != current) ? current->next_ : NULL;
		if (NULL != current) {
			current->next_ = NULL;
			list->tail_ = current;
		}

		while (NULL != discard) {
			MemoryNode* next = discard->next_;
			discard->ops_->free(discard);
			discard = next;
		}

		list->length_ = new_size;
	}

	list->capacity_ = new_size;
	return kErrorCode_Ok;
}


u16 DLLIST_capacity(DLList* list){
	return (NULL != list) ? list->capacity_ : 0;
}

u16 DLLIST_length(DLList* list){
	return (NULL != list) ? list->length_ : 0;
}

boolean DLLIST_isEmpty(DLList* list){
	if (NULL == list) {
		return False;
	}
	return (0 == list->length_) ? True : False;
}

boolean DLLIST_isFull(DLList* list){
	if (NULL == list) {
		return False;
	}
	return (list->length_ == list->capacity_) ? True : False;
}


void* DLLIST_first(DLList* list, u16* size){
	if (NULL == list || NULL == size) {
		return NULL;
	}
	if (NULL == list->head_) {
		return NULL;
	}

	*size = list->head_->size_;
	return list->head_->data_;
}

void* DLLIST_last(DLList* list, u16* size){
	if (NULL == list || NULL == size) {
		return NULL;
	}
	if (NULL == list->tail_) {
		return NULL;
	}

	*size = list->tail_->size_;
	return list->tail_->data_;
}

void* DLLIST_at(DLList* list, u16* size, u16 position){
	if (NULL == list || NULL == size) {
		return NULL;
	}
	if (position >= list->length_) {
		return NULL;
	}

	MemoryNode* current;
	if (position <= list->length_ / 2) {
		current = list->head_;
		for (u16 i = 0; i < position; ++i) {
			current = current->next_;
		}
	}
	else {
		current = list->tail_;
		for (u16 i = list->length_ - 1; i > position; --i) {
			current = current->prev_;
		}
	}

	*size = current->size_;
	return current->data_;
}


s16 DLLIST_insertFirst(DLList* list, void* data, u16 bytes){
	if (NULL == list) {
		return kErrorCode_Null;
	}
	if (NULL == data) {
		return kErrorCode_NullData;
	}
	if (0 == bytes) {
		return kErrorCode_ZeroSize;
	}
	if (DLLIST_isFull(list)) {
		return kErrorCode_IsFull;
	}

	MemoryNode* new_node = MEMNODE_create();
	if (NULL == new_node) {
		return kErrorCode_Memory;
	}
	new_node->ops_->setData(new_node, data, bytes);

	if (NULL == list->head_) {
		list->head_ = new_node;
		list->tail_ = new_node;
	}
	else {
		new_node->next_ = list->head_;
		list->head_->prev_ = new_node;
		list->head_ = new_node;
	}
	list->length_++;

	return kErrorCode_Ok;
}

s16 DLLIST_insertLast(DLList* list, void* data, u16 bytes){
	if (NULL == list) {
		return kErrorCode_Null;
	}
	if (NULL == data) {
		return kErrorCode_NullData;
	}
	if (0 == bytes) {
		return kErrorCode_ZeroSize;
	}
	if (DLLIST_isFull(list)) {
		return kErrorCode_IsFull;
	}

	MemoryNode* new_node = MEMNODE_create();
	if (NULL == new_node) {
		return kErrorCode_Memory;
	}
	new_node->ops_->setData(new_node, data, bytes);

	if (NULL == list->tail_) {
		list->head_ = new_node;
		list->tail_ = new_node;
	}
	else {
		new_node->prev_ = list->tail_;
		list->tail_->next_ = new_node;
		list->tail_ = new_node;
	}
	list->length_++;

	return kErrorCode_Ok;
}

s16 DLLIST_insertAt(DLList* list, void* data, u16 bytes, u16 position){
	if (NULL == list) {
		return kErrorCode_Null;
	}
	if (NULL == data) {
		return kErrorCode_NullData;
	}
	if (0 == bytes) {
		return kErrorCode_ZeroSize;
	}
	if (DLLIST_isFull(list)) {
		return kErrorCode_IsFull;
	}

	if (0 == position) {
		return DLLIST_insertFirst(list, data, bytes);
	}
	if (position >= list->length_) {
		return DLLIST_insertLast(list, data, bytes);
	}

	MemoryNode* prev_node = list->head_;
	for (u16 i = 0; i < position - 1; ++i) {
		prev_node = prev_node->next_;
	}

	MemoryNode* new_node = MEMNODE_create();
	if (NULL == new_node) {
		return kErrorCode_Memory;
	}
	new_node->ops_->setData(new_node, data, bytes);

	new_node->next_ = prev_node->next_;
	new_node->prev_ = prev_node;
	prev_node->next_->prev_ = new_node;
	prev_node->next_ = new_node;
	list->length_++;

	return kErrorCode_Ok;
}


void* DLLIST_extractFirst(DLList* list, u16* size){
	if (NULL == list || NULL == size) {
		return NULL;
	}
	if (NULL == list->head_) {
		return NULL;
	}

	MemoryNode* old_head = list->head_;
	void* extracted_data = old_head->data_;
	*size = old_head->size_;

	list->head_ = old_head->next_;
	if (NULL == list->head_) {
		list->tail_ = NULL;
	}
	else {
		list->head_->prev_ = NULL;
	}
	list->length_--;

	old_head->ops_->softFree(old_head);
	return extracted_data;
}

void* DLLIST_extractLast(DLList* list, u16* size){
	if (NULL == list || NULL == size) {
		return NULL;
	}
	if (NULL == list->tail_) {
		return NULL;
	}

	MemoryNode* old_tail = list->tail_;
	void* extracted_data = old_tail->data_;
	*size = old_tail->size_;

	list->tail_ = old_tail->prev_;
	if (NULL == list->tail_) {
		list->head_ = NULL;
	}
	else {
		list->tail_->next_ = NULL;
	}
	list->length_--;

	old_tail->ops_->softFree(old_tail);
	return extracted_data;
}

void* DLLIST_extractAt(DLList* list, u16* size, u16 position){
	if (NULL == list || NULL == size) {
		return NULL;
	}
	if (position >= list->length_) {
		return NULL;
	}

	if (0 == position) {
		return DLLIST_extractFirst(list, size);
	}
	if (position == list->length_ - 1) {
		return DLLIST_extractLast(list, size);
	}

	MemoryNode* target;
	if (position <= list->length_ / 2) {
		target = list->head_;
		for (u16 i = 0; i < position; ++i) {
			target = target->next_;
		}
	}
	else {
		target = list->tail_;
		for (u16 i = list->length_ - 1; i > position; --i) {
			target = target->prev_;
		}
	}

	void* extracted_data = target->data_;
	*size = target->size_;

	target->prev_->next_ = target->next_;
	target->next_->prev_ = target->prev_;
	list->length_--;

	target->ops_->softFree(target);
	return extracted_data;
}


s16 DLLIST_concat(DLList* list, DLList* list_src){
	if (NULL == list || NULL == list_src) {
		return kErrorCode_Null;
	}

	u16 total_after = list->length_ + list_src->length_;
	if (total_after > list->capacity_) {
		s16 expansion = DLLIST_resize(list, list->capacity_ + list_src->capacity_);
		if (kErrorCode_Ok != expansion) {
			return expansion;
		}
	}

	MemoryNode* source_runner = list_src->head_;
	while (NULL != source_runner) {
		MemoryNode* new_node = MEMNODE_create();
		if (NULL == new_node) {
			return kErrorCode_Memory;
		}

		s16 copy_result = new_node->ops_->memCopy(new_node,
			source_runner->data_,
			source_runner->size_);
		if (kErrorCode_Ok != copy_result) {
			new_node->ops_->softFree(new_node);
			return copy_result;
		}

		if (NULL == list->tail_) {
			list->head_ = new_node;
			list->tail_ = new_node;
		}
		else {
			new_node->prev_ = list->tail_;
			list->tail_->next_ = new_node;
			list->tail_ = new_node;
		}
		list->length_++;

		source_runner = source_runner->next_;
	}

	return kErrorCode_Ok;
}

s16 DLLIST_traverse(DLList* list, void (*callback)(MemoryNode*)){
	if (NULL == list) {
		return kErrorCode_Null;
	}
	if (NULL == callback) {
		return kErrorCode_Null;
	}

	MemoryNode* current = list->head_;
	while (NULL != current) {
		callback(current);
		current = current->next_;
	}

	return kErrorCode_Ok;
}


void DLLIST_print(DLList* list){
	if (NULL == list) {
		printf("[DLList Info] Null DLList\n");
		return;
	}

	printf("[DLList Info] Address: %p\n", (void*)list);
	printf("[DLList Info] Length: %d\n", list->length_);
	printf("[DLList Info] Capacity: %d\n", list->capacity_);
	printf("[DLList Info] Head: %p\n", (void*)list->head_);
	printf("[DLList Info] Tail: %p\n", (void*)list->tail_);

	MemoryNode* current = list->head_;
	u16 idx = 0;
	while (NULL != current) {
		printf("    [DLList Info] Node #%d\n", idx);
		printf("        [Node Info] Address: %p\n", (void*)current);
		printf("        [Node Info] Size: %d\n", current->size_);
		printf("        [Node Info] Prev: %p\n", (void*)current->prev_);
		printf("        [Node Info] Next: %p\n", (void*)current->next_);

		if (NULL == current->data_) {
			printf("        [Node Info] Data Address: NULL\n");
		}
		else {
			printf("        [Node Info] Data Address: %p\n", (void*)current->data_);
			printf("        [Node Info] Data Content: ");
			u8* byte_data = (u8*)current->data_;
			for (u16 j = 0; j < current->size_; j++) {
				printf("%c", byte_data[j]);
			}
			printf("\n");
		}

		current = current->next_;
		idx++;
	}
}
