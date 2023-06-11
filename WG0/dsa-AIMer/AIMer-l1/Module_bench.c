#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "rng.h"
#include "api.h"
#include "hash.h"
#include "field/field.h"
#include "aimer_instances.h"



#define TEST_LOOP 100000

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

    // SHA256 test variables
    hash_instance ctx;
    size_t field_size = 16;
    size_t block_size = 16;
    unsigned char initial_vector[block_size];
    randombytes(initial_vector, block_size);

    // Ranbombytes test variables
    unsigned char pk[48];    

    printf("BENCHMARK ENVIRONMENTS  ============================= \n");
    printf("Number of loop: %d \n", TEST_LOOP);
   printf("MODULE NAME: SHAKE128 ///////////////////////////////// \n");
    printf("Length of message: %lld \n", block_size);
    printf("Size of Digest_size: %d \n", field_size <<1 );
    kcycles = 0;
    for(int i = 0 ; i < TEST_LOOP ; i++)
    {
        cycles1 = cpucycles();
        hash_init(&ctx, (field_size << 1));
        hash_update(&ctx, initial_vector, field_size);
        hash_final(&ctx);
        cycles2 = cpucycles();
        kcycles += cycles2 - cycles1;
    }
    printf("  SHAKE128 runs in ............... %8lld cycles \n", kcycles / TEST_LOOP);
    printf("MODULE NAME: RandomBytes //////////////////////////// \n");
    printf("Size of SEEDBYTES: %d \n", 48);
    kcycles = 0;
    for(int i = 0 ; i < TEST_LOOP ; i++)
    {
        cycles1 = cpucycles();
        randombytes(pk, 48);
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