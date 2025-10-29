// test_memory_manager.c
// Escuela Superior de Arte y Tecnologia
// Algoritmos & Inteligencia Artificial
// ESAT 2020-2021
//

#include <stdio.h>
#include "EDK_MemoryManager/edk_memory_manager.h"

#define NORMAL_TEST 1

typedef struct custom_struct {
  int data_int;
  float data_float;
  boolean data_boolean;
} CustomStruct;

const s32 kNS8 = 100;
const s32 kNS8Bad = 4;
const s32 kNInts = 4;
const s32 kNCustomStructs = 1;


void InitData(s8* data_s8, int* data_int, CustomStruct* custom_struct) {
  for (s32 i = 0; i < kNS8; ++i) {
    data_s8[i] = (s8) (i * 2);
  }
  for (s32 i = 0; i < kNInts; ++i) {
    data_int[i] = (i + 1) * 10 + 1;
  }
  for (s32 i = 0; i < kNCustomStructs; ++i) {
    custom_struct[i].data_int = i * 3;
    custom_struct[i].data_float = (float)(i * kNCustomStructs),
      custom_struct[i].data_boolean = True;
  }
}

void PrintData(s8* data_s8, int* data_int, CustomStruct* custom_struct) {
  for (s32 i = 0; i < kNS8; ++i) {
    printf("%d, ", data_s8[i]);
  }
  printf("\n");
  for (s32 i = 0; i < kNInts; ++i) {
    printf("%d, ", data_int[i]);
  }
  printf("\n");
  for (s32 i = 0; i < kNCustomStructs; ++i) {
    printf("%d %f %s, ", custom_struct[i].data_int, custom_struct[i].data_float,
      custom_struct[i].data_boolean == True ? "true" : "false");
  }
  printf("\n");
}

int main(int argc, char** argv) {
#if NORMAL_TEST == 1
  //************************** GOOD USAGE SECTION **************************//
  //Basic usage example of memory allocation, deallocation and status check.
  //Memory manager is created the first time a function is called:
  s8* data_s8_1 = (s8*)MM->malloc(sizeof(s8) * kNS8);
  int* data_int_1 = (int*)MM->malloc(sizeof(int) * kNInts);
  CustomStruct* custom_struct_1 = (CustomStruct*)MM->malloc(sizeof(CustomStruct) * 
                                                            kNCustomStructs);
  InitData(data_s8_1, data_int_1, custom_struct_1);
  PrintData(data_s8_1, data_int_1, custom_struct_1);
  MM->status();

  s8* data_s8_2 = (s8*)MM->malloc(sizeof(s8) * kNS8);
  int* data_int_2 = (int*)MM->malloc(sizeof(int) * kNInts);
  CustomStruct* custom_struct_2 = (CustomStruct*)MM->malloc(sizeof(CustomStruct) * 
                                                            kNCustomStructs);
  InitData(data_s8_2, data_int_2, custom_struct_2);
  PrintData(data_s8_2, data_int_2, custom_struct_2);
  MM->status();

  MM->free(data_s8_2);
  MM->free(data_int_2);
  MM->free(custom_struct_2);
  MM->status();

  MM->free(data_s8_1);
  MM->free(data_int_1);
  MM->free(custom_struct_1);
  MM->status();

  //************************** BAD USAGE SECTION **************************//
	//Releasing the pointer more times than allowed:
  MM->free(data_s8_1);
  MM->free(data_s8_1);
  MM->free(data_s8_1);

  //Trying to releasing a NULL pointer:
  data_s8_1 = NULL;
  MM->free(data_s8_1);
  MM->free(data_s8_1);
  MM->free(data_s8_1);

  //Any memory corruption checked in a "free" function will automatically close
  //the program:
  data_s8_1 = (s8*)MM->malloc(sizeof(s8) * kNS8Bad);
  for (s8 i = 0; i < kNS8Bad + 1; ++i) {
    data_s8_1[i] = (i + 1) * 10;
  }
  MM->free(data_s8_1);

#elif NORMAL_TEST == 0
  //********************** MM ADVANCED USAGE SECTION **********************//
  //For this test, configure your ".cfg" file with only blocks of 1, 2 and 8 bytes.
  //Allocating should be related with the ".cfg" data:
  s8 * data_1[100];
  for (s32 i = 0; i < 100; ++i) {
    data_1[i] = (s8*)MM->malloc(sizeof(s8) * 2);
  }
  s8* data_2[101];
  for (s32 i = 0; i < 101; ++i) {
    data_2[i] = (s8*)MM->malloc(sizeof(s8));
  }
  //Deallocating:
  for (s32 i = 0; i < 101; ++i) {
    MM->free(data_2[i]);
  }
  for (s32 i = 0; i < 100; ++i) {
    MM->free(data_1[i]);
  }
  MM->status();
#endif

  //If everything would gone well... the last action is destroying the memory
  //manager:
  MM->destroy();

	printf("Press ENTER to continue\n");
	getchar();
	return 0;
}
