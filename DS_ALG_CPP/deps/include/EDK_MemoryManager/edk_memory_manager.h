/**
 *
 *  @brief ESAT Development Kit (EDK) Memory Manager header file.
 *
 *  @brief Multithreading and cross-platform integration:
 *         Windows OS
 *         Linux OS
 *         Mac OS
 *
 *  @author Ivan Sancho, Escuela Superior de Arte y Tecnologia (ESAT) Valencia, 2020/21
 *  @subject Algorithms & Data Structures
 *
 *  All rights are reserved.
 *
 */


#ifndef __EDK_CPP_MEMORY_MANAGER_H__
#define __EDK_CPP_MEMORY_MANAGER_H__ 1

#ifndef size_t
#include <stdlib.h>
#endif

void* operator new(size_t size);
void* operator new[](size_t size);
void operator delete(void* addr) noexcept;
void operator delete[](void* addr) noexcept;
void operator delete(void* addr, size_t size) noexcept;
void operator delete[](void* addr, size_t size) noexcept;

struct EDK_MemoryManager {
  void* malloc(int size);
  void* calloc(int size);
  void free(void* ptr);
  int usage(); //Returns total memory usage in bytes.
  void status();
  void destroy();
};

EDK_MemoryManager* EDK_MemMngrInstance();
#define MM (EDK_MemMngrInstance())

#endif //__EDK_CPP_MEMORY_MANAGER_H__
