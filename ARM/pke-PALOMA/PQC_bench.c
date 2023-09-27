#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "rng.h"
#include "api.h"
#include "gf_table_gen.h"

#include <time.h>
#include <stdint.h>

#define TEST_LOOP 100

#define flag    3   // 1, 2, 3


uint64_t t[TEST_LOOP];


int64_t cpucycles(void)
{
	struct timespec time;

    clock_gettime(CLOCK_REALTIME, &time);
    return (int64_t)(time.tv_sec*1e9 + time.tv_nsec);
}

#define MAX_CRYPTO_PUBLICKEYBYTES   1025024
#define MAX_CRYPTO_SECRETKEYBYTES   357064
#define MAX_CRYPTO_CIPHERTEXTBYTES  240
#define MAX_CRYPTO_BYTES            32

int PQC_bench(void)
{
    unsigned char       ct[MAX_CRYPTO_CIPHERTEXTBYTES]={0,}, ss[MAX_CRYPTO_BYTES]={0,}, ss1[MAX_CRYPTO_BYTES]={0,};
    unsigned char       pk[MAX_CRYPTO_PUBLICKEYBYTES]={0,}, sk[MAX_CRYPTO_SECRETKEYBYTES]={0,};
    int n,t,k;
    int                 CRYPTO_CIPHERTEXTBYTES, CRYPTO_BYTES, CRYPTO_PUBLICKEYBYTES, CRYPTO_SECRETKEYBYTES;

    PARAM PALOMAParam;

    if(flag == 1){
        t = 64;     
        n = 3904;   
        k = (n-(13*t));
        CRYPTO_CIPHERTEXTBYTES = PALOMA1_CIPHERTEXTBYTES;
        CRYPTO_BYTES           = PALOMA1_CRYPTO_BYTES;
        CRYPTO_PUBLICKEYBYTES  = PALOMA1_PUBLICKEYBYTES;
        CRYPTO_SECRETKEYBYTES  = PALOMA1_SECRETKEYBYTES;
    }
    else if(flag == 2){
        t = 128;     
        n = 5568;   
        k = (n-(13*t));    
        CRYPTO_CIPHERTEXTBYTES  = PALOMA2_CIPHERTEXTBYTES;
        CRYPTO_BYTES            = PALOMA2_CRYPTO_BYTES;
        CRYPTO_PUBLICKEYBYTES   = PALOMA2_PUBLICKEYBYTES;
        CRYPTO_SECRETKEYBYTES   = PALOMA2_SECRETKEYBYTES;
    }
    else if(flag == 3){
        t = 128;     
        n = 6592;   
        k = (n-(13*t));  
        CRYPTO_CIPHERTEXTBYTES  = PALOMA3_CIPHERTEXTBYTES;
        CRYPTO_BYTES            = PALOMA3_CRYPTO_BYTES;
        CRYPTO_PUBLICKEYBYTES   = PALOMA3_PUBLICKEYBYTES;
        CRYPTO_SECRETKEYBYTES   = PALOMA3_SECRETKEYBYTES;
    }
    else {
        return -3;      // KAT_DATA_ERROR
    }


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
        crypto_PALOMA_keypair(pk, sk, &PALOMAParam);
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
        crypto_PALOMA_enc(ct, ss, pk,&PALOMAParam);
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
        crypto_PALOMA_dec(ss1, ct, sk,&PALOMAParam);
        cycles2 = cpucycles();
        fprintf(fp_dec, "%lld \n", cycles2-cycles1);
        kcycles += cycles2-cycles1;
    }

    fclose(fp_dec);
    

    printf("  Decapsulation  runs in ......... %8lld nsec", kcycles/TEST_LOOP);
    printf("\n");

    printf("==================================================== \n");

	return 0;
}

int main(int argc, char const *argv[])
{
	PQC_bench();
}