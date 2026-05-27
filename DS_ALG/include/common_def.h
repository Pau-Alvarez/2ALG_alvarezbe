/*
 *
 * common_def.h
 * Toni Barella, Ivan Sancho as Unreal Authorized Instructor (UAI).
 * Algorithms and Data Structures.
 * ESAT 2020-2021
 *
 */

#ifndef __COMMON_DEF_H__
#define __COMMON_DEF_H__ 1

#define VERBOSE_

/**
 * @brief Standard error codes returned by every ADT operation.
 * @details Every API call that can fail returns one of these codes so the
 *          caller can detect the exact reason of the failure.
 */
typedef enum {
  kErrorCode_Ok = 0,                  /**< Operation finished successfully. */
  kErrorCode_Null = -1,               /**< A required pointer was NULL. */
  kErrorCode_NullData = -2,           /**< The data buffer was NULL. */
  kErrorCode_Memory = -3,             /**< Memory allocation attempt failed. */
  kErrorCode_ZeroSize = -4,           /**< Size or capacity equal to zero. */
  kErrorCode_AlreadyInitialized = -5, /**< Structure already initialized. */
  kErrorCode_IsFull = -6,             /**< Storage has no free slots left. */
  kErrorCode_IsEmpty = -7,            /**< Storage has no elements stored. */
  kErrorCode_OutOfRange = -8,         /**< Position outside the valid range. */
  kErrorCode_NotSupported = -9,       /**< Operation not supported by ADT. */
  kErrorCode_Counter = 10,
} ErrorCode;

/**
 * @brief Human-readable description for each #ErrorCode value.
 * @details Indexed using the absolute value of the error code.
 */
static const char* const kErrorMsg[] = {
  "Ok",
  "Null pointer",
  "Null data pointer",
  "Memory insufficient",
  "Has no size",
  "Already initialized",
  "Storage is full",
  "Storage is empty",
  "Position out of range",
  "Operation not supported",
  "Counter",
};

#endif // __COMMON_DEF_H__
