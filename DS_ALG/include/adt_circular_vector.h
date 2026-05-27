/*
 *
 * adt_circular_vector.h
 * Toni Barella, Ivan Sancho as Unreal Authorized Instructor (UAI).
 * Algorithms and Data Structures.
 * ESAT 2020-2021
 *
 */

#ifndef __ADT_CIRCULAR_VECTOR_H__
#define __ADT_CIRCULAR_VECTOR_H__

#include "adt_memory_node.h"

/**
 * @brief Circular vector ADT backed by a wrap-around MemoryNode array.
 * @details Indices @p head_ and @p tail_ wrap around @p capacity_, so both
 *          ends can grow/shrink in O(1) without shifting elements. The
 *          field @p length_ keeps the actual amount of stored elements.
 */
typedef struct adt_circ_vector {
	u16 head_;                        /**< Index of the first element. */
	u16 tail_;                        /**< Index of the first free slot. */
	u16 length_;                      /**< Number of stored elements. */
	u16 capacity_;                    /**< Maximum number of elements. */
	MemoryNode *storage_;             /**< Circular MemoryNode storage. */
	struct adt_circ_vector_s *ops_;   /**< Pointer to the CircularVector API. */
} CircularVector;

/**
 * @brief Public API for the CircularVector ADT.
 */
struct adt_circ_vector_s {
	/**
	 * @brief Releases every node of the vector and frees the vector itself.
	 * @param vector The vector to destroy.
	 * @return kErrorCode_Null if @p vector is NULL,
	 *         kErrorCode_Ok otherwise.
	 */
	s16 (*destroy)(CircularVector* vector);

	/**
	 * @brief Empties the vector without freeing the data buffers.
	 * @param vector The vector to soft reset.
	 * @return kErrorCode_Null if @p vector is NULL,
	 *         kErrorCode_NullData if the storage is NULL,
	 *         kErrorCode_Ok otherwise.
	 */
	s16 (*softReset)(CircularVector* vector);

	/**
	 * @brief Empties the vector freeing every data buffer.
	 * @param vector The vector to reset.
	 * @return kErrorCode_Null if @p vector is NULL,
	 *         kErrorCode_NullData if the storage is NULL,
	 *         kErrorCode_Ok otherwise.
	 */
	s16 (*reset)(CircularVector* vector);

	/**
	 * @brief Changes the maximum capacity of the vector.
	 * @details When @p new_size is smaller than the current length the
	 *          extra elements located at the back are released.
	 * @param vector The vector to resize.
	 * @param new_size New capacity.
	 * @return kErrorCode_Null if @p vector is NULL,
	 *         kErrorCode_ZeroSize if @p new_size is 0,
	 *         kErrorCode_Memory on allocation failure,
	 *         kErrorCode_Ok otherwise.
	 */
	s16 (*resize)(CircularVector* vector, u16 new_size);

	/**
	 * @brief Returns the maximum capacity of the vector.
	 * @param vector The vector to query.
	 * @return Capacity, or 0 if @p vector is NULL.
	 */
	u16 (*capacity)(CircularVector* vector);

	/**
	 * @brief Returns the current number of stored elements.
	 * @param vector The vector to query.
	 * @return Length, or 0 if @p vector is NULL.
	 */
	u16 (*length)(CircularVector* vector);

	/**
	 * @brief Tells whether the vector is empty.
	 * @param vector The vector to query.
	 * @return True if empty, False otherwise or if @p vector is NULL.
	 */
	boolean (*isEmpty)(CircularVector* vector);

	/**
	 * @brief Tells whether the vector has reached its capacity.
	 * @param vector The vector to query.
	 * @return True if full, False otherwise or if @p vector is NULL.
	 */
	boolean (*isFull)(CircularVector* vector);

	/**
	 * @brief Returns the data stored at the first logical position.
	 * @param vector The vector to query.
	 * @param size Output parameter that receives the size of the element.
	 * @return Pointer to the data, or NULL on invalid input.
	 */
	void* (*first)(CircularVector* vector, u16* size);

	/**
	 * @brief Returns the data stored at the last logical position.
	 * @param vector The vector to query.
	 * @param size Output parameter that receives the size of the element.
	 * @return Pointer to the data, or NULL on invalid input.
	 */
	void* (*last)(CircularVector* vector, u16* size);

	/**
	 * @brief Returns the data stored at @p position counted from the head.
	 * @param vector The vector to query.
	 * @param size Output parameter that receives the size of the element.
	 * @param position Zero based logical index of the element.
	 * @return Pointer to the data, or NULL on invalid input.
	 */
	void* (*at)(CircularVector* vector, u16* size, u16 position);

	/**
	 * @brief Inserts an element at the first logical position.
	 * @param vector The vector to modify.
	 * @param data Pointer to the data to insert.
	 * @param bytes Size in bytes of @p data.
	 * @return kErrorCode_Null if @p vector is NULL,
	 *         kErrorCode_NullData if @p data is NULL,
	 *         kErrorCode_ZeroSize if @p bytes is 0,
	 *         kErrorCode_IsFull if the vector is full,
	 *         kErrorCode_Ok otherwise.
	 */
	s16 (*insertFirst)(CircularVector* vector, void *data, u16 bytes);

	/**
	 * @brief Inserts an element at the last logical position.
	 * @param vector The vector to modify.
	 * @param data Pointer to the data to insert.
	 * @param bytes Size in bytes of @p data.
	 * @return kErrorCode_Null if @p vector is NULL,
	 *         kErrorCode_NullData if @p data is NULL,
	 *         kErrorCode_ZeroSize if @p bytes is 0,
	 *         kErrorCode_IsFull if the vector is full,
	 *         kErrorCode_Ok otherwise.
	 */
	s16 (*insertLast)(CircularVector* vector, void *data, u16 bytes);

	/**
	 * @brief Inserts an element at a given logical position.
	 * @details Positions beyond the current length are clamped to the tail.
	 * @param vector The vector to modify.
	 * @param data Pointer to the data to insert.
	 * @param bytes Size in bytes of @p data.
	 * @param position Zero based logical index.
	 * @return kErrorCode_Null if @p vector is NULL,
	 *         kErrorCode_NullData if @p data is NULL,
	 *         kErrorCode_ZeroSize if @p bytes is 0,
	 *         kErrorCode_IsFull if the vector is full,
	 *         kErrorCode_Ok otherwise.
	 */
	s16 (*insertAt)(CircularVector* vector, void *data, u16 bytes, u16 position);

	/**
	 * @brief Extracts and removes the first logical element.
	 * @param vector The vector to modify.
	 * @param size Output parameter that receives the size of the element.
	 * @return Pointer to the extracted data, or NULL on invalid input.
	 */
	void* (*extractFirst)(CircularVector* vector, u16* size);

	/**
	 * @brief Extracts and removes the last logical element.
	 * @param vector The vector to modify.
	 * @param size Output parameter that receives the size of the element.
	 * @return Pointer to the extracted data, or NULL on invalid input.
	 */
	void* (*extractLast)(CircularVector* vector, u16* size);

	/**
	 * @brief Extracts and removes the element at logical @p position.
	 * @param vector The vector to modify.
	 * @param size Output parameter that receives the size of the element.
	 * @param position Zero based logical index of the element to extract.
	 * @return Pointer to the extracted data, or NULL on invalid input.
	 */
	void* (*extractAt)(CircularVector* vector, u16* size, u16 position);

	/**
	 * @brief Concatenates @p vector_src at the tail of @p vector.
	 * @details Data of @p vector_src is deep copied so both vectors stay
	 *          independent. The destination capacity grows when needed.
	 * @param vector Destination vector.
	 * @param vector_src Source vector.
	 * @return kErrorCode_Null if any vector is NULL,
	 *         kErrorCode_Memory on allocation failure,
	 *         kErrorCode_Ok otherwise.
	 */
	s16 (*concat)(CircularVector* vector, CircularVector *vector_src);

	/**
	 * @brief Calls @p callback for every stored element from head to tail.
	 * @param vector The vector to traverse.
	 * @param callback Function applied to each MemoryNode.
	 * @return kErrorCode_Null if @p vector or @p callback are NULL,
	 *         kErrorCode_Ok otherwise.
	 */
	s16 (*traverse)(CircularVector* vector, void (*callback)(MemoryNode *));

	/**
	 * @brief Prints the content of the vector to standard output.
	 * @param vector The vector to print.
	 */
	void (*print)(CircularVector* vector);
};

/**
 * @brief Creates a new circular vector with the specified capacity.
 * @param capacity Maximum number of elements the vector will hold.
 * @return Pointer to the new vector, or NULL on invalid input or failure.
 */
CircularVector* CIRCULARVECTOR_create(u16 capacity);

#endif //__ADT_CIRCULAR_VECTOR_H__
