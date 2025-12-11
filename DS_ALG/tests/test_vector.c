/*
 *
 * Ivan Sancho as Unreal Authorized Instructor (UAI), 2025.
 *
 * Vector unitary test.
 * Algorithms and Data Structures.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>

#include "adt_vector.h"
#include "common_def.h"
#include "EDK_MemoryManager/edk_memory_manager.h"


#define PrintTest(str) \
printf("\033[1;37m    %s\033[1;37m---------> \033[1;32m[OK]\033[1;37m\n", str);

#define PrintErrorTest(str) \
printf("\033[1;31m    %s\033[1;37m---------> \033[1;32m[OK]\033[1;37m\n", str);

#define PrintTestSimple(str) \
printf("\033[1;36m  %s\033[1;37m\n", str);

#define CheckMMUsage(q_x32, q_x64) \
  assert(4 == sizeof(void*) ? q_x32 == MM->usage() : \
	                            q_x64 == MM->usage());



#define kNumberTest01 5
#define kNumberTest02 5
#define kNumberTest03 6
#define kNumberTest04 5

typedef struct data_pack {
	void* ptr;
	u16 size;
} DataPack;

typedef struct test_data {
	DataPack pack_01[kNumberTest01];
	DataPack pack_02[kNumberTest02];
	DataPack pack_03[kNumberTest03];
	DataPack full_test_pack;
	DataPack pack_04[kNumberTest04];
} Data;


int main() {
	s16 err_code = kErrorCode_Ok;
	u32 cmp_output = 0;
	u16 length = 0;
	boolean outcome = False;
	void* output = NULL;
	u16 size = NULL;
	Data data;

	#define kVectorCapacity_01 16
	#define kVectorCapacity_01_01 18
	#define kVectorCapacity_01_02 14
	#define kVectorCapacity_02 4
	#define kVectorCapacity_03 5


	//************************************
	//********************* STATUS CHECK:
	MM->status();


	//************************************
	//********************* PREPARING DATA:
	PrintTestSimple("Preparing data for tests...");

	//032 to 512:
	u16 n = 3;
	for (u16 i = 0; i < kNumberTest01; ++i) {
		if (i == 2) {
			n = 4;
		}
		u8* text = (u8*)MM->calloc(n);
		u16 number = (u16) pow(2, 5 + i);
		sprintf(text, "%d\0", number);
		data.pack_01[i].ptr = text;
		data.pack_01[i].size = n;
	}

	//1024 to 16384:
	n = 5;
	for (u16 i = 0; i < kNumberTest02; ++i) {
		if (i == kNumberTest02 - 1) {
			n = 6;
		}
		u8* text = (u8*) MM->calloc(n);
		u16 number = (u16)pow(2, 10 + i);
		sprintf(text, "%d\0", number);
		data.pack_02[i].ptr = text;
		data.pack_02[i].size = n;
	}

	//A to BC:
	n = 1;
	u16 character = 65;
	for (u16 i = 0; i < kNumberTest03; ++i) {
		u8* text = (u8*) MM->calloc(n);
		for (u16 i = character; i < character + n; ++i) {
			sprintf(text, "%s%c", text, i);
		}
		data.pack_03[i].ptr = text;
		data.pack_03[i].size = n + 1;
		character += n;
		n++;
	}

	//Fernando Alonso:
	data.full_test_pack.ptr = MM->calloc(sizeof(u8) * 15);
	sprintf(data.full_test_pack.ptr, "Fernando Alonso");
	data.full_test_pack.size = 15;

	//Magic Nano
	{
		//0:
		u8* text = (u8*) MM->calloc(6);
		sprintf(text, "Magic\0");
		data.pack_04[0].ptr = text;
		data.pack_04[0].size = 6;

		//1:
		text = (u8*) MM->calloc(5);
		sprintf(text, "Nano\0");
		data.pack_04[1].ptr = text;
		data.pack_04[1].size = 5;

		//2:
		text = (u8*) MM->calloc(11);
		sprintf(text, "Magic Nano\0");
		data.pack_04[2].ptr = text;
		data.pack_04[2].size = 11;

		//3:
		text = (u8*) MM->calloc(23);
		sprintf(text, "Magic Nano Game Studio\0");
		data.pack_04[3].ptr = text;
		data.pack_04[3].size = 23;

		//4:
		text = (u8*) MM->calloc(28);
		sprintf(text, "Magic Nano Kabushiki Kaisha\0");
		data.pack_04[4].ptr = text;
		data.pack_04[4].size = 28;
	}

	//************************************
	//********************* STATUS CHECK:
	MM->status();


	//************************************
	//********************* CREATION TEST:
	PrintTestSimple("Creation test:");
	Vector* v = NULL;

	//Create:
	v = VECTOR_create(0);
	assert(NULL == v);
	PrintTest("Vector_create #1              ");

	v = VECTOR_create(kVectorCapacity_01);
	assert(NULL != v);
	assert(NULL != v->storage_);
	assert(NULL != v->ops_);
	assert(0 == v->head_ && 0 == v->tail_);
	assert(kVectorCapacity_01 == v->capacity_);
	for (u16 i = 0; i < kVectorCapacity_01; ++i) {
		assert(NULL == v->storage_[i].data_);
		assert(0 == v->storage_[i].size_);
		assert(NULL == v->storage_[i].next_);
		assert(NULL == v->storage_[i].prev_);
		assert(NULL != v->storage_[i].ops_);
	}
	PrintTest("Vector_create #2              ");

	Vector* v_02 = VECTOR_create(kVectorCapacity_02);
	assert(NULL != v_02);
	assert(NULL != v_02->storage_);
	assert(NULL != v_02->ops_);
	assert(0 == v_02->head_ && 0 == v_02->tail_);
	assert(kVectorCapacity_02 == v_02->capacity_);
	for (u16 i = 0; i < kVectorCapacity_02; ++i) {
		assert(NULL == v_02->storage_[i].data_);
		assert(0 == v_02->storage_[i].size_);
		assert(NULL == v_02->storage_[i].next_);
		assert(NULL == v_02->storage_[i].prev_);
		assert(NULL != v_02->storage_[i].ops_);
	}
	PrintTest("Vector_create #3              ");

	Vector* v_03 = VECTOR_create(kVectorCapacity_03);
	assert(NULL != v_03);
	assert(NULL != v_03->storage_);
	assert(NULL != v_03->ops_);
	assert(0 == v_03->head_ && 0 == v_03->tail_);
	assert(kVectorCapacity_03 == v_03->capacity_);
	for (u16 i = 0; i < kVectorCapacity_03; ++i) {
		assert(NULL == v_03->storage_[i].data_);
		assert(0 == v_03->storage_[i].size_);
		assert(NULL == v_03->storage_[i].next_);
		assert(NULL == v_03->storage_[i].prev_);
		assert(NULL != v_03->storage_[i].ops_);
	}
	PrintTest("Vector_create #4              ");


	//************************************
	//********************* INSERT FIRST TEST:
	PrintTestSimple("Insert First test:");

	err_code = v->ops_->insertFirst(NULL, NULL, 0);
	assert(kErrorCode_Null == err_code);
	PrintTest("Vector_insertFirst #1         ");

	err_code = v->ops_->insertFirst(v, NULL, 0);
	assert(kErrorCode_NullData == err_code);
	PrintTest("Vector_insertFirst #2         ");

	err_code = v->ops_->insertFirst(v, data.pack_01[0].ptr, 0);
	assert(kErrorCode_ZeroSize == err_code);
	PrintTest("Vector_insertFirst #3         ");

	for (u16 i = 0; i < kNumberTest01; ++i) {
		err_code = v->ops_->insertFirst(v, data.pack_01[i].ptr,
																		data.pack_01[i].size);
		assert(kErrorCode_Ok == err_code);

		data.pack_01[i].ptr = NULL;
		data.pack_01[i].size = 0;
		
		u8 msg[60] = { '\0' };
		sprintf(msg, "Vector_insertFirst #%d         ", 4 + i);
		PrintTest(msg);
	}

	length = v->ops_->length(v);
	assert(5 == length);
	PrintTest("Vector_length #1              ");

	//************************************
	//********************* INSERT FIRST DATA CHECKS TEST:
	PrintTestSimple("Insert First data checks test:");
	u8 q_00[4] = "512\0";
	u8 q_01[4] = "256\0";
	u8 q_02[4] = "128\0";
	u8 q_03[3] = "64\0";
	u8 q_04[3] = "32\0";

	cmp_output = strncmp(v->storage_[0].data_, q_00, 4);
	assert(0 == cmp_output);
	PrintTest("Vector_insertFirst #Check 0   ");

	cmp_output = strncmp(v->storage_[1].data_, q_01, 4);
	assert(0 == cmp_output);
	PrintTest("Vector_insertFirst #Check 1   ");

	cmp_output = strncmp(v->storage_[2].data_, q_02, 4);
	assert(0 == cmp_output);
	PrintTest("Vector_insertFirst #Check 2   ");

	cmp_output = strncmp(v->storage_[3].data_, q_03, 3);
	assert(0 == cmp_output);
	PrintTest("Vector_insertFirst #Check 3   ");

	cmp_output = strncmp(v->storage_[4].data_, q_04, 3);
	assert(0 == cmp_output);
	PrintTest("Vector_insertFirst #Check 4   ");


	//************************************
	//********************* INSERT LAST TEST #1:
	PrintTestSimple("Insert Last test #1:");

	err_code = v->ops_->insertLast(NULL, NULL, 0);
	assert(kErrorCode_Null == err_code);
	PrintTest("Vector_insertLast #1          ");

	err_code = v->ops_->insertLast(v, NULL, 0);
	assert(kErrorCode_NullData == err_code);
	PrintTest("Vector_insertLast #2          ");

	err_code = v->ops_->insertLast(v, data.pack_02[0].ptr, 0);
	assert(kErrorCode_ZeroSize == err_code);
	PrintTest("Vector_insertLast #3          ");

	for (u16 i = 0; i < kNumberTest02; ++i) {
		err_code = v->ops_->insertLast(v, data.pack_02[i].ptr,
																	 data.pack_02[i].size);
		assert(kErrorCode_Ok == err_code);

		data.pack_02[i].ptr = NULL;
		data.pack_02[i].size = 0;

		u8 msg[60] = { '\0' };
		sprintf(msg, "Vector_insertLast #%d          ", 4 + i);
		PrintTest(msg);
	}

	length = v->ops_->length(v);
	assert(10 == length);
	PrintTest("Vector_length #2              ");

	//************************************
	//********************* INSERT LAST DATA CHECKS TEST:
	PrintTestSimple("Insert Last data checks test:");
	u8 q_05[5] = "1024\0";
	u8 q_06[5] = "2048\0";
	u8 q_07[5] = "4096\0";
	u8 q_08[5] = "8192\0";
	u8 q_09[6] = "16384\0";

	cmp_output = strncmp(v->storage_[5].data_, q_05, 5);
	assert(0 == cmp_output);
	PrintTest("Vector_insertLast #Check 0    ");

	cmp_output = strncmp(v->storage_[6].data_, q_06, 5);
	assert(0 == cmp_output);
	PrintTest("Vector_insertLast #Check 1    ");

	cmp_output = strncmp(v->storage_[7].data_, q_07, 5);
	assert(0 == cmp_output);
	PrintTest("Vector_insertLast #Check 2    ");

	cmp_output = strncmp(v->storage_[8].data_, q_08, 5);
	assert(0 == cmp_output);
	PrintTest("Vector_insertLast #Check 3    ");

	cmp_output = strncmp(v->storage_[9].data_, q_09, 6);
	assert(0 == cmp_output);
	PrintTest("Vector_insertLast #Check 4    ");


	//************************************
	//********************* INSERT AT TEST:
	PrintTestSimple("Insert At test:");

	err_code = v->ops_->insertAt(NULL, NULL, 0, 0);
	assert(kErrorCode_Null == err_code);
	PrintTest("Vector_insertAt #1            ");

	err_code = v->ops_->insertAt(v, NULL, 0, 0);
	assert(kErrorCode_NullData == err_code);
	PrintTest("Vector_insertAt #2            ");

	err_code = v->ops_->insertAt(v, data.pack_03[0].ptr, 0, 0);
	assert(kErrorCode_ZeroSize == err_code);
	PrintTest("Vector_insertAt #3            ");

	for (u16 i = 0; i < kNumberTest03 - 1; ++i) {
		err_code = v->ops_->insertAt(v, data.pack_03[i].ptr,
																 data.pack_03[i].size, 5);
		assert(kErrorCode_Ok == err_code);

		data.pack_03[i].ptr = NULL;
		data.pack_03[i].size = 0;

		u8 msg[60] = { '\0' };
		sprintf(msg, "Vector_insertAt #%d            ", 4 + i);
		PrintTest(msg);
	}
	
	//A kind of insert last exceding the tail...
	err_code = v->ops_->insertAt(v, data.pack_03[5].ptr,
															 data.pack_03[5].size, 30);
	assert(kErrorCode_Ok == err_code);

	data.pack_03[5].ptr = NULL;
	data.pack_03[5].size = 0;

	length = v->ops_->length(v);
	assert(16 == length);
	PrintTest("Vector_length #3              ");

	//************************************
	//********************* INSERT AT DATA CHECKS TEST:
	PrintTestSimple("Insert At data checks test:");
	u8 q_10[2] = "A\0";
	u8 q_11[3] = "BC\0";
	u8 q_12[4] = "DEF\0";
	u8 q_13[5] = "GHIJ\0";
	u8 q_14[6] = "KLMNO\0";
	u8 q_15[7] = "PQRSTU\0";

	cmp_output = strncmp(v->storage_[5].data_, q_14, 6);
	assert(0 == cmp_output);
	PrintTest("Vector_insertAt #Check 0      ");

	cmp_output = strncmp(v->storage_[6].data_, q_13, 5);
	assert(0 == cmp_output);
	PrintTest("Vector_insertAt #Check 1      ");

	cmp_output = strncmp(v->storage_[7].data_, q_12, 4);
	assert(0 == cmp_output);
	PrintTest("Vector_insertAt #Check 2      ");

	cmp_output = strncmp(v->storage_[8].data_, q_11, 3);
	assert(0 == cmp_output);
	PrintTest("Vector_insertAt #Check 3      ");

	cmp_output = strncmp(v->storage_[9].data_, q_10, 2);
	assert(0 == cmp_output);
	PrintTest("Vector_insertAt #Check 4      ");

	cmp_output = strncmp(v->storage_[15].data_, q_15, 7);
	assert(0 == cmp_output);
	PrintTest("Vector_insertAt #Check 5      ");


	//************************************
	//********************* FULL TEST CHECKS TEST:
	PrintTestSimple("Insert when full test:");

	err_code = v->ops_->insertFirst(v, data.full_test_pack.ptr, 
																	data.full_test_pack.size);
	assert(kErrorCode_IsFull == err_code);
	PrintTest("Vector_insertFirst full #1    ");

	err_code = v->ops_->insertLast(v, data.full_test_pack.ptr, 
																 data.full_test_pack.size);
	assert(kErrorCode_IsFull == err_code);
	PrintTest("Vector_insertLast full #2     ");

	err_code = v->ops_->insertAt(v, data.full_test_pack.ptr, 
															 data.full_test_pack.size, 6);
	assert(kErrorCode_IsFull == err_code);
	PrintTest("Vector_insertAt full #3       ");
	

	//************************************
	//********************* STATUS CHECK:
	CheckMMUsage(701, 1225)
	MM->status();


	//************************************
	//********************* PRINT TEST:
	PrintTestSimple("Print test #1:");
	PrintTestSimple("    Vector #0:");
	v->ops_->print(NULL);

	PrintTestSimple("    Vector #1:");
	v->ops_->print(v);


	//************************************
	//********************* CAPACITY TEST:
	PrintTestSimple("Capacity test:");

	length = v->ops_->capacity(NULL);
	assert(0 == length);
	PrintTest("Vector_capacity #1            ");

	length = v->ops_->capacity(v);
	assert(kVectorCapacity_01 == length);
	PrintTest("Vector_capacity #2            ");

	length = v->ops_->capacity(v_02);
	assert(kVectorCapacity_02 == length);
	PrintTest("Vector_capacity #3            ");


	//************************************
	//********************* LENGTH TEST:
	PrintTestSimple("Length test:");

	length = v->ops_->length(NULL);
	assert(0 == length);
	PrintTest("Vector_length #4              ");

	length = v->ops_->length(v);
	assert(16 == length);
	PrintTest("Vector_length #5              ");

	length = v->ops_->length(v_02);
	assert(0 == length);
	PrintTest("Vector_length #6              ");


	//************************************
	//********************* IS EMPTY TEST:
	PrintTestSimple("Is Empty test:");

	outcome = v->ops_->isEmpty(NULL);
	assert(False == outcome);
	PrintTest("Vector_isEmpty #1             ");

	outcome = v->ops_->isEmpty(v);
	assert(False == outcome);
	PrintTest("Vector_isEmpty #2             ");

	outcome = v->ops_->isEmpty(v_02);
	assert(True == outcome);
	PrintTest("Vector_isEmpty #3             ");


	//************************************
	//********************* IS FULL TEST:
	PrintTestSimple("Is Full test:");

	outcome = v->ops_->isFull(NULL);
	assert(False == outcome);
	PrintTest("Vector_isFull #1              ");

	outcome = v->ops_->isFull(v);
	assert(True == outcome);
	PrintTest("Vector_isFull #2              ");

	outcome = v->ops_->isFull(v_02);
	assert(False == outcome);
	PrintTest("Vector_isFull #3              ");


	//************************************
	//********************* FIRST TEST:
	PrintTestSimple("First test:");

	output = v->ops_->first(NULL, NULL);
	assert(NULL == output);
	PrintTest("Vector_first #1               ");

	output = v->ops_->first(v, NULL);
	assert(NULL == output);
	PrintTest("Vector_first #2               ");

	output = v->ops_->first(v, &size);
	cmp_output = strncmp(output, q_00, 4);
	assert(NULL != output);
	assert(0 == cmp_output);
	assert(4 == size);
	PrintTest("Vector_first #3               ");

	size = 0;
	output = v->ops_->first(v_02, &size);
	assert(NULL == output);
	assert(0 == size);
	PrintTest("Vector_first #4               ");


	//************************************
	//********************* LAST TEST:
	PrintTestSimple("Last test:");

	output = v->ops_->last(NULL, NULL);
	assert(NULL == output);
	PrintTest("Vector_last #1                ");

	output = v->ops_->last(v, NULL);
	assert(NULL == output);
	PrintTest("Vector_last #2                ");

	size = 0;
	output = v->ops_->last(v, &size);
	cmp_output = strncmp(output, q_15, 4);
	assert(NULL != output);
	assert(0 == cmp_output);
	assert(7 == size);
	PrintTest("Vector_last #3                ");

	size = 0;
	output = v->ops_->last(v_02, &size);
	assert(NULL == output);
	assert(0 == size);
	PrintTest("Vector_last #4                ");


	//************************************
	//********************* AT TEST:
	PrintTestSimple("At test:");

	output = v->ops_->at(NULL, NULL, 0);
	assert(NULL == output);
	PrintTest("Vector_at #1                  ");

	output = v->ops_->at(NULL, NULL, 17);
	assert(NULL == output);
	PrintTest("Vector_at #2                  ");

	output = v->ops_->at(v, NULL, 0);
	assert(NULL == output);
	PrintTest("Vector_at #3                  ");

	output = v->ops_->at(v, &size, 0);
	cmp_output = strncmp(output, q_00, 4);
	assert(0 == cmp_output);
	assert(4 == size);
	PrintTest("Vector_at #4                  ");

	size = 0;
	output = v->ops_->at(v, &size, 5);
	cmp_output = strncmp(output, q_14, 4);
	assert(0 == cmp_output);
	assert(6 == size);
	PrintTest("Vector_at #5                  ");

	size = 0;
	output = v->ops_->at(v, &size, 16);
	assert(NULL == output);
	PrintTest("Vector_at #6                  ");

	size = 0;
	output = v->ops_->last(v_02, &size);
	assert(NULL == output);
	assert(0 == size);
	PrintTest("Vector_at #7                  ");


	//************************************
	//********************* EXTRACT FIRST TEST:
	PrintTestSimple("Extract First test:");

	output = v->ops_->extractFirst(NULL, NULL);
	assert(NULL == output);
	PrintTest("Vector_extractFirst #1        ");

	output = v->ops_->extractFirst(v, NULL);
	assert(NULL == output);
	PrintTest("Vector_extractFirst #2        ");

	output = v->ops_->extractFirst(v_02, size);
	assert(NULL == output);
	PrintTest("Vector_extractFirst #3        ");

	for (u16 i = 0; i < kNumberTest01; ++i) {
		DataPack* tmp = &data.pack_01[kNumberTest01 - i - 1];
		tmp->ptr = v->ops_->extractFirst(v, &tmp->size);
		assert(NULL != tmp->ptr);

		u8 msg[60] = { '\0' };
		sprintf(msg, "Vector_extractFirst #%d        ", 4 + i);
		PrintTest(msg);
	}

	length = v->ops_->length(v);
	assert(11 == length);
	PrintTest("Vector_length #7              ");

	//************************************
	//********************* EXTRACT FIRST DATA CHECKS TEST:
	PrintTestSimple("Extract First data checks test:");

	cmp_output = strncmp(data.pack_01[0].ptr, q_04, 3);
	assert(0 == cmp_output);
	assert(3 == data.pack_01[0].size);
	PrintTest("Vector_insertFirst #Check 0   ");

	cmp_output = strncmp(data.pack_01[1].ptr, q_03, 3);
	assert(0 == cmp_output);
	assert(3 == data.pack_01[1].size);
	PrintTest("Vector_insertFirst #Check 1   ");

	cmp_output = strncmp(data.pack_01[2].ptr, q_02, 4);
	assert(0 == cmp_output);
	assert(4 == data.pack_01[2].size);
	PrintTest("Vector_insertFirst #Check 2   ");

	cmp_output = strncmp(data.pack_01[3].ptr, q_01, 4);
	assert(0 == cmp_output);
	assert(4 == data.pack_01[3].size);
	PrintTest("Vector_insertFirst #Check 3   ");

	cmp_output = strncmp(data.pack_01[4].ptr, q_00, 4);
	assert(0 == cmp_output);
	assert(4 == data.pack_01[4].size);
	PrintTest("Vector_insertFirst #Check 4   ");


	//************************************
	//********************* EXTRACT LAST TEST:
	PrintTestSimple("Extract Last test:");

	output = v->ops_->extractLast(NULL, NULL);
	assert(NULL == output);
	PrintTest("Vector_extractLast #1         ");

	output = v->ops_->extractLast(v, NULL);
	assert(NULL == output);
	PrintTest("Vector_extractLast #2         ");

	output = v->ops_->extractLast(v_02, size);
	assert(NULL == output);
	PrintTest("Vector_extractLast #3         ");

	{
		DataPack* tmp = &data.pack_03[kNumberTest03 - 1];
		tmp->ptr = v->ops_->extractLast(v, &tmp->size);
		assert(NULL != tmp->ptr);
		assert(7 == tmp->size);
		PrintTest("Vector_extractLast #4         ");
	}

	for (u16 i = 0; i < kNumberTest02; ++i) {
		DataPack* tmp = &data.pack_02[kNumberTest02 - i - 1];
		tmp->ptr = v->ops_->extractLast(v, &tmp->size);
		assert(NULL != tmp->ptr);

		u8 msg[60] = { '\0' };
		sprintf(msg, "Vector_extractLast #%d         ", 5 + i);
		PrintTest(msg);
	}

	length = v->ops_->length(v);
	assert(5 == length);
	PrintTest("Vector_length #8              ");

	//************************************
	//********************* EXTRACT LAST DATA CHECKS TEST:
	PrintTestSimple("Extract Last data checks test:");

	cmp_output = strncmp(data.pack_02[0].ptr, q_05, 5);
	assert(0 == cmp_output);
	assert(5 == data.pack_02[0].size);
	PrintTest("Vector_extractLast #Check 0   ");

	cmp_output = strncmp(data.pack_02[1].ptr, q_06, 5);
	assert(0 == cmp_output);
	assert(5 == data.pack_02[1].size);
	PrintTest("Vector_extractLast #Check 1   ");

	cmp_output = strncmp(data.pack_02[2].ptr, q_07, 5);
	assert(0 == cmp_output);
	assert(5 == data.pack_02[2].size);
	PrintTest("Vector_extractLast #Check 2   ");

	cmp_output = strncmp(data.pack_02[3].ptr, q_08, 5);
	assert(0 == cmp_output);
	assert(5 == data.pack_02[3].size);
	PrintTest("Vector_extractLast #Check 3   ");

	cmp_output = strncmp(data.pack_02[4].ptr, q_09, 6);
	assert(0 == cmp_output);
	assert(6 == data.pack_02[4].size);
	PrintTest("Vector_extractLast #Check 4   ");

	cmp_output = strncmp(data.pack_03[5].ptr, q_15, 7);
	assert(0 == cmp_output);
	assert(7 == data.pack_03[5].size);
	PrintTest("Vector_extractLast #Check 5   ");


	//************************************
	//********************* EXTRACT AT TEST:
	PrintTestSimple("Extract At test:");

	output = v->ops_->extractAt(NULL, NULL, 0);
	assert(NULL == output);
	PrintTest("Vector_extractAt #1           ");

	output = v->ops_->extractAt(v, NULL, 0);
	assert(NULL == output);
	PrintTest("Vector_extractAt #2           ");

	output = v->ops_->extractAt(v_02, size, 1);
	assert(NULL == output);
	PrintTest("Vector_extractAt #3           ");

	output = v->ops_->extractAt(v, size, 8);
	assert(NULL == output);
	PrintTest("Vector_extractAt #4           ");

	for (u16 i = 1; i < kNumberTest03 - 1; ++i) {
		DataPack* tmp = &data.pack_03[kNumberTest03 - i - 2];
		tmp->ptr = v->ops_->extractAt(v, &tmp->size, 1);
		assert(NULL != tmp->ptr);
		assert(6 - i == tmp->size);
		u8 msg[60] = { '\0' };
		sprintf(msg, "Vector_extractAt #%d           ", 4 + i);
		PrintTest(msg);
	}

	{
		DataPack* tmp = &data.pack_03[4];
		tmp->ptr = v->ops_->extractAt(v, &tmp->size, 0);
		assert(NULL != tmp->ptr);
		assert(6 == tmp->size);
		PrintTest("Vector_extractAt #9           ");
	}

	length = v->ops_->length(v);
	assert(0 == length);
	PrintTest("Vector_length #9              ");

	outcome = v->ops_->isEmpty(NULL);
	assert(False == outcome);
	PrintTest("Vector_isEmpty #4             ");

	length = v->ops_->capacity(v);
	assert(kVectorCapacity_01 == length);
	PrintTest("Vector_capacity #4            ");


	//************************************
	//********************* STATUS CHECK:
	CheckMMUsage(701, 1225)
	MM->status();


	//************************************
	//********************* PRINT TEST:
	PrintTestSimple("Print test #2:");
	PrintTestSimple("    Vector #1:");
	v->ops_->print(v);
	PrintTestSimple("    Vector #2:");
	v->ops_->print(v_02);
	PrintTestSimple("    Vector #3:");
	v->ops_->print(v_03);


	//************************************
	//********************* INSERT LAST TEST #2:
	PrintTestSimple("Insert Last test #2:");
	for (u16 i = 0; i < kNumberTest04; ++i) {
		if (i < 3) {
			err_code = v->ops_->insertLast(v_02, data.pack_04[i].ptr,
																		 data.pack_04[i].size);
		}
		else {
			err_code = v->ops_->insertLast(v_03, data.pack_04[i].ptr,
																		 data.pack_04[i].size);
		}

		data.pack_04[i].ptr = NULL;
		data.pack_04[i].size = 0;

		u8 msg[60] = { '\0' };
		sprintf(msg, "Vector_insertLast #%02d         ", i);
		PrintTest(msg);
	}

	//Reinserting data to the first vector, part 01:
	for (u16 i = 0; i < kNumberTest01; ++i) {
		err_code = v->ops_->insertLast(v, data.pack_01[i].ptr,
																		data.pack_01[i].size);
		data.pack_01[i].ptr = NULL;
		data.pack_01[i].size = 0;

		u8 msg[60] = { '\0' };
		sprintf(msg, "Vector_insertLast #%02d         ", kNumberTest04 + i);
		PrintTest(msg);
	}

	//Reinserting data to the first vector, part 02:
	for (u16 i = 0; i < kNumberTest02; ++i) {
		err_code = v->ops_->insertLast(v, data.pack_02[i].ptr,
																		data.pack_02[i].size);
		data.pack_02[i].ptr = NULL;
		data.pack_02[i].size = 0;

		u8 msg[60] = { '\0' };
		sprintf(msg, "Vector_insertLast #%02d         ", kNumberTest04 + 
						kNumberTest01 + i);
		PrintTest(msg);
	}

	//Reinserting data to the first vector, part 03:
	for (u16 i = 0; i < kNumberTest03; ++i) {
		err_code = v->ops_->insertLast(v, data.pack_03[i].ptr,
																		data.pack_03[i].size);
		data.pack_03[i].ptr = NULL;
		data.pack_03[i].size = 0;

		u8 msg[60] = { '\0' };
		sprintf(msg, "Vector_insertLast #%02d         ", kNumberTest04 + 
						kNumberTest01 + kNumberTest02 + i);
		PrintTest(msg);
	}
	

	//************************************
	//********************* TRAVERSE TEST:
	PrintTestSimple("Traverse test:");

	err_code = v->ops_->traverse(NULL, v->storage_->ops_->print);
	assert(kErrorCode_Null == err_code);
	PrintTest("Vector_traverse #1            ");

	err_code = v->ops_->traverse(v_02, NULL);
	assert(kErrorCode_Null == err_code);
	PrintTest("Vector_traverse #2            ");

	err_code = v->ops_->traverse(v_03, v->storage_->ops_->print);
	assert(kErrorCode_Ok == err_code);
	PrintTest("Vector_traverse #3            ");


	//************************************
	//********************* CONCAT TEST:
	PrintTestSimple("Concat test:");

	err_code = v->ops_->concat(NULL, v_03);
	assert(kErrorCode_Null == err_code);
	PrintTest("Vector_concat #1              ");

	err_code = v->ops_->concat(v_02, NULL);
	assert(kErrorCode_Null == err_code);
	PrintTest("Vector_concat #2              ");

	u8 mn_01[6] = "Magic\0";
	u8 mn_02[5] = "Nano\0";
	u8 mn_03[11] = "Magic Nano\0";
	u8 mn_04[23] = "Magic Nano Game Studio\0";
	u8 mn_05[28] = "Magic Nano Kabushiki Kaisha\0";

	err_code = v->ops_->concat(v_02, v_03);
	assert(kErrorCode_Ok == err_code);
	assert(0 == v_02->head_);
	assert(5 == v_02->tail_);
	assert(9 == v_02->capacity_);
	cmp_output = strncmp(v_02->storage_[0].data_, mn_01, 6);
	assert(0 == cmp_output);
	cmp_output = strncmp(v_02->storage_[1].data_, mn_02, 5);
	assert(0 == cmp_output);
	cmp_output = strncmp(v_02->storage_[2].data_, mn_03, 11);
	assert(0 == cmp_output);
	cmp_output = strncmp(v_02->storage_[3].data_, mn_04, 23);
	assert(0 == cmp_output);
	assert(v_02->storage_[3].data_ != v_02->storage_[0].data_);
	cmp_output = strncmp(v_02->storage_[4].data_, mn_05, 28);
	assert(0 == cmp_output);
	assert(v_02->storage_[4].data_ != v_02->storage_[1].data_);
	PrintTest("Vector_concat #3              ");


	//************************************
	//********************* RESIZE TEST:
	PrintTestSimple("Resize test:");

	err_code = v->ops_->resize(NULL, 0);
	assert(kErrorCode_Null == err_code);
	PrintTest("Vector_resize #1              ");

	err_code = v->ops_->resize(v, 0);
	assert(kErrorCode_ZeroSize == err_code);
	PrintTest("Vector_resize #2              ");

	err_code = v->ops_->resize(v, v->capacity_);
	assert(kErrorCode_Ok == err_code);
	PrintTest("Vector_resize #3              ");

	CheckMMUsage(852, 1476)

	//Precatching data for comparison:
	void* storage_addr = v->storage_;
	void* addr_data[kVectorCapacity_01];
	for (u16 i = 0; i < kVectorCapacity_01; ++i) {
		addr_data[i] = v->storage_[i].data_;
	}

	err_code = v->ops_->resize(v, kVectorCapacity_01_01);
	assert(kErrorCode_Ok == err_code);
	assert(storage_addr != v->storage_);
	assert(NULL != v->ops_);
	assert(kVectorCapacity_01_01 == v->capacity_);
	assert(0 == v->head_);
	assert(kVectorCapacity_01 == v->tail_);
	for (u16 i = 0; i < kVectorCapacity_01; ++i) {
		assert(v->storage_[i].data_ == addr_data[i]);
		assert(v->storage_[i].ops_);
	}
	for (u16 i = kVectorCapacity_01; i < kVectorCapacity_01_01; ++i) {
		assert(v->storage_[i].data_ == NULL);
		assert(v->storage_[i].ops_);
	}
	PrintTest("Vector_resize #4              ");

	CheckMMUsage(892, 1556)

	storage_addr = v->storage_;

	err_code = v->ops_->resize(v, kVectorCapacity_01_02);
	assert(kErrorCode_Ok == err_code);
	assert(storage_addr != v->storage_);
	assert(NULL != v->ops_);
	assert(kVectorCapacity_01_02 == v->capacity_);
	assert(0 == v->head_);
	assert(kVectorCapacity_01_02 == v->tail_);
	for (u16 i = 0; i < kVectorCapacity_01_02; ++i) {
		assert(v->storage_[i].data_ == addr_data[i]);
		assert(v->storage_[i].ops_);
	}
	PrintTest("Vector_resize #5              ");

	CheckMMUsage(801, 1385)


	//************************************
	//********************* RESET TEST:
	PrintTestSimple("Reset test:");

	err_code = v->ops_->reset(NULL);
	assert(kErrorCode_Null == err_code);
	PrintTest("Vector_resize #1              ");

	storage_addr = v->storage_;

	err_code = v->ops_->reset(v);
	assert(kErrorCode_Ok == err_code);
	assert(0 == v->head_);
	assert(0 == v->tail_);
	assert(kVectorCapacity_01_02 == v->capacity_);
	assert(NULL != v->ops_);
	assert(storage_addr == v->storage_);
	PrintTest("Vector_resize #2              ");

	CheckMMUsage(747, 1331)


	//************************************
	//********************* SOFTRESET TEST:
	PrintTestSimple("Soft Reset test:");

	err_code = v->ops_->insertLast(v, data.full_test_pack.ptr, 
											           data.full_test_pack.size);
	assert(kErrorCode_Ok == err_code);
	assert(0 == v->head_);
	assert(1 == v->tail_);
	assert(NULL != v->storage_[0].data_);
	assert(data.full_test_pack.ptr == v->storage_[0].data_);
	assert(kVectorCapacity_01_02 == v->capacity_);

	err_code = v->ops_->softReset(NULL);
	assert(kErrorCode_Null == err_code);
	PrintTest("Vector_softReset #1           ");

	err_code = v->ops_->softReset(v);
	assert(kErrorCode_Ok == err_code);
	assert(0 == v->head_);
	assert(0 == v->tail_);
	assert(NULL == v->storage_[0].data_);
	assert(kVectorCapacity_01_02 == v->capacity_);
	PrintTest("Vector_softReset #2           ");

	MM->free(data.full_test_pack.ptr);
	data.full_test_pack.ptr = NULL;
	data.full_test_pack.size = 0;

	CheckMMUsage(732, 1316)


	//************************************
	//********************* DESTROY TEST:
	PrintTestSimple("Destroy test:");

	err_code = v->ops_->destroy(NULL);
	assert(kErrorCode_Null == err_code);
	PrintTest("Vector_destroy #1             ");

	err_code = v->ops_->destroy(v);
	assert(kErrorCode_Ok == err_code);
	PrintTest("Vector_destroy #2             ");

	CheckMMUsage(436, 732)

	err_code = v_02->ops_->destroy(v_02);
	assert(kErrorCode_Ok == err_code);
	PrintTest("Vector_destroy #3             ");

	CheckMMUsage(167, 275)

	err_code = v_03->ops_->destroy(v_03);
	assert(kErrorCode_Ok == err_code);
	PrintTest("Vector_destroy #4             ");

	CheckMMUsage(0, 0)


	//************************************
	//********************* STATUS CHECK:
	MM->status();


	//************************************
	//********************* DESTROY MM:
	PrintTestSimple("Destroying Memory Manager...");
	MM->destroy();

  return 0;
}
