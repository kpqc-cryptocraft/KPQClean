#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "sha2.h"
#include "rng.h"
#include "sign.h"
#include "fips202.h"


#define TEST_LOOP 100

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
    int8_t seed[32];
    randombytes(seed, 32);
    unsigned char tmp[POLY_BYTES];
    unsigned char  m[100] = "kpqc benchmark system";
    unsigned long long  mlen = 0;
    mlen = strlen(m);
    sha256ctx ctx;

    // Ranbombytes test variables
    unsigned char pk[32];    

    printf("BENCHMARK ENVIRONMENTS  ============================= \n");
    printf("Number of loop: %d \n", TEST_LOOP);
    printf("MODULE NAME: SHA256 ///////////////////////////////// \n");
    printf("Length of message: %lld \n", mlen);
    printf("Size of POLY_BYTES: %d \n", POLY_BYTES);
    kcycles = 0;
    for(int i = 0 ; i < TEST_LOOP ; i++)
    {
        cycles1 = cpucycles();
        sha256_inc_init(&ctx);
		sha256_inc_blocks(&ctx, tmp, POLY_BYTES/64);
		sha256_inc_finalize(seed, &ctx, m, mlen);
        cycles2 = cpucycles();
        kcycles += cycles2 - cycles1;
    }
    printf("  SHA256 runs in ................. %8lld nsec \n", kcycles / TEST_LOOP);
    printf("MODULE NAME: RandomBytes //////////////////////////// \n");
    printf("Size of SEEDBYTES: %d \n", 32);
    kcycles = 0;
    for(int i = 0 ; i < TEST_LOOP ; i++)
    {
        cycles1 = cpucycles();
        randombytes(pk, 32);
        cycles2 = cpucycles();
        kcycles += cycles2 - cycles1;
    }
    printf("  RandomBytes runs in ............ %8lld nsec \n", kcycles / TEST_LOOP);
    printf("===================================================== \n");



}

int main(int argc, char const *argv[])
{
	Module_bench();
}