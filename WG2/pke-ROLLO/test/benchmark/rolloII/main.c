#define _GNU_SOURCE

#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/syscall.h>

#include "seedexpander.h"
#include "rbc.h"


#define NB_TEST 100
#define NB_SAMPLES 100



inline static uint64_t cpucyclesStart (void) {
	unsigned hi, lo;
	__asm__ __volatile__ (	"CPUID\n\t"
				"RDTSC\n\t"
				"mov %%edx, %0\n\t"
				"mov %%eax, %1\n\t"
				: "=r" (hi), "=r" (lo)
				:
				: "%rax", "%rbx", "%rcx", "%rdx");

	return ((uint64_t) lo) ^ (((uint64_t) hi) << 32);
}



inline static uint64_t cpucyclesStop (void) {
	unsigned hi, lo;
	__asm__ __volatile__(	"RDTSCP\n\t"
				"mov %%edx, %0\n\t"
				"mov %%eax, %1\n\t"
				"CPUID\n\t"
				: "=r" (hi), "=r" (lo)
				:
				: "%rax", "%rbx", "%rcx", "%rdx");

	return ((uint64_t) lo) ^ (((uint64_t) hi) << 32);
}



int main(int argc, char** argv) {

  unsigned char rolloII_128_pk[ROLLOII_128_PUBLIC_KEY_BYTES];
  unsigned char rolloII_128_sk[ROLLOII_128_SECRET_KEY_BYTES];
  unsigned char rolloII_128_ct[ROLLOII_128_CIPHERTEXT_BYTES];
  unsigned char rolloII_128_ss1[ROLLOII_128_SHARED_SECRET_BYTES];
  unsigned char rolloII_128_ss2[ROLLOII_128_SHARED_SECRET_BYTES];

  unsigned char rolloII_192_pk[ROLLOII_192_PUBLIC_KEY_BYTES];
  unsigned char rolloII_192_sk[ROLLOII_192_SECRET_KEY_BYTES];
  unsigned char rolloII_192_ct[ROLLOII_192_CIPHERTEXT_BYTES];
  unsigned char rolloII_192_ss1[ROLLOII_192_SHARED_SECRET_BYTES];
  unsigned char rolloII_192_ss2[ROLLOII_192_SHARED_SECRET_BYTES];

  unsigned char rolloII_256_pk[ROLLOII_256_PUBLIC_KEY_BYTES];
  unsigned char rolloII_256_sk[ROLLOII_256_SECRET_KEY_BYTES];
  unsigned char rolloII_256_ct[ROLLOII_256_CIPHERTEXT_BYTES];
  unsigned char rolloII_256_ss1[ROLLOII_256_SHARED_SECRET_BYTES];
  unsigned char rolloII_256_ss2[ROLLOII_256_SHARED_SECRET_BYTES];

  unsigned char seed[48];
  syscall(SYS_getrandom, seed, 48, 0);
  randombytes_init(seed, NULL, 256);

  unsigned long long timer, t1, t2;
  unsigned long long rolloII_128_keygen_mean = 0, rolloII_128_encaps_mean = 0, rolloII_128_decaps_mean = 0;
  unsigned long long rolloII_192_keygen_mean = 0, rolloII_192_encaps_mean = 0, rolloII_192_decaps_mean = 0;
  unsigned long long rolloII_256_keygen_mean = 0, rolloII_256_encaps_mean = 0, rolloII_256_decaps_mean = 0;
  int rolloII_128_failures = 0, rolloII_192_failures = 0, rolloII_256_failures = 0;


  
  /***********/
  /* ROLLOII-128 */
  /***********/

	// Cache memory heating
	for(size_t i = 0 ; i < NB_TEST ; i++) {
    rolloII_128_keygen(rolloII_128_pk, rolloII_128_sk);
	}

	// Measurement
	for(size_t i = 0 ; i < NB_SAMPLES ; i++) {
    rolloII_128_keygen(rolloII_128_pk, rolloII_128_sk);
		timer = 0;

		for(size_t j = 0 ; j < NB_TEST ; j++) {
			t1 = cpucyclesStart();
      rolloII_128_keygen(rolloII_128_pk, rolloII_128_sk);
			t2 = cpucyclesStop();

			timer += t2 - t1;
      // printf("\nrolloII_128_keygen: %i - %i", i, j);
		}

		rolloII_128_keygen_mean += timer / NB_TEST;
	}



	// Cache memory heating
	for(size_t i = 0 ; i < NB_TEST ; i++) {
    rolloII_128_encaps(rolloII_128_ct, rolloII_128_ss1, rolloII_128_pk);
	}

	// Measurement
	for(size_t i = 0 ; i < NB_SAMPLES ; i++) {
    rolloII_128_keygen(rolloII_128_pk, rolloII_128_sk);
		timer = 0;

		for(size_t j = 0 ; j < NB_TEST ; j++) {
			t1 = cpucyclesStart();
      rolloII_128_encaps(rolloII_128_ct, rolloII_128_ss1, rolloII_128_pk);
			t2 = cpucyclesStop();

			timer += t2 - t1;
      // printf("\nrolloII_128_encaps: %i - %i", i, j);
		}

		rolloII_128_encaps_mean += timer / NB_TEST;
	}



	// Cache memory heating
	for(size_t i = 0 ; i < NB_TEST ; i++) {
    rolloII_128_decaps(rolloII_128_ss2, rolloII_128_ct, rolloII_128_sk);
	}


	// Measurement
	for(size_t i = 0 ; i < NB_SAMPLES ; i++) {
    rolloII_128_keygen(rolloII_128_pk, rolloII_128_sk);
    rolloII_128_encaps(rolloII_128_ct, rolloII_128_ss1, rolloII_128_pk);
    if(rolloII_128_decaps(rolloII_128_ss2, rolloII_128_ct, rolloII_128_sk)) rolloII_128_failures++;
		timer = 0;

	  if(memcmp(rolloII_128_ss1, rolloII_128_ss2, ROLLOII_128_SHARED_SECRET_BYTES)) rolloII_128_failures++;

		for(size_t j = 0 ; j < NB_TEST ; j++) {
			t1 = cpucyclesStart();
      rolloII_128_decaps(rolloII_128_ss2, rolloII_128_ct, rolloII_128_sk);
			t2 = cpucyclesStop();

			timer += t2 - t1;
      // printf("\nrolloII_128_decaps: %i - %i", i, j);
		}

		rolloII_128_decaps_mean += timer / NB_TEST;
	}



  /***********/
  /* ROLLOII-192 */
  /***********/

	// Cache memory heating
	for(size_t i = 0 ; i < NB_TEST ; i++) {
    rolloII_192_keygen(rolloII_192_pk, rolloII_192_sk);
	}

	// Measurement
	for(size_t i = 0 ; i < NB_SAMPLES ; i++) {
    rolloII_192_keygen(rolloII_192_pk, rolloII_192_sk);
		timer = 0;

		for(size_t j = 0 ; j < NB_TEST ; j++) {
			t1 = cpucyclesStart();
      rolloII_192_keygen(rolloII_192_pk, rolloII_192_sk);
			t2 = cpucyclesStop();

			timer += t2 - t1;
      // printf("\nrolloII_192_keygen: %i - %i", i, j);
		}

		rolloII_192_keygen_mean += timer / NB_TEST;
	}



	// Cache memory heating
	for(size_t i = 0 ; i < NB_TEST ; i++) {
    rolloII_192_encaps(rolloII_192_ct, rolloII_192_ss1, rolloII_192_pk);
	}

	// Measurement
	for(size_t i = 0 ; i < NB_SAMPLES ; i++) {
    rolloII_192_keygen(rolloII_192_pk, rolloII_192_sk);
		timer = 0;

		for(size_t j = 0 ; j < NB_TEST ; j++) {
			t1 = cpucyclesStart();
      rolloII_192_encaps(rolloII_192_ct, rolloII_192_ss1, rolloII_192_pk);
			t2 = cpucyclesStop();

			timer += t2 - t1;
      // printf("\nrolloII_192_encaps: %i - %i", i, j);
		}

		rolloII_192_encaps_mean += timer / NB_TEST;
	}



	// Cache memory heating
	for(size_t i = 0 ; i < NB_TEST ; i++) {
    rolloII_192_decaps(rolloII_192_ss2, rolloII_192_ct, rolloII_192_sk);
	}


	// Measurement
	for(size_t i = 0 ; i < NB_SAMPLES ; i++) {
    rolloII_192_keygen(rolloII_192_pk, rolloII_192_sk);
    rolloII_192_encaps(rolloII_192_ct, rolloII_192_ss1, rolloII_192_pk);
    if(rolloII_192_decaps(rolloII_192_ss2, rolloII_192_ct, rolloII_192_sk)) rolloII_192_failures++;
		timer = 0;

	  if(memcmp(rolloII_192_ss1, rolloII_192_ss2, ROLLOII_192_SHARED_SECRET_BYTES)) rolloII_192_failures++;

		for(size_t j = 0 ; j < NB_TEST ; j++) {
			t1 = cpucyclesStart();
      rolloII_192_decaps(rolloII_192_ss2, rolloII_192_ct, rolloII_192_sk);
			t2 = cpucyclesStop();

			timer += t2 - t1;
      // printf("\nrolloII_192_decaps: %i - %i", i, j);
		}

		rolloII_192_decaps_mean += timer / NB_TEST;
	}



  /***********/
  /* ROLLOII-256 */
  /***********/

	// Cache memory heating
	for(size_t i = 0 ; i < NB_TEST ; i++) {
    rolloII_256_keygen(rolloII_256_pk, rolloII_256_sk);
	}

	// Measurement
	for(size_t i = 0 ; i < NB_SAMPLES ; i++) {
    rolloII_256_keygen(rolloII_256_pk, rolloII_256_sk);
		timer = 0;

		for(size_t j = 0 ; j < NB_TEST ; j++) {
			t1 = cpucyclesStart();
      rolloII_256_keygen(rolloII_256_pk, rolloII_256_sk);
			t2 = cpucyclesStop();

			timer += t2 - t1;
      // printf("\nrolloII_256_keygen: %i - %i", i, j);
		}

		rolloII_256_keygen_mean += timer / NB_TEST;
	}



	// Cache memory heating
	for(size_t i = 0 ; i < NB_TEST ; i++) {
    rolloII_256_encaps(rolloII_256_ct, rolloII_256_ss1, rolloII_256_pk);
	}

	// Measurement
	for(size_t i = 0 ; i < NB_SAMPLES ; i++) {
    rolloII_256_keygen(rolloII_256_pk, rolloII_256_sk);
		timer = 0;

		for(size_t j = 0 ; j < NB_TEST ; j++) {
			t1 = cpucyclesStart();
      rolloII_256_encaps(rolloII_256_ct, rolloII_256_ss1, rolloII_256_pk);
			t2 = cpucyclesStop();

			timer += t2 - t1;
      // printf("\nrolloII_256_encaps: %i - %i", i, j);
		}

		rolloII_256_encaps_mean += timer / NB_TEST;
	}



	// Cache memory heating
	for(size_t i = 0 ; i < NB_TEST ; i++) {
    rolloII_256_decaps(rolloII_256_ss2, rolloII_256_ct, rolloII_256_sk);
	}


	// Measurement
	for(size_t i = 0 ; i < NB_SAMPLES ; i++) {
    rolloII_256_keygen(rolloII_256_pk, rolloII_256_sk);
    rolloII_256_encaps(rolloII_256_ct, rolloII_256_ss1, rolloII_256_pk);
    if(rolloII_256_decaps(rolloII_256_ss2, rolloII_256_ct, rolloII_256_sk)) rolloII_256_failures++;
		timer = 0;

	  if(memcmp(rolloII_256_ss1, rolloII_256_ss2, ROLLOII_256_SHARED_SECRET_BYTES)) rolloII_256_failures++;

		for(size_t j = 0 ; j < NB_TEST ; j++) {
			t1 = cpucyclesStart();
      rolloII_256_decaps(rolloII_256_ss2, rolloII_256_ct, rolloII_256_sk);
			t2 = cpucyclesStop();

			timer += t2 - t1;
      // printf("\nrolloII_256_decaps: %i - %i", i, j);
		}

		rolloII_256_decaps_mean += timer / NB_TEST;
	}



  printf("\n\n");
  printf("\nROLLOII-128");
  printf("\n  Failures: %i", rolloII_128_failures);
  printf("\n  Keygen: %lld CPU cycles", rolloII_128_keygen_mean / NB_SAMPLES);
  printf("\n  Encaps: %lld CPU cycles", rolloII_128_encaps_mean / NB_SAMPLES);
  printf("\n  Decaps: %lld CPU cycles", rolloII_128_decaps_mean / NB_SAMPLES);

  printf("\n\nROLLOII-192");
  printf("\n  Failures: %i", rolloII_192_failures);
  printf("\n  Keygen: %lld CPU cycles", rolloII_192_keygen_mean / NB_SAMPLES);
  printf("\n  Encaps: %lld CPU cycles", rolloII_192_encaps_mean / NB_SAMPLES);
  printf("\n  Decaps: %lld CPU cycles", rolloII_192_decaps_mean / NB_SAMPLES);

  printf("\n\nROLLOII-256");
  printf("\n  Failures: %i", rolloII_256_failures);
  printf("\n  Keygen: %lld CPU cycles", rolloII_256_keygen_mean / NB_SAMPLES);
  printf("\n  Encaps: %lld CPU cycles", rolloII_256_encaps_mean / NB_SAMPLES);
  printf("\n  Decaps: %lld CPU cycles", rolloII_256_decaps_mean / NB_SAMPLES);
  printf("\n\n");

  return 0;
}
