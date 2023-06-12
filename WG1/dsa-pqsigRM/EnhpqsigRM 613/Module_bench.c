#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>
#include "src/rng.h"
#include "src/api.h"


#define TEST_LOOP 1000000

uint64_t t[TEST_LOOP];

int64_t cpucycles(void)
{
	unsigned int hi, lo;

    __asm__ __volatile__ ("rdtsc\n\t" : "=a" (lo), "=d"(hi));

    return ((int64_t)lo) | (((int64_t)hi) << 32);
}

int Module_bench(void)
{
    unsigned long long kcycles;
    unsigned long long cycles1, cycles2;

    // Ranbombytes test variables
    unsigned char pk[128];    

    printf("BENCHMARK ENVIRONMENTS  ============================= \n");
    printf("Number of loop: %d \n", TEST_LOOP);
    printf("MODULE NAME: RandomBytes //////////////////////////// \n");
    printf("Size of SEEDBYTES: %d \n", 128);
    kcycles = 0;
    for(int i = 0 ; i < TEST_LOOP ; i++)
    {
        cycles1 = cpucycles();
        randombytes(pk, 128);
        cycles2 = cpucycles();
        kcycles += cycles2 - cycles1;
    }
    printf("  RandomBytes runs in ............ %8lld cycles \n", kcycles / TEST_LOOP);
    printf("===================================================== \n");



}

int main(int argc, char const *argv[])
{
	Module_bench();
}