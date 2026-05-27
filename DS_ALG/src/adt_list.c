/*
 * Ivan Sancho as Unreal Authorized Instructor (UAI), 2025.
 *
 * List source code.
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
#include "adt_list.h"

#include "EDK_MemoryManager/edk_memory_manager.h"

//List forward declarations

static s16 LIST_destroy(List* list);
static s16 LIST_softReset(List* list);
static s16 LIST_reset(List* list);
static s16 LIST_resize(List* list, u16 new_size);

static u16 LIST_capacity(List* list);
static u16 LIST_length(List* list);
static boolean LIST_isEmpty(List* list);
static boolean LIST_isFull(List* list);

static void* LIST_first(List* list, u16* size);
static void* LIST_last(List* list, u16* size);
static void* LIST_at(List* list, u16* size, u16 position);

static s16 LIST_insertFirst(List* list, void* data, u16 bytes);
static s16 LIST_insertLast(List* list, void* data, u16 bytes);
static s16 LIST_insertAt(List* list, void* data, u16 bytes, u16 position);

static void* LIST_extractFirst(List* list, u16* size);
static void* LIST_extractLast(List* list, u16* size);
static void* LIST_extractAt(List* list, u16* size, u16 position);

static s16 LIST_concat(List* list, List* list_src);
static s16 LIST_traverse(List* list, void (*callback)(MemoryNode*));

static void LIST_print(List* list);


struct list_ops_s list_ops = {	.destroy = LIST_destroy,
								.softReset = LIST_softReset,
								.reset = LIST_reset,
								.resize = LIST_resize,
								.capacity = LIST_capacity,
								.length = LIST_length,
								.isEmpty = LIST_isEmpty,
								.isFull = LIST_isFull,
								.first = LIST_first,
								.last = LIST_last,
								.at = LIST_at,
								.insertFirst = LIST_insertFirst,
								.insertLast = LIST_insertLast,
								.insertAt = LIST_insertAt,
								.extractFirst = LIST_extractFirst,
								.extractLast = LIST_extractLast,
								.extractAt = LIST_extractAt,
								.concat = LIST_concat,
								.traverse = LIST_traverse,
								.print = LIST_print,
							};


List* LIST_create(u16 capacity){
	if (0 == capacity) {
		return NULL;
	}

	List* new_list = (List*)MM->malloc(sizeof(List));
	if (NULL == new_list) {
		return NULL;
	}

	new_list->head_ = NULL;
	new_list->tail_ = NULL;
	new_list->length_ = 0;
	new_list->capacity_ = capacity;
	new_list->ops_ = &list_ops;

	return new_list;
}

s16 LIST_destroy(List* list) {
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

s16 LIST_softReset(List* list){
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

s16 LIST_reset(List* list){
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

s16 LIST_resize(List* list, u16 new_size){
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


u16 LIST_capacity(List* list){
	return (NULL != list) ? list->capacity_ : 0;
}

u16 LIST_length(List* list){
	return (NULL != list) ? list->length_ : 0;
}

boolean LIST_isEmpty(List* list){
	if (NULL == list) {
		return False;
	}
	return (0 == list->length_) ? True : False;
}

boolean LIST_isFull(List* list){
	if (NULL == list) {
		return False;
	}
	return (list->length_ == list->capacity_) ? True : False;
}


void* LIST_first(List* list, u16* size){
	if (NULL == list || NULL == size) {
		return NULL;
	}
	if (NULL == list->head_) {
		return NULL;
	}

	*size = list->head_->size_;
	return list->head_->data_;
}

void* LIST_last(List* list, u16* size){
	if (NULL == list || NULL == size) {
		return NULL;
	}
	if (NULL == list->tail_) {
		return NULL;
	}

	*size = list->tail_->size_;
	return list->tail_->data_;
}

void* LIST_at(List* list, u16* size, u16 position){
	if (NULL == list || NULL == size) {
		return NULL;
	}
	if (position >= list->length_) {
		return NULL;
	}

	MemoryNode* current = list->head_;
	for (u16 i = 0; i < position; ++i) {
		current = current->next_;
	}

	*size = current->size_;
	return current->data_;
}


s16 LIST_insertFirst(List* list, void* data, u16 bytes){
	if (NULL == list) {
		return kErrorCode_Null;
	}
	if (NULL == data) {
		return kErrorCode_NullData;
	}
	if (0 == bytes) {
		return kErrorCode_ZeroSize;
	}
	if (LIST_isFull(list)) {
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
		list->head_ = new_node;
	}
	list->length_++;

	return kErrorCode_Ok;
}

s16 LIST_insertLast(List* list, void* data, u16 bytes){
	if (NULL == list) {
		return kErrorCode_Null;
	}
	if (NULL == data) {
		return kErrorCode_NullData;
	}
	if (0 == bytes) {
		return kErrorCode_ZeroSize;
	}
	if (LIST_isFull(list)) {
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
		list->tail_->next_ = new_node;
		list->tail_ = new_node;
	}
	list->length_++;

	return kErrorCode_Ok;
}

s16 LIST_insertAt(List* list, void* data, u16 bytes, u16 position){
	if (NULL == list) {
		return kErrorCode_Null;
	}
	if (NULL == data) {
		return kErrorCode_NullData;
	}
	if (0 == bytes) {
		return kErrorCode_ZeroSize;
	}
	if (LIST_isFull(list)) {
		return kErrorCode_IsFull;
	}

	if (0 == position) {
		return LIST_insertFirst(list, data, bytes);
	}
	if (position >= list->length_) {
		return LIST_insertLast(list, data, bytes);
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
	prev_node->next_ = new_node;
	list->length_++;

	return kErrorCode_Ok;
}


void* LIST_extractFirst(List* list, u16* size){
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
	list->length_--;

	old_head->ops_->softFree(old_head);
	return extracted_data;
}

void* LIST_extractLast(List* list, u16* size){
	if (NULL == list || NULL == size) {
		return NULL;
	}
	if (NULL == list->tail_) {
		return NULL;
	}

	MemoryNode* old_tail = list->tail_;
	void* extracted_data = old_tail->data_;
	*size = old_tail->size_;

	if (list->head_ == list->tail_) {
		list->head_ = NULL;
		list->tail_ = NULL;
	}
	else {
		MemoryNode* runner = list->head_;
		while (runner->next_ != old_tail) {
			runner = runner->next_;
		}
		runner->next_ = NULL;
		list->tail_ = runner;
	}
	list->length_--;

	old_tail->ops_->softFree(old_tail);
	return extracted_data;
}

void* LIST_extractAt(List* list, u16* size, u16 position){
	if (NULL == list || NULL == size) {
		return NULL;
	}
	if (position >= list->length_) {
		return NULL;
	}

	if (0 == position) {
		return LIST_extractFirst(list, size);
	}
	if (position == list->length_ - 1) {
		return LIST_extractLast(list, size);
	}

	MemoryNode* prev_node = list->head_;
	for (u16 i = 0; i < position - 1; ++i) {
		prev_node = prev_node->next_;
	}

	MemoryNode* target = prev_node->next_;
	void* extracted_data = target->data_;
	*size = target->size_;

	prev_node->next_ = target->next_;
	list->length_--;

	target->ops_->softFree(target);
	return extracted_data;
}


s16 LIST_concat(List* list, List* list_src){
	if (NULL == list || NULL == list_src) {
		return kErrorCode_Null;
	}

	u16 total_after = list->length_ + list_src->length_;
	if (total_after > list->capacity_) {
		s16 expansion = LIST_resize(list, list->capacity_ + list_src->capacity_);
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
			list->tail_->next_ = new_node;
			list->tail_ = new_node;
		}
		list->length_++;

		source_runner = source_runner->next_;
	}

	return kErrorCode_Ok;
}

s16 LIST_traverse(List* list, void (*callback)(MemoryNode*)){
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


void LIST_print(List* list){
	if (NULL == list) {
		printf("[List Info] Null List\n");
		return;
	}

	printf("[List Info] Address: %p\n", (void*)list);
	printf("[List Info] Length: %d\n", list->length_);
	printf("[List Info] Capacity: %d\n", list->capacity_);
	printf("[List Info] Head: %p\n", (void*)list->head_);
	printf("[List Info] Tail: %p\n", (void*)list->tail_);

	MemoryNode* current = list->head_;
	u16 idx = 0;
	while (NULL != current) {
		printf("    [List Info] Node #%d\n", idx);
		printf("        [Node Info] Address: %p\n", (void*)current);
		printf("        [Node Info] Size: %d\n", current->size_);

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
