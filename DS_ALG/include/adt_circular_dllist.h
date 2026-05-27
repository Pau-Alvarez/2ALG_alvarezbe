/*
 *
 * adt_circular_dllist.h
 * Toni Barella, Ivan Sancho as Unreal Authorized Instructor (UAI).
 * Algorithms and Data Structures.
 * ESAT 2020-2021
 *
 */

#ifndef __ADT_CIRCULAR_DLLIST_H__
#define __ADT_CIRCULAR_DLLIST_H__

#include "adt_memory_node.h"

/**
 * @brief Doubly linked circular list ADT with bounded capacity.
 * @details Every node has @p next_ and @p prev_ pointers. The tail's
 *          @p next_ references the head and the head's @p prev_ references
 *          the tail, so the chain can be traversed in both directions.
 */
typedef struct adt_circular_dllist_s {
	MemoryNode *head_;                          /**< First node, NULL when empty. */
	MemoryNode *tail_;                          /**< Last node, NULL when empty. */
	u16 length_;                                /**< Number of stored elements. */
	u16 capacity_;                              /**< Maximum number of elements. */
	struct circular_dllist_ops_s *ops_;         /**< Pointer to the CircularDLList API. */
} CircularDLList;

/**
 * @brief Public API for the CircularDLList ADT.
 */
struct circular_dllist_ops_s {
	/**
	 * @brief Releases every node of the list and frees the list itself.
	 * @param list The list to destroy.
	 * @return kErrorCode_Null if @p list is NULL, kErrorCode_Ok otherwise.
	 */
	s16 (*destroy)(CircularDLList* list);

	/**
	 * @brief Empties the list freeing the nodes but not their data buffers.
	 * @param list The list to soft reset.
	 * @return kErrorCode_Null if @p list is NULL, kErrorCode_Ok otherwise.
	 */
	s16 (*softReset)(CircularDLList* list);

	/**
	 * @brief Empties the list freeing both nodes and data buffers.
	 * @param list The list to reset.
	 * @return kErrorCode_Null if @p list is NULL, kErrorCode_Ok otherwise.
	 */
	s16 (*reset)(CircularDLList* list);

	/**
	 * @brief Changes the maximum capacity of the list.
	 * @param list The list to resize.
	 * @param new_size New maximum capacity.
	 * @return kErrorCode_Null if @p list is NULL,
	 *         kErrorCode_ZeroSize if @p new_size is 0,
	 *         kErrorCode_Ok otherwise.
	 */
	s16 (*resize)(CircularDLList* list, u16 new_size);

	/**
	 * @brief Returns the maximum capacity of the list.
	 * @param list The list to query.
	 * @return Capacity, or 0 if @p list is NULL.
	 */
	u16 (*capacity)(CircularDLList* list);

	/**
	 * @brief Returns the current number of stored elements.
	 * @param list The list to query.
	 * @return Length, or 0 if @p list is NULL.
	 */
	u16 (*length)(CircularDLList* list);

	/**
	 * @brief Tells whether the list is empty.
	 * @param list The list to query.
	 * @return True if empty or @p list is NULL, False otherwise.
	 */
	boolean (*isEmpty)(CircularDLList* list);

	/**
	 * @brief Tells whether the list has reached its capacity.
	 * @param list The list to query.
	 * @return True if full, False otherwise or if @p list is NULL.
	 */
	boolean (*isFull)(CircularDLList* list);

	/**
	 * @brief Returns the data stored at the head.
	 * @param list The list to query.
	 * @param size Output parameter that receives the size of the element.
	 * @return Pointer to the data, or NULL on invalid input.
	 */
	void* (*first)(CircularDLList* list, u16* size);

	/**
	 * @brief Returns the data stored at the tail.
	 * @param list The list to query.
	 * @param size Output parameter that receives the size of the element.
	 * @return Pointer to the data, or NULL on invalid input.
	 */
	void* (*last)(CircularDLList* list, u16* size);

	/**
	 * @brief Returns the data stored at @p position from the head.
	 * @param list The list to query.
	 * @param size Output parameter that receives the size of the element.
	 * @param position Zero based logical index of the element.
	 * @return Pointer to the data, or NULL on invalid input.
	 */
	void* (*at)(CircularDLList* list, u16* size, u16 position);

	/**
	 * @brief Inserts an element at the head.
	 * @param list The list to modify.
	 * @param data Pointer to the data to insert.
	 * @param bytes Size in bytes of @p data.
	 * @return kErrorCode_Null/NullData/ZeroSize/IsFull/Memory or kErrorCode_Ok.
	 */
	s16 (*insertFirst)(CircularDLList* list, void *data, u16 bytes);

	/**
	 * @brief Inserts an element at the tail.
	 * @param list The list to modify.
	 * @param data Pointer to the data to insert.
	 * @param bytes Size in bytes of @p data.
	 * @return kErrorCode_Null/NullData/ZeroSize/IsFull/Memory or kErrorCode_Ok.
	 */
	s16 (*insertLast)(CircularDLList* list, void *data, u16 bytes);

	/**
	 * @brief Inserts an element at @p position.
	 * @param list The list to modify.
	 * @param data Pointer to the data to insert.
	 * @param bytes Size in bytes of @p data.
	 * @param position Zero based logical index.
	 * @return kErrorCode_Null/NullData/ZeroSize/IsFull/Memory or kErrorCode_Ok.
	 */
	s16 (*insertAt)(CircularDLList* list, void *data, u16 bytes, u16 position);

	/**
	 * @brief Extracts and removes the head element.
	 * @param list The list to modify.
	 * @param size Output parameter that receives the size of the element.
	 * @return Pointer to the extracted data, or NULL on invalid input.
	 */
	void* (*extractFirst)(CircularDLList* list, u16* size);

	/**
	 * @brief Extracts and removes the tail element.
	 * @param list The list to modify.
	 * @param size Output parameter that receives the size of the element.
	 * @return Pointer to the extracted data, or NULL on invalid input.
	 */
	void* (*extractLast)(CircularDLList* list, u16* size);

	/**
	 * @brief Extracts and removes the element at @p position.
	 * @param list The list to modify.
	 * @param size Output parameter that receives the size of the element.
	 * @param position Zero based logical index of the element to extract.
	 * @return Pointer to the extracted data, or NULL on invalid input.
	 */
	void* (*extractAt)(CircularDLList* list, u16* size, u16 position);

	/**
	 * @brief Concatenates @p list_src at the tail of @p list.
	 * @param list Destination list.
	 * @param list_src Source list.
	 * @return kErrorCode_Null/Memory or kErrorCode_Ok.
	 */
	s16 (*concat)(CircularDLList* list, CircularDLList *list_src);

	/**
	 * @brief Calls @p callback for every stored element.
	 * @param list The list to traverse.
	 * @param callback Function applied to each MemoryNode.
	 * @return kErrorCode_Null or kErrorCode_Ok.
	 */
	s16 (*traverse)(CircularDLList* list, void (*callback)(MemoryNode *));

	/**
	 * @brief Prints the content of the list to standard output.
	 * @param list The list to print.
	 */
	void (*print)(CircularDLList* list);
};

/**
 * @brief Creates a new circular doubly linked list with the specified capacity.
 * @param capacity Maximum number of elements the list will hold.
 * @return Pointer to the new list, or NULL on invalid input or failure.
 */
CircularDLList* CIRCULARDLLIST_create(u16 capacity);

#endif //__ADT_CIRCULAR_DLLIST_H__
