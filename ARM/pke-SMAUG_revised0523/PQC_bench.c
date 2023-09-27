#include <stdio.h>
#include <string.h>
#include <stdint.h>

#include "parameters.h"
#include "kem.h"

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
    unsigned char pk[PUBLICKEY_BYTES]; //CRYPTO_PUBLICKEYBYTES->PUBLICKEY_BYTES
    unsigned char sk[KEM_SECRETKEY_BYTES]; //CRYPTO_SECRETKEYBYTES->KEM_SECRETKEY_BYTES
    unsigned char ctxt[CIPHERTEXT_BYTES];//ct->ctxt, CRYPTO_CIPHERTEXTBYTES->CIPHERTEXT_BYTES
    unsigned char ss1[CRYPTO_BYTES]; //ss->ss1
    unsigned char ss2[CRYPTO_BYTES]; //dss->ss2

    unsigned long long kcycles;
    unsigned long long cycles1, cycles2;

    FILE *fp_keygen, *fp_enc, *fp_dec;

	fp_keygen = fopen("bench_keygen.txt", "w");
	fp_enc = fopen("bench_enc.txt", "w");
	fp_dec = fopen("bench_dec.txt", "w");


    printf("BENCHMARK ENVIRONMENTS  ============================= \n");
    printf("CRYPTO_PUBLICKEYBYTES: %d\n", PUBLICKEY_BYTES);
    printf("CRYPTO_SECRETKEYBYTES: %d\n", KEM_SECRETKEY_BYTES);
    printf("CRYPTO_BYTES: %d\n", CRYPTO_BYTES);
    printf("CRYPTO_CIPHERTEXTBYTES: %d\n", CIPHERTEXT_BYTES);
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
        crypto_kem_encap(ctxt, ss1, pk); //crypto_kem_enc->crypto_kem_encap
        cycles2 = cpucycles();
        fprintf(fp_enc, "%lld \n", cycles2-cycles1);
        kcycles += cycles2-cycles1;

    }

    printf("  Encapsulation  runs in ......... %8lld nsec", kcycles/TEST_LOOP);
    printf("\n");

    fclose(fp_enc);

    printf("Decapsulation /////////////////////////////////////// \n");
    kcycles=0;
    for (int i = 0; i < TEST_LOOP; i++)
    {
        cycles1 = cpucycles();
        crypto_kem_decap(ss2, sk, pk, ctxt); //crypto_kem_dec->crypto_kem_decap
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
}
