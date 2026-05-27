// comparative.c
// Escuela Superior de Arte y Tecnologia
// Algoritmos & Inteligencia Artificial
// ESAT 2020-2021
//
// File for comparative of ADTs

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "EDK_MemoryManager/edk_memory_manager.h"
#include "EDK_MemoryManager/edk_platform_types.h"
#include "common_def.h"
#include "adt_vector.h"
#include "adt_list.h"
#include "adt_dllist.h"

#define kComparativeSize 10000

void* data[kComparativeSize];
u16   sizes[kComparativeSize];

static LARGE_INTEGER g_frequency;
static Vector* g_vector;
static List*   g_list;
static DLList* g_dllist;
static Vector* g_vector_b;
static List*   g_list_b;
static DLList* g_dllist_b;


void TESTBASE_generateDataForComparative() {
	for (u32 i = 0; i < kComparativeSize; ++i) {
		data[i] = MM->malloc(sizeof(u32));
		*(u32*)data[i] = i;
		sizes[i] = sizeof(u32);
	}
}

void TESTBASE_freeDataForComparative() {
	for (u32 i = 0; i < kComparativeSize; ++i) {
		if (NULL != data[i]) {
			MM->free(data[i]);
			data[i] = NULL;
		}
	}
}


// Vector
static void op_vector_insertFirst (u32 i) { g_vector->ops_->insertFirst(g_vector, data[i], sizes[i]); }
static void op_vector_insertLast  (u32 i) { g_vector->ops_->insertLast (g_vector, data[i], sizes[i]); }
static void op_vector_insertAtRnd (u32 i) {
	u16 len = g_vector->ops_->length(g_vector);
	u16 pos = (0 == len) ? 0 : (u16)(rand() % len);
	g_vector->ops_->insertAt(g_vector, data[i], sizes[i], pos);
}
static void op_vector_extractFirst(u32 i) { u16 s = 0; (void)i; g_vector->ops_->extractFirst(g_vector, &s); }
static void op_vector_extractLast (u32 i) { u16 s = 0; (void)i; g_vector->ops_->extractLast (g_vector, &s); }
static void op_vector_extractAtRnd(u32 i) {
	u16 s = 0;
	(void)i;
	u16 len = g_vector->ops_->length(g_vector);
	if (0 == len) return;
	u16 pos = (u16)(rand() % len);
	g_vector->ops_->extractAt(g_vector, &s, pos);
}

// List
static void op_list_insertFirst (u32 i) { g_list->ops_->insertFirst(g_list, data[i], sizes[i]); }
static void op_list_insertLast  (u32 i) { g_list->ops_->insertLast (g_list, data[i], sizes[i]); }
static void op_list_insertAtRnd (u32 i) {
	u16 len = g_list->ops_->length(g_list);
	u16 pos = (0 == len) ? 0 : (u16)(rand() % len);
	g_list->ops_->insertAt(g_list, data[i], sizes[i], pos);
}
static void op_list_extractFirst(u32 i) { u16 s = 0; (void)i; g_list->ops_->extractFirst(g_list, &s); }
static void op_list_extractLast (u32 i) { u16 s = 0; (void)i; g_list->ops_->extractLast (g_list, &s); }
static void op_list_extractAtRnd(u32 i) {
	u16 s = 0;
	(void)i;
	u16 len = g_list->ops_->length(g_list);
	if (0 == len) return;
	u16 pos = (u16)(rand() % len);
	g_list->ops_->extractAt(g_list, &s, pos);
}

// DLList
static void op_dllist_insertFirst (u32 i) { g_dllist->ops_->insertFirst(g_dllist, data[i], sizes[i]); }
static void op_dllist_insertLast  (u32 i) { g_dllist->ops_->insertLast (g_dllist, data[i], sizes[i]); }
static void op_dllist_insertAtRnd (u32 i) {
	u16 len = g_dllist->ops_->length(g_dllist);
	u16 pos = (0 == len) ? 0 : (u16)(rand() % len);
	g_dllist->ops_->insertAt(g_dllist, data[i], sizes[i], pos);
}
static void op_dllist_extractFirst(u32 i) { u16 s = 0; (void)i; g_dllist->ops_->extractFirst(g_dllist, &s); }
static void op_dllist_extractLast (u32 i) { u16 s = 0; (void)i; g_dllist->ops_->extractLast (g_dllist, &s); }
static void op_dllist_extractAtRnd(u32 i) {
	u16 s = 0;
	(void)i;
	u16 len = g_dllist->ops_->length(g_dllist);
	if (0 == len) return;
	u16 pos = (u16)(rand() % len);
	g_dllist->ops_->extractAt(g_dllist, &s, pos);
}

typedef void (*ComparativeOp)(u32);

void calculateTimeForFunction(const char* name, ComparativeOp op, u32 repetitions) {
	LARGE_INTEGER time_start, time_end;

	QueryPerformanceCounter(&time_start);
	for (u32 rep = 0; rep < repetitions; ++rep) {
		op(rep);
	}
	QueryPerformanceCounter(&time_end);

	double elapsed_time = (double)(time_end.QuadPart - time_start.QuadPart) * 1000000.0 / (double)g_frequency.QuadPart;
	double average_time = elapsed_time / (double)repetitions;
	printf("  %-30s %12.4f ms\n", name, average_time);
}

static void RUN_internalInsertions() {
	printf("\n[ INTERNAL: insertion at head / random / tail ]\n");

	g_vector = VECTOR_create(kComparativeSize);
	calculateTimeForFunction("Vector  insertFirst", op_vector_insertFirst, kComparativeSize);
	g_vector->ops_->softReset(g_vector);
	g_vector->ops_->destroy(g_vector);

	g_list = LIST_create(kComparativeSize);
	calculateTimeForFunction("List    insertFirst", op_list_insertFirst, kComparativeSize);
	g_list->ops_->softReset(g_list);
	g_list->ops_->destroy(g_list);

	g_dllist = DLLIST_create(kComparativeSize);
	calculateTimeForFunction("DLList  insertFirst", op_dllist_insertFirst, kComparativeSize);
	g_dllist->ops_->softReset(g_dllist);
	g_dllist->ops_->destroy(g_dllist);

	g_vector = VECTOR_create(kComparativeSize);
	calculateTimeForFunction("Vector  insertAt(random)", op_vector_insertAtRnd, kComparativeSize);
	g_vector->ops_->softReset(g_vector);
	g_vector->ops_->destroy(g_vector);

	g_list = LIST_create(kComparativeSize);
	calculateTimeForFunction("List    insertAt(random)", op_list_insertAtRnd, kComparativeSize);
	g_list->ops_->softReset(g_list);
	g_list->ops_->destroy(g_list);

	g_dllist = DLLIST_create(kComparativeSize);
	calculateTimeForFunction("DLList  insertAt(random)", op_dllist_insertAtRnd, kComparativeSize);
	g_dllist->ops_->softReset(g_dllist);
	g_dllist->ops_->destroy(g_dllist);

	g_vector = VECTOR_create(kComparativeSize);
	calculateTimeForFunction("Vector  insertLast", op_vector_insertLast, kComparativeSize);
	g_vector->ops_->softReset(g_vector);
	g_vector->ops_->destroy(g_vector);

	g_list = LIST_create(kComparativeSize);
	calculateTimeForFunction("List    insertLast", op_list_insertLast, kComparativeSize);
	g_list->ops_->softReset(g_list);
	g_list->ops_->destroy(g_list);

	g_dllist = DLLIST_create(kComparativeSize);
	calculateTimeForFunction("DLList  insertLast", op_dllist_insertLast, kComparativeSize);
	g_dllist->ops_->softReset(g_dllist);
	g_dllist->ops_->destroy(g_dllist);
}

static void RUN_internalExtractions() {
	printf("\n[ INTERNAL: extraction at head / random / tail ]\n");

	g_vector = VECTOR_create(kComparativeSize);
	for (u32 i = 0; i < kComparativeSize; ++i) g_vector->ops_->insertLast(g_vector, data[i], sizes[i]);
	calculateTimeForFunction("Vector  extractFirst", op_vector_extractFirst, kComparativeSize);
	g_vector->ops_->softReset(g_vector);
	g_vector->ops_->destroy(g_vector);

	g_list = LIST_create(kComparativeSize);
	for (u32 i = 0; i < kComparativeSize; ++i) g_list->ops_->insertLast(g_list, data[i], sizes[i]);
	calculateTimeForFunction("List    extractFirst", op_list_extractFirst, kComparativeSize);
	g_list->ops_->softReset(g_list);
	g_list->ops_->destroy(g_list);

	g_dllist = DLLIST_create(kComparativeSize);
	for (u32 i = 0; i < kComparativeSize; ++i) g_dllist->ops_->insertLast(g_dllist, data[i], sizes[i]);
	calculateTimeForFunction("DLList  extractFirst", op_dllist_extractFirst, kComparativeSize);
	g_dllist->ops_->softReset(g_dllist);
	g_dllist->ops_->destroy(g_dllist);

	g_vector = VECTOR_create(kComparativeSize);
	for (u32 i = 0; i < kComparativeSize; ++i) g_vector->ops_->insertLast(g_vector, data[i], sizes[i]);
	calculateTimeForFunction("Vector  extractAt(random)", op_vector_extractAtRnd, kComparativeSize);
	g_vector->ops_->softReset(g_vector);
	g_vector->ops_->destroy(g_vector);

	g_list = LIST_create(kComparativeSize);
	for (u32 i = 0; i < kComparativeSize; ++i) g_list->ops_->insertLast(g_list, data[i], sizes[i]);
	calculateTimeForFunction("List    extractAt(random)", op_list_extractAtRnd, kComparativeSize);
	g_list->ops_->softReset(g_list);
	g_list->ops_->destroy(g_list);

	g_dllist = DLLIST_create(kComparativeSize);
	for (u32 i = 0; i < kComparativeSize; ++i) g_dllist->ops_->insertLast(g_dllist, data[i], sizes[i]);
	calculateTimeForFunction("DLList  extractAt(random)", op_dllist_extractAtRnd, kComparativeSize);
	g_dllist->ops_->softReset(g_dllist);
	g_dllist->ops_->destroy(g_dllist);

	g_vector = VECTOR_create(kComparativeSize);
	for (u32 i = 0; i < kComparativeSize; ++i) g_vector->ops_->insertLast(g_vector, data[i], sizes[i]);
	calculateTimeForFunction("Vector  extractLast", op_vector_extractLast, kComparativeSize);
	g_vector->ops_->softReset(g_vector);
	g_vector->ops_->destroy(g_vector);

	g_list = LIST_create(kComparativeSize);
	for (u32 i = 0; i < kComparativeSize; ++i) g_list->ops_->insertLast(g_list, data[i], sizes[i]);
	calculateTimeForFunction("List    extractLast", op_list_extractLast, kComparativeSize);
	g_list->ops_->softReset(g_list);
	g_list->ops_->destroy(g_list);

	g_dllist = DLLIST_create(kComparativeSize);
	for (u32 i = 0; i < kComparativeSize; ++i) g_dllist->ops_->insertLast(g_dllist, data[i], sizes[i]);
	calculateTimeForFunction("DLList  extractLast", op_dllist_extractLast, kComparativeSize);
	g_dllist->ops_->softReset(g_dllist);
	g_dllist->ops_->destroy(g_dllist);
}

static void cleanupDeepCopiesVector(Vector* v, u16 from_position) {
	u16 len = v->ops_->length(v);
	for (u16 i = from_position; i < len; ++i) {
		u16 sz = 0;
		void* deep = v->ops_->at(v, &sz, i);
		if (NULL != deep) MM->free(deep);
	}
}

static void cleanupDeepCopiesList(List* l, u16 from_position) {
	u16 len = l->ops_->length(l);
	for (u16 i = from_position; i < len; ++i) {
		u16 sz = 0;
		void* deep = l->ops_->at(l, &sz, i);
		if (NULL != deep) MM->free(deep);
	}
}

static void cleanupDeepCopiesDLList(DLList* l, u16 from_position) {
	u16 len = l->ops_->length(l);
	for (u16 i = from_position; i < len; ++i) {
		u16 sz = 0;
		void* deep = l->ops_->at(l, &sz, i);
		if (NULL != deep) MM->free(deep);
	}
}

static void RUN_externalConcat() {
	printf("\n[ EXTERNAL: concat ]\n");

	u16 half = (u16)(kComparativeSize / 2);
	LARGE_INTEGER start, end;
	double elapsed;

	g_vector   = VECTOR_create(half);
	g_vector_b = VECTOR_create((u16)(kComparativeSize - half));
	for (u32 i = 0; i < half; ++i) g_vector->ops_->insertLast(g_vector, data[i], sizes[i]);
	for (u32 i = half; i < kComparativeSize; ++i) g_vector_b->ops_->insertLast(g_vector_b, data[i], sizes[i]);

	QueryPerformanceCounter(&start);
	g_vector->ops_->concat(g_vector, g_vector_b);
	QueryPerformanceCounter(&end);
	elapsed = (double)(end.QuadPart - start.QuadPart) * 1000000.0 / (double)g_frequency.QuadPart;
	printf("  %-30s %12.4f ms\n", "Vector  concat", elapsed);

	cleanupDeepCopiesVector(g_vector, half);
	g_vector->ops_->softReset(g_vector);
	g_vector->ops_->destroy(g_vector);
	g_vector_b->ops_->softReset(g_vector_b);
	g_vector_b->ops_->destroy(g_vector_b);

	g_list   = LIST_create(half);
	g_list_b = LIST_create((u16)(kComparativeSize - half));
	for (u32 i = 0; i < half; ++i) g_list->ops_->insertLast(g_list, data[i], sizes[i]);
	for (u32 i = half; i < kComparativeSize; ++i) g_list_b->ops_->insertLast(g_list_b, data[i], sizes[i]);

	QueryPerformanceCounter(&start);
	g_list->ops_->concat(g_list, g_list_b);
	QueryPerformanceCounter(&end);
	elapsed = (double)(end.QuadPart - start.QuadPart) * 1000000.0 / (double)g_frequency.QuadPart;
	printf("  %-30s %12.4f ms\n", "List    concat", elapsed);

	cleanupDeepCopiesList(g_list, half);
	g_list->ops_->softReset(g_list);
	g_list->ops_->destroy(g_list);
	g_list_b->ops_->softReset(g_list_b);
	g_list_b->ops_->destroy(g_list_b);

	g_dllist   = DLLIST_create(half);
	g_dllist_b = DLLIST_create((u16)(kComparativeSize - half));
	for (u32 i = 0; i < half; ++i) g_dllist->ops_->insertLast(g_dllist, data[i], sizes[i]);
	for (u32 i = half; i < kComparativeSize; ++i) g_dllist_b->ops_->insertLast(g_dllist_b, data[i], sizes[i]);

	QueryPerformanceCounter(&start);
	g_dllist->ops_->concat(g_dllist, g_dllist_b);
	QueryPerformanceCounter(&end);
	elapsed = (double)(end.QuadPart - start.QuadPart) * 1000000.0 / (double)g_frequency.QuadPart;
	printf("  %-30s %12.4f ms\n", "DLList  concat", elapsed);

	cleanupDeepCopiesDLList(g_dllist, half);
	g_dllist->ops_->softReset(g_dllist);
	g_dllist->ops_->destroy(g_dllist);
	g_dllist_b->ops_->softReset(g_dllist_b);
	g_dllist_b->ops_->destroy(g_dllist_b);
}


int main(int argc, char** argv) {
	(void)argc;
	(void)argv;

	srand((unsigned)time(NULL));
	QueryPerformanceFrequency(&g_frequency);

	printf("=================== ADT COMPARATIVE ===================\n");
	printf("  Elements per measurement : %u\n", kComparativeSize);
	printf("-------------------------------------------------------\n");

	TESTBASE_generateDataForComparative();

	RUN_internalInsertions();
	RUN_internalExtractions();
	RUN_externalConcat();

	TESTBASE_freeDataForComparative();

	printf("-------------------------------------------------------\n");
	MM->status();
	printf("Press ENTER to continue\n");
	getchar();
	MM->destroy();
	return 0;
}
