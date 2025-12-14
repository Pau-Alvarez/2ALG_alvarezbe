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

typedef struct adt_vector_s {
	u16 head_;
	u16 tail_; //First free place.
	u16 capacity_;
	MemoryNode *storage_;
	struct vector_ops_s *ops_;
} Vector;

struct vector_ops_s {
/**
 * @brief Destroys the vector and frees all its data
 * @param vector The vector to destroy
 * @return Status code indicating success or failure (s16)
 */
s16 (*destroy)(Vector* vector);

/**
 * @brief Soft resets the vector's data without freeing memory
 * @param vector The vector to soft reset
 * @return Status code indicating success or failure (s16)
 */
s16 (*softReset)(Vector* vector);

/**
 * @brief Resets the vector's data
 * @param vector The vector to reset
 * @return Status code indicating success or failure (s16)
 */
s16 (*reset)(Vector* vector);

/**
 * @brief Resizes the capacity of the vector (may delete elements)
 * @param vector The vector to resize
 * @param new_size The new capacity for the vector
 * @return Status code indicating success or failure (s16)
 */
s16 (*resize)(Vector* vector, u16 new_size);

// State queries:

/**
 * @brief Returns the maximum number of elements the vector can store
 * @param vector The vector to query
 * @return Maximum capacity of the vector (u16)
 */
u16 (*capacity)(Vector* vector);

/**
 * @brief Returns the current number of elements in the vector
 * @param vector The vector to query
 * @return Current number of elements, always <= capacity (u16)
 */
u16 (*length)(Vector* vector);

/**
 * @brief Checks if the vector is empty
 * @param vector The vector to check
 * @return True if the vector is empty, false otherwise
 */
boolean (*isEmpty)(Vector* vector);

/**
 * @brief Checks if the vector is full
 * @param vector The vector to check
 * @return True if the vector is full, false otherwise
 */
boolean (*isFull)(Vector* vector);

// Data queries:

/**
 * @brief Returns a reference to the first element of the vector
 * @param vector The vector to query
 * @param size Pointer to store the size of the element
 * @return Pointer to the first element's data
 */
void* (*first)(Vector* vector, u16* size);

/**
 * @brief Returns a reference to the last element of the vector
 * @param vector The vector to query
 * @param size Pointer to store the size of the element
 * @return Pointer to the last element's data
 */
void* (*last)(Vector* vector, u16* size);

/**
 * @brief Returns a reference to the element at a given position
 * @param vector The vector to query
 * @param size Pointer to store the size of the element
 * @param position The index of the element to retrieve
 * @return Pointer to the element's data at the specified position
 */
void* (*at)(Vector* vector, u16* size, u16 position);

// Insertion:

/**
 * @brief Inserts an element at the first position of the vector
 * @param vector The vector to modify
 * @param data Pointer to the data to insert
 * @param bytes Number of bytes to insert
 * @return Status code indicating success or failure (s16)
 */
s16 (*insertFirst)(Vector* vector, void *data, u16 bytes);

/**
 * @brief Inserts an element at the last position of the vector
 * @param vector The vector to modify
 * @param data Pointer to the data to insert
 * @param bytes Number of bytes to insert
 * @return Status code indicating success or failure (s16)
 */
s16 (*insertLast)(Vector* vector, void *data, u16 bytes);

/**
 * @brief Inserts an element at a given position in the vector
 * @param vector The vector to modify
 * @param data Pointer to the data to insert
 * @param bytes Number of bytes to insert
 * @param position The index where the element will be inserted
 * @return Status code indicating success or failure (s16)
 */
s16 (*insertAt)(Vector* vector, void *data, u16 bytes, u16 position);

// Extraction:

/**
 * @brief Extracts and removes the first element of the vector
 * @param vector The vector to modify
 * @param size Pointer to store the size of the extracted element
 * @return Pointer to the extracted element's data
 */
void* (*extractFirst)(Vector* vector, u16* size);

/**
 * @brief Extracts and removes the last element of the vector
 * @param vector The vector to modify
 * @param size Pointer to store the size of the extracted element
 * @return Pointer to the extracted element's data
 */
void* (*extractLast)(Vector* vector, u16* size);

/**
 * @brief Extracts and removes the element at a given position
 * @param vector The vector to modify
 * @param size Pointer to store the size of the extracted element
 * @param position The index of the element to extract
 * @return Pointer to the extracted element's data
 */
void* (*extractAt)(Vector* vector, u16* size, u16 position);

// Miscellaneous:

/**
 * @brief Concatenates two vectors
 * @param vector The destination vector
 * @param vector_src The source vector to concatenate
 * @return Status code indicating success or failure (s16)
 */
s16 (*concat)(Vector* vector, Vector *vector_src);

/**
 * @brief Traverses all elements of the vector and applies a callback function
 * @param vector The vector to traverse
 * @param callback Function to call for each element
 * @return Status code indicating success or failure (s16)
 */
s16 (*traverse)(Vector* vector, void (*callback)(MemoryNode *));

/**
 * @brief Prints the features and content of the vector
 * @param vector The vector to print
 */
void (*print)(Vector* vector);
};

/**
 * @brief Creates a new vector with the specified capacity
 * @param capacity The maximum number of elements the vector can store
 * @return Pointer to the newly created vector
 */
Vector* VECTOR_create(u16 capacity);

#endif //__ADT_VECTOR_H__











