/*
 * Ivan Sancho as Unreal Authorized Instructor (UAI), 2025.
 *
 * Quicksort demo and time measurements.
 * Algorithms and Data Structures.
 *
 * Student:
 *   Pau Alvarez Belenguer
 *
 */

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "common_def.h"
#include "algo_quicksort.h"

#include "EDK_MemoryManager/edk_memory_manager.h"

static LARGE_INTEGER g_frequency;

static s32 SORTING_compareS32(const void* lhs, const void* rhs) {
	s32 left = *(const s32*)lhs;
	s32 right = *(const s32*)rhs;
	if (left < right) return -1;
	if (left > right) return 1;
	return 0;
}

static void SORTING_fillRandom(s32* values, u32 count) {
	for (u32 i = 0; i < count; ++i) {
		values[i] = rand() % 100000;
	}
}

static void SORTING_fillSorted(s32* values, u32 count) {
	for (u32 i = 0; i < count; ++i) {
		values[i] = (s32)i;
	}
}

static void SORTING_fillReverse(s32* values, u32 count) {
	for (u32 i = 0; i < count; ++i) {
		values[i] = (s32)(count - 1 - i);
	}
}

static double SORTING_measureSort(s32* values, u32 count) {
	LARGE_INTEGER start, end;
	QueryPerformanceCounter(&start);
	QUICKSORT_sort(values, count, sizeof(s32), SORTING_compareS32);
	QueryPerformanceCounter(&end);
	return (double)(end.QuadPart - start.QuadPart) * 1000000.0 / (double)g_frequency.QuadPart;
}

static void SORTING_printArray(const s32* values, u32 count) {
	for (u32 i = 0; i < count; ++i) {
		printf("%d ", values[i]);
	}
	printf("\n");
}

static void Demo() {
	u32 demo_count = 32;
	s32* values = (s32*)MM->malloc(sizeof(s32) * demo_count);
	if (NULL == values) return;

	SORTING_fillRandom(values, demo_count);
	printf("Unsorted: ");
	SORTING_printArray(values, demo_count);

	QUICKSORT_sort(values, demo_count, sizeof(s32), SORTING_compareS32);
	printf("  Sorted: ");
	SORTING_printArray(values, demo_count);

	MM->free(values);
}

static void RUN_timeMeasurements() {
	u32 test_sizes[] = { 100, 1000, 5000, 10000 };
	u32 num_sizes = sizeof(test_sizes) / sizeof(test_sizes[0]);
	u32 repetitions = 10;

	printf("\n============= SORTING TIME MEASUREMENTS ================\n");
	printf("  Repetitions per measurement : %u (average shown)\n", repetitions);
	printf("-------------------------------------------------------\n");

	for (u32 s = 0; s < num_sizes; ++s) {
		u32 count = test_sizes[s];
		s32* values = (s32*)MM->malloc(sizeof(s32) * count);
		if (NULL == values) continue;

		printf("\n  [ n = %u ]\n", count);

		// Random
		{
			double total = 0.0;
			for (u32 r = 0; r < repetitions; ++r) {
				SORTING_fillRandom(values, count);
				total += SORTING_measureSort(values, count);
			}
			printf("    %-26s %12.4f ms\n", "Quicksort (random)", total / (double)repetitions);
		}

		// Already sorted
		{
			double total = 0.0;
			for (u32 r = 0; r < repetitions; ++r) {
				SORTING_fillSorted(values, count);
				total += SORTING_measureSort(values, count);
			}
			printf("    %-26s %12.4f ms\n", "Quicksort (sorted)", total / (double)repetitions);
		}

		// Reverse sorted
		{
			double total = 0.0;
			for (u32 r = 0; r < repetitions; ++r) {
				SORTING_fillReverse(values, count);
				total += SORTING_measureSort(values, count);
			}
			printf("    %-26s %12.4f ms\n", "Quicksort (reverse)", total / (double)repetitions);
		}

		MM->free(values);
	}
}

int main(int argc, char** argv) {
	(void)argc;
	(void)argv;

	srand((unsigned)time(NULL));
	QueryPerformanceFrequency(&g_frequency);

	printf("=============== SORTING ALGORITHMS =====================\n");

	Demo();
	RUN_timeMeasurements();

	printf("-------------------------------------------------------\n");
	MM->status();
	printf("Press ENTER to continue\n");
	getchar();
	MM->destroy();
	return 0;
}
