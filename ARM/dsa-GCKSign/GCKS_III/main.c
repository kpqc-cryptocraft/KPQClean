#include <stdio.h>
#include <string.h>
#include <stdint.h>
//#include <openssl/sha.h>
#include "sha2.h"
#include "NIST/rng.h"
#include "sign.h"
#include "reduce.h"
#include "pack.h"
#include "sample.h"


#define TEST_LOOP 100

uint64_t t[TEST_LOOP];
extern unsigned long long rejwctr;
extern unsigned long long rejyzctr;
extern unsigned long long ctr_keygen;
extern unsigned long long ctr_sign;

int64_t cpucycles(void)
{
	unsigned int hi, lo;

    __asm__ __volatile__ ("rdtsc\n\t" : "=a" (lo), "=d"(hi));

    return ((int64_t)lo) | (((int64_t)hi) << 32);
}

int test_speed(void)
{
  	
	int r;
	double rejw=.0, rejyz=.0, rejctr=.0, rejctrkg=.0;
  	unsigned long long i, j;

	unsigned char pk[CRYPTO_PUBLICKEYBYTES];
	unsigned char sk[CRYPTO_SECRETKEYBYTES];
	int8_t seed[32];
	unsigned char  m[100] = "hello world!";
	unsigned char sm[CRYPTO_BYTES + 200];

	unsigned char  m2[100];

	unsigned long long  mlen = 0;
	unsigned long long smlen;

	uint16_t nonce = 0;
	unsigned long long  m2len;

	int result;

    unsigned long long kcycles, ecycles, dcycles;
    unsigned long long cycles1, cycles2;
    unsigned char tmp[POLY_BYTES];
    mlen = strlen(m);

 	int64_t a[_M][_N];
	int64_t s[_M][_N];
	int64_t y[_M][_N];
	int64_t t[_N];
	int64_t c[_N];
	//SHA256_CTX ctx;
	sha256ctx ctx;
	int check = 0;

	



    printf("randombytes\n");
	kcycles=0;
	cycles1 = cpucycles();
	for(i = 0; i < TEST_LOOP; ++i)   	
		randombytes(pk, 32);
	cycles2 = cpucycles();
    kcycles = cycles2-cycles1;
    printf("  randombytes runs in ................. %8lld nsec", kcycles/TEST_LOOP);
    printf("\n"); 
 		
    //generate vector a
	printf("sample_a\n");
	kcycles=0;
 	cycles1 = cpucycles();
  	for(i = 0; i < TEST_LOOP; ++i) 
    	sample_a(a, pk, nonce);
  	cycles2 = cpucycles();
  	kcycles = cycles2-cycles1;
  	printf("  sample_a runs in ................. %8lld nsec", kcycles/TEST_LOOP);
    printf("\n"); 
	

    randombytes(seed, 32);

    //generate vector s
	printf("sample_s\n");
	kcycles=0;
	cycles1 = cpucycles();
	for(i = 0; i < TEST_LOOP; ++i) 
		sample_s(s, seed, nonce);
	cycles2 = cpucycles();
    kcycles = cycles2-cycles1;
    printf("  sample_s runs in ................. %8lld nsec", kcycles/TEST_LOOP);
    printf("\n"); 

    printf("pack_poly\n");
	kcycles=0;
	cycles1 = cpucycles();
	for(i = 0; i < TEST_LOOP; ++i)   	
		pack_poly(sk, s[0]);
	cycles2 = cpucycles();
    kcycles = cycles2-cycles1;
    printf("  pack_poly runs in ................. %8lld nsec", kcycles/TEST_LOOP);
    printf("\n"); 

    printf("unpack_poly\n");
	kcycles=0;
	cycles1 = cpucycles();
	for(i = 0; i < TEST_LOOP; ++i)   	
		unpack_poly(s[0], sk);
	cycles2 = cpucycles();
    kcycles = cycles2-cycles1;
    printf("  unpack_poly runs in ................. %8lld nsec", kcycles/TEST_LOOP);
    printf("\n"); 

    printf("pack_z\n");
	kcycles=0;
	cycles1 = cpucycles();
	for(i = 0; i < TEST_LOOP; ++i)   	
		pack_z(sm, y);
	cycles2 = cpucycles();
    kcycles = cycles2-cycles1;
    printf("  pack_z runs in ................. %8lld nsec", kcycles/TEST_LOOP);
    printf("\n"); 

    printf("sample_c \n");
	kcycles=0;
	cycles1 = cpucycles();
	for(i = 0; i < TEST_LOOP; ++i) 
	{
		/*
		SHA256_Init(&ctx);
		SHA256_Update(&ctx, tmp, POLY_BYTES);
		SHA256_Update(&ctx, m, mlen);
		SHA256_Final(seed, &ctx);
		*/
		sha256_inc_init(&ctx);
		sha256_inc_blocks(&ctx, tmp, POLY_BYTES);
		sha256_inc_finalize(seed, &ctx, m, mlen);
		sha256_inc_ctx_release(&ctx);
		
		sample_c(c, seed);
	}
	cycles2 = cpucycles();
    kcycles = cycles2-cycles1;
    printf("  sample_c runs in ................. %8lld nsec", kcycles/TEST_LOOP);
    printf("\n");

    printf("sample_y \n");
	kcycles=0;
	cycles1 = cpucycles();
	for(i = 0; i < TEST_LOOP; ++i) 
		sample_y(y, seed, nonce);
	cycles2 = cpucycles();
    kcycles = cycles2-cycles1;
    printf("  sample_y runs in ................. %8lld nsec", kcycles/TEST_LOOP);
    printf("\n");

	printf("polyvec_cadd\n");
	kcycles=0;
	cycles1 = cpucycles();
	for(i = 0; i < TEST_LOOP; ++i) 
		polyvec_cadd(s);
	cycles2 = cpucycles();
    kcycles = cycles2-cycles1;
    printf("  polyvec_cadd runs in ................. %8lld nsec", kcycles/TEST_LOOP);
    printf("\n"); 


	printf("ntt \n");
	kcycles=0;
	cycles1 = cpucycles();
	for(i = 0; i < TEST_LOOP; ++i) 
		ntt(a[0]); 
	cycles2 = cpucycles();
    kcycles = cycles2-cycles1;
    printf("  ntt runs in ................. %8lld nsec", kcycles/TEST_LOOP);
    printf("\n"); 

	printf("invntt \n");
	kcycles=0;
	cycles1 = cpucycles();
	for(i = 0; i < TEST_LOOP; ++i) 
		invntt(a[0]); 
	cycles2 = cpucycles();
    kcycles = cycles2-cycles1;
    printf("  invntt runs in ................. %8lld nsec", kcycles/TEST_LOOP);
    printf("\n");
	
	printf("mont_cvt\n");
	kcycles=0;
	cycles1 = cpucycles();
	for(i = 0; i < TEST_LOOP; ++i) 
    	mont_cvt(a[0][0]);
    cycles2 = cpucycles();
    kcycles = cycles2-cycles1;
    printf("  mont_cvt runs in ................. %8lld nsec", kcycles/TEST_LOOP);
    printf("\n"); 

    printf("poly_mul_NTT\n");
	kcycles=0;
	cycles1 = cpucycles();
	for(i = 0; i < TEST_LOOP; ++i) 
    	poly_mul_NTT(t, a[0],s[0]);
    cycles2 = cpucycles();
    kcycles = cycles2-cycles1;
    printf("  poly_mul_NTT runs in ................. %8lld nsec", kcycles/TEST_LOOP);
    printf("\n"); 

	printf("polyvec_mul_NTT_wocvt\n");
	kcycles=0;
	cycles1 = cpucycles();
	for(i = 0; i < TEST_LOOP; ++i) 
		polyvec_mul_NTT_wocvt(t, a, s);
	cycles2 = cpucycles();
    kcycles = cycles2-cycles1;
    printf("  polyvec_mul_NTT2_wocvt runs in ................. %8lld nsec", kcycles/TEST_LOOP);
    printf("\n"); 

	printf("poly_reject_z \n");
	kcycles=0;
	cycles1 = cpucycles();
	for(i = 0; i < TEST_LOOP; ++i) 
		check = poly_reject_z(a);
	cycles2 = cpucycles();
    kcycles = cycles2-cycles1;
    printf("  poly_reject_z runs in ................. %8lld nsec", kcycles/TEST_LOOP);
    printf("\n");
   
  return 0;
}

int test_Correctness()
{
	int r;
  	double rejw=.0, rejyz=.0, rejctr=.0, rejctrkg=.0;
  	unsigned long long i, j;

	unsigned char pk[CRYPTO_PUBLICKEYBYTES];
	unsigned char sk[CRYPTO_SECRETKEYBYTES];

	unsigned char  m[100] = "hello world!";
	unsigned char sm[CRYPTO_BYTES + 200];

	unsigned char  m2[100];

	unsigned long long  mlen = 0;
	unsigned long long smlen;

	unsigned long long  m2len;

	int result;

    unsigned long long kcycles, ecycles, dcycles;
    unsigned long long cycles1, cycles2;

    mlen = strlen(m);

	printf("CRYPTO_PUBLICKEYBYTES : %d\n", CRYPTO_PUBLICKEYBYTES);
	printf("CRYPTO_SECRETKEYBYTES : %d\n", CRYPTO_SECRETKEYBYTES);
	printf("CRYPTO_BYTES : %d\n", CRYPTO_BYTES);

	printf("KeyGen//////////////////////////////////////////////\n");
	kcycles=0;
	for (int i = 0; i < TEST_LOOP; i++)
	{
		cycles1 = cpucycles();
		crypto_sign_keypair(pk, sk);
		cycles2 = cpucycles();
        kcycles += cycles2-cycles1;
	}
    printf("  KeyGen runs in ................. %8lld nsec", kcycles/TEST_LOOP);
    printf("\n"); 


	printf("Sign////////////////////////////////////////////////\n");
	kcycles=0;
	for (int i = 0; i < TEST_LOOP; i++)
	{
		cycles1 = cpucycles();
		crypto_sign(sm, &smlen, m, mlen, sk);
		cycles2 = cpucycles();
        kcycles += cycles2-cycles1;
        //printf("test[%d] = %lld \n", i, ctr_sign);
	    rejctr+=ctr_sign;
	}
    printf("  Sign runs in ................. %8lld nsec", kcycles/TEST_LOOP);
    printf("\n"); 


	printf("Verify///////////////////////////////////////////////\n");
	kcycles=0;
	for (int i = 0; i < TEST_LOOP; i++)
	{
		cycles1 = cpucycles();
		result = crypto_sign_open(m2, &m2len, sm, smlen, pk);
		cycles2 = cpucycles();
        kcycles += cycles2-cycles1;
	}
    printf("  Verify runs in ................. %8lld nsec", kcycles/TEST_LOOP);
    printf("\n"); 

    printf("Acceptance rate of Signing: %.2f\n",(double)TEST_LOOP/rejctr);
    printf("\n");   

  	if(result == 0)
	{
		printf("Verification Success!\n");
		printf("msg : ");
		for (int i = 0; i < m2len; ++i)
		{
			printf("%c", m2[i]);
		}
		printf("\n");
	}
	else
	{
		printf("Verification Fail\n");
	}

	return 0;
}

int Correctness(void)
{
  unsigned int i, j;
  int ret;
  
  uint8_t m[100] = {0};
  uint8_t sm[200 + CRYPTO_BYTES];
  uint8_t m2[100];
	unsigned char pk[CRYPTO_PUBLICKEYBYTES];
	unsigned char sk[CRYPTO_SECRETKEYBYTES];

	
	unsigned long long  mlen = 100;
	unsigned long long smlen;

	unsigned long long  m2len;

	int result;
  	

    mlen = strlen(m);

    for(i = 0; i < TEST_LOOP; ++i) {
    randombytes(m, mlen);

    crypto_sign_keypair(pk, sk);
    crypto_sign(sm, &smlen, m, mlen, sk);
    result = crypto_sign_open(m2, &m2len, sm, smlen, pk);

  	if(result == 0)
	{
		printf("Verification Success!\n");
		printf("msg : ");
		for (int i = 0; i < m2len; ++i)
		{
			printf("%c", m2[i]);
		}
		printf("\n");
	}
	else
	{
		printf("Verification Fail\n");
	}

	return 0;
}
}

int main(int argc, char const *argv[])
{
	//Correctness();
	test_Correctness();
	//test_speed();
		
}