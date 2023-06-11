#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include "TIGER_CPAPKE.h"
#include "rng.h"
#include "fips202.h"
#include "xef.h"
#include "D2.h"

int Keygen(unsigned char *pk, unsigned char *sk){
	unsigned char pk_a[LWE_N];
	unsigned char pk_b[LWE_N*2]={0,};
	unsigned char seed_a[SEED_LEN], seed_s[SEED_LEN];
	int i, j;

//// Step1 : Gen Seed_a and Seed_s using PRG(randombytes) ////
	randombytes(seed_a, SEED_LEN);
	randombytes(seed_s, SEED_LEN);


//// Step2 : Gen poly a using Expender(SHAKE256) //// 
	shake256(pk_a, LWE_N, seed_a, SEED_LEN);


//// Step3 : Gens poly s and s_idx. s_idx is a index in -1 or 1 ////
	unsigned char tmp_s[HS*4];
	uint16_t sk_s[HS];
	unsigned int sk_random_idx;
	int hw=0, count = 0, neg_start=0, back_position = HS;
	memset(sk, 0, LWE_N);

	shake256(tmp_s, HS*4, seed_s, SEED_LEN);

	while (hw < HS) {
		sk_random_idx = tmp_s[count++]; 
		sk_random_idx <<= 8;
		sk_random_idx ^= tmp_s[count++];
		sk_random_idx &= (LWE_N - 1);
		if (sk[sk_random_idx] == 0) {
			sk[sk_random_idx] = (tmp_s[count++] & 0x02) - 1;
			hw++;
			if (sk[sk_random_idx]==0x01){sk_s[neg_start++] = sk_random_idx;}
			if (sk[sk_random_idx]==0xff){sk_s[--back_position] = sk_random_idx;}
		}
		if (count >= HS*4 - 3) {
			shake256(tmp_s, HS*4, tmp_s, HS*4);
			printf("Make the tmp_s. \n");
			count = 0;
		}
	}


//// Step4 : Gen poly_b := (p/q)*a*s ; p=64 ////
	for (i = 0; i < HS; ++i) {
		uint16_t deg = sk_s[i];
		uint16_t branch = (2 * ((i - neg_start) >> sft & 0x1) - 1);
		for (j = 0; j < LWE_N; ++j) {pk_b[deg + j] += branch * pk_a[j];}
	}
	for (i = 0; i < LWE_N; ++i) {pk_b[i] -= pk_b[LWE_N + i];}
	for (i = 0; i < LWE_N; ++i) {pk_b[i] = ((pk_b[i] + 0x02) & 0xfc);} // 16=0x08/0xf0, 32=0x04/0xf8 64=0x02/0xfc, 128= 0x01 0xfe


//// Step5 : Concat seed_genA || pk_b ////
	memcpy(pk, seed_a, SEED_LEN);
	memcpy(pk+SEED_LEN, pk_b, LWE_N);

	return 0;
}



int Encryption(unsigned char *c, const unsigned char *pk, unsigned char *Message, unsigned char *coin){ 
	int i, j;
	unsigned char c1[LWE_N*2]={0,};
	unsigned char c2[LWE_N*2]={0,};	
	

//// Step1 : Gen r_poly and r_idx ////
	unsigned char tmp_r[HR*4];
	uint16_t r_idx[HR];
	unsigned int r_random_idx; 
	int hw=0, count = 0, neg_start = 0, back_position = HR;

	unsigned char r[LWE_N]={0,};
	shake256(tmp_r, HR*4, coin, SEED_LEN);
	
	while (hw < HR) {
		r_random_idx = tmp_r[count++]; 
		r_random_idx <<= 8;	
		r_random_idx ^= tmp_r[count++];
		r_random_idx = r_random_idx & (LWE_N - 1);  
		if (r[r_random_idx] == 0) {
			r[r_random_idx] = (tmp_r[count++] & 0x02) - 1;
			hw++;
			if (r[r_random_idx] == 0x01){r_idx[neg_start++] = r_random_idx;}
			if (r[r_random_idx] == 0xff){r_idx[--back_position] = r_random_idx;}
		}
		if (count >= (HR*4 - 3)) { 
			shake256(tmp_r, HR*4, tmp_r, HR*4);
			count = 0;
			printf("Make the tmp_r. \n");
		}
	}


//// Step2~3 : Gen Seed_e1, Seed_e2 ////
	coin[SEED_LEN-1]=coin[SEED_LEN-1]+1; // ADD NONCE (is 1)
	
	unsigned char Seed_e1[SEED_LEN];
	memcpy(Seed_e1, coin, SEED_LEN);

	unsigned char Seed_e2[SEED_LEN];
	coin[SEED_LEN-1]=coin[SEED_LEN-1]+1;  // ADD NONCE (is 1)
	memcpy(Seed_e2, coin, SEED_LEN);


//// Step4 : Gen poly_e1 and poly_e2. //// 
	unsigned char tmp_e1[HE*4], tmp_e2[HE*4];
	unsigned int e1_random_idx, e2_random_idx; 
	hw=0, count = 0;
	
	shake256(tmp_e1, HE*4, Seed_e1, 32);
	
	while (hw < HE) {
		e1_random_idx = tmp_e1[count++]; 
		e1_random_idx <<= 8;	
		e1_random_idx ^= tmp_e1[count++];
		e1_random_idx = e1_random_idx & (LWE_N - 1);  
		if (c1[e1_random_idx] == 0) {
			c1[e1_random_idx] = (tmp_e1[count++] & 0x02) - 1;
			hw++;
		}
		if (count >= (HE*4 - 3)) { 
			shake256(tmp_e1, HE*4, tmp_e1, HE*4);
			count = 0;
			printf("Make the tmp_e1.\n");
		}
	}

	hw=0, count = 0;
	
	shake256(tmp_e2, HE*4, Seed_e2, 32);
	
	while (hw < HE) {
		e2_random_idx = tmp_e2[count++]; 
		e2_random_idx <<= 8;	
		e2_random_idx ^= tmp_e2[count++];
		e2_random_idx = e2_random_idx & (LWE_N - 1);  
		if (c2[e2_random_idx] == 0) {
			c2[e2_random_idx] = ((tmp_e2[count++] & 0x02) - 1)<<(LOG_Q-LOG_P);
			hw++;
		}
		if (count >= (HE*4 - 3)) { 
			shake256(tmp_e2, HE*4, tmp_e2, HE*4);
			count = 0;
			printf("Make the tmp_e2.\n");
		}
	}


//// Step5 : Parse seed_a||pk_b from pk //// 
	unsigned char pk_a[LWE_N];
	unsigned char pk_b[LWE_N];
	unsigned char seed_a[SEED_LEN];

	memcpy(seed_a, pk, SEED_LEN);
	memcpy(pk_b, pk+SEED_LEN, LWE_N);


//// Step6 : Gen poly_a //// 
	shake256(pk_a, LWE_N, seed_a, SEED_LEN);


//// Step7~9 : (1) eccENC(M) -> (2) c1=a*r+e1 and c2=b*r+e2 -> (3) c={(k1/q)*c1 || (k2/q)*c2} //// 
// (1-1) Encoding Message using XEf
	unsigned char MSG_hat[MESSAGE_LEN*2]={0,};
	memcpy(MSG_hat, Message, MESSAGE_LEN);
	xe5_234_compute(MSG_hat); // f=5
// (1-2) Encoding Code-word(=MSG_hat) using D2 and add to c2.
   /* We gratefully used the code by https://github.com/newhopecrypto/newhope.git */
	unsigned int mask;
	for(i=0;i<MESSAGE_LEN*2;i++) 
	{
		for(j=0;j<8;j++)
		{
			mask = -((MSG_hat[i] >> j)&1);
			c2[8*i+j] = mask & (Modulus_Q/2);
			c2[8*i+j+LWE_N/2] = mask & (Modulus_Q/2);
		}
	}
// (2) Compute a * r + e1 = c1 and b * r + e2 = c2.
	for(i = 0; i < HR; ++i){
		uint16_t branch = (2 * ((i - neg_start) >> sft & 0x1) - 1);
		uint16_t deg = r_idx[i];
		for(j = 0; j < LWE_N; ++j){
			c1[deg+j] += branch * pk_a[j];
			c2[deg+j] += branch * pk_b[j];
		}
	}
	for(j = 0; j < LWE_N; ++j){
		c1[j] -= c1[LWE_N+j];
		c2[j] -= c2[LWE_N+j];
	}
// (3) Send c1h_a and c1h_b from mod q to mod k1(64) and mod k2(4).
	for (i=0; i< LWE_N; ++i) {
		c[i] = ((c1[i] + 0x02) & 0xfc);
		c[LWE_N + i] = ((c2[i] + 0x20) & 0xc0);    // 4=0x20/0xc0 8=0x10/0xe0, 16=0x08/0xf0, 32=0x04/0xf8 64=0x02/0xfc, 128= 0x01 0xfe
	}

	return 0;
}


int Decryption(unsigned char *Message, const unsigned char *c, const unsigned char *sk){
	int i, j;

	unsigned char c1_hat[LWE_N*2] = { 0, };
	unsigned char c2_hat[LWE_N*2] = { 0, };
	unsigned char decomp_delta[LWE_N*2]={0,};

//// Step1 : (1) Parsing c and (2) Gen poly s. ////

// (1) Parsing c. where, decomp_delta = c2_hat.
	memcpy(c1_hat, c, LWE_N);
	memcpy(decomp_delta, c+LWE_N, LWE_N);
	
// (2) Gen s_idx
	uint16_t sk_s[HS]={0,};
	int neg_start = 0, back_position = HS;	
	for (i = 0; i < LWE_N; ++i) {
		if (sk[i] == 0x01){sk_s[neg_start++] = i;}
		if (sk[i] == 0xff){sk_s[--back_position] = i;}
	}

//// Step2 : Compute Message_hat_prime = c2 - c1 * s. ////

	for(i = 0; i < HS; ++i){
		uint16_t branch = (2 * ((i - neg_start) >> sft & 0x1) - 1);
		uint16_t deg = sk_s[i];
		for(j = 0; j < LWE_N; ++j){
			decomp_delta[deg+j] -= branch * c1_hat[j];
	    }
	}
	for(j = 0; j < LWE_N; ++j){decomp_delta[j] -= decomp_delta[LWE_N+j];}


//// Step3 : Get original Message using D2 -> Xef. ////
// (1) D2 Decoding
  	uint16_t t;
	unsigned char delta_hat[size_of_delta*2]={0,};
	
	for(i=0;i<LWE_N/2;i++) 
	{
	    t  = flipabs(decomp_delta[i+  0]);
	    t += flipabs(decomp_delta[i+LWE_N/2]);
		t = ((t - Modulus_Q/2));
	    t >>= 15;
		delta_hat[i>>3] |= t<<(i&7);
	}
// (2) Xef Decoding
	xe5_234_compute(delta_hat); // f=5
	xe5_234_fixerr(delta_hat);
	memcpy(Message, delta_hat, size_of_delta);
	
	return 0;
}
