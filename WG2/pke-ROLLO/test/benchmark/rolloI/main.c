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

  unsigned char rolloI_128_pk[ROLLOI_128_PUBLIC_KEY_BYTES];
  unsigned char rolloI_128_sk[ROLLOI_128_SECRET_KEY_BYTES];
  unsigned char rolloI_128_ct[ROLLOI_128_CIPHERTEXT_BYTES];
  unsigned char rolloI_128_ss1[ROLLOI_128_SHARED_SECRET_BYTES];
  unsigned char rolloI_128_ss2[ROLLOI_128_SHARED_SECRET_BYTES];

  unsigned char rolloI_192_pk[ROLLOI_192_PUBLIC_KEY_BYTES];
  unsigned char rolloI_192_sk[ROLLOI_192_SECRET_KEY_BYTES];
  unsigned char rolloI_192_ct[ROLLOI_192_CIPHERTEXT_BYTES];
  unsigned char rolloI_192_ss1[ROLLOI_192_SHARED_SECRET_BYTES];
  unsigned char rolloI_192_ss2[ROLLOI_192_SHARED_SECRET_BYTES];

  unsigned char rolloI_256_pk[ROLLOI_256_PUBLIC_KEY_BYTES];
  unsigned char rolloI_256_sk[ROLLOI_256_SECRET_KEY_BYTES];
  unsigned char rolloI_256_ct[ROLLOI_256_CIPHERTEXT_BYTES];
  unsigned char rolloI_256_ss1[ROLLOI_256_SHARED_SECRET_BYTES];
  unsigned char rolloI_256_ss2[ROLLOI_256_SHARED_SECRET_BYTES];

  unsigned char seed[48];
  syscall(SYS_getrandom, seed, 48, 0);
  randombytes_init(seed, NULL, 256);

  unsigned long long timer, t1, t2;
  unsigned long long rolloI_128_keygen_mean = 0, rolloI_128_encaps_mean = 0, rolloI_128_decaps_mean = 0;
  unsigned long long rolloI_192_keygen_mean = 0, rolloI_192_encaps_mean = 0, rolloI_192_decaps_mean = 0;
  unsigned long long rolloI_256_keygen_mean = 0, rolloI_256_encaps_mean = 0, rolloI_256_decaps_mean = 0;
  int rolloI_128_failures = 0, rolloI_192_failures = 0, rolloI_256_failures = 0;


  
  /***********/
  /* ROLLOI-128 */
  /***********/

	// Cache memory heating
	for(size_t i = 0 ; i < NB_TEST ; i++) {
    rolloI_128_keygen(rolloI_128_pk, rolloI_128_sk);
	}

	// Measurement
	for(size_t i = 0 ; i < NB_SAMPLES ; i++) {
    rolloI_128_keygen(rolloI_128_pk, rolloI_128_sk);
		timer = 0;

		for(size_t j = 0 ; j < NB_TEST ; j++) {
			t1 = cpucyclesStart();
      rolloI_128_keygen(rolloI_128_pk, rolloI_128_sk);
			t2 = cpucyclesStop();

			timer += t2 - t1;
      // printf("\nrolloI_128_keygen: %i - %i", i, j);
		}

		rolloI_128_keygen_mean += timer / NB_TEST;
	}



	// Cache memory heating
	for(size_t i = 0 ; i < NB_TEST ; i++) {
    rolloI_128_encaps(rolloI_128_ct, rolloI_128_ss1, rolloI_128_pk);
	}

	// Measurement
	for(size_t i = 0 ; i < NB_SAMPLES ; i++) {
    rolloI_128_keygen(rolloI_128_pk, rolloI_128_sk);
		timer = 0;

		for(size_t j = 0 ; j < NB_TEST ; j++) {
			t1 = cpucyclesStart();
      rolloI_128_encaps(rolloI_128_ct, rolloI_128_ss1, rolloI_128_pk);
			t2 = cpucyclesStop();

			timer += t2 - t1;
      // printf("\nrolloI_128_encaps: %i - %i", i, j);
		}

		rolloI_128_encaps_mean += timer / NB_TEST;
	}



	// Cache memory heating
	for(size_t i = 0 ; i < NB_TEST ; i++) {
    rolloI_128_decaps(rolloI_128_ss2, rolloI_128_ct, rolloI_128_sk);
	}


	// Measurement
	for(size_t i = 0 ; i < NB_SAMPLES ; i++) {
    rolloI_128_keygen(rolloI_128_pk, rolloI_128_sk);
    rolloI_128_encaps(rolloI_128_ct, rolloI_128_ss1, rolloI_128_pk);
    if(rolloI_128_decaps(rolloI_128_ss2, rolloI_128_ct, rolloI_128_sk)) rolloI_128_failures++;
		timer = 0;

	  if(memcmp(rolloI_128_ss1, rolloI_128_ss2, ROLLOI_128_SHARED_SECRET_BYTES)) rolloI_128_failures++;

		for(size_t j = 0 ; j < NB_TEST ; j++) {
			t1 = cpucyclesStart();
      rolloI_128_decaps(rolloI_128_ss2, rolloI_128_ct, rolloI_128_sk);
			t2 = cpucyclesStop();

			timer += t2 - t1;
      // printf("\nrolloI_128_decaps: %i - %i", i, j);
		}

		rolloI_128_decaps_mean += timer / NB_TEST;
	}



  /***********/
  /* ROLLOI-192 */
  /***********/

	// Cache memory heating
	for(size_t i = 0 ; i < NB_TEST ; i++) {
    rolloI_192_keygen(rolloI_192_pk, rolloI_192_sk);
	}

	// Measurement
	for(size_t i = 0 ; i < NB_SAMPLES ; i++) {
    rolloI_192_keygen(rolloI_192_pk, rolloI_192_sk);
		timer = 0;

		for(size_t j = 0 ; j < NB_TEST ; j++) {
			t1 = cpucyclesStart();
      rolloI_192_keygen(rolloI_192_pk, rolloI_192_sk);
			t2 = cpucyclesStop();

			timer += t2 - t1;
      // printf("\nrolloI_192_keygen: %i - %i", i, j);
		}

		rolloI_192_keygen_mean += timer / NB_TEST;
	}



	// Cache memory heating
	for(size_t i = 0 ; i < NB_TEST ; i++) {
    rolloI_192_encaps(rolloI_192_ct, rolloI_192_ss1, rolloI_192_pk);
	}

	// Measurement
	for(size_t i = 0 ; i < NB_SAMPLES ; i++) {
    rolloI_192_keygen(rolloI_192_pk, rolloI_192_sk);
		timer = 0;

		for(size_t j = 0 ; j < NB_TEST ; j++) {
			t1 = cpucyclesStart();
      rolloI_192_encaps(rolloI_192_ct, rolloI_192_ss1, rolloI_192_pk);
			t2 = cpucyclesStop();

			timer += t2 - t1;
      // printf("\nrolloI_192_encaps: %i - %i", i, j);
		}

		rolloI_192_encaps_mean += timer / NB_TEST;
	}



	// Cache memory heating
	for(size_t i = 0 ; i < NB_TEST ; i++) {
    rolloI_192_decaps(rolloI_192_ss2, rolloI_192_ct, rolloI_192_sk);
	}


	// Measurement
	for(size_t i = 0 ; i < NB_SAMPLES ; i++) {
    rolloI_192_keygen(rolloI_192_pk, rolloI_192_sk);
    rolloI_192_encaps(rolloI_192_ct, rolloI_192_ss1, rolloI_192_pk);
    if(rolloI_192_decaps(rolloI_192_ss2, rolloI_192_ct, rolloI_192_sk)) rolloI_192_failures++;
		timer = 0;

	  if(memcmp(rolloI_192_ss1, rolloI_192_ss2, ROLLOI_192_SHARED_SECRET_BYTES)) rolloI_192_failures++;

		for(size_t j = 0 ; j < NB_TEST ; j++) {
			t1 = cpucyclesStart();
      rolloI_192_decaps(rolloI_192_ss2, rolloI_192_ct, rolloI_192_sk);
			t2 = cpucyclesStop();

			timer += t2 - t1;
      // printf("\nrolloI_192_decaps: %i - %i", i, j);
		}

		rolloI_192_decaps_mean += timer / NB_TEST;
	}



  /***********/
  /* ROLLOI-256 */
  /***********/

	// Cache memory heating
	for(size_t i = 0 ; i < NB_TEST ; i++) {
    rolloI_256_keygen(rolloI_256_pk, rolloI_256_sk);
	}

	// Measurement
	for(size_t i = 0 ; i < NB_SAMPLES ; i++) {
    rolloI_256_keygen(rolloI_256_pk, rolloI_256_sk);
		timer = 0;

		for(size_t j = 0 ; j < NB_TEST ; j++) {
			t1 = cpucyclesStart();
      rolloI_256_keygen(rolloI_256_pk, rolloI_256_sk);
			t2 = cpucyclesStop();

			timer += t2 - t1;
      // printf("\nrolloI_256_keygen: %i - %i", i, j);
		}

		rolloI_256_keygen_mean += timer / NB_TEST;
	}



	// Cache memory heating
	for(size_t i = 0 ; i < NB_TEST ; i++) {
    rolloI_256_encaps(rolloI_256_ct, rolloI_256_ss1, rolloI_256_pk);
	}

	// Measurement
	for(size_t i = 0 ; i < NB_SAMPLES ; i++) {
    rolloI_256_keygen(rolloI_256_pk, rolloI_256_sk);
		timer = 0;

		for(size_t j = 0 ; j < NB_TEST ; j++) {
			t1 = cpucyclesStart();
      rolloI_256_encaps(rolloI_256_ct, rolloI_256_ss1, rolloI_256_pk);
			t2 = cpucyclesStop();

			timer += t2 - t1;
      // printf("\nrolloI_256_encaps: %i - %i", i, j);
		}

		rolloI_256_encaps_mean += timer / NB_TEST;
	}



	// Cache memory heating
	for(size_t i = 0 ; i < NB_TEST ; i++) {
    rolloI_256_decaps(rolloI_256_ss2, rolloI_256_ct, rolloI_256_sk);
	}


	// Measurement
	for(size_t i = 0 ; i < NB_SAMPLES ; i++) {
    rolloI_256_keygen(rolloI_256_pk, rolloI_256_sk);
    rolloI_256_encaps(rolloI_256_ct, rolloI_256_ss1, rolloI_256_pk);
    if(rolloI_256_decaps(rolloI_256_ss2, rolloI_256_ct, rolloI_256_sk)) rolloI_256_failures++;
		timer = 0;

	  if(memcmp(rolloI_256_ss1, rolloI_256_ss2, ROLLOI_256_SHARED_SECRET_BYTES)) rolloI_256_failures++;

		for(size_t j = 0 ; j < NB_TEST ; j++) {
			t1 = cpucyclesStart();
      rolloI_256_decaps(rolloI_256_ss2, rolloI_256_ct, rolloI_256_sk);
			t2 = cpucyclesStop();

			timer += t2 - t1;
      // printf("\nrolloI_256_decaps: %i - %i", i, j);
		}

		rolloI_256_decaps_mean += timer / NB_TEST;
	}



  printf("\n\n");
  printf("\nROLLOI-128");
  printf("\n  Failures: %i", rolloI_128_failures);
  printf("\n  Keygen: %lld CPU cycles", rolloI_128_keygen_mean / NB_SAMPLES);
  printf("\n  Encaps: %lld CPU cycles", rolloI_128_encaps_mean / NB_SAMPLES);
  printf("\n  Decaps: %lld CPU cycles", rolloI_128_decaps_mean / NB_SAMPLES);

  printf("\n\nROLLOI-192");
  printf("\n  Failures: %i", rolloI_192_failures);
  printf("\n  Keygen: %lld CPU cycles", rolloI_192_keygen_mean / NB_SAMPLES);
  printf("\n  Encaps: %lld CPU cycles", rolloI_192_encaps_mean / NB_SAMPLES);
  printf("\n  Decaps: %lld CPU cycles", rolloI_192_decaps_mean / NB_SAMPLES);

  printf("\n\nROLLOI-256");
  printf("\n  Failures: %i", rolloI_256_failures);
  printf("\n  Keygen: %lld CPU cycles", rolloI_256_keygen_mean / NB_SAMPLES);
  printf("\n  Encaps: %lld CPU cycles", rolloI_256_encaps_mean / NB_SAMPLES);
  printf("\n  Decaps: %lld CPU cycles", rolloI_256_decaps_mean / NB_SAMPLES);
  printf("\n\n");

  return 0;
}
