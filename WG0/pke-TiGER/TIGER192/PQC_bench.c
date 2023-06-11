#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

#include "TIGER_CPAPKE.h"
#include "TIGER_CCAKEM.h"
#include "fips202.h"
#include "rng.h"
#include "params.h"

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
	unsigned char c[2*LWE_N];

    unsigned char pk[32+LWE_N];
    unsigned char sk[LWE_N+MESSAGE_LEN];

	unsigned char shared_k1[KK_LEN] = {0,};
	unsigned char shared_k2[KK_LEN] = {0,};

    unsigned long long kcycles;
    unsigned long long cycles1, cycles2;

    printf("BENCHMARK ENVIRONMENTS  ============================= \n");
	printf("CRYPTO_PUBLICKEYBYTES: %d\n", 32+LWE_N);
	printf("CRYPTO_SECRETKEYBYTES: %d\n", LWE_N+MESSAGE_LEN);
	printf("CRYPTO_BYTES: %d\n", KK_LEN);
    printf("CRYPTO_CIPHERTEXTBYTES: %d\n", 2*LWE_N);
    printf("Number of loop: %d \n", TEST_LOOP);
	printf("KeyGen ////////////////////////////////////////////// \n");
	kcycles=0;
	for (int i = 0; i < TEST_LOOP; i++)
	{
		cycles1 = cpucycles();
        KEM_Keygen(pk, sk);
		cycles2 = cpucycles();
        kcycles += cycles2-cycles1;
	}
    printf("  KeyGen runs in ................. %8lld cycles", kcycles/TEST_LOOP);
    printf("\n"); 

	printf("Encapsulation /////////////////////////////////////// \n");
    kcycles=0;
	for (int i = 0; i < TEST_LOOP; i++)
	{
        cycles1 = cpucycles();
        KEM_Enc(c, shared_k1, pk);
        cycles2 = cpucycles();
        kcycles += cycles2-cycles1;

	}

    printf("  Encapsulation  runs in ......... %8lld cycles", kcycles/TEST_LOOP);
    printf("\n");

    printf("Decapsulation /////////////////////////////////////// \n");
    kcycles=0;
	for (int i = 0; i < TEST_LOOP; i++)
	{
        cycles1 = cpucycles();
        KEM_dec(shared_k2, c, sk, pk);
        cycles2 = cpucycles();
        kcycles += cycles2-cycles1;
    }
    

    printf("  Decapsulation  runs in ......... %8lld cycles", kcycles/TEST_LOOP);
    printf("\n");

    printf("==================================================== \n");

	return 0;
}

int main(int argc, char const *argv[])
{
	PQC_bench();
}