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

  unsigned char rqc_128_pk[RQC_128_PUBLIC_KEY_BYTES];
  unsigned char rqc_128_sk[RQC_128_SECRET_KEY_BYTES];
  unsigned char rqc_128_ct[RQC_128_CIPHERTEXT_BYTES];
  unsigned char rqc_128_ss1[RQC_128_SHARED_SECRET_BYTES];
  unsigned char rqc_128_ss2[RQC_128_SHARED_SECRET_BYTES];

  unsigned char rqc_192_pk[RQC_192_PUBLIC_KEY_BYTES];
  unsigned char rqc_192_sk[RQC_192_SECRET_KEY_BYTES];
  unsigned char rqc_192_ct[RQC_192_CIPHERTEXT_BYTES];
  unsigned char rqc_192_ss1[RQC_192_SHARED_SECRET_BYTES];
  unsigned char rqc_192_ss2[RQC_192_SHARED_SECRET_BYTES];

  unsigned char rqc_256_pk[RQC_256_PUBLIC_KEY_BYTES];
  unsigned char rqc_256_sk[RQC_256_SECRET_KEY_BYTES];
  unsigned char rqc_256_ct[RQC_256_CIPHERTEXT_BYTES];
  unsigned char rqc_256_ss1[RQC_256_SHARED_SECRET_BYTES];
  unsigned char rqc_256_ss2[RQC_256_SHARED_SECRET_BYTES];

  unsigned char seed[48];
  syscall(SYS_getrandom, seed, 48, 0);
  randombytes_init(seed, NULL, 256);

  unsigned long long timer, t1, t2;
  unsigned long long rqc_128_keygen_mean = 0, rqc_128_encaps_mean = 0, rqc_128_decaps_mean = 0;
  unsigned long long rqc_192_keygen_mean = 0, rqc_192_encaps_mean = 0, rqc_192_decaps_mean = 0;
  unsigned long long rqc_256_keygen_mean = 0, rqc_256_encaps_mean = 0, rqc_256_decaps_mean = 0;
  int rqc_128_failures = 0, rqc_192_failures = 0, rqc_256_failures = 0;


  
  /***********/
  /* RQC-128 */
  /***********/

	// Cache memory heating
	for(size_t i = 0 ; i < NB_TEST ; i++) {
    rqc_128_keygen(rqc_128_pk, rqc_128_sk);
	}

	// Measurement
	for(size_t i = 0 ; i < NB_SAMPLES ; i++) {
    rqc_128_keygen(rqc_128_pk, rqc_128_sk);
		timer = 0;

		for(size_t j = 0 ; j < NB_TEST ; j++) {
			t1 = cpucyclesStart();
      rqc_128_keygen(rqc_128_pk, rqc_128_sk);
			t2 = cpucyclesStop();

			timer += t2 - t1;
      // printf("\nrqc_128_keygen: %i - %i", i, j);
		}

		rqc_128_keygen_mean += timer / NB_TEST;
	}



	// Cache memory heating
	for(size_t i = 0 ; i < NB_TEST ; i++) {
    rqc_128_encaps(rqc_128_ct, rqc_128_ss1, rqc_128_pk);
	}

	// Measurement
	for(size_t i = 0 ; i < NB_SAMPLES ; i++) {
    rqc_128_keygen(rqc_128_pk, rqc_128_sk);
		timer = 0;

		for(size_t j = 0 ; j < NB_TEST ; j++) {
			t1 = cpucyclesStart();
      rqc_128_encaps(rqc_128_ct, rqc_128_ss1, rqc_128_pk);
			t2 = cpucyclesStop();

			timer += t2 - t1;
      // printf("\nrqc_128_encaps: %i - %i", i, j);
		}

		rqc_128_encaps_mean += timer / NB_TEST;
	}



	// Cache memory heating
	for(size_t i = 0 ; i < NB_TEST ; i++) {
    rqc_128_decaps(rqc_128_ss2, rqc_128_ct, rqc_128_sk);
	}


	// Measurement
	for(size_t i = 0 ; i < NB_SAMPLES ; i++) {
    rqc_128_keygen(rqc_128_pk, rqc_128_sk);
    rqc_128_encaps(rqc_128_ct, rqc_128_ss1, rqc_128_pk);
    if(rqc_128_decaps(rqc_128_ss2, rqc_128_ct, rqc_128_sk)) rqc_128_failures++;
		timer = 0;

	  if(memcmp(rqc_128_ss1, rqc_128_ss2, RQC_128_SHARED_SECRET_BYTES)) rqc_128_failures++;

		for(size_t j = 0 ; j < NB_TEST ; j++) {
			t1 = cpucyclesStart();
      rqc_128_decaps(rqc_128_ss2, rqc_128_ct, rqc_128_sk);
			t2 = cpucyclesStop();

			timer += t2 - t1;
      // printf("\nrqc_128_decaps: %i - %i", i, j);
		}

		rqc_128_decaps_mean += timer / NB_TEST;
	}



  /***********/
  /* RQC-192 */
  /***********/

	// Cache memory heating
	for(size_t i = 0 ; i < NB_TEST ; i++) {
    rqc_192_keygen(rqc_192_pk, rqc_192_sk);
	}

	// Measurement
	for(size_t i = 0 ; i < NB_SAMPLES ; i++) {
    rqc_192_keygen(rqc_192_pk, rqc_192_sk);
		timer = 0;

		for(size_t j = 0 ; j < NB_TEST ; j++) {
			t1 = cpucyclesStart();
      rqc_192_keygen(rqc_192_pk, rqc_192_sk);
			t2 = cpucyclesStop();

			timer += t2 - t1;
      // printf("\nrqc_192_keygen: %i - %i", i, j);
		}

		rqc_192_keygen_mean += timer / NB_TEST;
	}



	// Cache memory heating
	for(size_t i = 0 ; i < NB_TEST ; i++) {
    rqc_192_encaps(rqc_192_ct, rqc_192_ss1, rqc_192_pk);
	}

	// Measurement
	for(size_t i = 0 ; i < NB_SAMPLES ; i++) {
    rqc_192_keygen(rqc_192_pk, rqc_192_sk);
		timer = 0;

		for(size_t j = 0 ; j < NB_TEST ; j++) {
			t1 = cpucyclesStart();
      rqc_192_encaps(rqc_192_ct, rqc_192_ss1, rqc_192_pk);
			t2 = cpucyclesStop();

			timer += t2 - t1;
      // printf("\nrqc_192_encaps: %i - %i", i, j);
		}

		rqc_192_encaps_mean += timer / NB_TEST;
	}



	// Cache memory heating
	for(size_t i = 0 ; i < NB_TEST ; i++) {
    rqc_192_decaps(rqc_192_ss2, rqc_192_ct, rqc_192_sk);
	}


	// Measurement
	for(size_t i = 0 ; i < NB_SAMPLES ; i++) {
    rqc_192_keygen(rqc_192_pk, rqc_192_sk);
    rqc_192_encaps(rqc_192_ct, rqc_192_ss1, rqc_192_pk);
    if(rqc_192_decaps(rqc_192_ss2, rqc_192_ct, rqc_192_sk)) rqc_192_failures++;
		timer = 0;

	  if(memcmp(rqc_192_ss1, rqc_192_ss2, RQC_192_SHARED_SECRET_BYTES)) rqc_192_failures++;

		for(size_t j = 0 ; j < NB_TEST ; j++) {
			t1 = cpucyclesStart();
      rqc_192_decaps(rqc_192_ss2, rqc_192_ct, rqc_192_sk);
			t2 = cpucyclesStop();

			timer += t2 - t1;
      // printf("\nrqc_192_decaps: %i - %i", i, j);
		}

		rqc_192_decaps_mean += timer / NB_TEST;
	}



  /***********/
  /* RQC-256 */
  /***********/

	// Cache memory heating
	for(size_t i = 0 ; i < NB_TEST ; i++) {
    rqc_256_keygen(rqc_256_pk, rqc_256_sk);
	}

	// Measurement
	for(size_t i = 0 ; i < NB_SAMPLES ; i++) {
    rqc_256_keygen(rqc_256_pk, rqc_256_sk);
		timer = 0;

		for(size_t j = 0 ; j < NB_TEST ; j++) {
			t1 = cpucyclesStart();
      rqc_256_keygen(rqc_256_pk, rqc_256_sk);
			t2 = cpucyclesStop();

			timer += t2 - t1;
      // printf("\nrqc_256_keygen: %i - %i", i, j);
		}

		rqc_256_keygen_mean += timer / NB_TEST;
	}



	// Cache memory heating
	for(size_t i = 0 ; i < NB_TEST ; i++) {
    rqc_256_encaps(rqc_256_ct, rqc_256_ss1, rqc_256_pk);
	}

	// Measurement
	for(size_t i = 0 ; i < NB_SAMPLES ; i++) {
    rqc_256_keygen(rqc_256_pk, rqc_256_sk);
		timer = 0;

		for(size_t j = 0 ; j < NB_TEST ; j++) {
			t1 = cpucyclesStart();
      rqc_256_encaps(rqc_256_ct, rqc_256_ss1, rqc_256_pk);
			t2 = cpucyclesStop();

			timer += t2 - t1;
      // printf("\nrqc_256_encaps: %i - %i", i, j);
		}

		rqc_256_encaps_mean += timer / NB_TEST;
	}



	// Cache memory heating
	for(size_t i = 0 ; i < NB_TEST ; i++) {
    rqc_256_decaps(rqc_256_ss2, rqc_256_ct, rqc_256_sk);
	}


	// Measurement
	for(size_t i = 0 ; i < NB_SAMPLES ; i++) {
    rqc_256_keygen(rqc_256_pk, rqc_256_sk);
    rqc_256_encaps(rqc_256_ct, rqc_256_ss1, rqc_256_pk);
    if(rqc_256_decaps(rqc_256_ss2, rqc_256_ct, rqc_256_sk)) rqc_256_failures++;
		timer = 0;

	  if(memcmp(rqc_256_ss1, rqc_256_ss2, RQC_256_SHARED_SECRET_BYTES)) rqc_256_failures++;

		for(size_t j = 0 ; j < NB_TEST ; j++) {
			t1 = cpucyclesStart();
      rqc_256_decaps(rqc_256_ss2, rqc_256_ct, rqc_256_sk);
			t2 = cpucyclesStop();

			timer += t2 - t1;
      // printf("\nrqc_256_decaps: %i - %i", i, j);
		}

		rqc_256_decaps_mean += timer / NB_TEST;
	}



  printf("\n\n");
  printf("\nRQC-128");
  printf("\n  Failures: %i", rqc_128_failures);
  printf("\n  Keygen: %lld CPU cycles", rqc_128_keygen_mean / NB_SAMPLES);
  printf("\n  Encaps: %lld CPU cycles", rqc_128_encaps_mean / NB_SAMPLES);
  printf("\n  Decaps: %lld CPU cycles", rqc_128_decaps_mean / NB_SAMPLES);

  printf("\n\nRQC-192");
  printf("\n  Failures: %i", rqc_192_failures);
  printf("\n  Keygen: %lld CPU cycles", rqc_192_keygen_mean / NB_SAMPLES);
  printf("\n  Encaps: %lld CPU cycles", rqc_192_encaps_mean / NB_SAMPLES);
  printf("\n  Decaps: %lld CPU cycles", rqc_192_decaps_mean / NB_SAMPLES);

  printf("\n\nRQC-256");
  printf("\n  Failures: %i", rqc_256_failures);
  printf("\n  Keygen: %lld CPU cycles", rqc_256_keygen_mean / NB_SAMPLES);
  printf("\n  Encaps: %lld CPU cycles", rqc_256_encaps_mean / NB_SAMPLES);
  printf("\n  Decaps: %lld CPU cycles", rqc_256_decaps_mean / NB_SAMPLES);
  printf("\n\n");

  return 0;
}
