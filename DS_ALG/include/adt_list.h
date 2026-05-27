/*
 *
 * adt_list.h
 * Toni Barella, Ivan Sancho as Unreal Authorized Instructor (UAI).
 * Algorithms and Data Structures.
 * ESAT 2020-2021
 *
 */

#ifndef __ADT_LIST_H__
#define __ADT_LIST_H__

#include "adt_memory_node.h"

/**
 * @brief Singly linked list ADT with bounded capacity.
 * @details Each element is held by a dynamically allocated MemoryNode
 *          linked through @p next_. The list keeps direct references to
 *          its first (@p head_) and last (@p tail_) nodes so that
 *          insertions and extractions at the ends are O(1).
 */
typedef struct adt_list_s {
	MemoryNode *head_;                /**< First node, NULL when empty. */
	MemoryNode *tail_;                /**< Last node, NULL when empty. */
	u16 length_;                      /**< Number of stored elements. */
	u16 capacity_;                    /**< Maximum number of elements. */
	struct list_ops_s *ops_;          /**< Pointer to the List API. */
} List;

/**
 * @brief Public API for the List ADT.
 */
struct list_ops_s {
	/**
	 * @brief Releases every node of the list and frees the list itself.
	 * @param list The list to destroy.
	 * @return kErrorCode_Null if @p list is NULL,
	 *         kErrorCode_Ok otherwise.
	 */
	s16 (*destroy)(List* list);

	/**
	 * @brief Empties the list freeing the nodes but not their data buffers.
	 * @param list The list to soft reset.
	 * @return kErrorCode_Null if @p list is NULL,
	 *         kErrorCode_Ok otherwise.
	 */
	s16 (*softReset)(List* list);

	/**
	 * @brief Empties the list freeing both nodes and data buffers.
	 * @param list The list to reset.
	 * @return kErrorCode_Null if @p list is NULL,
	 *         kErrorCode_Ok otherwise.
	 */
	s16 (*reset)(List* list);

	/**
	 * @brief Changes the maximum capacity of the list.
	 * @details When @p new_size is smaller than the current length the
	 *          excess nodes are released starting from the tail.
	 * @param list The list to resize.
	 * @param new_size New maximum capacity.
	 * @return kErrorCode_Null if @p list is NULL,
	 *         kErrorCode_ZeroSize if @p new_size is 0,
	 *         kErrorCode_Ok otherwise.
	 */
	s16 (*resize)(List* list, u16 new_size);

	/**
	 * @brief Returns the maximum capacity of the list.
	 * @param list The list to query.
	 * @return Capacity, or 0 if @p list is NULL.
	 */
	u16 (*capacity)(List* list);

	/**
	 * @brief Returns the current number of stored elements.
	 * @param list The list to query.
	 * @return Length, or 0 if @p list is NULL.
	 */
	u16 (*length)(List* list);

	/**
	 * @brief Tells whether the list is empty.
	 * @param list The list to query.
	 * @return True if empty or @p list is NULL, False otherwise.
	 */
	boolean (*isEmpty)(List* list);

	/**
	 * @brief Tells whether the list has reached its capacity.
	 * @param list The list to query.
	 * @return True if full, False otherwise or if @p list is NULL.
	 */
	boolean (*isFull)(List* list);

	/**
	 * @brief Returns a reference to the data stored at the head.
	 * @param list The list to query.
	 * @param size Output parameter that receives the size of the element.
	 * @return Pointer to the data, or NULL on invalid input.
	 */
	void* (*first)(List* list, u16* size);

	/**
	 * @brief Returns a reference to the data stored at the tail.
	 * @param list The list to query.
	 * @param size Output parameter that receives the size of the element.
	 * @return Pointer to the data, or NULL on invalid input.
	 */
	void* (*last)(List* list, u16* size);

	/**
	 * @brief Returns a reference to the data stored at @p position.
	 * @param list The list to query.
	 * @param size Output parameter that receives the size of the element.
	 * @param position Zero based index of the element.
	 * @return Pointer to the data, or NULL on invalid input.
	 */
	void* (*at)(List* list, u16* size, u16 position);

	/**
	 * @brief Inserts an element at the head of the list.
	 * @param list The list to modify.
	 * @param data Pointer to the data to insert.
	 * @param bytes Size in bytes of @p data.
	 * @return kErrorCode_Null if @p list is NULL,
	 *         kErrorCode_NullData if @p data is NULL,
	 *         kErrorCode_ZeroSize if @p bytes is 0,
	 *         kErrorCode_IsFull if the list is full,
	 *         kErrorCode_Memory on allocation failure,
	 *         kErrorCode_Ok otherwise.
	 */
	s16 (*insertFirst)(List* list, void *data, u16 bytes);

	/**
	 * @brief Inserts an element at the tail of the list.
	 * @param list The list to modify.
	 * @param data Pointer to the data to insert.
	 * @param bytes Size in bytes of @p data.
	 * @return kErrorCode_Null if @p list is NULL,
	 *         kErrorCode_NullData if @p data is NULL,
	 *         kErrorCode_ZeroSize if @p bytes is 0,
	 *         kErrorCode_IsFull if the list is full,
	 *         kErrorCode_Memory on allocation failure,
	 *         kErrorCode_Ok otherwise.
	 */
	s16 (*insertLast)(List* list, void *data, u16 bytes);

	/**
	 * @brief Inserts an element at a given position.
	 * @details Positions greater than the current length are clamped so the
	 *          element is appended to the tail.
	 * @param list The list to modify.
	 * @param data Pointer to the data to insert.
	 * @param bytes Size in bytes of @p data.
	 * @param position Zero based target index.
	 * @return kErrorCode_Null if @p list is NULL,
	 *         kErrorCode_NullData if @p data is NULL,
	 *         kErrorCode_ZeroSize if @p bytes is 0,
	 *         kErrorCode_IsFull if the list is full,
	 *         kErrorCode_Memory on allocation failure,
	 *         kErrorCode_Ok otherwise.
	 */
	s16 (*insertAt)(List* list, void *data, u16 bytes, u16 position);

	/**
	 * @brief Extracts and removes the first element of the list.
	 * @param list The list to modify.
	 * @param size Output parameter that receives the size of the element.
	 * @return Pointer to the extracted data, or NULL on invalid input.
	 */
	void* (*extractFirst)(List* list, u16* size);

	/**
	 * @brief Extracts and removes the last element of the list.
	 * @param list The list to modify.
	 * @param size Output parameter that receives the size of the element.
	 * @return Pointer to the extracted data, or NULL on invalid input.
	 */
	void* (*extractLast)(List* list, u16* size);

	/**
	 * @brief Extracts and removes the element at @p position.
	 * @param list The list to modify.
	 * @param size Output parameter that receives the size of the element.
	 * @param position Zero based index of the element to remove.
	 * @return Pointer to the extracted data, or NULL on invalid input.
	 */
	void* (*extractAt)(List* list, u16* size, u16 position);

	/**
	 * @brief Concatenates @p list_src at the tail of @p list.
	 * @details Data of @p list_src is deep copied so both lists remain
	 *          independent. The destination capacity is grown to fit.
	 * @param list Destination list.
	 * @param list_src Source list.
	 * @return kErrorCode_Null if any list is NULL,
	 *         kErrorCode_Memory on allocation failure,
	 *         kErrorCode_Ok otherwise.
	 */
	s16 (*concat)(List* list, List *list_src);

	/**
	 * @brief Calls @p callback for every stored element.
	 * @param list The list to traverse.
	 * @param callback Function applied to each MemoryNode.
	 * @return kErrorCode_Null if @p list or @p callback are NULL,
	 *         kErrorCode_Ok otherwise.
	 */
	s16 (*traverse)(List* list, void (*callback)(MemoryNode *));

	/**
	 * @brief Prints the content of the list to standard output.
	 * @param list The list to print.
	 */
	void (*print)(List* list);
};

/**
 * @brief Creates a new list with the specified capacity.
 * @param capacity Maximum number of elements the list will hold.
 * @return Pointer to the new list, or NULL on invalid input or failure.
 */
List* LIST_create(u16 capacity);

#endif //__ADT_LIST_H__
