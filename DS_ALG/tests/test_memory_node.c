/*
 *
 * Ivan Sancho as Unreal Authorized Instructor (UAI), 2025.
 * 
 * Memory Node unitary test.
 * Algorithms and Data Structures.
 * 
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "common_def.h"
#include "adt_memory_node.h"
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


int main(int argc, char** argv) {
	s16 err_code = kErrorCode_Ok;
	u8* output = NULL;
	u16 size = 0;
	u32 cmp_output = 0;

	MM->status();

	//************************************
	//********************* CREATION TEST:
	PrintTestSimple("Creation test:");
	assert(0 == MM->usage());

	//Create:
	MemoryNode* mn_01 = MEMNODE_create();
	assert(mn_01);
	assert(NULL == mn_01->data_ && 0 == mn_01->size_);
	assert(NULL != mn_01->ops_);
	assert(NULL == mn_01->next_ && NULL == mn_01->prev_);
	CheckMMUsage(20, 40)
	PrintTest("MemNode_create             ");

	//CreateLite:
	MemoryNode* mn_02 = NULL;
	err_code = MEMNODE_createLite(mn_02);
	assert(kErrorCode_Null == err_code);
	PrintTest("MemNode_createLite #1      ");

	mn_02 = (MemoryNode*) MM->malloc(sizeof(MemoryNode));
	err_code = MEMNODE_createLite(mn_02);
	assert(kErrorCode_Ok == err_code);
	assert(mn_02);
	assert(NULL == mn_02->data_ && 0 == mn_02->size_);
	assert(NULL != mn_02->ops_);
	assert(NULL == mn_02->next_ && NULL == mn_02->prev_);
	CheckMMUsage(40, 80)
	PrintTest("MemNode_createLite #2      ");

	//CreateFromRef:
	err_code = MEMNODE_createFromRef(NULL);
	assert(kErrorCode_Null == err_code);
	PrintTest("MemNode_createFromRef #1   ");

	err_code = MEMNODE_createFromRef(&mn_02);
	assert(kErrorCode_AlreadyInitialized == err_code);
	PrintTest("MemNode_createFromRef #2   ");

	MemoryNode* mn_03 = NULL;
	err_code = MEMNODE_createFromRef(&mn_03);
	assert(NULL != mn_03);
	assert(kErrorCode_Memory != err_code);
	assert(kErrorCode_Ok == err_code);
	assert(NULL == mn_03->data_ && 0 == mn_03->size_);
	assert(NULL != mn_03->ops_);
	assert(NULL == mn_03->next_ && NULL == mn_03->prev_);
	CheckMMUsage(60, 120)
	PrintTest("MemNode_createFromRef #3   ");

	MemoryNode* mn_04 = NULL;


	//************************************
	//********************* DATA TEST:
	PrintTestSimple("Data test:");

	u32 len_01 = 4;
	u8* data_01 = MM->malloc(sizeof(u8) * len_01);
	memcpy(data_01, "MNGS", len_01);

	u32 len_02 = 10;
	u8* data_02 = MM->malloc(sizeof(u8) * len_02);
	memcpy(data_02, "magic nano", len_02);

	u32 len_03 = 14;
	u8* data_03 = MM->malloc(sizeof(u8) * len_03);
	memcpy(data_03, "fernandoalonso", len_03);

	CheckMMUsage(88, 148)

	//Set data:
	err_code = mn_01->ops_->setData(NULL, data_01, 0);
	assert(kErrorCode_Null == err_code);
	PrintTest("MemNode_setData #1         ");

	err_code = mn_01->ops_->setData(mn_01, NULL, 0);
	assert(kErrorCode_Null == err_code);
	PrintTest("MemNode_setData #2         ");

	err_code = mn_01->ops_->setData(mn_01, data_01, 0);
	assert(kErrorCode_ZeroSize == err_code);
	PrintTest("MemNode_setData #3         ");

	err_code = mn_01->ops_->setData(mn_01, data_01, len_01);
	assert(kErrorCode_Ok == err_code);
	PrintTest("MemNode_setData #4         ");

	//Data:
	output = (u8*)mn_01->ops_->data(mn_04);
	assert(NULL == output);
	PrintTest("MemNode_data #1            ");

	output = mn_01->ops_->data(mn_01);
	assert(data_01 == output);
	PrintTest("MemNode_data #2            ");

	output = mn_01->ops_->data(mn_02);
	assert(NULL == output);
	PrintTest("MemNode_data #3            ");

	//Size:
	size = mn_01->ops_->size(mn_04);
	assert(0 == size);
	PrintTest("MemNode_size #1            ");

	size = mn_01->ops_->size(mn_01);
	assert(len_01 == size);
	PrintTest("MemNode_size #2            ");

	size = mn_01->ops_->size(mn_02);
	assert(0 == size);
	PrintTest("MemNode_size #3            ");

	//Set data:
	err_code = mn_01->ops_->setData(mn_02, data_02, len_02);
	assert(kErrorCode_Ok == err_code);
	PrintTest("MemNode_setData #5         ");

	err_code = mn_01->ops_->setData(mn_03, data_03, len_03);
	assert(kErrorCode_Ok == err_code);
	PrintTest("MemNode_setData #6         ");


	//************************************
	//********************* STATUS CHECK:
	PrintTestSimple("Status:");
	MM->status();


	//************************************
	//********************* RESETS / FREES TEST:
	PrintTestSimple("Resets / frees test:");

	//SoftReset:
	err_code = mn_01->ops_->softReset(NULL);
	assert(kErrorCode_Null == err_code);
	MM->free(data_01);
	data_01 = NULL;
	PrintTest("MemNode_softReset #1       ");

	err_code = mn_01->ops_->softReset(mn_01);
	assert(kErrorCode_Ok == err_code);
	assert(NULL == mn_01->data_ && 0 == mn_01->size_);
	assert(NULL != mn_01->ops_);
	assert(NULL == mn_01->next_ && NULL == mn_01->prev_);
	PrintTest("MemNode_softReset #2       ");

	//Reset:
	err_code = mn_01->ops_->reset(NULL);
	assert(kErrorCode_Null == err_code);
	PrintTest("MemNode_Reset #1           ");

	err_code = mn_01->ops_->reset(mn_01);
	assert(kErrorCode_Ok == err_code);
	PrintTest("MemNode_Reset #2           ");

	err_code = mn_01->ops_->reset(mn_02);
	assert(kErrorCode_Ok == err_code);
	assert(NULL == mn_02->data_ && 0 == mn_02->size_);
	assert(NULL != mn_02->ops_);
	assert(NULL == mn_02->next_ && NULL == mn_02->prev_);
	data_02 = NULL;
	len_02 = 0;
	PrintTest("MemNode_Reset #3           ");

	//SoftFree:
	err_code = mn_01->ops_->softFree(NULL);
	assert(kErrorCode_Null == err_code);
	PrintTest("MemNode_softFree #1        ");

	err_code = mn_01->ops_->softFree(mn_02);
	assert(kErrorCode_Ok == err_code);
	mn_02 = NULL;
	PrintTest("MemNode_softFree #2        ");

	err_code = mn_01->ops_->softFree(mn_02);
	assert(kErrorCode_Null == err_code);
	PrintTest("MemNode_softFree #3        ");

	//Free:
	err_code = mn_01->ops_->free(NULL);
	assert(kErrorCode_Null == err_code);
	PrintTest("MemNode_free #1            ");

	err_code = mn_01->ops_->free(mn_02);
	assert(kErrorCode_Null == err_code);
	PrintTest("MemNode_free #2            ");

	err_code = mn_01->ops_->free(mn_01);
	assert(kErrorCode_Ok == err_code);
	mn_01 = NULL;
	data_01 = NULL;
	len_01 = 0;
	PrintTest("MemNode_free #3            ");

	CheckMMUsage(34, 54)


	//************************************
	//********************* STATUS CHECK:
	PrintTestSimple("Status:");
	MM->status();


	//************************************
	//********************* PRINT TEST:
	/*PrintTestSimple("Print test #1:");
	PrintTestSimple("    Node #1:");
	mn_03->ops_->print(mn_01);
	PrintTestSimple("    Node #2:");
	mn_03->ops_->print(mn_02);
	PrintTestSimple("    Node #3:");
	mn_03->ops_->print(mn_03);
	PrintTestSimple("    Node #4:");
	mn_03->ops_->print(mn_04);*/


	//************************************
	//********************* OPERATIONS TEST:
	mn_01 = MEMNODE_create();

	//More data creation:
	len_01 = 6;
	data_01 = MM->malloc(sizeof(u8) * len_01);
	memcpy(data_01, "magic ", len_01);

	len_02 = 4;
	data_02 = MM->malloc(sizeof(u8) * len_02);
	memcpy(data_02, "nano", len_02);

	u32 len_04 = 6;
	u8* data_04 = MM->malloc(sizeof(u8) * len_04);
	memcpy(data_04, "AAAAAA", len_04);

	u32 len_05 = 14;
	u8* data_05 = MM->malloc(sizeof(u8) * len_05);
	memcpy(data_05, "FERNANDOALONSO", len_05);

	u32 len_06 = 10;
	u8* data_06 = MM->malloc(sizeof(u8) * len_06);
	memcpy(data_06, "magic nano", len_06);

	PrintTestSimple("Operations test:");

	//MemCopy:
	err_code = mn_01->ops_->memCopy(NULL, data_01, len_01);
	assert(kErrorCode_Null == err_code);
	PrintTest("MemNode_memCopy #1         ");

	err_code = mn_01->ops_->memCopy(mn_01, NULL, len_01);
	assert(kErrorCode_Null == err_code);
	PrintTest("MemNode_memCopy #2         ");

	err_code = mn_01->ops_->memCopy(mn_01, data_01, 0);
	assert(kErrorCode_ZeroSize == err_code);
	PrintTest("MemNode_memCopy #3         ");

	err_code = mn_01->ops_->memCopy(mn_01, data_01, len_01);
	assert(kErrorCode_Memory != err_code);
	assert(kErrorCode_Ok == err_code);
	assert(mn_01->ops_->data(mn_01) != data_01);
	PrintTest("MemNode_memCopy #4         ");

	u8* aux = mn_01->ops_->data(mn_01);
	assert(data_01 != aux);
	PrintTest("MemNode_data #4            ");

	//MemSet:
	err_code = mn_01->ops_->memSet(NULL, 65);
	assert(kErrorCode_Null == err_code);
	PrintTest("MemNode_memSet #1          ");

	err_code = mn_01->ops_->memSet(mn_01, 65);
	assert(kErrorCode_Ok == err_code);
	cmp_output = memcmp(mn_01->ops_->data(mn_01), data_04, len_04);
	assert(0 == cmp_output);
	PrintTest("MemNode_memSet #2          ");

	err_code = mn_01->ops_->reset(mn_01);
	assert(kErrorCode_Ok == err_code);
	PrintTest("MemNode_reset #4           ");

	err_code = mn_01->ops_->memSet(mn_01, 65);
	assert(kErrorCode_NullData == err_code);
	PrintTest("MemNode_memSet #3          ");

	//MemMask:
	err_code = mn_01->ops_->memMask(NULL, 0x5F);
	assert(kErrorCode_Null == err_code);
	PrintTest("MemNode_memMask #1         ");

	err_code = mn_01->ops_->memMask(mn_01, 0x5F);
	assert(kErrorCode_NullData == err_code);
	PrintTest("MemNode_memMask #2         ");

	err_code = mn_01->ops_->memMask(mn_03, 0x5F);
	assert(kErrorCode_Ok == err_code);
	cmp_output = memcmp(mn_01->ops_->data(mn_03), data_05, len_05);
	assert(0 == cmp_output);
	PrintTest("MemNode_memMask #3         ");

	CheckMMUsage(94, 134)

	//MemConcat:
	err_code = mn_01->ops_->setData(mn_01, data_01, len_01);
	assert(kErrorCode_Ok == err_code);
	data_01 = NULL;
	len_01 = 0;
	PrintTest("MemNode_setData #7         ");

	mn_02 = MEMNODE_create();

	err_code = mn_01->ops_->setData(mn_02, data_02, len_02);
	assert(kErrorCode_Ok == err_code);
	data_02 = NULL;
	len_02 = 0;
	PrintTest("MemNode_setData #8         ");

	err_code = mn_01->ops_->memConcat(NULL, mn_02->ops_->data(mn_02),
										mn_02->ops_->size(mn_02));
	assert(kErrorCode_Null == err_code);
	PrintTest("MemNode_memConcat #1       ");

	err_code = mn_01->ops_->memConcat(mn_01, NULL,
																		mn_02->ops_->size(mn_02));
	assert(kErrorCode_Null == err_code);
	PrintTest("MemNode_memConcat #2       ");

	err_code = mn_01->ops_->memConcat(mn_01, mn_02->ops_->data(mn_02), 0);
	assert(kErrorCode_ZeroSize == err_code);
	PrintTest("MemNode_memConcat #3       ");

	err_code = mn_01->ops_->memConcat(mn_01, mn_02->ops_->data(mn_02),
																		mn_02->ops_->size(mn_02));
	assert(kErrorCode_Memory != err_code);
	assert(kErrorCode_Ok == err_code);
	cmp_output = memcmp(mn_01->ops_->data(mn_01), data_06, len_06);
	assert(0 == cmp_output);
	PrintTest("MemNode_memConcat #4       ");

	CheckMMUsage(118, 178)

	//************************************
	//********************* STATUS CHECK:
	PrintTestSimple("Status:");
	MM->status();


	//************************************
	//********************* PRINT TEST:
	PrintTestSimple("Print test #2:");
	PrintTestSimple("    Node #1:");
	mn_01->ops_->print(mn_01);
	PrintTestSimple("    Node #2:");
	mn_01->ops_->print(mn_02);
	PrintTestSimple("    Node #3:");
	mn_01->ops_->print(mn_03);
	PrintTestSimple("    Node #4:");
	mn_01->ops_->print(mn_04);


	//************************************
	//********************* PRINT TEST:
	PrintTestSimple("Releasing nodes...:");
	err_code = mn_01->ops_->free(mn_03);
	assert(kErrorCode_Ok == err_code);

	err_code = mn_01->ops_->free(mn_02);
	assert(kErrorCode_Ok == err_code);

	err_code = mn_01->ops_->free(mn_01);
	assert(kErrorCode_Ok == err_code);

	CheckMMUsage(30, 30)


	//************************************
	//********************* STATUS CHECK:
	PrintTestSimple("Status:");
	MM->status();


	//************************************
	//********************* PRINT TEST:
	PrintTestSimple("Releasing pendent data...:");

	MM->free(data_04);
	data_04 = NULL;
	len_04 = 0;
	CheckMMUsage(24, 24)

	MM->free(data_05);
	data_05 = NULL;
	len_05 = 0;
	CheckMMUsage(10, 10)

	MM->free(data_06);
	data_06 = NULL;
	len_06 = 0;
	CheckMMUsage(0, 0)


	//************************************
	//********************* STATUS CHECK:
	PrintTestSimple("Status:");
	MM->status();


	//************************************
	//********************* DESTROY MM:
	PrintTestSimple("Destroying Memory Manager...");
	MM->destroy();

	return 0;
}
