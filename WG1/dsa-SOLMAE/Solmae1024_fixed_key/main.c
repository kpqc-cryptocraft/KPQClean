#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#include "api.h"
#include "poly.h"
#include "test_dist.h"
#include "benchmarks.h"
#include "cpucycles.h"

#include "precomp.h"
#include "normaldist.h"
#include "samplerZ.h"
#include "randombytes.h"



#define TEST_LOOP 10000

uint64_t t[TEST_LOOP];

int PQC_bench(void)
{
	int r;
  	double rejw=.0, rejyz=.0, rejctr=.0, rejctrkg=.0;
  	unsigned long long i, j;

  secret_key sk;
  public_key pk;
  signature s;

  uint8_t m[32] = {0x46, 0xb6, 0xc4, 0x83, 0x3f, 0x61, 0xfa, 0x3e, 0xaa, 0xe9, 0xad, 0x4a, 0x68, 0x8c, 0xd9, 0x6e, 0x22, 0x6d, 0x93, 0x3e, 0xde, 0xc4, 0x64, 0x9a, 0xb2, 0x18, 0x45, 0x2, 0xad, 0xf3, 0xc, 0x61};

	int result;

    unsigned long long kcycles, ecycles, dcycles;
    unsigned long long cycles1, cycles2;

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
		keygen_full(&sk, &pk);
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
		sign(m, &sk, &s);
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
		result = verify(m, &pk, &s);
		cycles2 = cpucycles();
        kcycles += cycles2 - cycles1;
	}
    printf("  Verify runs in ................. %8lld cycles", kcycles / TEST_LOOP);
    printf("\n"); 

    //printf("Acceptance rate of Signing: %.2f\n",(double) TEST_LOOP / rejctr);
    
    printf("==================================================== \n");

	return 0;
}

int main(int argc, char const *argv[])
{
	PQC_bench();
	
	return 0;
}
