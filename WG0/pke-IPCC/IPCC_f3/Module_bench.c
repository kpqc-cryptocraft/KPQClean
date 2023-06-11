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
    unsigned long long kcycles;
    unsigned long long cycles1, cycles2;

    unsigned char x[4];

    printf("BENCHMARK ENVIRONMENTS  ============================= \n");
    printf("Number of loop: %d \n", TEST_LOOP);
    printf("MODULE NAME: RandomBytes ///////////////////////////////// \n");
    printf("Size of bytes: 4 \n");
	kcycles=0;
	for (int i = 0; i < TEST_LOOP; i++)
	{
		cycles1 = cpucycles();
        randombytes(x, 4);
		cycles2 = cpucycles();
        kcycles += cycles2-cycles1;
	}
    printf("  RandomBytes runs in ............... %8lld cycles \n", kcycles / TEST_LOOP);
    printf("\n");

    printf("==================================================== \n");

	return 0;
}

int main(int argc, char const *argv[])
{
	PQC_bench();
}
