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
	//Destroys the vector and its data:
	s16 (*destroy)(Vector* vector);	
	//Soft resets vector's data:
  s16 (*softReset)(Vector* vector);		
	//Resets vector's data:
  s16 (*reset)(Vector* vector);		
	//Resizes the capacity of the vector. Some elements should be deleted:
	s16 (*resize)(Vector* vector, u16 new_size); 

	//State queries:
	//Returns the maximum number of elemets to store:
	u16 (*capacity)(Vector* vector);		
	//Current number of elements (<= capacity):
	u16 (*length)(Vector* vector);			
	boolean (*isEmpty)(Vector* vector);
	boolean (*isFull)(Vector* vector);
	
	//Data queries:
	//Returns a reference to the first element of the vector:
	void* (*first)(Vector* vector, u16* size);
	//Returns a reference to the last element of the vector:
	void* (*last)(Vector* vector, u16* size);
	//Returns a reference to the element oat a given position:
	void* (*at)(Vector* vector, u16* size, u16 position);

	//Insertion:
	//Inserts an element in the first position of the vector:
	s16 (*insertFirst)(Vector* vector, void *data, u16 bytes);
	//Inserts an element in the last position of the vector:
	s16 (*insertLast)(Vector* vector, void *data, u16 bytes);
	//Inserts an element at the given position of the vector:
	s16 (*insertAt)(Vector* vector, void *data, u16 bytes, u16 position);
	
	//Extraction:
	//Extracts the first element of the vector:
	void* (*extractFirst)(Vector* vector, u16* size);	
	//Extracts the last element of the vector:
	void* (*extractLast)(Vector* vector, u16* size);
	//Extracts the element of the vector at the given position:
	void* (*extractAt)(Vector* vector, u16* size, u16 position);
	
	//Miscellaneous:
	//Concatenates two vectors:
	s16 (*concat)(Vector* vector, Vector *vector_src);
	//Calls to a function from all elements of the vector:
	s16 (*traverse)(Vector* vector, void (*callback)(MemoryNode *));
	//Prints the features and content of the vector:
	void (*print)(Vector* vector);
};

//Creates a new vector:
Vector* VECTOR_create(u16 capacity);

#endif //__ADT_VECTOR_H__











