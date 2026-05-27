/*
 * Ivan Sancho as Unreal Authorized Instructor (UAI), 2025.
 *
 * Circular List source code.
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
#include "adt_circular_list.h"

#include "EDK_MemoryManager/edk_memory_manager.h"

//Circular List forward declarations

static s16 CIRCULARLIST_destroy(CircularList* list);
static s16 CIRCULARLIST_softReset(CircularList* list);
static s16 CIRCULARLIST_reset(CircularList* list);
static s16 CIRCULARLIST_resize(CircularList* list, u16 new_size);

static u16 CIRCULARLIST_capacity(CircularList* list);
static u16 CIRCULARLIST_length(CircularList* list);
static boolean CIRCULARLIST_isEmpty(CircularList* list);
static boolean CIRCULARLIST_isFull(CircularList* list);

static void* CIRCULARLIST_first(CircularList* list, u16* size);
static void* CIRCULARLIST_last(CircularList* list, u16* size);
static void* CIRCULARLIST_at(CircularList* list, u16* size, u16 position);

static s16 CIRCULARLIST_insertFirst(CircularList* list, void* data, u16 bytes);
static s16 CIRCULARLIST_insertLast(CircularList* list, void* data, u16 bytes);
static s16 CIRCULARLIST_insertAt(CircularList* list, void* data, u16 bytes, u16 position);

static void* CIRCULARLIST_extractFirst(CircularList* list, u16* size);
static void* CIRCULARLIST_extractLast(CircularList* list, u16* size);
static void* CIRCULARLIST_extractAt(CircularList* list, u16* size, u16 position);

static s16 CIRCULARLIST_concat(CircularList* list, CircularList* list_src);
static s16 CIRCULARLIST_traverse(CircularList* list, void (*callback)(MemoryNode*));

static void CIRCULARLIST_print(CircularList* list);


struct circular_list_ops_s circular_list_ops = {	.destroy = CIRCULARLIST_destroy,
													.softReset = CIRCULARLIST_softReset,
													.reset = CIRCULARLIST_reset,
													.resize = CIRCULARLIST_resize,
													.capacity = CIRCULARLIST_capacity,
													.length = CIRCULARLIST_length,
													.isEmpty = CIRCULARLIST_isEmpty,
													.isFull = CIRCULARLIST_isFull,
													.first = CIRCULARLIST_first,
													.last = CIRCULARLIST_last,
													.at = CIRCULARLIST_at,
													.insertFirst = CIRCULARLIST_insertFirst,
													.insertLast = CIRCULARLIST_insertLast,
													.insertAt = CIRCULARLIST_insertAt,
													.extractFirst = CIRCULARLIST_extractFirst,
													.extractLast = CIRCULARLIST_extractLast,
													.extractAt = CIRCULARLIST_extractAt,
													.concat = CIRCULARLIST_concat,
													.traverse = CIRCULARLIST_traverse,
													.print = CIRCULARLIST_print,
												};


CircularList* CIRCULARLIST_create(u16 capacity){
	if (0 == capacity) {
		return NULL;
	}

	CircularList* new_list = (CircularList*)MM->malloc(sizeof(CircularList));
	if (NULL == new_list) {
		return NULL;
	}

	new_list->head_ = NULL;
	new_list->tail_ = NULL;
	new_list->length_ = 0;
	new_list->capacity_ = capacity;
	new_list->ops_ = &circular_list_ops;

	return new_list;
}

static void CIRCULARLIST_releaseNodes(CircularList* list, boolean release_data) {
	MemoryNode* current = list->head_;
	for (u16 i = 0; i < list->length_; ++i) {
		MemoryNode* next = current->next_;
		if (release_data) {
			current->ops_->free(current);
		}
		else {
			current->ops_->softFree(current);
		}
		current = next;
	}
	list->head_ = NULL;
	list->tail_ = NULL;
	list->length_ = 0;
}

s16 CIRCULARLIST_destroy(CircularList* list) {
	if (NULL == list) {
		return kErrorCode_Null;
	}

	if (NULL != list->head_) {
		CIRCULARLIST_releaseNodes(list, True);
	}

	list->capacity_ = 0;
	list->ops_ = NULL;

	MM->free(list);
	return kErrorCode_Ok;
}

s16 CIRCULARLIST_softReset(CircularList* list){
	if (NULL == list) {
		return kErrorCode_Null;
	}
	if (NULL == list->head_) {
		return kErrorCode_Ok;
	}
	CIRCULARLIST_releaseNodes(list, False);
	return kErrorCode_Ok;
}

s16 CIRCULARLIST_reset(CircularList* list){
	if (NULL == list) {
		return kErrorCode_Null;
	}
	if (NULL == list->head_) {
		return kErrorCode_Ok;
	}
	CIRCULARLIST_releaseNodes(list, True);
	return kErrorCode_Ok;
}

s16 CIRCULARLIST_resize(CircularList* list, u16 new_size){
	if (NULL == list) {
		return kErrorCode_Null;
	}
	if (0 == new_size) {
		return kErrorCode_ZeroSize;
	}

	if (new_size < list->length_) {
		MemoryNode* current = list->head_;
		for (u16 i = 0; i < new_size - 1; ++i) {
			current = current->next_;
		}

		MemoryNode* discard = current->next_;
		u16 discard_count = list->length_ - new_size;
		current->next_ = list->head_;
		list->tail_ = current;

		for (u16 i = 0; i < discard_count; ++i) {
			MemoryNode* next = discard->next_;
			discard->ops_->free(discard);
			discard = next;
		}
		list->length_ = new_size;
	}

	list->capacity_ = new_size;
	return kErrorCode_Ok;
}


u16 CIRCULARLIST_capacity(CircularList* list){
	return (NULL != list) ? list->capacity_ : 0;
}

u16 CIRCULARLIST_length(CircularList* list){
	return (NULL != list) ? list->length_ : 0;
}

boolean CIRCULARLIST_isEmpty(CircularList* list){
	if (NULL == list) {
		return False;
	}
	return (0 == list->length_) ? True : False;
}

boolean CIRCULARLIST_isFull(CircularList* list){
	if (NULL == list) {
		return False;
	}
	return (list->length_ == list->capacity_) ? True : False;
}


void* CIRCULARLIST_first(CircularList* list, u16* size){
	if (NULL == list || NULL == size) {
		return NULL;
	}
	if (NULL == list->head_) {
		return NULL;
	}
	*size = list->head_->size_;
	return list->head_->data_;
}

void* CIRCULARLIST_last(CircularList* list, u16* size){
	if (NULL == list || NULL == size) {
		return NULL;
	}
	if (NULL == list->tail_) {
		return NULL;
	}
	*size = list->tail_->size_;
	return list->tail_->data_;
}

void* CIRCULARLIST_at(CircularList* list, u16* size, u16 position){
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


s16 CIRCULARLIST_insertFirst(CircularList* list, void* data, u16 bytes){
	if (NULL == list) {
		return kErrorCode_Null;
	}
	if (NULL == data) {
		return kErrorCode_NullData;
	}
	if (0 == bytes) {
		return kErrorCode_ZeroSize;
	}
	if (CIRCULARLIST_isFull(list)) {
		return kErrorCode_IsFull;
	}

	MemoryNode* new_node = MEMNODE_create();
	if (NULL == new_node) {
		return kErrorCode_Memory;
	}
	new_node->ops_->setData(new_node, data, bytes);

	if (NULL == list->head_) {
		new_node->next_ = new_node;
		list->head_ = new_node;
		list->tail_ = new_node;
	}
	else {
		new_node->next_ = list->head_;
		list->tail_->next_ = new_node;
		list->head_ = new_node;
	}
	list->length_++;
	return kErrorCode_Ok;
}

s16 CIRCULARLIST_insertLast(CircularList* list, void* data, u16 bytes){
	if (NULL == list) {
		return kErrorCode_Null;
	}
	if (NULL == data) {
		return kErrorCode_NullData;
	}
	if (0 == bytes) {
		return kErrorCode_ZeroSize;
	}
	if (CIRCULARLIST_isFull(list)) {
		return kErrorCode_IsFull;
	}

	MemoryNode* new_node = MEMNODE_create();
	if (NULL == new_node) {
		return kErrorCode_Memory;
	}
	new_node->ops_->setData(new_node, data, bytes);

	if (NULL == list->tail_) {
		new_node->next_ = new_node;
		list->head_ = new_node;
		list->tail_ = new_node;
	}
	else {
		new_node->next_ = list->head_;
		list->tail_->next_ = new_node;
		list->tail_ = new_node;
	}
	list->length_++;
	return kErrorCode_Ok;
}

s16 CIRCULARLIST_insertAt(CircularList* list, void* data, u16 bytes, u16 position){
	if (NULL == list) {
		return kErrorCode_Null;
	}
	if (NULL == data) {
		return kErrorCode_NullData;
	}
	if (0 == bytes) {
		return kErrorCode_ZeroSize;
	}
	if (CIRCULARLIST_isFull(list)) {
		return kErrorCode_IsFull;
	}

	if (0 == position) {
		return CIRCULARLIST_insertFirst(list, data, bytes);
	}
	if (position >= list->length_) {
		return CIRCULARLIST_insertLast(list, data, bytes);
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


void* CIRCULARLIST_extractFirst(CircularList* list, u16* size){
	if (NULL == list || NULL == size) {
		return NULL;
	}
	if (NULL == list->head_) {
		return NULL;
	}

	MemoryNode* old_head = list->head_;
	void* extracted_data = old_head->data_;
	*size = old_head->size_;

	if (1 == list->length_) {
		list->head_ = NULL;
		list->tail_ = NULL;
	}
	else {
		list->head_ = old_head->next_;
		list->tail_->next_ = list->head_;
	}
	list->length_--;

	old_head->ops_->softFree(old_head);
	return extracted_data;
}

void* CIRCULARLIST_extractLast(CircularList* list, u16* size){
	if (NULL == list || NULL == size) {
		return NULL;
	}
	if (NULL == list->tail_) {
		return NULL;
	}

	MemoryNode* old_tail = list->tail_;
	void* extracted_data = old_tail->data_;
	*size = old_tail->size_;

	if (1 == list->length_) {
		list->head_ = NULL;
		list->tail_ = NULL;
	}
	else {
		MemoryNode* runner = list->head_;
		while (runner->next_ != old_tail) {
			runner = runner->next_;
		}
		runner->next_ = list->head_;
		list->tail_ = runner;
	}
	list->length_--;

	old_tail->ops_->softFree(old_tail);
	return extracted_data;
}

void* CIRCULARLIST_extractAt(CircularList* list, u16* size, u16 position){
	if (NULL == list || NULL == size) {
		return NULL;
	}
	if (position >= list->length_) {
		return NULL;
	}

	if (0 == position) {
		return CIRCULARLIST_extractFirst(list, size);
	}
	if (position == list->length_ - 1) {
		return CIRCULARLIST_extractLast(list, size);
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


s16 CIRCULARLIST_concat(CircularList* list, CircularList* list_src){
	if (NULL == list || NULL == list_src) {
		return kErrorCode_Null;
	}

	u16 total_after = list->length_ + list_src->length_;
	if (total_after > list->capacity_) {
		s16 expansion = CIRCULARLIST_resize(list, list->capacity_ + list_src->capacity_);
		if (kErrorCode_Ok != expansion) {
			return expansion;
		}
	}

	MemoryNode* source_runner = list_src->head_;
	for (u16 i = 0; i < list_src->length_; ++i) {
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
			new_node->next_ = new_node;
			list->head_ = new_node;
			list->tail_ = new_node;
		}
		else {
			new_node->next_ = list->head_;
			list->tail_->next_ = new_node;
			list->tail_ = new_node;
		}
		list->length_++;

		source_runner = source_runner->next_;
	}

	return kErrorCode_Ok;
}

s16 CIRCULARLIST_traverse(CircularList* list, void (*callback)(MemoryNode*)){
	if (NULL == list) {
		return kErrorCode_Null;
	}
	if (NULL == callback) {
		return kErrorCode_Null;
	}

	MemoryNode* current = list->head_;
	for (u16 i = 0; i < list->length_; ++i) {
		callback(current);
		current = current->next_;
	}
	return kErrorCode_Ok;
}


void CIRCULARLIST_print(CircularList* list){
	if (NULL == list) {
		printf("[CircularList Info] Null List\n");
		return;
	}

	printf("[CircularList Info] Address: %p\n", (void*)list);
	printf("[CircularList Info] Length: %d\n", list->length_);
	printf("[CircularList Info] Capacity: %d\n", list->capacity_);
	printf("[CircularList Info] Head: %p\n", (void*)list->head_);
	printf("[CircularList Info] Tail: %p\n", (void*)list->tail_);

	MemoryNode* current = list->head_;
	for (u16 i = 0; i < list->length_; ++i) {
		printf("    [CircularList Info] Node #%d\n", i);
		printf("        [Node Info] Address: %p\n", (void*)current);
		printf("        [Node Info] Size: %d\n", current->size_);
		printf("        [Node Info] Next: %p\n", (void*)current->next_);

		if (NULL == current->data_) {
			printf("        [Node Info] Data: NULL\n");
		}
		else {
			printf("        [Node Info] Data Content: ");
			u8* byte_data = (u8*)current->data_;
			for (u16 j = 0; j < current->size_; j++) {
				printf("%c", byte_data[j]);
			}
			printf("\n");
		}

		current = current->next_;
	}
}
