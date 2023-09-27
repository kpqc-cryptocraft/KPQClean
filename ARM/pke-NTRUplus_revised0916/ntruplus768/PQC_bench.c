#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "api.h"
#include "rng.h"

#include <time.h>

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
	unsigned char pk[CRYPTO_PUBLICKEYBYTES];
	unsigned char sk[CRYPTO_SECRETKEYBYTES];
    unsigned char ct[CRYPTO_CIPHERTEXTBYTES];
    unsigned char ss[CRYPTO_BYTES];
    unsigned char dss[CRYPTO_BYTES];

    unsigned long long kcycles;
    unsigned long long cycles1, cycles2;

    FILE *fp_keygen, *fp_enc, *fp_dec;

	fp_keygen = fopen("bench_keygen.txt", "w");
	fp_enc = fopen("bench_enc.txt", "w");
	fp_dec = fopen("bench_dec.txt", "w");

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
        fprintf(fp_keygen, "%lld \n", cycles2-cycles1);
        kcycles += cycles2-cycles1;
	}
    printf("  KeyGen runs in ................. %8lld nsec", kcycles/TEST_LOOP);
    printf("\n"); 

    fclose(fp_keygen);

	printf("Encapsulation /////////////////////////////////////// \n");
    kcycles=0;
	for (int i = 0; i < TEST_LOOP; i++)
	{
        cycles1 = cpucycles();
        crypto_kem_enc(ct, ss, pk);
        cycles2 = cpucycles();
        fprintf(fp_enc, "%lld \n", cycles2-cycles1);
        kcycles += cycles2-cycles1;

	}
    fclose(fp_enc);

    printf("  Encapsulation  runs in ......... %8lld nsec", kcycles/TEST_LOOP);
    printf("\n");

    printf("Decapsulation /////////////////////////////////////// \n");
    kcycles=0;
	for (int i = 0; i < TEST_LOOP; i++)
	{
        cycles1 = cpucycles();
        crypto_kem_dec(dss, ct, sk);
        cycles2 = cpucycles();
        fprintf(fp_dec, "%lld \n", cycles2-cycles1);
        kcycles += cycles2-cycles1;
    }
    

    printf("  Decapsulation  runs in ......... %8lld nsec", kcycles/TEST_LOOP);
    printf("\n");

    fclose(fp_dec);

    printf("==================================================== \n");

	return 0;
}

int main(int argc, char const *argv[])
{
	PQC_bench();

    return 0;
}
