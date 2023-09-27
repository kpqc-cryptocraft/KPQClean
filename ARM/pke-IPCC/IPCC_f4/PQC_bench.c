#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#include "Keygen.h"
#include "Encryption.h"
#include "MultiEnc.h"
#include "Decryption.h"
#include "rng.h"
#include "api.h"
#include "fips202.h"

#define TEST_LOOP 100

uint64_t t[TEST_LOOP];

int64_t cpucycles(void)
{
	struct timespec time;

    clock_gettime(CLOCK_REALTIME, &time);
    return (int64_t)(time.tv_sec*1e9 + time.tv_nsec);
}


int PQC_bench(void)
{
	unsigned int    pk1[CRYPTO_PUBLICKEYBYTES_ROW][CRYPTO_PUBLICKEYBYTES_COL];
    unsigned int    pk2[CRYPTO_PUBLICKEYBYTES_ROW][CRYPTO_PUBLICKEYBYTES_COL];
    unsigned int    sk[CRYPTO_SECRETKEYBYTES];
    unsigned int    ct[CRYPTO_CIPHERTEXTBYTES_ROW][CRYPTO_CIPHERTEXTBYTES_COL];
    unsigned int    clen[1];
    unsigned int    m[1];
    //unsigned char ss[CRYPTO_BYTES];
    //unsigned char dss[CRYPTO_BYTES];

    unsigned long long kcycles;
    unsigned long long cycles1, cycles2;

    FILE *fp_keygen, *fp_sign, *fp_verify;

	fp_keygen = fopen("bench_keygen.txt", "w");
	fp_sign = fopen("bench_sign.txt", "w");
	fp_verify = fopen("bench_verify.txt", "w");


    printf("BENCHMARK ENVIRONMENTS  ============================= \n");
	printf("CRYPTO_PUBLICKEYBYTES_ROW: %d\n", CRYPTO_PUBLICKEYBYTES_ROW);
    printf("CRYPTO_PUBLICKEYBYTES_COL: %d\n", CRYPTO_PUBLICKEYBYTES_COL);
	printf("CRYPTO_SECRETKEYBYTES: %d\n", CRYPTO_SECRETKEYBYTES);
	printf("CRYPTO_CIPHERTEXTBYTES_ROW: %d\n", CRYPTO_CIPHERTEXTBYTES_ROW);
    printf("CRYPTO_CIPHERTEXTBYTES_COL: %d\n", CRYPTO_CIPHERTEXTBYTES_COL);
    printf("SIZE OF INT: %d \n", sizeof(int));
    printf("Number of loop: %d \n", TEST_LOOP);
	printf("KeyGen ////////////////////////////////////////////// \n");
	kcycles=0;
	for (int i = 0; i < TEST_LOOP; i++)
	{
		cycles1 = cpucycles();
        crypto_encrypt_keypair(pk1, pk2, sk);
		cycles2 = cpucycles();
        fprintf(fp_keygen, "%lld \n", cycles2-cycles1);
        kcycles += cycles2-cycles1;
	}
    printf("  KeyGen runs in ................. %8lld nsec", kcycles/TEST_LOOP);
    printf("\n"); 

    fclose(fp_keygen);

    while (1) {
        m[0] = randomint()%MOD_N;
        if (m[0]>=5) break;
    }
	printf("Encapsulation /////////////////////////////////////// \n");
    kcycles=0;
	for (int i = 0; i < TEST_LOOP; i++)
	{
        cycles1 = cpucycles();
        crypto_encrypt(ct, clen, m, sizeof(m), pk1, pk2);
        cycles2 = cpucycles();
        fprintf(fp_sign, "%lld \n", cycles2-cycles1);
        kcycles += cycles2-cycles1;

	}
    printf("  Encapsulation  runs in ......... %8lld nsec", kcycles/TEST_LOOP);
    printf("\n");

    fclose(fp_sign);

    printf("Decapsulation /////////////////////////////////////// \n");
    kcycles=0;
	for (int i = 0; i < TEST_LOOP; i++)
	{
        cycles1 = cpucycles();
        crypto_encrypt_open(m, sizeof(m), ct, clen, sk);
        cycles2 = cpucycles();
        fprintf(fp_verify, "%lld \n", cycles2-cycles1);
        kcycles += cycles2-cycles1;
    }

    printf("  Decapsulation  runs in ......... %8lld nsec", kcycles/TEST_LOOP);
    printf("\n");

    fclose(fp_verify);

    printf("==================================================== \n");

	return 0;
}

int main(int argc, char const *argv[])
{
	PQC_bench();
}