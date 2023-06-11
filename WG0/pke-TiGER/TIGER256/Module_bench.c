#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

#include "TIGER_CPAPKE.h"
#include "TIGER_CCAKEM.h"
#include "fips202.h"
#include "rng.h"
#include "params.h"

#define TEST_LOOP 1000000

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

    unsigned char u[MESSAGE_LEN]; //MESSAGE_LEN = 16
    unsigned char delta_kem[size_of_delta] = {0, };
    unsigned char coin[32];
    unsigned char hash_t[2*LWE_N+size_of_delta];
    
	unsigned char shared_k1[KK_LEN] = {0,};
	unsigned char shared_k2[KK_LEN] = {0,};

    unsigned long long kcycles;
    unsigned long long cycles1, cycles2;

    printf("BENCHMARK ENVIRONMENTS  ============================= \n");
    printf("Number of loop: %d \n", TEST_LOOP);
    printf("MODULE NAME: SHA3_256 ///////////////////////////////// \n");
	kcycles=0;
    printf("SHA3-256 input size: %d \n", size_of_delta);
	for (int i = 0; i < TEST_LOOP; i++)
	{
		cycles1 = cpucycles();
        sha3_256(coin, delta_kem, size_of_delta);
		cycles2 = cpucycles();
        kcycles += cycles2-cycles1;
	}
    printf("  SHA3_256 runs in ............... %8lld cycles \n", kcycles / TEST_LOOP);
    printf("\n");

    printf("MODULE NAME: SHAKE256 ///////////////////////////////// \n");
    kcycles=0;
    printf("SHAKE256 input size: %d \n", 2*LWE_N+size_of_delta);
	for (int i = 0; i < TEST_LOOP; i++)
	{
        cycles1 = cpucycles();
        shake256(shared_k1, KK_LEN, hash_t, 2*LWE_N+size_of_delta);
        cycles2 = cpucycles();
        kcycles += cycles2-cycles1;

	}

    printf("  SHAKE256 runs in ............... %8lld cycles \n", kcycles / TEST_LOOP);
    printf("\n");

    printf("MODULE NAME: RandomBytes //////////////////////////// \n");
    printf("Size of SEEDBYTES: %d \n", 16);
    kcycles=0;
	for (int i = 0; i < TEST_LOOP; i++)
	{
        cycles1 = cpucycles();
        randombytes(u, 16);
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
	PQC_bench();
}
