// common_def.h
// Toni Barella
// Algoritmos & Inteligencia Artificial
// ESAT 2016-2020
//
#ifndef __COMMON_DEF_H__
#define __COMMON_DEF_H__ 1

#define VERBOSE_

typedef enum {
  kErrorCode_Ok = 0,
  kErrorCode_Null = -1,
  kErrorCode_NullData = -2,
  kErrorCode_Memory = -3,
  kErrorCode_ZeroSize=-4,
  kErrorCode_AlreadyInitialized=-5,
  kErrorCode_IsFull = -6,
  //...
  kErrorCode_Counter = 7,
} ErrorCode;

static const char* const kErrorMsg[] = {
  "Ok",
  "Null pointer",
  "Null data pointer",
  "Memory insufficient",
  "Has no size"
  "AlreadyInitialized"
  "Memory Is Full"
  "File",
  //...
  "Counter",
};

#endif // __COMMON_DEF_H__