#include <stdio.h>
#include <string.h>
#include <stdint.h>

#include "parameters.h"
#include "kem.h"

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
    unsigned char delta[32];
    unsigned char ss[CRYPTO_BYTES];
    
    const size_t input_size = DELTA_BYTES + SHA3_256_HashSize; //32+32
    
    unsigned char input[input_size];
    
    printf("BENCHMARK ENVIRONMENTS  ============================= \n");
    printf("Number of loop: %d \n", TEST_LOOP);
    
    printf("MODULE NAME: SHAKE256 ///////////////////////////////// \n");
    kcycles=0;
    printf("SHAKE256 input size: %d \n", input_size);
    for (int i = 0; i < TEST_LOOP; i++)
    {
        cycles1 = cpucycles();
        shake256(ss, CRYPTO_BYTES, input, input_size);

        cycles2 = cpucycles();
        kcycles += cycles2-cycles1;
    }
    printf("  SHAKE256 runs in ............... %8lld cycles \n", kcycles / TEST_LOOP);
    printf("\n");
    
    
    printf("MODULE NAME: RandomBytes //////////////////////////// \n");
    printf("Size of SEEDBYTES: %d \n", DELTA_BYTES);
    kcycles=0;
    for (int i = 0; i < TEST_LOOP; i++)
    {
        cycles1 = cpucycles();
        randombytes(delta, DELTA_BYTES);
        cycles2 = cpucycles();
        kcycles += cycles2-cycles1;
    }
    

    printf("  RandomBytes runs in ............ %8lld cycles \n", kcycles / TEST_LOOP);
    printf("\n");

    printf("==================================================== \n");

    return 0;
}

int main(int argc, char const *argv[])
{
    Module_bench();
}
