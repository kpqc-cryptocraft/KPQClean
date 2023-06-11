#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "api.h"
#include "rng.h"
#include "sha2.h"

#define TEST_LOOP 1000000

uint64_t t[TEST_LOOP];

int64_t cpucycles(void)
{
    unsigned int hi, lo;

    __asm__ __volatile__ ("rdtsc\n\t" : "=a" (lo), "=d"(hi));

    return ((int64_t)lo) | (((int64_t)hi) << 32);
}

int Module_bench(void)
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
    printf("RandomByte in Encryption////////////////////////////////////////////// \n");
    kcycles=0;
    uint8_t msg[NTRUPLUS_N/8];
    printf("MSG size: %d \n", NTRUPLUS_N/8);
    for (int i = 0; i < TEST_LOOP; i++)
    {
        cycles1 = cpucycles();
        randombytes(msg, NTRUPLUS_N/8);
        cycles2 = cpucycles();
        kcycles += cycles2-cycles1;
    }
    printf("  RandomByte in Encryption runs in ................. %8lld cycles", kcycles/TEST_LOOP);
    printf("\n");

    
    printf("RandomByte in Keypair////////////////////////////////////////////// \n");
    kcycles=0;
    uint8_t buf[NTRUPLUS_N/2];
    printf("Keypair buf size: %d \n", 32);
    for (int i = 0; i < TEST_LOOP; i++)
    {
        cycles1 = cpucycles();
        randombytes(buf, 32);
        cycles2 = cpucycles();
        kcycles += cycles2-cycles1;
    }
    printf("  RandomByte in Keypair runs in ................. %8lld cycles", kcycles/TEST_LOOP);
    printf("\n");
    
    
    printf("SHA512 /////////////////////////////////////// \n");
    kcycles=0;
    uint8_t buf1[NTRUPLUS_SYMBYTES + NTRUPLUS_N/4];
    printf("SHA512 input size: %d \n", NTRUPLUS_N/8);
    for (int i = 0; i < TEST_LOOP; i++)
    {
        cycles1 = cpucycles();
        sha512(buf1, msg, NTRUPLUS_N/8);
        cycles2 = cpucycles();
        kcycles += cycles2-cycles1;

    }

    printf("  SHA512  runs in ......... %8lld cycles", kcycles/TEST_LOOP);
    printf("\n");

    
    printf("SHA256 /////////////////////////////////////// \n");
    kcycles=0;
    uint8_t buf2[NTRUPLUS_POLYBYTES];
    printf("SHA256 input size: %d \n", NTRUPLUS_POLYBYTES);
    for (int i = 0; i < TEST_LOOP; i++)
    {
        cycles1 = cpucycles();
        sha256(buf2, buf2, NTRUPLUS_POLYBYTES);
        cycles2 = cpucycles();
        kcycles += cycles2-cycles1;
    }

    printf("  SHA256  runs in ......... %8lld cycles", kcycles/TEST_LOOP);
    printf("\n");

    printf("==================================================== \n");

    return 0;
}

int main(int argc, char const *argv[])
{
    Module_bench();
}
