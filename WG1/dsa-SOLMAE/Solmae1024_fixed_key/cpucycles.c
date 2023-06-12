#include "cpucycles.h"
#include <time.h>

int64_t cpucycles(void)
{
	unsigned int hi, lo;

    __asm__ __volatile__ ("rdtsc\n\t" : "=a" (lo), "=d"(hi));

    return ((int64_t)lo) | (((int64_t)hi) << 32);
}


// int64_t cpucycles(void)
// { // Access system counter for benchmarking
//   unsigned int hi, lo;

//   asm volatile ("rdtsc\n\t" : "=a" (lo), "=d"(hi));
//   return ((int64_t)lo) | (((int64_t)hi) << 32);
// }