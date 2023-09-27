#include "fips202.h"
#include "packing.h"
#include "randombytes.h"
#include "reduce.h"
#include "rounding.h"
#include "sign.h"
#include <stdio.h>
#include <string.h>


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
    unsigned char sk[CRYPTO_PUBLICKEYBYTES];
    randombytes(sk, CRYPTO_PUBLICKEYBYTES);
    unsigned char  m[100] = "kpqc benchmark system";
    unsigned long long  mlen = 0;
    unsigned char mu[SEEDBYTES] = {0};
    mlen = strlen(m);
    keccak_state state;

    // Ranbombytes test variables
    unsigned char pk[32];    

    printf("BENCHMARK ENVIRONMENTS  ============================= \n");
    printf("Number of loop: %d \n", TEST_LOOP);
    printf("MODULE NAME: SHAKE256 /////////////////////////////// \n");
    printf("Length of message: %lld \n", mlen);
    printf("Size of CRYPTO_PUBLICKEYBYTES: %d \n", CRYPTO_PUBLICKEYBYTES);
    printf("Size of SEEDBYTES: %d \n", SEEDBYTES);
    kcycles = 0;
    for(int i = 0 ; i < TEST_LOOP ; i++)
    {
        cycles1 = cpucycles();
        shake256_init(&state);
        shake256_absorb(&state, sk, CRYPTO_PUBLICKEYBYTES);
        shake256_absorb(&state, m, mlen);
        shake256_finalize(&state);
        shake256_squeeze(mu, SEEDBYTES, &state);
        cycles2 = cpucycles();
        kcycles += cycles2 - cycles1;
    }
    printf("  SHAKE256 runs in ............... %8lld nsec \n", kcycles / TEST_LOOP);
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