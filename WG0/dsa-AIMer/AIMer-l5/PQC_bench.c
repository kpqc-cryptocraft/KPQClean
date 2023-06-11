#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "rng.h"
#include "api.h"


#include <stdint.h>

#define TEST_LOOP 10000

uint64_t t[TEST_LOOP];

int64_t cpucycles(void)
{
	unsigned int hi, lo;

    __asm__ __volatile__ ("rdtsc\n\t" : "=a" (lo), "=d"(hi));

    return ((int64_t)lo) | (((int64_t)hi) << 32);
}

int PQC_bench(void)
{
	int r;
  	unsigned long long i, j;

	unsigned char pk[CRYPTO_PUBLICKEYBYTES];
	unsigned char sk[CRYPTO_SECRETKEYBYTES];

	unsigned char  m[100] = "kpqc benchmark system";
	unsigned char sm[CRYPTO_BYTES + 200];

	unsigned char  m2[100];

	unsigned long long mlen = 0;
	unsigned long long smlen;

	unsigned long long m2len;

	int result;

    unsigned long long kcycles, ecycles, dcycles;
    unsigned long long cycles1, cycles2;

    mlen = strlen(m);

    printf("BENCHMARK ENVIRONMENTS  ============================= \n");
	printf("CRYPTO_PUBLICKEYBYTES: %d\n", CRYPTO_PUBLICKEYBYTES);
	printf("CRYPTO_SECRETKEYBYTES: %d\n", CRYPTO_SECRETKEYBYTES);
	printf("CRYPTO_BYTES: %d\n", CRYPTO_BYTES);
    printf("Number of loop: %d \n", TEST_LOOP);
	printf("KeyGen ////////////////////////////////////////////// \n");
	kcycles=0;
	for (int i = 0; i < TEST_LOOP; i++)
	{
		cycles1 = cpucycles();
		crypto_sign_keypair(pk, sk);
		cycles2 = cpucycles();
        kcycles += cycles2-cycles1;
	}
    printf("  KeyGen runs in ................. %8lld cycles", kcycles/TEST_LOOP);
    printf("\n"); 


	printf("Sign //////////////////////////////////////////////// \n");
	kcycles=0;
	for (int i = 0; i < TEST_LOOP; i++)
	{
		cycles1 = cpucycles();
		crypto_sign(sm, &smlen, m, mlen, sk);
		cycles2 = cpucycles();
        kcycles += cycles2-cycles1;
	}
    printf("  Sign runs in ................. %8lld cycles", kcycles / TEST_LOOP);
    printf("\n"); 


	printf("Verify ////////////////////////////////////////////// \n");
	kcycles = 0;
	for (int i = 0; i < TEST_LOOP; i++)
	{
		cycles1 = cpucycles();
		result = crypto_sign_open(m2, &m2len, sm, smlen, pk);
		cycles2 = cpucycles();
        kcycles += cycles2 - cycles1;
	}
    printf("  Verify runs in ................. %8lld cycles", kcycles / TEST_LOOP);
    printf("\n"); 
    
    printf("==================================================== \n");

	return 0;
}

int main(int argc, char const *argv[])
{
	PQC_bench();
}
