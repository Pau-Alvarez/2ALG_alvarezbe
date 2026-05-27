/*
 *
 * adt_vector.h
 * Toni Barella, Ivan Sancho as Unreal Authorized Instructor (UAI).
 * Algorithms and Data Structures.
 * ESAT 2020-2021
 *
 */

#ifndef __ADT_VECTOR_H__
#define __ADT_VECTOR_H__

#include "adt_memory_node.h"

/**
 * @brief Linear array based ADT with bounded capacity.
 * @details Elements are stored sequentially in a contiguous array of
 *          MemoryNode. The fixed slot 0 acts as the head and @p tail_
 *          points to the first free position.
 */
typedef struct adt_vector_s {
	u16 head_;                        /**< Always 0 for a linear vector. */
	u16 tail_;                        /**< Index of the first free slot. */
	u16 capacity_;                    /**< Maximum number of elements. */
	MemoryNode *storage_;             /**< Storage of MemoryNode entries. */
	struct vector_ops_s *ops_;        /**< Pointer to the Vector API. */
} Vector;

/**
 * @brief Public API for the Vector ADT.
 */
struct vector_ops_s {
	/**
	 * @brief Releases every node of the vector and frees the vector itself.
	 * @param vector The vector to destroy.
	 * @return kErrorCode_Null if @p vector is NULL,
	 *         kErrorCode_Ok otherwise.
	 */
	s16 (*destroy)(Vector* vector);

	/**
	 * @brief Empties the vector without freeing the data buffers.
	 * @param vector The vector to soft reset.
	 * @return kErrorCode_Null if @p vector is NULL,
	 *         kErrorCode_NullData if the storage is NULL,
	 *         kErrorCode_Ok otherwise.
	 */
	s16 (*softReset)(Vector* vector);

	/**
	 * @brief Empties the vector freeing every data buffer.
	 * @param vector The vector to reset.
	 * @return kErrorCode_Null if @p vector is NULL,
	 *         kErrorCode_NullData if the storage is NULL,
	 *         kErrorCode_Ok otherwise.
	 */
	s16 (*reset)(Vector* vector);

	/**
	 * @brief Resizes the capacity of the vector.
	 * @details When @p new_size is smaller than the current length the
	 *          extra elements are released.
	 * @param vector The vector to resize.
	 * @param new_size The new capacity.
	 * @return kErrorCode_Null if @p vector is NULL,
	 *         kErrorCode_ZeroSize if @p new_size is 0,
	 *         kErrorCode_Memory on allocation failure,
	 *         kErrorCode_Ok otherwise.
	 */
	s16 (*resize)(Vector* vector, u16 new_size);

	/**
	 * @brief Returns the maximum number of elements the vector can hold.
	 * @param vector The vector to query.
	 * @return Capacity of the vector, or 0 if @p vector is NULL.
	 */
	u16 (*capacity)(Vector* vector);

	/**
	 * @brief Returns the current number of stored elements.
	 * @param vector The vector to query.
	 * @return Number of elements, or 0 if @p vector is NULL.
	 */
	u16 (*length)(Vector* vector);

	/**
	 * @brief Tells whether the vector contains no elements.
	 * @param vector The vector to query.
	 * @return True if empty or @p vector is NULL, False otherwise.
	 */
	boolean (*isEmpty)(Vector* vector);

	/**
	 * @brief Tells whether the vector has reached its capacity.
	 * @param vector The vector to query.
	 * @return True if full, False otherwise or if @p vector is NULL.
	 */
	boolean (*isFull)(Vector* vector);

	/**
	 * @brief Returns a reference to the data stored at position 0.
	 * @param vector The vector to query.
	 * @param size Output parameter that receives the size of the element.
	 * @return Pointer to the data, or NULL when the vector is empty.
	 */
	void* (*first)(Vector* vector, u16* size);

	/**
	 * @brief Returns a reference to the data stored at the last position.
	 * @param vector The vector to query.
	 * @param size Output parameter that receives the size of the element.
	 * @return Pointer to the data, or NULL when the vector is empty.
	 */
	void* (*last)(Vector* vector, u16* size);

	/**
	 * @brief Returns a reference to the data stored at @p position.
	 * @param vector The vector to query.
	 * @param size Output parameter that receives the size of the element.
	 * @param position Zero based index of the element to read.
	 * @return Pointer to the data, or NULL on invalid input.
	 */
	void* (*at)(Vector* vector, u16* size, u16 position);

	/**
	 * @brief Inserts an element at the first position of the vector.
	 * @param vector The vector to modify.
	 * @param data Pointer to the data to insert.
	 * @param bytes Size in bytes of @p data.
	 * @return kErrorCode_Null if @p vector is NULL,
	 *         kErrorCode_NullData if @p data is NULL,
	 *         kErrorCode_ZeroSize if @p bytes is 0,
	 *         kErrorCode_IsFull if the vector is full,
	 *         kErrorCode_Ok otherwise.
	 */
	s16 (*insertFirst)(Vector* vector, void *data, u16 bytes);

	/**
	 * @brief Inserts an element at the last position of the vector.
	 * @param vector The vector to modify.
	 * @param data Pointer to the data to insert.
	 * @param bytes Size in bytes of @p data.
	 * @return kErrorCode_Null if @p vector is NULL,
	 *         kErrorCode_NullData if @p data is NULL,
	 *         kErrorCode_ZeroSize if @p bytes is 0,
	 *         kErrorCode_IsFull if the vector is full,
	 *         kErrorCode_Ok otherwise.
	 */
	s16 (*insertLast)(Vector* vector, void *data, u16 bytes);

	/**
	 * @brief Inserts an element at a given position.
	 * @details Positions beyond the current length are clamped to the tail.
	 * @param vector The vector to modify.
	 * @param data Pointer to the data to insert.
	 * @param bytes Size in bytes of @p data.
	 * @param position Zero based index where the element will be placed.
	 * @return kErrorCode_Null if @p vector is NULL,
	 *         kErrorCode_NullData if @p data is NULL,
	 *         kErrorCode_ZeroSize if @p bytes is 0,
	 *         kErrorCode_IsFull if the vector is full,
	 *         kErrorCode_Ok otherwise.
	 */
	s16 (*insertAt)(Vector* vector, void *data, u16 bytes, u16 position);

	/**
	 * @brief Extracts and removes the first element of the vector.
	 * @param vector The vector to modify.
	 * @param size Output parameter that receives the size of the element.
	 * @return Pointer to the extracted data, or NULL on invalid input.
	 */
	void* (*extractFirst)(Vector* vector, u16* size);

	/**
	 * @brief Extracts and removes the last element of the vector.
	 * @param vector The vector to modify.
	 * @param size Output parameter that receives the size of the element.
	 * @return Pointer to the extracted data, or NULL on invalid input.
	 */
	void* (*extractLast)(Vector* vector, u16* size);

	/**
	 * @brief Extracts and removes the element at @p position.
	 * @param vector The vector to modify.
	 * @param size Output parameter that receives the size of the element.
	 * @param position Zero based index of the element to extract.
	 * @return Pointer to the extracted data, or NULL on invalid input.
	 */
	void* (*extractAt)(Vector* vector, u16* size, u16 position);

	/**
	 * @brief Concatenates @p vector_src at the tail of @p vector.
	 * @details The destination grows its capacity when needed. Data in the
	 *          source is deep copied so both vectors stay independent.
	 * @param vector Destination vector.
	 * @param vector_src Source vector.
	 * @return kErrorCode_Null if any vector is NULL,
	 *         kErrorCode_Memory on allocation failure,
	 *         kErrorCode_Ok otherwise.
	 */
	s16 (*concat)(Vector* vector, Vector *vector_src);

	/**
	 * @brief Calls @p callback for every stored element.
	 * @param vector The vector to traverse.
	 * @param callback Function applied to each MemoryNode.
	 * @return kErrorCode_Null if @p vector or @p callback are NULL,
	 *         kErrorCode_Ok otherwise.
	 */
	s16 (*traverse)(Vector* vector, void (*callback)(MemoryNode *));

	/**
	 * @brief Prints the state of the vector to standard output.
	 * @param vector The vector to print.
	 */
	void (*print)(Vector* vector);
};

/**
 * @brief Creates a new vector with the specified capacity.
 * @param capacity Maximum number of elements the vector will hold.
 * @return Pointer to the new vector, or NULL on invalid input or failure.
 */
Vector* VECTOR_create(u16 capacity);

#endif //__ADT_VECTOR_H__
