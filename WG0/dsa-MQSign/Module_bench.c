#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "mqs_config.h"
#include "utils.h"      
#include "utils_prng.h"
#include "api.h"


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

    // SHA256 test variables
    unsigned char  m[100] = "kpqc benchmark system";
    unsigned long long  mlen = 0;
    mlen = strlen(m);
    unsigned char digest[_HASH_LEN];

    // Ranbombytes test variables
    unsigned char pk[32];    

    printf("BENCHMARK ENVIRONMENTS  ============================= \n");
    printf("Number of loop: %d \n", TEST_LOOP);
    printf("MODULE NAME: SHA256 ///////////////////////////////// \n");
    printf("Length of message: %lld \n", mlen);
    kcycles = 0;
    for(int i = 0 ; i < TEST_LOOP ; i++)
    {
        cycles1 = cpucycles();
        sha256(digest, m, mlen);
        cycles2 = cpucycles();
        kcycles += cycles2 - cycles1;
    }
    printf("  SHA256 runs in ................. %8lld cycles \n", kcycles / TEST_LOOP);
    printf("MODULE NAME: SHA384 ///////////////////////////////// \n");
    printf("Length of message: %lld \n", mlen);
    kcycles = 0;
    for(int i = 0 ; i < TEST_LOOP ; i++)
    {
        cycles1 = cpucycles();
        sha384(digest, m, mlen);
        cycles2 = cpucycles();
        kcycles += cycles2 - cycles1;
    }
    printf("  SHA384 runs in ................. %8lld cycles \n", kcycles / TEST_LOOP);
    printf("MODULE NAME: SHA512 ///////////////////////////////// \n");
    printf("Length of message: %lld \n", mlen);
    kcycles = 0;
    for(int i = 0 ; i < TEST_LOOP ; i++)
    {
        cycles1 = cpucycles();
        sha512(digest, m, mlen);
        cycles2 = cpucycles();
        kcycles += cycles2 - cycles1;
    }
    printf("  SHA512 runs in ................. %8lld cycles \n", kcycles / TEST_LOOP);
    printf("===================================================== \n");


}

int main(int argc, char const *argv[])
{
	Module_bench();
}