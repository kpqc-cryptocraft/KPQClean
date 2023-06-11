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
        kcycles += cycles2-cycles1;
	}
    printf("  KeyGen runs in ................. %8lld cycles", kcycles/TEST_LOOP);
    printf("\n"); 


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
        kcycles += cycles2-cycles1;

	}

    printf("  Encapsulation  runs in ......... %8lld cycles", kcycles/TEST_LOOP);
    printf("\n");

    printf("Decapsulation /////////////////////////////////////// \n");
    kcycles=0;
	for (int i = 0; i < TEST_LOOP; i++)
	{
        cycles1 = cpucycles();
        crypto_encrypt_open(m, sizeof(m), ct, clen, sk);
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