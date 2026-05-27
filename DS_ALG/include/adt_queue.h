/*
 *
 * adt_queue.h
 * Toni Barella, Ivan Sancho as Unreal Authorized Instructor (UAI).
 * Algorithms and Data Structures.
 * ESAT 2020-2021
 *
 */

#ifndef __ADT_QUEUE_H__
#define __ADT_QUEUE_H__

#include "adt_memory_node.h"

/**
 * @brief Queue ADT (FIFO) backed by a circular MemoryNode array.
 * @details Elements are stored between @p head_ (first element) and the
 *          position just before @p tail_ (first free slot). Indices wrap
 *          around the capacity so enqueue and dequeue stay O(1).
 */
typedef struct adt_queue_s {
	u16 head_;                        /**< Index of the first element. */
	u16 tail_;                        /**< Index of the first free slot. */
	u16 length_;                      /**< Number of stored elements. */
	u16 capacity_;                    /**< Maximum number of elements. */
	MemoryNode *storage_;             /**< Circular storage of MemoryNode. */
	struct queue_ops_s *ops_;         /**< Pointer to the Queue API. */
} Queue;

/**
 * @brief Public API for the Queue ADT.
 */
struct queue_ops_s {
	/**
	 * @brief Releases every node of the queue and frees the queue itself.
	 * @param queue The queue to destroy.
	 * @return kErrorCode_Null if @p queue is NULL,
	 *         kErrorCode_Ok otherwise.
	 */
	s16 (*destroy)(Queue* queue);

	/**
	 * @brief Empties the queue without freeing the data buffers.
	 * @param queue The queue to soft reset.
	 * @return kErrorCode_Null if @p queue is NULL,
	 *         kErrorCode_NullData if the storage is NULL,
	 *         kErrorCode_Ok otherwise.
	 */
	s16 (*softReset)(Queue* queue);

	/**
	 * @brief Empties the queue freeing every data buffer.
	 * @param queue The queue to reset.
	 * @return kErrorCode_Null if @p queue is NULL,
	 *         kErrorCode_NullData if the storage is NULL,
	 *         kErrorCode_Ok otherwise.
	 */
	s16 (*reset)(Queue* queue);

	/**
	 * @brief Changes the maximum capacity of the queue.
	 * @details When @p new_size is smaller than the current length the
	 *          extra elements located at the back are released.
	 * @param queue The queue to resize.
	 * @param new_size New capacity.
	 * @return kErrorCode_Null if @p queue is NULL,
	 *         kErrorCode_ZeroSize if @p new_size is 0,
	 *         kErrorCode_Memory on allocation failure,
	 *         kErrorCode_Ok otherwise.
	 */
	s16 (*resize)(Queue* queue, u16 new_size);

	/**
	 * @brief Returns the maximum capacity of the queue.
	 * @param queue The queue to query.
	 * @return Capacity, or 0 if @p queue is NULL.
	 */
	u16 (*capacity)(Queue* queue);

	/**
	 * @brief Returns the current number of stored elements.
	 * @param queue The queue to query.
	 * @return Length, or 0 if @p queue is NULL.
	 */
	u16 (*length)(Queue* queue);

	/**
	 * @brief Tells whether the queue is empty.
	 * @param queue The queue to query.
	 * @return True if empty, False otherwise or if @p queue is NULL.
	 */
	boolean (*isEmpty)(Queue* queue);

	/**
	 * @brief Tells whether the queue has reached its capacity.
	 * @param queue The queue to query.
	 * @return True if full, False otherwise or if @p queue is NULL.
	 */
	boolean (*isFull)(Queue* queue);

	/**
	 * @brief Returns the data of the element at the front of the queue.
	 * @param queue The queue to query.
	 * @return Pointer to the front data, or NULL on invalid input.
	 */
	void* (*front)(Queue* queue);

	/**
	 * @brief Returns the data of the element at the back of the queue.
	 * @param queue The queue to query.
	 * @return Pointer to the back data, or NULL on invalid input.
	 */
	void* (*back)(Queue* queue);

	/**
	 * @brief Inserts an element at the back of the queue.
	 * @param queue The queue to modify.
	 * @param data Pointer to the data to enqueue.
	 * @param bytes Size in bytes of @p data.
	 * @return kErrorCode_Null if @p queue is NULL,
	 *         kErrorCode_NullData if @p data is NULL,
	 *         kErrorCode_ZeroSize if @p bytes is 0,
	 *         kErrorCode_IsFull if the queue is full,
	 *         kErrorCode_Ok otherwise.
	 */
	s16 (*enqueue)(Queue* queue, void *data, u16 bytes);

	/**
	 * @brief Removes the element at the front of the queue.
	 * @param queue The queue to modify.
	 * @return Pointer to the dequeued data, or NULL on invalid input.
	 */
	void* (*dequeue)(Queue* queue);

	/**
	 * @brief Concatenates @p queue_src at the back of @p queue.
	 * @details Source data is deep copied so both queues stay independent.
	 *          The destination capacity is grown to fit if needed.
	 * @param queue Destination queue.
	 * @param queue_src Source queue.
	 * @return kErrorCode_Null if any queue is NULL,
	 *         kErrorCode_Memory on allocation failure,
	 *         kErrorCode_Ok otherwise.
	 */
	s16 (*concat)(Queue* queue, Queue *queue_src);

	/**
	 * @brief Calls @p callback for every stored element from front to back.
	 * @param queue The queue to traverse.
	 * @param callback Function applied to each MemoryNode.
	 * @return kErrorCode_Null if @p queue or @p callback are NULL,
	 *         kErrorCode_Ok otherwise.
	 */
	s16 (*traverse)(Queue* queue, void (*callback)(MemoryNode *));

	/**
	 * @brief Prints the content of the queue to standard output.
	 * @param queue The queue to print.
	 */
	void (*print)(Queue* queue);
};

/**
 * @brief Creates a new queue with the specified capacity.
 * @param capacity Maximum number of elements the queue will hold.
 * @return Pointer to the new queue, or NULL on invalid input or failure.
 */
Queue* QUEUE_create(u16 capacity);

#endif //__ADT_QUEUE_H__
