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

typedef struct adt_movable_vector_s {
	u16 head_;
	u16 tail_;
	u16 capacity_;
	MemoryNode* storage_;
	struct movable_vector_ops_s* ops_;
} MHVector;

struct movable_vector_ops_s {
	/**
	* @brief Destroys the vector and its data:
	* @param *vector Pointer to the vector.
	* @return s16 kErrorCode_Null When the vector is null or the vector's
									storage is null.
	* @return s16 kErrorCode_Ok When the fuction has done his job.
	*/
	s16(*destroy)(MHVector* vector);
	/**
	* @brief Soft resets vector's data (without freeing memory).
	* @param *vector Pointer to the vector.
	* @return s16 kErrorCode_Null When the vector is null or the vector's
									storage is null.
	* @return s16 kErrorCode_Ok When the fuction has done his job.
	*/
	s16(*softReset)(MHVector* vector);
	/**
	* @brief Resets vector's data (free memory data).
	* @param *vector Pointer to the vector.
	* @return s16 kErrorCode_Null When the vector is null or the vector's
									storage is null.
	* @return s16 kErrorCode_Ok When the fuction has done his job.
	*/
	s16(*reset)(MHVector* vector);
	/**
	* @brief Resizes the capacity of the vector. Some elements should be
			 deleted if new_size < capacity.
	* @param *vector Pointer to the vector.
	* @param u16 new_size The new size (capacity) of the vector.
	* @return s16 kErrorCode_Null When the vector is null or
	* @return s16 kErrorCode_Memory When failer memory allocation attempt.
	* @return s16 kErrorCode_NullData When the vector's storage data is null.
	* @return s16 kErrorCode_ZeroSize When new_size is equal to zero.
	* @return s16 kErrorCode_Ok When new_size is equal to capacity, or when
	*                           the function has donde his job.
	*/
	s16(*resize)(MHVector* vector, u16 new_size);

	//State queries:

	/**
	* @brief Returns the maximum number of elemets to store.
	* @param *vector Pointer to the vector.
	* @return u16 capacity_ Vector's capacity.
	* @return u16 0 When the vector is null.
	*/
	u16(*capacity)(MHVector* vector);
	/**
	* @brief Returns current number of elements being used (<= capacity).
	* @param *vector Pointer to the vector.
	* @return u16 tail_ Vector's tail.
	* @return u16 0 When the vector is null.
	*/
	u16(*length)(MHVector* vector);
	/**
	* @brief Returns a boolean if the vector is empty or not.
	* @param *vector Pointer to the vector.
	* @return boolean true When vector's tail is equal to 0.
	* @return boolean false When vector's tail is not equal to 0,
							or when the vector is null.
	*/
	boolean(*isEmpty)(MHVector* vector);
	/**
	* @brief Returns a boolean if the vector is full (is at its maximum capcity).
	* @param *vector Pointer to the vector.
	* @return boolean true When vector's tail is equal to vector's capacity.
	* @return boolean false When vector's tail is not equal to vector's capacity,
									or when the vector is null.
	*/
	boolean(*isFull)(MHVector* vector);


	//Data queries:
	/**
	* @brief Returns a reference to the data of the first element of the vector.
	* @param *vector Pointer to the vector.
	* @param *u16 size Variable used to store the size of the returned data.
	* @return *void NULL when the vector is null, or when the variable "size" is null.
	* @return *void data_ Pointer to the first strorage's data.
	*/
	void* (*first)(MHVector* vector, u16* size);
	/**
	* @brief Returns a reference to the data of the last element of the vector.
	* @param *vector Pointer to the vector.
	* @param *u16 size Variable used to store the size of the returned data.
	* @return *void NULL when the vector is null, or when the variable "size" is null.
	* @return *void data_ Pointer to the last strorage's data.
	*/
	void* (*last)(MHVector* vector, u16* size);
	/**
	* @brief Returns a reference to the element oat a given position.
	* @param *vector Pointer to the vector.
	* @param *u16 size Variable used to store the size of the returned data.
	* @param u16 position Position of the element in the vector.
	* @return *void NULL when the vector is null, or when the variable "size" is null.
	* @return *void data_ Pointer to the strorage's data.
	*/
	void* (*at)(MHVector* vector, u16* size, u16 position);

	/**
	* @brief Inserts an element in the first position of the vector.
	* @param *vector Pointer to the vector.
	* @param *void data Pointer to the new data to insert.
	* @param u16 bytes Size of the new data to insert.
	* @return s16 kErrorCode_Null When the vector is null or the vector's
									storage is null.
	* @return s16 kErrorCode_NullData When the new data is null.
	* @return s16 kErrorCode_ZeroSize When "bytes" is equal to 0.
	* @return s16 kErrorCode_IsFull When the vector is full.
	* @return s16 kErrorCode_Ok When the fuction has done his job.
	*/
	s16(*insertFirst)(MHVector* vector, void* data, u16 bytes);
	/**
	* @brief Inserts an element in the last position of the vector.
	* @param *vector Pointer to the vector.
	* @param *void data Pointer to the new data to insert.
	* @param u16 bytes Size of the new data to insert.
	* @return s16 kErrorCode_Null When the vector is null or the vector's
									storage is null.
	* @return s16 kErrorCode_NullData When the new data is null.
	* @return s16 kErrorCode_ZeroSize When "bytes" is equal to 0.
	* @return s16 kErrorCode_IsFull When the vector is full.
	* @return s16 kErrorCode_Ok When the fuction has done his job.
	*/
	s16(*insertLast)(MHVector* vector, void* data, u16 bytes);
	/**
	* @brief Inserts an element at the given position of the vector.
	* @param *vector Pointer to the vector.
	* @param *void data Pointer to the new data to insert.
	* @param u16 bytes Size of the new data to insert.
	* @param u16 position Position in the vector to insert the data.
	* @return s16 kErrorCode_Null When the vector is null or the vector's
									storage is null.
	* @return s16 kErrorCode_NullData When the new data is null.
	* @return s16 kErrorCode_ZeroSize When "bytes" is equal to 0.
	* @return s16 kErrorCode_IsFull When the vector is full.
	* @return s16 kErrorCode_Ok When the fuction has done his job.
	*/
	s16(*insertAt)(MHVector* vector, void* data, u16 bytes, u16 position);

	//Extraction:

	/**
	* @brief Extracts the first element of a vector given.
	* @param *Vector vector Pointer to the vector.
	* @param *u16 size Variable used to store the size of the returned data.
	* @return *void NULL when the vector is null, when the variable "size" is null or the vector is empty.
	* @return *void data_ Pointer to the strorage's data.
	*/
	void* (*extractFirst)(MHVector* vector, u16* size);

	/**
	* @brief Extracts the last element of a vector given.
	* @param *Vector vector Pointer to the vector.
	* @param *u16 size Variable used to store the size of the returned data.
	* @return *void NULL when the vector is null, when the variable "size" is null or the vector is empty.
	* @return *void data_ Pointer to the strorage's data.
	*/
	void* (*extractLast)(MHVector* vector, u16* size);

	/**
	* @brief Extracts the element of a vector at the given position.
	* @param *Vector vector Pointer to the vector.
	* @param *u16 size Variable used to store the size of the returned data.
	* @param u16 position Position of the element to extract.
	* @return *void NULL when the vector is null, when the variable "size" is null or the vector is empty.
	* @return *void data_ Pointer to the strorage's data.
	*/
	void* (*extractAt)(MHVector* vector, u16* size, u16 position);

	//Miscellaneous:
	/**
	* @brief Concatenate the storages of two vectors.
	* @param *Vector vector Pointer to the first vector.
	* @param *Vector vector_src Pointer to the second vector.
	* @return s16 kErrorCode_Ok When the function has done his job.
	* @return s16 kErrorCode_Null When the any vector received is NULL.
	* @return s16 kErrorCode_NullData When any storage received is NULL.
	* @return s16 kErrorCode_Memory When failed memory allocation attempt.
	*/
	s16(*concat)(MHVector* vector, MHVector* vector_src);
	/**
	* @brief Calls to a function from all elements of the vector.
	* @param *vector Pointer to the vector.
	* @param *callback *MemoryNode A callback to a function from MemoryNode.
	* @return *s16 kErrorCode_Null When the vector is null or the callback is null.
	* @return s16 kErrorCode_Ok When the fuction has done his job.
	*/
	s16(*traverse)(MHVector* vector, void (*callback)(MemoryNode*));
	/**
	* @brief Prints the features and content of the vector.
	* @param *vector Pointer to the vector.
	* @return void kErrorCode_Null When the vector is null or the vector's
									 storage is null.
	* @return void kErrorCode_Ok When the fuction has done his job.
	*/
	void (*print)(MHVector* vector);
};

/**
* @brief Initialices a new vector without sanity checks.
* @param *Vector vec Pointer to the vector.
* @param u16 capacity The storage's capacity the vector will have.
* @return void kErrorCode_Ok When the fuction has done his job.
* @return s16 kErrorCode_Memory When failer memory allocation attempt.
*/
s16 MOVVECTOR_initWithoutCheck(MHVector* vec, u16 capacity);

/**
* @brief Creates a new vector.
* @param u16 capacity The storage's capacity the vector will have.
* @return *Vector NULL When "capacity" is equal to 0, or the memory allocation attempt fails.
* @return *Vector vec When the vector was created successfully.
*/
MHVector* MHVECTOR_create(u16 capacity);


#endif //__ADT_VECTOR_H__












