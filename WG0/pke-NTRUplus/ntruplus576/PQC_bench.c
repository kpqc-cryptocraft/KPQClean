#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "api.h"
#include "rng.h"

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
	unsigned char pk[CRYPTO_PUBLICKEYBYTES];
	unsigned char sk[CRYPTO_SECRETKEYBYTES];
    unsigned char ct[CRYPTO_CIPHERTEXTBYTES];
    unsigned char ss[CRYPTO_BYTES];
    unsigned char dss[CRYPTO_BYTES];

    unsigned long long kcycles;
    unsigned long long cycles1, cycles2;

    printf("BENCHMARK ENVIRONMENTS  ============================= \n");
	printf("CRYPTO_PUBLICKEYBYTES: %d\n", CRYPTO_PUBLICKEYBYTES);
	printf("CRYPTO_SECRETKEYBYTES: %d\n", CRYPTO_SECRETKEYBYTES);
	printf("CRYPTO_BYTES: %d\n", CRYPTO_BYTES);
    printf("CRYPTO_CIPHERTEXTBYTES: %d\n", CRYPTO_CIPHERTEXTBYTES);
    printf("Number of loop: %d \n", TEST_LOOP);
	printf("KeyGen ////////////////////////////////////////////// \n");
	kcycles=0;
	for (int i = 0; i < TEST_LOOP; i++)
	{
		cycles1 = cpucycles();
        crypto_kem_keypair(pk, sk);
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
        crypto_kem_enc(ct, ss, pk);
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
        crypto_kem_dec(dss, ct, sk);
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
	
	return 0;
}