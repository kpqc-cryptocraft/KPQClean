#ifndef POISON_H
#define POISON_H
#include <valgrind/memcheck.h>
/**
   Poisons a memory region of len bytes, starting at addr, indicating that
   execution time must not depend on the content of this memory region.
   Use this function to mark any memory regions containing secret data.
 */
#define poison(addr, len) VALGRIND_MAKE_MEM_UNDEFINED(addr, len);
/**
   Use this function to indicate that the specified memory region does no longer
   contain data that must not affect execution time.
 */
#define unpoison(addr, len) VALGRIND_MAKE_MEM_DEFINED(addr, len);
/**
   Checks whether the memory region of len bytes, starting at addr,
   contains any poisoned bits.
   Returns 0 if the code is running natively, rather than within valgrind.
   If valgrind is running, it returns the first address containing poisoned
   data, or 0 if there is no poisoned data in the specified memory region.
   You can use RUNNING_ON_VALGRIND from valgrind.h to check whether the code
   is being executed within valgrind.
 */
#define is_poisoned(addr, len) VALGRIND_CHECK_MEM_IS_DEFINED(addr, len);
#endif // POISON_H