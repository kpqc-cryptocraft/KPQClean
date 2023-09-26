
#include <stdio.h>
#include "rbc.h"

#define VERBOSE

#define TEST_LOOP 10000

uint64_t t[TEST_LOOP];

int64_t cpucycles(void)
{
	unsigned int hi, lo;

    __asm__ __volatile__ ("rdtsc\n\t" : "=a" (lo), "=d"(hi));

    return ((int64_t)lo) | (((int64_t)hi) << 32);
}

// static long long cpucycles() {
//   unsigned long long result;
//   __asm__ volatile(".byte 15;.byte 49;shlq $32,%%rdx;orq %%rdx,%%rax" : "=a" (result) ::  "%rdx");
//   return result;
// }

int main() {

  unsigned char pk[ROLLOI_PUBLIC_KEY_BYTES] = {0};
  unsigned char sk[ROLLOI_SECRET_KEY_BYTES] = {0};
  unsigned char ct[ROLLOI_CIPHERTEXT_BYTES] = {0};
  unsigned char ss1[ROLLOI_SHARED_SECRET_BYTES] = {0};
  unsigned char ss2[ROLLOI_SHARED_SECRET_BYTES] = {0};


  unsigned long long kcycles, ecycles, dcycles;
  unsigned long long cycles1, cycles2;

	FILE *fp_keygen, *fp_sign, *fp_verify;

	fp_keygen = fopen("bench_keygen.txt", "w");
	fp_sign = fopen("bench_sign.txt", "w");
	fp_verify = fopen("bench_verify.txt", "w");

  printf("BENCHMARK ENVIRONMENTS  ============================= \n");
  printf("Number of loop: %d \n", TEST_LOOP);
	printf("KeyGen ////////////////////////////////////////////// \n");

  kcycles=0;
	for (int i = 0; i < TEST_LOOP; i++)
	{
		cycles1 = cpucycles();
		rolloI_keygen(pk, sk);
		cycles2 = cpucycles();
		fprintf(fp_keygen, "%lld \n", cycles2-cycles1);
        kcycles += cycles2-cycles1;
	}
    printf("  KeyGen runs in ................. %8lld cycles", kcycles/TEST_LOOP);
    printf("\n"); 


	fclose(fp_keygen);

	printf("Sign //////////////////////////////////////////////// \n");
	kcycles=0;
	for (int i = 0; i < TEST_LOOP; i++)
	{
		cycles1 = cpucycles();
		rolloI_encaps(ct, ss1, pk);
		cycles2 = cpucycles();
		fprintf(fp_sign, "%lld \n", cycles2-cycles1);
        kcycles += cycles2-cycles1;
	}
    printf("  Sign runs in ................. %8lld cycles", kcycles / TEST_LOOP);
    printf("\n"); 

	fclose(fp_sign);

	printf("Verify ////////////////////////////////////////////// \n");
	kcycles = 0;
	for (int i = 0; i < TEST_LOOP; i++)
	{
		cycles1 = cpucycles();
		rolloI_decaps(ss2, ct, sk);
		cycles2 = cpucycles();
		fprintf(fp_verify, "%lld \n", cycles2-cycles1);
        kcycles += cycles2 - cycles1;
	}
    printf("  Verify runs in ................. %8lld cycles", kcycles / TEST_LOOP);
    printf("\n"); 

	fclose(fp_verify);    
  printf("==================================================== \n");

  printf("\n\nsecret1: ");
  for(int i = 0 ; i < ROLLOI_SHARED_SECRET_BYTES ; ++i) printf("%02x", ss1[i]);

  printf("\nsecret2: ");
  for(int i = 0 ; i < ROLLOI_SHARED_SECRET_BYTES ; ++i) printf("%02x", ss2[i]);
  printf("\n\n");
}

