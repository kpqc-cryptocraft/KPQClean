#include <sign.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <openssl/sha.h>
#include "sha2.h"

#include "rng.h"

#include "poly.h"
#include "pack.h"
#include "sample.h"
#include "reduce.h"

unsigned long long ctr_sign;



int crypto_sign_keypair(unsigned char *pk, unsigned char *sk)
{
	uint16_t nonce = 0;
	int8_t seed_s[32];
	int64_t a[_M][_N];
	int64_t s[_M][_N];
	int64_t t[_N] = {0};
	
	randombytes(pk, 32);
	sample_a(a, pk, nonce++);

	randombytes(seed_s, 32);
	sample_s(s, seed_s, nonce++);

	#ifdef Raw_s // Setting secret key with original small s
	pack_s(sk, s);
	#endif

	polyvec_cadd(s);

	for(int i = 0; i < _M; i++)
		ntt(s[i]);
	
	// t = a * s
	polyvec_mul_NTT_wocvt(t, a, s);

	for(int j = 0; j < _N; j++)
       	t[j] = mont_cvt(t[j]);
	
	#ifdef Raw_s
	memcpy(sk + N, pk, 32);
	#endif

	#ifdef NTT_s // Setting secret key with ntt(s) to reduce the number of ntt operations in sign phase
	pack_poly(sk, s[0]);
	pack_poly(sk + POLY_BYTES    , s[1]);
	pack_poly(sk + POLY_BYTES * 2, s[2]);
	pack_poly(sk + POLY_BYTES * 3, s[3]);
	memcpy(sk + 4 * POLY_BYTES, pk, 32);
	#endif

	pack_poly(pk + 32, t);

	return 0;
}

int crypto_sign(unsigned char *sm, unsigned long long *smlen,
            unsigned char *m, unsigned long long mlen,
            unsigned char *sk)
{
    uint16_t nonce = 0;
	unsigned char seed[32];
	unsigned char tmp[POLY_BYTES];

	int check = 0;
	ctr_sign=0;

	//SHA256_CTX ctx;
	sha256ctx ctx;

	int64_t a[_M][_N];
	int64_t s[_M][_N];
	int64_t y[_M][_N];
	int64_t z[_M][_N] = {0};
	int64_t c[_N];	
	
	#ifdef Raw_s
	//unpack public key
	sample_a(a, sk + N, nonce++);
	//decode secret key
	unpack_s(s, sk);
	for(int i = 0; i < _M; i++)
			ntt(s[i]);
	#endif

	#ifdef NTT_s
	//unpack public key
	sample_a(a, sk + 4*POLY_BYTES, nonce++);
	//decode secret key
	unpack_poly(s[0], sk);
	unpack_poly(s[1], sk +   POLY_BYTES);
	unpack_poly(s[2], sk + 2*POLY_BYTES);
	unpack_poly(s[3], sk + 3*POLY_BYTES); 
	#endif

	randombytes(seed, 32);

	while(check == 0)
	{
		ctr_sign++;
		int64_t v[_N] = {0};

		
		sample_y(y, seed, nonce++);
		polyvec_cadd(y);
	
		//generate challenge c = H(ay, m)
		for(int i = 0; i < _M; i++)
			ntt(y[i]); 
		polyvec_mul_NTT_wocvt(v, a, y); 
		//invntt(v); 
		pack_poly(tmp ,v);

		//c = H(v,m)
		/*
		SHA256_Init(&ctx);
		SHA256_Update(&ctx, tmp, POLY_BYTES);
		SHA256_Update(&ctx, m, mlen);
		SHA256_Final(seed, &ctx);
		*/

		sha256_inc_init(&ctx);
		sha256_inc_blocks(&ctx, tmp, 30);	// 1920/64= 30
		sha256_inc_finalize(seed, &ctx, m, mlen);

		sample_c(c, seed);

		poly_cadd(c);		
		ntt(c); 
		
		//z = y + sc	
		for(int j = 0; j < _N; j++)
    		c[j] = mont_cvt(c[j]);
    	
    	for(int i = 0; i < _M; i++)
			poly_mul_NTT_wocvt(z[i], c, s[i]);
			
		polyvec_add(z, z, y); 
		
		
		for(int i = 0; i < _M; i++)
			invntt(z[i]);
			
		//check rejection
		check = poly_reject_z(z);

		//if(poly_reject_cs(cs) == 0) continue;
	}

	pack_z(sm, z);
	memcpy(sm + Z_BYTES, seed, 32);
	memcpy(sm + CRYPTO_BYTES, m, mlen);

	*smlen = CRYPTO_BYTES + mlen;

	return 0;
}

int crypto_sign_open(unsigned char *m, unsigned long long *mlen,
                 unsigned char *sm, unsigned long long smlen,
                 unsigned char *pk)
{
    uint16_t nonce = 0;
	int64_t a[_M][_N];
	int64_t z[_M][_N];	
	int64_t t[_N];
	int64_t c[_N];	
	int64_t tmp1[_N] = {0};
	int64_t tmp2[_N];
	
	unsigned char digest[32];
	unsigned char tmp[POLY_BYTES];

	//SHA256_CTX ctx;
	sha256ctx ctx;

	//unpack public key
	sample_a(a, pk, nonce);
	unpack_poly(t, pk + 32);

	//unpack signature
	unpack_z(z, sm);
	if(poly_reject_z(z) == 0) return 0;
	polyvec_cadd(z);

	sample_c(c, sm + Z_BYTES);		
	poly_cadd(c);
	ntt(c);

	//v = a z - c t
	for(int j = 0; j < _M; j++)
		ntt(z[j]);
	
	polyvec_mul_NTT_wocvt(tmp1, a, z);
 	poly_mul_NTT_wocvt(tmp2, t, c);
	poly_sub(tmp1, tmp1, tmp2); // tmp1 = az - tc

	//c = H(v,m)
	pack_poly(tmp, tmp1);
	/*
	SHA256_Init(&ctx);
	SHA256_Update(&ctx, tmp, POLY_BYTES);
	SHA256_Update(&ctx, sm + CRYPTO_BYTES, smlen - CRYPTO_BYTES);
	SHA256_Final(digest, &ctx);
	*/

	sha256_inc_init(&ctx);
	sha256_inc_blocks(&ctx, tmp, 30);	// 1920/64= 30
	sha256_inc_finalize(digest, &ctx, (sm + CRYPTO_BYTES), (smlen - CRYPTO_BYTES));

	//compare
	if(memcmp(digest, sm + Z_BYTES, 32) == 0)
	{
		memcpy(m, sm + CRYPTO_BYTES, smlen - CRYPTO_BYTES);
		*mlen = smlen - CRYPTO_BYTES;

		return 0;
	}
	else
		return 1;
}