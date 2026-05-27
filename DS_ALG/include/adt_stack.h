/*
 *
 * adt_stack.h
 * Toni Barella, Ivan Sancho as Unreal Authorized Instructor (UAI).
 * Algorithms and Data Structures.
 * ESAT 2020-2021
 *
 */

#ifndef __ADT_STACK_H__
#define __ADT_STACK_H__

#include "adt_memory_node.h"

/**
 * @brief Stack ADT (LIFO) backed by a contiguous MemoryNode array.
 * @details The top of the stack is located at index @p tail_ - 1, so pushes
 *          and pops happen at the tail.
 */
typedef struct adt_stack_s {
	u16 head_;                        /**< Always 0 for the stack. */
	u16 tail_;                        /**< Index of the first free slot. */
	u16 capacity_;                    /**< Maximum number of elements. */
	MemoryNode *storage_;             /**< Storage of MemoryNode entries. */
	struct stack_ops_s *ops_;         /**< Pointer to the Stack API. */
} Stack;

/**
 * @brief Public API for the Stack ADT.
 */
struct stack_ops_s {
	/**
	 * @brief Releases every node of the stack and frees the stack itself.
	 * @param stack The stack to destroy.
	 * @return kErrorCode_Null if @p stack is NULL,
	 *         kErrorCode_Ok otherwise.
	 */
	s16 (*destroy)(Stack* stack);

	/**
	 * @brief Empties the stack without freeing the data buffers.
	 * @param stack The stack to soft reset.
	 * @return kErrorCode_Null if @p stack is NULL,
	 *         kErrorCode_NullData if the storage is NULL,
	 *         kErrorCode_Ok otherwise.
	 */
	s16 (*softReset)(Stack* stack);

	/**
	 * @brief Empties the stack freeing every data buffer.
	 * @param stack The stack to reset.
	 * @return kErrorCode_Null if @p stack is NULL,
	 *         kErrorCode_NullData if the storage is NULL,
	 *         kErrorCode_Ok otherwise.
	 */
	s16 (*reset)(Stack* stack);

	/**
	 * @brief Changes the maximum capacity of the stack.
	 * @details When @p new_size is smaller than the current length the
	 *          extra elements at the top are released.
	 * @param stack The stack to resize.
	 * @param new_size New capacity.
	 * @return kErrorCode_Null if @p stack is NULL,
	 *         kErrorCode_ZeroSize if @p new_size is 0,
	 *         kErrorCode_Memory on allocation failure,
	 *         kErrorCode_Ok otherwise.
	 */
	s16 (*resize)(Stack* stack, u16 new_size);

	/**
	 * @brief Returns the maximum capacity of the stack.
	 * @param stack The stack to query.
	 * @return Capacity, or 0 if @p stack is NULL.
	 */
	u16 (*capacity)(Stack* stack);

	/**
	 * @brief Returns the current number of stored elements.
	 * @param stack The stack to query.
	 * @return Length, or 0 if @p stack is NULL.
	 */
	u16 (*length)(Stack* stack);

	/**
	 * @brief Tells whether the stack is empty.
	 * @param stack The stack to query.
	 * @return True if empty, False otherwise or if @p stack is NULL.
	 */
	boolean (*isEmpty)(Stack* stack);

	/**
	 * @brief Tells whether the stack has reached its capacity.
	 * @param stack The stack to query.
	 * @return True if full, False otherwise or if @p stack is NULL.
	 */
	boolean (*isFull)(Stack* stack);

	/**
	 * @brief Returns the data of the element at the top of the stack.
	 * @param stack The stack to query.
	 * @return Pointer to the top element data, or NULL on invalid input.
	 */
	void* (*top)(Stack* stack);

	/**
	 * @brief Pushes an element on top of the stack.
	 * @param stack The stack to modify.
	 * @param data Pointer to the data to push.
	 * @param bytes Size in bytes of @p data.
	 * @return kErrorCode_Null if @p stack is NULL,
	 *         kErrorCode_NullData if @p data is NULL,
	 *         kErrorCode_ZeroSize if @p bytes is 0,
	 *         kErrorCode_IsFull if the stack is full,
	 *         kErrorCode_Ok otherwise.
	 */
	s16 (*push)(Stack* stack, void *data, u16 bytes);

	/**
	 * @brief Pops the element at the top of the stack.
	 * @param stack The stack to modify.
	 * @param size Output parameter that receives the size of the element.
	 *             It may be NULL when the caller is not interested.
	 * @return Pointer to the popped data, or NULL on invalid input.
	 */
	void* (*pop)(Stack* stack, u16* size);

	/**
	 * @brief Concatenates @p stack_src on top of @p stack.
	 * @details Source data is deep copied so both stacks stay independent.
	 *          Elements keep their relative order; the bottom of the source
	 *          becomes the element above the previous top of @p stack.
	 * @param stack Destination stack.
	 * @param stack_src Source stack.
	 * @return kErrorCode_Null if any stack is NULL,
	 *         kErrorCode_Memory on allocation failure,
	 *         kErrorCode_Ok otherwise.
	 */
	s16 (*concat)(Stack* stack, Stack *stack_src);

	/**
	 * @brief Calls @p callback for every stored element from bottom to top.
	 * @param stack The stack to traverse.
	 * @param callback Function applied to each MemoryNode.
	 * @return kErrorCode_Null if @p stack or @p callback are NULL,
	 *         kErrorCode_Ok otherwise.
	 */
	s16 (*traverse)(Stack* stack, void (*callback)(MemoryNode *));

	/**
	 * @brief Prints the content of the stack to standard output.
	 * @param stack The stack to print.
	 */
	void (*print)(Stack* stack);
};

/**
 * @brief Creates a new stack with the specified capacity.
 * @param capacity Maximum number of elements the stack will hold.
 * @return Pointer to the new stack, or NULL on invalid input or failure.
 */
Stack* STACK_create(u16 capacity);

#endif //__ADT_STACK_H__
