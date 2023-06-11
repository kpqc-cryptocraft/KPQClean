#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>
#include "rng.h"
#include "api.h"
#include "fips202.h"
#include "poly.h"



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
    unsigned char sk[CRYPTO_SECRETKEYBYTES] = {0,};
    unsigned char seedbuf[3 * SEEDBYTES + 2 * CRHBYTES];
    unsigned char *zeta, *tr, *key, *mu;
    poly mat, s1, t0, s2;
    keccak_state state;
    zeta = seedbuf;
    tr = zeta + SEEDBYTES;
	key = tr + SEEDBYTES;
	mu = key + SEEDBYTES;
    unpack_sk(zeta, tr, key, &t0, &s1, &s2, sk);
    unsigned char  m[100] = "kpqc benchmark system";
    unsigned long long  mlen = 0;
    mlen = strlen(m);

    // Ranbombytes test variables
    unsigned char pt[SEEDBYTES];

    printf("BENCHMARK ENVIRONMENTS  ============================= \n");
    printf("Number of loop: %d \n", TEST_LOOP);
    printf("MODULE NAME: SHAKE256 /////////////////////////////// \n");
    printf("Length of message: %lld \n", mlen);
    printf("Size of CRHBYTES: %d \n", CRHBYTES);
    kcycles = 0;
    for(int i = 0 ; i < TEST_LOOP ; i++)
    {
        cycles1 = cpucycles();
        shake256_init(&state);
        shake256_absorb(&state, tr, SEEDBYTES);
        shake256_absorb(&state, m, mlen);
        shake256_finalize(&state);
        shake256_squeeze(mu, CRHBYTES, &state);
        cycles2 = cpucycles();
        kcycles += cycles2 - cycles1;
    }
    printf("  SHAKE256 runs in ............... %8lld cycles \n", kcycles / TEST_LOOP);
    printf("MODULE NAME: RandomBytes //////////////////////////// \n");
    printf("Size of SEEDBYTES: %d \n", SEEDBYTES);
    kcycles = 0;
    for(int i = 0 ; i < TEST_LOOP ; i++)
    {
        cycles1 = cpucycles();
        randombytes(pt, SEEDBYTES);
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