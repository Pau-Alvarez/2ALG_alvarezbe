/*
 *
 * algo_quicksort.h
 * Toni Barella, Ivan Sancho as Unreal Authorized Instructor (UAI).
 * Algorithms and Data Structures.
 * ESAT 2020-2021
 *
 */

#ifndef __ALGO_QUICKSORT_H__
#define __ALGO_QUICKSORT_H__

#include "EDK_MemoryManager/edk_platform_types.h"
#include "common_def.h"

/**
 * @brief Signature of the comparison callback used by Quicksort.
 * @details Must return a negative value when @p lhs is less than @p rhs,
 *          0 when they are equal and a positive value when @p lhs is
 *          greater than @p rhs. Works just like @c strcmp.
 * @param lhs Pointer to the left-hand operand.
 * @param rhs Pointer to the right-hand operand.
 * @return Negative, zero or positive integer depending on the order.
 */
typedef s32 (*QuicksortCmp)(const void *lhs, const void *rhs);

/**
 * @brief Sorts an array in place using the Quicksort algorithm.
 * @details Uses the Lomuto partition scheme with the rightmost element as
 *          pivot. The function is generic: it works on any contiguous
 *          array whose element size and comparator are provided by the
 *          caller. Average and best case complexity is O(n log n); worst
 *          case (already sorted with a bad pivot) is O(n^2).
 * @param base Pointer to the first element of the array.
 * @param count Number of elements stored in @p base.
 * @param element_size Size in bytes of each element.
 * @param cmp Comparator callback used to order the elements.
 * @return kErrorCode_Null if any pointer parameter is NULL,
 *         kErrorCode_ZeroSize if @p element_size is 0,
 *         kErrorCode_Memory if the auxiliary swap buffer cannot be allocated,
 *         kErrorCode_Ok otherwise.
 */
s16 QUICKSORT_sort(void *base, u32 count, u16 element_size, QuicksortCmp cmp);

#endif //__ALGO_QUICKSORT_H__
