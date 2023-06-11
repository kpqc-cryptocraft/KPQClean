#include <time.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "mqs_config.h"
#include "utils.h"      
#include "utils_prng.h"
#include "api.h"

// Test Parameters 
#define TEST_KEYGEN		10
#define TEST_LOOPS		100
#define TEST_PASSED		1
#define TEST_FAILED		0

#define OS_WIN       1
#define OS_LINUX     2

#if defined(__WINDOWS__)        // Microsoft Windows OS
#define OS_TARGET OS_WIN
#else // Linux OS
#define OS_TARGET OS_LINUX 
#endif

#if defined(__i386__)

static __inline__ unsigned long long rdtsc(void)
{
	unsigned long long int x;
	__asm__ volatile (".byte 0x0f, 0x31" : "=A" (x));
	return x;
}
#elif defined(__x86_64__)


static __inline__ unsigned long long rdtsc(void)
{
	unsigned hi, lo;
	__asm__ __volatile__("rdtsc" : "=a"(lo), "=d"(hi));
	return ((unsigned long long)lo) | (((unsigned long long)hi) << 32);
}

#elif defined(__powerpc__)
static __inline__ unsigned long long rdtsc(void)
{
	unsigned long long int result = 0;
	unsigned long int upper, lower, tmp;
	__asm__ volatile(
		"0:                  \n"
		"\tmftbu   %0           \n"
		"\tmftb    %1           \n"
		"\tmftbu   %2           \n"
		"\tcmpw    %2,%0        \n"
		"\tbne     0b         \n"
		: "=r"(upper), "=r"(lower), "=r"(tmp)
		);
	result = upper;
	result = result << 32;
	result = result | lower;

	return(result);
}
#endif

int64_t cpucycles(void)
{ // Access system counter for benchmarking
#if (OS_TARGET == OS_WIN) && (TARGET == TARGET_AMD64 || TARGET == TARGET_x86)
	return __rdtsc();
#elif (OS_TARGET == OS_WIN) && (TARGET == TARGET_ARM)
	return __rdpmccntr64();
#elif (OS_TARGET == OS_LINUX) && (TARGET == TARGET_AMD64 || TARGET == TARGET_x86)
	unsigned int hi, lo;

	__asm__ __volatile__("rdtsc\n\t" : "=a" (lo), "=d"(hi));
	return ((int64_t)lo) | (((int64_t)hi) << 32);
#elif (OS_TARGET == OS_LINUX) && (TARGET == TARGET_ARM || TARGET == TARGET_ARM64)
	struct timespec time;

	clock_gettime(CLOCK_REALTIME, &time);
	return (int64_t)(time.tv_sec * 1e9 + time.tv_nsec);
#else
	return 0;
#endif
}

int
crypto_sign_keypair(unsigned char* pk, unsigned char* sk, unsigned char* sk_seed);

int
crypto_sign(unsigned char* sm, unsigned long long* smlen, const unsigned char* m, unsigned long long mlen, const unsigned char* sk, const uint8_t* sk_seed, uint8_t depth);
int
crypto_sign_open(unsigned char* m, unsigned long long* mlen, const unsigned char* sm, unsigned long long smlen, const unsigned char* pk);

int cryptotest_sig()
{
	unsigned i;
	unsigned char msg[20] = { 0, };
	unsigned long long smlen = 0;
	unsigned long long mlen = 20;
	unsigned char  entropy_input[48];
	// unsigned char pk[CRYPTO_PUBLICKEYBYTES] = { 0, };
	//unsigned char sk[CRYPTO_SECRETKEYBYTES] = { 0, };
	unsigned char ss[CRYPTO_BYTES + 20] = { 0, };
	int ret = TEST_PASSED;
	int r = 0;

	srand(time(NULL));
	for (int i = 0; i < 48; i++)
		entropy_input[i] = rand() % 256;

	randombytes_init(entropy_input, NULL, 256);
	randombytes(msg, mlen);

	unsigned char* pk = (unsigned char*)malloc(CRYPTO_PUBLICKEYBYTES);
	unsigned char* sk = (unsigned char*)malloc(CRYPTO_SECRETKEYBYTES);

	for (int depth = 0; depth <= MAX_DEPTH; depth++)
	{
		printf("\n\nTESTING MULTIVARIATE-BASED DIGITAL SIGNATURE - %s with depth %d  \n", CRYPTO_ALGNAME, depth);
		printf("--------------------------------------------------------------------------------------------------------\n\n");
		for (i = 0; i < 10; i++)
		{
			unsigned char sk_seed[LEN_SKSEED] = { 0 };
			randombytes(sk_seed, LEN_SKSEED);

			crypto_sign_keypair(pk, sk, sk_seed);

			crypto_sign(ss, &smlen, msg, mlen, sk, sk_seed, depth);
			r = crypto_sign_open(msg, &mlen, ss, mlen + CRYPTO_BYTES, pk);

			if (r)
			{
				ret = TEST_FAILED;
				break;
			}

		}
		if (ret == TEST_PASSED)
		{
			printf("  Signature Verification tests .................................................... PASSED \n");
			printf(" Test Loop : %d \n", i);
		}
		else
		{
			printf("\n\n%d\n\n", i);
			printf("  Signature Verification tests ... FAILED"); printf("\n");

			return TEST_FAILED;
		}
		printf("\n");
	}


	free(pk);
	free(sk);

	return ret;

}

int cryptorun_sig()
{
	unsigned int i;
	/*unsigned char pk[CRYPTO_PUBLICKEYBYTES] = { 0, };
	unsigned char sk[CRYPTO_SECRETKEYBYTES] = { 0, };*/
	unsigned char ss[CRYPTO_BYTES + 20] = { 0, };
	unsigned char msg[20] = { 0, };
	int ret = TEST_PASSED;
	unsigned long long smlen = 0;
	unsigned long long mlen = 20;
	unsigned long long cycles, cycles1, cycles2;
	unsigned char sk_seed[LEN_SKSEED] = { 0 };

	randombytes(msg, mlen);
	unsigned char* pk = (unsigned char*)malloc(CRYPTO_PUBLICKEYBYTES);
	unsigned char* sk = (unsigned char*)malloc(CRYPTO_SECRETKEYBYTES);
	cycles = 0;
	for (i = 0; i < TEST_KEYGEN; i++)
	{
		randombytes(sk_seed, LEN_SKSEED);
		cycles1 = cpucycles();
		// Key Generation
		crypto_sign_keypair(pk, sk, sk_seed);
		cycles2 = cpucycles();
		cycles = cycles + (cycles2 - cycles1);
	}
	printf("  Key generation runs in ....................................... %10lld ", cycles / TEST_KEYGEN);
	printf("\n");

	for (int depth = 0; depth <= MAX_DEPTH; depth++)
	{
		printf("\n\nBENCHMARKING MULTIVARIATE-BASED DIGITAL SIGNATURE - %s with depth %d \n", CRYPTO_ALGNAME, depth);
		printf("--------------------------------------------------------------------------------------------------------\n\n");

		cycles = 0;
		for (i = 0; i < TEST_LOOPS; i++)
		{
			cycles1 = cpucycles();
			// Signature Generation
			//randombytes(msg, mlen);
			crypto_sign(ss, &smlen, msg, mlen, sk, sk_seed, depth);
			cycles2 = cpucycles();
			cycles = cycles + (cycles2 - cycles1);

		}

		printf("  Signature Generation runs in ....................................... %10lld ", cycles / TEST_LOOPS);
		printf("\n");

		cycles = 0;
		for (i = 0; i < TEST_LOOPS; i++)
		{
			cycles1 = cpucycles();
			// Signature Verification
			//randombytes(msg, mlen);
			crypto_sign_open(msg, &mlen, ss, mlen + CRYPTO_BYTES, pk);
			cycles2 = cpucycles();
			cycles = cycles + (cycles2 - cycles1);

		}

		printf("  Signature Verification runs in ....................................... %10lld ", cycles / TEST_LOOPS);
		printf("\n");
	}
	free(pk);
	free(sk);
	return ret;
}

int main()
{
	int ret = TEST_PASSED;

	ret = cryptotest_sig();
	if (ret != TEST_PASSED)
	{
		printf("\n\n   Error detected: SIGNATURE_VERIFICATION_FAILED \n\n");
	}

	cryptorun_sig();

	printf("\n\n");
	printf("CRYPTO_ALGNAME : %s \n", CRYPTO_ALGNAME);
	printf("_SEC_KEY_LEN : %ld \n", (_SEC_KEY_LEN - 1));
	printf("_PUB_KEY_LEN : %d \n", (_PUB_KEY_LEN - 1));
	printf("TEST_LOOPS : %d \n", TEST_LOOPS);

	return ret;
}
