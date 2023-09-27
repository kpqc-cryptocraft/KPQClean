#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "rng.h"
#include "api.h"
#include "gf_table_gen.h"
#include "common.h"
#include "gf.h"
#include "include/lsh_def.h"
#include "include/lsh.h"

#include <stdint.h>

#define TEST_LOOP 100

#define flag    1   // 1, 2, 3


uint64_t t[TEST_LOOP];

int64_t cpucycles(void)
{
    unsigned int hi, lo;

    __asm__ __volatile__ ("rdtsc\n\t" : "=a" (lo), "=d"(hi));

    return ((int64_t)lo) | (((int64_t)hi) << 32);
}

#define MAX_CRYPTO_PUBLICKEYBYTES   1025024
#define MAX_CRYPTO_SECRETKEYBYTES   357064
#define MAX_CRYPTO_CIPHERTEXTBYTES  240
#define MAX_CRYPTO_BYTES            32

int Module_bench(void)
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

    printf("BENCHMARK ENVIRONMENTS  ============================= \n");
    printf("CRYPTO_PUBLICKEYBYTES: %d\n", CRYPTO_PUBLICKEYBYTES);
    printf("CRYPTO_SECRETKEYBYTES: %d\n", CRYPTO_SECRETKEYBYTES);
    printf("CRYPTO_BYTES: %d\n", CRYPTO_BYTES);
    printf("CRYPTO_CIPHERTEXTBYTES: %d\n", CRYPTO_CIPHERTEXTBYTES);
    printf("Number of loop: %d \n", TEST_LOOP);
    
    
    printf("Randbitgen in Encapsulation ////////////////////////////////////////////// \n");
    kcycles=0;
    u64 rp[4]={0,};
    printf("Bit size: 256 \n");
    for (int i = 0; i < TEST_LOOP; i++)
    {
        cycles1 = cpucycles();
        randbitgen(rp, 256);
        cycles2 = cpucycles();
        kcycles += cycles2-cycles1;
    }
    printf("  Randbitgen in Encapsulation in ................. %8lld nsec", kcycles/TEST_LOOP);
    printf("\n");
    
    

    printf("LSH512 hash function /////////////////////////////////////// \n");
    kcycles=0;
    lsh_type algtype = LSH_MAKE_TYPE(1, 512);
    int nset[3]={3904,5568,6592};
    int msglen = nset[flag-1];
    lsh_u8 src[8+(msglen/8)];
    lsh_u8 result[64] = {0, };
    printf("Data bit len: %d \n", 64+msglen);
    for (int i = 0; i < TEST_LOOP; i++)
    {
        cycles1 = cpucycles();
        lsh_digest(algtype, src, 64+msglen, result);
        cycles2 = cpucycles();
        kcycles += cycles2-cycles1;
    }
    

    printf("  LSH512 hash function  runs in ......... %8lld nsec", kcycles/TEST_LOOP);
    printf("\n");

    printf("==================================================== \n");

    return 0;
}

int main(int argc, char const *argv[])
{
    Module_bench();
}
