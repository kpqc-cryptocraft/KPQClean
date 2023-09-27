#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "mqs_config.h"
#include "utils.h"      
#include "utils_prng.h"
#include "api.h"

#include <time.h>

#define TEST_LOOP 100

// MQSign mode -> mqs_config.h

uint64_t t[TEST_LOOP];


int64_t cpucycles(void)
{
	struct timespec time;

    clock_gettime(CLOCK_REALTIME, &time);
    return (int64_t)(time.tv_sec*1e9 + time.tv_nsec);
}

int PQC_bench(void)
{
	int r;

	unsigned char pk[CRYPTO_PUBLICKEYBYTES];
	unsigned char sk[CRYPTO_SECRETKEYBYTES];

	unsigned char  m[100] = "kpqc benchmark system";
	unsigned char sm[CRYPTO_BYTES + 200];

	unsigned char  m2[100];

	unsigned long long mlen = 0;
	unsigned long long smlen = 0;

	unsigned long long m2len;

	int result;

    unsigned long long kcycles;
    unsigned long long cycles1, cycles2;

	FILE *fp_keygen, *fp_sign, *fp_verify;

	fp_keygen = fopen("bench_keygen.txt", "w");
	fp_sign = fopen("bench_sign.txt", "w");
	fp_verify = fopen("bench_verify.txt", "w");

    mlen = strlen(m);

    printf("BENCHMARK ENVIRONMENTS  ============================= \n");
	printf("CRYPTO_PUBLICKEYBYTES: %d\n", CRYPTO_PUBLICKEYBYTES);
	printf("CRYPTO_SECRETKEYBYTES: %d\n", CRYPTO_SECRETKEYBYTES);
	printf("CRYPTO_BYTES: %d\n", CRYPTO_BYTES);
    printf("Number of loop: %d \n", TEST_LOOP);
	printf("KeyGen ////////////////////////////////////////////// \n");
	kcycles=0;

    unsigned char sk_seed[LEN_SKSEED] = { 0 };
    randombytes(sk_seed, LEN_SKSEED);

	for (int i = 0; i < TEST_LOOP; i++)
	{
		cycles1 = cpucycles();
		crypto_sign_keypair(pk, sk, sk_seed);
		cycles2 = cpucycles();
		fprintf(fp_keygen, "%lld \n", cycles2-cycles1);
        kcycles += cycles2-cycles1;
	}
    printf("  KeyGen runs in ................. %8lld nsec", kcycles/TEST_LOOP);
    printf("\n"); 

	fclose(fp_keygen);

	printf("Sign //////////////////////////////////////////////// \n");
	kcycles=0;
	for (int i = 0; i < TEST_LOOP; i++)
	{
		cycles1 = cpucycles();
		crypto_sign(sm, &smlen, m, mlen, sk, sk_seed, 0);
		cycles2 = cpucycles();
		fprintf(fp_sign, "%lld \n", cycles2-cycles1);
        kcycles += cycles2-cycles1;
	}
    printf("  Sign runs in ................. %8lld nsec", kcycles / TEST_LOOP);
    printf("\n"); 

	fclose(fp_sign);

	printf("Verify ////////////////////////////////////////////// \n");
	kcycles = 0;
	for (int i = 0; i < TEST_LOOP; i++)
	{
		cycles1 = cpucycles();
		result = crypto_sign_open(m2, &m2len, sm, smlen, pk);
		cycles2 = cpucycles();
		fprintf(fp_verify, "%lld \n", cycles2-cycles1);
        kcycles += cycles2 - cycles1;
	}
    printf("  Verify runs in ................. %8lld nsec", kcycles / TEST_LOOP);
    printf("\n"); 

	fclose(fp_verify);
    
    printf("==================================================== \n");

	return 0;
}

int main(int argc, char const *argv[])
{
	PQC_bench();
}
