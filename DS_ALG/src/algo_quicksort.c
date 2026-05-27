/*
 * Ivan Sancho as Unreal Authorized Instructor (UAI), 2025.
 *
 * Quicksort source code.
 * Algorithms and Data Structures.
 *
 * Student:
 *   Pau Alvarez Belenguer
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common_def.h"
#include "algo_quicksort.h"

#include "EDK_MemoryManager/edk_memory_manager.h"


static void QUICKSORT_swap(u8* a, u8* b, u8* tmp, u16 element_size) {
	memcpy(tmp, a, element_size);
	memcpy(a, b, element_size);
	memcpy(b, tmp, element_size);
}

static s32 QUICKSORT_partition(u8* base,
                               s32 low,
                               s32 high,
                               u16 element_size,
                               QuicksortCmp cmp,
                               u8* swap_buffer) {
	u8* pivot = base + (s32)element_size * high;
	s32 store_index = low - 1;

	for (s32 i = low; i < high; ++i) {
		u8* current = base + (s32)element_size * i;
		if (cmp(current, pivot) <= 0) {
			store_index++;
			if (store_index != i) {
				QUICKSORT_swap(base + (s32)element_size * store_index,
				               current,
				               swap_buffer,
				               element_size);
			}
		}
	}

	store_index++;
	QUICKSORT_swap(base + (s32)element_size * store_index,
	               base + (s32)element_size * high,
	               swap_buffer,
	               element_size);
	return store_index;
}

static void QUICKSORT_recursive(u8* base,
                                s32 low,
                                s32 high,
                                u16 element_size,
                                QuicksortCmp cmp,
                                u8* swap_buffer) {
	while (low < high) {
		s32 pivot_index = QUICKSORT_partition(base, low, high, element_size, cmp, swap_buffer);

		if (pivot_index - low < high - pivot_index) {
			QUICKSORT_recursive(base, low, pivot_index - 1, element_size, cmp, swap_buffer);
			low = pivot_index + 1;
		}
		else {
			QUICKSORT_recursive(base, pivot_index + 1, high, element_size, cmp, swap_buffer);
			high = pivot_index - 1;
		}
	}
}

s16 QUICKSORT_sort(void* base, u32 count, u16 element_size, QuicksortCmp cmp) {
	if (NULL == base) {
		return kErrorCode_Null;
	}
	if (NULL == cmp) {
		return kErrorCode_Null;
	}
	if (0 == element_size) {
		return kErrorCode_ZeroSize;
	}
	if (count < 2) {
		return kErrorCode_Ok;
	}

	u8* swap_buffer = (u8*)MM->malloc(element_size);
	if (NULL == swap_buffer) {
		return kErrorCode_Memory;
	}

	QUICKSORT_recursive((u8*)base, 0, (s32)count - 1, element_size, cmp, swap_buffer);

	MM->free(swap_buffer);
	return kErrorCode_Ok;
}
