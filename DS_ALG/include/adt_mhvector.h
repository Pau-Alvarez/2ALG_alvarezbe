/*
 *
 * adt_mhvector.h
 * Toni Barella, Ivan Sancho as Unreal Authorized Instructor (UAI).
 * Algorithms and Data Structures.
 * ESAT 2020-2021
 *
 */

#ifndef __ADT_MHVECTOR_H__
#define __ADT_MHVECTOR_H__

#include "adt_memory_node.h"

/**
 * @brief Movable head vector ADT with over-allocated circular storage.
 * @details The internal buffer is allocated bigger than the logical
 *          capacity (see @c MOVABLE_VECTOR_SCALE) so that @p head_ can
 *          shift inside the buffer without copying every element. The
 *          fields @p head_ and @p tail_ wrap around @p capacity_ and the
 *          logical size returned to the user is @p capacity_ divided by
 *          the scale factor.
 */
typedef struct adt_movable_vector_s {
	u16 head_;                        /**< Index of the first element. */
	u16 tail_;                        /**< Number of stored elements. */
	u16 capacity_;                    /**< Allocated capacity (over-sized). */
	MemoryNode* storage_;             /**< Circular MemoryNode storage. */
	struct movable_vector_ops_s* ops_;/**< Pointer to the MHVector API. */
} MHVector;

/**
 * @brief Public API for the Movable Head Vector ADT.
 */
struct movable_vector_ops_s {
	/**
	 * @brief Releases every node of the vector and frees the vector itself.
	 * @param vector The vector to destroy.
	 * @return kErrorCode_Null if @p vector is NULL,
	 *         kErrorCode_Ok otherwise.
	 */
	s16 (*destroy)(MHVector* vector);

	/**
	 * @brief Empties the vector without freeing the data buffers.
	 * @param vector The vector to soft reset.
	 * @return kErrorCode_Null if @p vector or its storage is NULL,
	 *         kErrorCode_Ok otherwise.
	 */
	s16 (*softReset)(MHVector* vector);

	/**
	 * @brief Empties the vector freeing every data buffer.
	 * @param vector The vector to reset.
	 * @return kErrorCode_Null if @p vector or its storage is NULL,
	 *         kErrorCode_Ok otherwise.
	 */
	s16 (*reset)(MHVector* vector);

	/**
	 * @brief Changes the logical capacity of the vector.
	 * @details When @p new_size is smaller than the current length the
	 *          excess elements are released.
	 * @param vector The vector to resize.
	 * @param new_size New logical capacity.
	 * @return kErrorCode_Null if @p vector is NULL,
	 *         kErrorCode_NullData if the storage is NULL,
	 *         kErrorCode_ZeroSize if @p new_size is 0,
	 *         kErrorCode_Memory on allocation failure,
	 *         kErrorCode_Ok otherwise.
	 */
	s16 (*resize)(MHVector* vector, u16 new_size);

	/**
	 * @brief Returns the logical capacity of the vector.
	 * @param vector The vector to query.
	 * @return Capacity, or 0 if @p vector is NULL.
	 */
	u16 (*capacity)(MHVector* vector);

	/**
	 * @brief Returns the current number of stored elements.
	 * @param vector The vector to query.
	 * @return Length, or 0 if @p vector is NULL.
	 */
	s16 (*length)(MHVector* vector);

	/**
	 * @brief Tells whether the vector is empty.
	 * @param vector The vector to query.
	 * @return True if empty, False otherwise or if @p vector is NULL.
	 */
	boolean (*isEmpty)(MHVector* vector);

	/**
	 * @brief Tells whether the vector has reached its logical capacity.
	 * @param vector The vector to query.
	 * @return True if full, False otherwise or if @p vector is NULL.
	 */
	boolean (*isFull)(MHVector* vector);

	/**
	 * @brief Returns the data stored at the first logical position.
	 * @param vector The vector to query.
	 * @param size Output parameter that receives the size of the element.
	 * @return Pointer to the data, or NULL on invalid input.
	 */
	void* (*first)(MHVector* vector, u16* size);

	/**
	 * @brief Returns the data stored at the last logical position.
	 * @param vector The vector to query.
	 * @param size Output parameter that receives the size of the element.
	 * @return Pointer to the data, or NULL on invalid input.
	 */
	void* (*last)(MHVector* vector, u16* size);

	/**
	 * @brief Returns the data stored at @p position counted from the head.
	 * @param vector The vector to query.
	 * @param size Output parameter that receives the size of the element.
	 * @param position Zero based logical index of the element.
	 * @return Pointer to the data, or NULL on invalid input.
	 */
	void* (*at)(MHVector* vector, u16* size, u16 position);

	/**
	 * @brief Inserts an element at the first logical position.
	 * @param vector The vector to modify.
	 * @param data Pointer to the data to insert.
	 * @param bytes Size in bytes of @p data.
	 * @return kErrorCode_Null if @p vector or its storage is NULL,
	 *         kErrorCode_NullData if @p data is NULL,
	 *         kErrorCode_ZeroSize if @p bytes is 0,
	 *         kErrorCode_IsFull if the vector is full,
	 *         kErrorCode_Ok otherwise.
	 */
	s16 (*insertFirst)(MHVector* vector, void* data, u16 bytes);

	/**
	 * @brief Inserts an element at the last logical position.
	 * @param vector The vector to modify.
	 * @param data Pointer to the data to insert.
	 * @param bytes Size in bytes of @p data.
	 * @return kErrorCode_Null if @p vector or its storage is NULL,
	 *         kErrorCode_NullData if @p data is NULL,
	 *         kErrorCode_ZeroSize if @p bytes is 0,
	 *         kErrorCode_IsFull if the vector is full,
	 *         kErrorCode_Ok otherwise.
	 */
	s16 (*insertLast)(MHVector* vector, void* data, u16 bytes);

	/**
	 * @brief Inserts an element at a given logical position.
	 * @param vector The vector to modify.
	 * @param data Pointer to the data to insert.
	 * @param bytes Size in bytes of @p data.
	 * @param position Zero based logical index.
	 * @return kErrorCode_Null if @p vector or its storage is NULL,
	 *         kErrorCode_NullData if @p data is NULL,
	 *         kErrorCode_ZeroSize if @p bytes is 0,
	 *         kErrorCode_IsFull if the vector is full,
	 *         kErrorCode_Ok otherwise.
	 */
	s16 (*insertAt)(MHVector* vector, void* data, u16 bytes, u16 position);

	/**
	 * @brief Extracts and removes the first logical element.
	 * @param vector The vector to modify.
	 * @param size Output parameter that receives the size of the element.
	 * @return Pointer to the extracted data, or NULL on invalid input.
	 */
	void* (*extractFirst)(MHVector* vector, u16* size);

	/**
	 * @brief Extracts and removes the last logical element.
	 * @param vector The vector to modify.
	 * @param size Output parameter that receives the size of the element.
	 * @return Pointer to the extracted data, or NULL on invalid input.
	 */
	void* (*extractLast)(MHVector* vector, u16* size);

	/**
	 * @brief Extracts and removes the element at logical @p position.
	 * @param vector The vector to modify.
	 * @param size Output parameter that receives the size of the element.
	 * @param position Zero based logical index of the element to extract.
	 * @return Pointer to the extracted data, or NULL on invalid input.
	 */
	void* (*extractAt)(MHVector* vector, u16* size, u16 position);

	/**
	 * @brief Concatenates @p vector_src at the tail of @p vector.
	 * @param vector Destination vector.
	 * @param vector_src Source vector.
	 * @return kErrorCode_Null if any vector is NULL,
	 *         kErrorCode_NullData if any storage is NULL,
	 *         kErrorCode_Memory on allocation failure,
	 *         kErrorCode_Ok otherwise.
	 */
	s16 (*concat)(MHVector* vector, MHVector* vector_src);

	/**
	 * @brief Calls @p callback for every stored element from head to tail.
	 * @param vector The vector to traverse.
	 * @param callback Function applied to each MemoryNode.
	 * @return kErrorCode_Null if @p vector or @p callback are NULL,
	 *         kErrorCode_Ok otherwise.
	 */
	s16 (*traverse)(MHVector* vector, void (*callback)(MemoryNode*));

	/**
	 * @brief Prints the content of the vector to standard output.
	 * @param vector The vector to print.
	 */
	void (*print)(MHVector* vector);
};

/**
 * @brief Creates a new movable head vector with the given logical capacity.
 * @param capacity Logical maximum number of elements.
 * @return Pointer to the new vector, or NULL on invalid input or failure.
 */
MHVector* MHVECTOR_create(u16 capacity);

#endif //__ADT_MHVECTOR_H__
