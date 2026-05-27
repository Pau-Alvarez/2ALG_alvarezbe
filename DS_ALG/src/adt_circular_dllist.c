/*
 * Ivan Sancho as Unreal Authorized Instructor (UAI), 2025.
 *
 * Circular Double Linked List source code.
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
#include "adt_circular_dllist.h"

#include "EDK_MemoryManager/edk_memory_manager.h"

//Circular DLList forward declarations

static s16 CIRCULARDLLIST_destroy(CircularDLList* list);
static s16 CIRCULARDLLIST_softReset(CircularDLList* list);
static s16 CIRCULARDLLIST_reset(CircularDLList* list);
static s16 CIRCULARDLLIST_resize(CircularDLList* list, u16 new_size);

static u16 CIRCULARDLLIST_capacity(CircularDLList* list);
static u16 CIRCULARDLLIST_length(CircularDLList* list);
static boolean CIRCULARDLLIST_isEmpty(CircularDLList* list);
static boolean CIRCULARDLLIST_isFull(CircularDLList* list);

static void* CIRCULARDLLIST_first(CircularDLList* list, u16* size);
static void* CIRCULARDLLIST_last(CircularDLList* list, u16* size);
static void* CIRCULARDLLIST_at(CircularDLList* list, u16* size, u16 position);

static s16 CIRCULARDLLIST_insertFirst(CircularDLList* list, void* data, u16 bytes);
static s16 CIRCULARDLLIST_insertLast(CircularDLList* list, void* data, u16 bytes);
static s16 CIRCULARDLLIST_insertAt(CircularDLList* list, void* data, u16 bytes, u16 position);

static void* CIRCULARDLLIST_extractFirst(CircularDLList* list, u16* size);
static void* CIRCULARDLLIST_extractLast(CircularDLList* list, u16* size);
static void* CIRCULARDLLIST_extractAt(CircularDLList* list, u16* size, u16 position);

static s16 CIRCULARDLLIST_concat(CircularDLList* list, CircularDLList* list_src);
static s16 CIRCULARDLLIST_traverse(CircularDLList* list, void (*callback)(MemoryNode*));

static void CIRCULARDLLIST_print(CircularDLList* list);


struct circular_dllist_ops_s circular_dllist_ops = {	.destroy = CIRCULARDLLIST_destroy,
														.softReset = CIRCULARDLLIST_softReset,
														.reset = CIRCULARDLLIST_reset,
														.resize = CIRCULARDLLIST_resize,
														.capacity = CIRCULARDLLIST_capacity,
														.length = CIRCULARDLLIST_length,
														.isEmpty = CIRCULARDLLIST_isEmpty,
														.isFull = CIRCULARDLLIST_isFull,
														.first = CIRCULARDLLIST_first,
														.last = CIRCULARDLLIST_last,
														.at = CIRCULARDLLIST_at,
														.insertFirst = CIRCULARDLLIST_insertFirst,
														.insertLast = CIRCULARDLLIST_insertLast,
														.insertAt = CIRCULARDLLIST_insertAt,
														.extractFirst = CIRCULARDLLIST_extractFirst,
														.extractLast = CIRCULARDLLIST_extractLast,
														.extractAt = CIRCULARDLLIST_extractAt,
														.concat = CIRCULARDLLIST_concat,
														.traverse = CIRCULARDLLIST_traverse,
														.print = CIRCULARDLLIST_print,
													};


CircularDLList* CIRCULARDLLIST_create(u16 capacity){
	if (0 == capacity) {
		return NULL;
	}

	CircularDLList* new_list = (CircularDLList*)MM->malloc(sizeof(CircularDLList));
	if (NULL == new_list) {
		return NULL;
	}

	new_list->head_ = NULL;
	new_list->tail_ = NULL;
	new_list->length_ = 0;
	new_list->capacity_ = capacity;
	new_list->ops_ = &circular_dllist_ops;

	return new_list;
}

static void CIRCULARDLLIST_releaseNodes(CircularDLList* list, boolean release_data) {
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

s16 CIRCULARDLLIST_destroy(CircularDLList* list) {
	if (NULL == list) {
		return kErrorCode_Null;
	}

	if (NULL != list->head_) {
		CIRCULARDLLIST_releaseNodes(list, True);
	}

	list->capacity_ = 0;
	list->ops_ = NULL;

	MM->free(list);
	return kErrorCode_Ok;
}

s16 CIRCULARDLLIST_softReset(CircularDLList* list){
	if (NULL == list) {
		return kErrorCode_Null;
	}
	if (NULL == list->head_) {
		return kErrorCode_Ok;
	}
	CIRCULARDLLIST_releaseNodes(list, False);
	return kErrorCode_Ok;
}

s16 CIRCULARDLLIST_reset(CircularDLList* list){
	if (NULL == list) {
		return kErrorCode_Null;
	}
	if (NULL == list->head_) {
		return kErrorCode_Ok;
	}
	CIRCULARDLLIST_releaseNodes(list, True);
	return kErrorCode_Ok;
}

s16 CIRCULARDLLIST_resize(CircularDLList* list, u16 new_size){
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
		list->head_->prev_ = current;
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


u16 CIRCULARDLLIST_capacity(CircularDLList* list){
	return (NULL != list) ? list->capacity_ : 0;
}

u16 CIRCULARDLLIST_length(CircularDLList* list){
	return (NULL != list) ? list->length_ : 0;
}

boolean CIRCULARDLLIST_isEmpty(CircularDLList* list){
	if (NULL == list) {
		return False;
	}
	return (0 == list->length_) ? True : False;
}

boolean CIRCULARDLLIST_isFull(CircularDLList* list){
	if (NULL == list) {
		return False;
	}
	return (list->length_ == list->capacity_) ? True : False;
}


void* CIRCULARDLLIST_first(CircularDLList* list, u16* size){
	if (NULL == list || NULL == size) {
		return NULL;
	}
	if (NULL == list->head_) {
		return NULL;
	}
	*size = list->head_->size_;
	return list->head_->data_;
}

void* CIRCULARDLLIST_last(CircularDLList* list, u16* size){
	if (NULL == list || NULL == size) {
		return NULL;
	}
	if (NULL == list->tail_) {
		return NULL;
	}
	*size = list->tail_->size_;
	return list->tail_->data_;
}

void* CIRCULARDLLIST_at(CircularDLList* list, u16* size, u16 position){
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


s16 CIRCULARDLLIST_insertFirst(CircularDLList* list, void* data, u16 bytes){
	if (NULL == list) {
		return kErrorCode_Null;
	}
	if (NULL == data) {
		return kErrorCode_NullData;
	}
	if (0 == bytes) {
		return kErrorCode_ZeroSize;
	}
	if (CIRCULARDLLIST_isFull(list)) {
		return kErrorCode_IsFull;
	}

	MemoryNode* new_node = MEMNODE_create();
	if (NULL == new_node) {
		return kErrorCode_Memory;
	}
	new_node->ops_->setData(new_node, data, bytes);

	if (NULL == list->head_) {
		new_node->next_ = new_node;
		new_node->prev_ = new_node;
		list->head_ = new_node;
		list->tail_ = new_node;
	}
	else {
		new_node->next_ = list->head_;
		new_node->prev_ = list->tail_;
		list->head_->prev_ = new_node;
		list->tail_->next_ = new_node;
		list->head_ = new_node;
	}
	list->length_++;
	return kErrorCode_Ok;
}

s16 CIRCULARDLLIST_insertLast(CircularDLList* list, void* data, u16 bytes){
	if (NULL == list) {
		return kErrorCode_Null;
	}
	if (NULL == data) {
		return kErrorCode_NullData;
	}
	if (0 == bytes) {
		return kErrorCode_ZeroSize;
	}
	if (CIRCULARDLLIST_isFull(list)) {
		return kErrorCode_IsFull;
	}

	MemoryNode* new_node = MEMNODE_create();
	if (NULL == new_node) {
		return kErrorCode_Memory;
	}
	new_node->ops_->setData(new_node, data, bytes);

	if (NULL == list->tail_) {
		new_node->next_ = new_node;
		new_node->prev_ = new_node;
		list->head_ = new_node;
		list->tail_ = new_node;
	}
	else {
		new_node->next_ = list->head_;
		new_node->prev_ = list->tail_;
		list->tail_->next_ = new_node;
		list->head_->prev_ = new_node;
		list->tail_ = new_node;
	}
	list->length_++;
	return kErrorCode_Ok;
}

s16 CIRCULARDLLIST_insertAt(CircularDLList* list, void* data, u16 bytes, u16 position){
	if (NULL == list) {
		return kErrorCode_Null;
	}
	if (NULL == data) {
		return kErrorCode_NullData;
	}
	if (0 == bytes) {
		return kErrorCode_ZeroSize;
	}
	if (CIRCULARDLLIST_isFull(list)) {
		return kErrorCode_IsFull;
	}

	if (0 == position) {
		return CIRCULARDLLIST_insertFirst(list, data, bytes);
	}
	if (position >= list->length_) {
		return CIRCULARDLLIST_insertLast(list, data, bytes);
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


void* CIRCULARDLLIST_extractFirst(CircularDLList* list, u16* size){
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
		list->head_->prev_ = list->tail_;
		list->tail_->next_ = list->head_;
	}
	list->length_--;

	old_head->ops_->softFree(old_head);
	return extracted_data;
}

void* CIRCULARDLLIST_extractLast(CircularDLList* list, u16* size){
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
		list->tail_ = old_tail->prev_;
		list->tail_->next_ = list->head_;
		list->head_->prev_ = list->tail_;
	}
	list->length_--;

	old_tail->ops_->softFree(old_tail);
	return extracted_data;
}

void* CIRCULARDLLIST_extractAt(CircularDLList* list, u16* size, u16 position){
	if (NULL == list || NULL == size) {
		return NULL;
	}
	if (position >= list->length_) {
		return NULL;
	}

	if (0 == position) {
		return CIRCULARDLLIST_extractFirst(list, size);
	}
	if (position == list->length_ - 1) {
		return CIRCULARDLLIST_extractLast(list, size);
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


s16 CIRCULARDLLIST_concat(CircularDLList* list, CircularDLList* list_src){
	if (NULL == list || NULL == list_src) {
		return kErrorCode_Null;
	}

	u16 total_after = list->length_ + list_src->length_;
	if (total_after > list->capacity_) {
		s16 expansion = CIRCULARDLLIST_resize(list, list->capacity_ + list_src->capacity_);
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
			new_node->prev_ = new_node;
			list->head_ = new_node;
			list->tail_ = new_node;
		}
		else {
			new_node->next_ = list->head_;
			new_node->prev_ = list->tail_;
			list->tail_->next_ = new_node;
			list->head_->prev_ = new_node;
			list->tail_ = new_node;
		}
		list->length_++;

		source_runner = source_runner->next_;
	}

	return kErrorCode_Ok;
}

s16 CIRCULARDLLIST_traverse(CircularDLList* list, void (*callback)(MemoryNode*)){
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


void CIRCULARDLLIST_print(CircularDLList* list){
	if (NULL == list) {
		printf("[CircularDLList Info] Null List\n");
		return;
	}

	printf("[CircularDLList Info] Address: %p\n", (void*)list);
	printf("[CircularDLList Info] Length: %d\n", list->length_);
	printf("[CircularDLList Info] Capacity: %d\n", list->capacity_);
	printf("[CircularDLList Info] Head: %p\n", (void*)list->head_);
	printf("[CircularDLList Info] Tail: %p\n", (void*)list->tail_);

	MemoryNode* current = list->head_;
	for (u16 i = 0; i < list->length_; ++i) {
		printf("    [CircularDLList Info] Node #%d\n", i);
		printf("        [Node Info] Address: %p\n", (void*)current);
		printf("        [Node Info] Size: %d\n", current->size_);
		printf("        [Node Info] Prev: %p\n", (void*)current->prev_);
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
