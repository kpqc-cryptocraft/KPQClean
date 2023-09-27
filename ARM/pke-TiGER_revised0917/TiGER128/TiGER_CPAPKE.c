#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include "TiGER_CPAPKE.h"
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
	unsigned char tmp_s[HS * 3];
	uint16_t sk_s[HS];
	unsigned char sk_t[LWE_N];
	unsigned int sk_random_idx;
	int hw = 0, count = 0, neg_start = 0, back_position = HS;

	// Initialize sk_t array with zeros
	memset(sk_t, 0, LWE_N);

	// Create temporary array with specific seed
	shake256(tmp_s, HS * 3, seed_s, SEED_LEN);
	unsigned char ts;

	while (hw < HS) {
		sk_random_idx = tmp_s[count++]; 
		sk_random_idx <<= 8;
		sk_random_idx ^= tmp_s[count++];

		ts = (sk_random_idx & 0x02) - 1;

		sk_random_idx >>= 2;
		sk_random_idx &= (LWE_N - 1);
		
		// Constant-time replacement for the side-channel (rejection sampling)
		unsigned char condition = (unsigned char)(sk_t[sk_random_idx] == 0);
		unsigned char increment = condition & (ts != 0);
		
		sk_t[sk_random_idx] += increment;
		sk_t[sk_random_idx] -= (condition & (ts == 0xff));
		
		hw += increment;
		
		sk_s[neg_start] = sk_random_idx;
		sk_s[back_position - 1] = sk_random_idx;
		neg_start += condition & (ts == 0x01);
		back_position -= condition & (ts == 0xff);
		
		// Re-seed if needed
		if (count >= HS * 3 - 3) {
		    shake256(tmp_s, HS * 3, tmp_s, HS * 3);
		    printf("Re-seeding tmp_s.\n");
		    count = 0;
		}
	}
	// compress uint16_t*HS bit => LOG_N*HS bit.
    for (i=0; i< (int)(117 / 9); i++){
        compress9to8(&sk_s[8*i], &sk[9*i]);
    }
    sk[117] = neg_start;

	
//// Step4 : Gen poly_b := (p/q)*a*s ; p=128 ////
    for (i = 0; i < HS; ++i) {
        uint16_t deg = sk_s[i];
        int branch = (2 * ((i - neg_start) >> sft & 0x1) - 1);

        // Use bitwise operations to ensure constant time operations
        int addMask = (branch == 0x0001);
        int subMask = ~addMask & 0x01;
        
        for (j = 0; j < LWE_N; ++j) {
            pk_b[deg + j] += (pk_a[j] & -addMask) + ((~pk_a[j] + 0x01) & -subMask);
        }
    } 
    for (i = 0; i < LWE_N; ++i) {
        pk_b[i] -= pk_b[LWE_N + i];
        pk_b[i] = ((pk_b[i] + 0x01) & 0xfe);
    }


//// Step5 : Concat seed_genA || pk_b ////
	memcpy(pk, seed_a, SEED_LEN);
	// compress unsigned char * LWE_N bit => LOG_P * LWE_N bit
	for (i = 0; i < LWE_N/8; i++) {
		compress7to8(&pk[SEED_LEN+LOG_P*i], &pk_b[8*i]);
	}

	return 0;
}



int Encryption(unsigned char *c, const unsigned char *pk, unsigned char *Message, unsigned char *coin){ 
	int i, j;
	unsigned char c1[LWE_N*2]={0,};
	unsigned char c2[LWE_N*2]={0,};	
	

//// Step1 : Gen r_poly and r_idx ////
	unsigned char tmp_r[HR*3];
	uint16_t r_idx[HR];
	unsigned int r_random_idx; 
	int hw=0, count = 0, neg_start = 0, back_position = HR;

	unsigned char r[LWE_N]={0,};
	shake256(tmp_r, HR*3, coin, SEED_LEN);
	unsigned char tr;
	while (hw < HR) {
		r_random_idx = tmp_r[count++]; 
		r_random_idx <<= 8;	
		r_random_idx ^= tmp_r[count++];
		tr=(r_random_idx&0x02)-1;
		r_random_idx >>= 2;		
		r_random_idx &= (LWE_N - 1);  
		
		// Constant-time replacement for the side-channel (rejection sampling)
		unsigned char condition = (unsigned char)(r[r_random_idx] == 0);
		unsigned char increment = condition & (tr != 0);
		
		r[r_random_idx] += increment;
		r[r_random_idx] -= (condition & (tr == 0xff));
		
		hw += increment;
		
		r_idx[neg_start] = r_random_idx;
		r_idx[back_position - 1] = r_random_idx;
		neg_start += condition & (tr == 0x01);
		back_position -= condition & (tr == 0xff);

		if (count >= (HR*3 - 3)) { 
			shake256(tmp_r, HR*3, tmp_r, HR*3);
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
	unsigned char tmp_e1[HE*3], tmp_e2[HE*3];
	unsigned int e1_random_idx, e2_random_idx; 
	hw=0, count = 0;
	
	shake256(tmp_e1, HE*3, Seed_e1, SEED_LEN);
	unsigned char te1;
	while (hw < HE) {
		e1_random_idx = tmp_e1[count++]; 
		e1_random_idx <<= 8;	
		e1_random_idx ^= tmp_e1[count++];
		te1=(e1_random_idx&0x02)-1;
		e1_random_idx >>= 2;
		e1_random_idx &= (LWE_N - 1);  

		// Constant-time replacement for the side-channel (rejection sampling)
		unsigned char condition = (unsigned char)(c1[e1_random_idx] == 0);
		unsigned char increment = condition & (te1 != 0);
		
		c1[e1_random_idx] += increment;
		c1[e1_random_idx] -= (condition & (te1 == 0xff));
		
		hw += increment;
		
		if (count >= (HE*3 - 3)) { 
			shake256(tmp_e1, HE*3, tmp_e1, HE*3);
			count = 0;
			printf("Make the tmp_e1.\n");
		}
	}

	hw=0, count = 0;
	
	shake256(tmp_e2, HE*3, Seed_e2, SEED_LEN);
	unsigned char te2;
	while (hw < HE) {
		e2_random_idx = tmp_e2[count++]; 
		e2_random_idx <<= 8;	
		e2_random_idx ^= tmp_e2[count++];
		te2=(e2_random_idx&0x02)-1;
		e2_random_idx >>= 2;
		e2_random_idx &= (LWE_N - 1);  
		
		// Constant-time replacement for the side-channel (rejection sampling)
		unsigned char condition = (unsigned char)(c2[e2_random_idx] == 0);
		unsigned char increment = condition & (te2 != 0);
		
		c2[e2_random_idx] += increment;
		c2[e2_random_idx] -= (condition & (te2 == 0xff));
		
		hw += increment;

		if (count >= (HE*3 - 3)) { 
			shake256(tmp_e2, HE*3, tmp_e2, HE*3);
			count = 0;
			printf("Make the tmp_e2.\n");
		}
	}


//// Step5 : Parse seed_a||pk_b from pk //// 
	unsigned char pk_a[LWE_N];
	unsigned char pk_b[LWE_N];
	unsigned char seed_a[SEED_LEN];

	memcpy(seed_a, pk, SEED_LEN);
	// recover LOG_P*LWE_N bit => unsigned char * LWE_N bit
    for (i=0; i< LWE_N/8; i++){recover8to7(&pk_b[8*i], &pk[SEED_LEN+LOG_P*i]);}


//// Step6 : Gen poly_a //// 
	shake256(pk_a, LWE_N, seed_a, SEED_LEN);


//// Step7~9 : (1) eccENC(M) -> (2) c1=a*r+e1 and c2=b*r+e2 -> (3) c={(k1/q)*c1 || (k2/q)*c2} //// 
// (1-1) Encoding Message using XEf
	unsigned char MSG_hat[MESSAGE_LEN*2]={0,};
	memcpy(MSG_hat, Message, MESSAGE_LEN);
	xef_compute(MSG_hat, MESSAGE_LEN, 3); // f=3 
// (1-2) Encoding Code-word(=MSG_hat) using D2 and add to c2.
	for(i = 0; i < MESSAGE_LEN * 2; i++) {
		for(j = 0; j < 8; j++) {
		    // Extract the j-th bit of MSG_hat[i] and adjust to 0 or 1
		    unsigned int mask = (MSG_hat[i] >> j) & 1;

		    // Multiply by Modulus_Q / 2 for constant time operation
		    // Bitwise AND operation to maintain mask either 0 or Modulus_Q / 2
		    mask = (mask * (Modulus_Q / 2)) & (Modulus_Q / 2);

		    // Apply mask to c2
		    c2[8 * i + j] = mask;
		    c2[8 * i + j + LWE_N / 2] = mask;
		}
}	
// (2) Compute a * r + e1 = c1 and b * r + e2 = c2.
	for(i = 0; i < HR; ++i){
		uint16_t deg = r_idx[i];
        int branch = (2 * ((i - neg_start) >> sft & 0x1) - 1);
		
        // Use bitwise operations to ensure constant time operations
        int addMask = (branch == 0x0001);
        int subMask = ~addMask & 0x01;
        
		for(j = 0; j < LWE_N; ++j){
		    c1[deg + j] += (pk_a[j] & -addMask) + ((~pk_a[j] + 0x01) & -subMask);
		    c2[deg + j] += (pk_b[j] & -addMask) + ((~pk_b[j] + 0x01) & -subMask);
		}
	}
	for(j = 0; j < LWE_N; ++j){
		c1[j] -= c1[LWE_N+j];
		c2[j] -= c2[LWE_N+j];
	}
	
	
// (3) Send c1h_a and c1h_b from mod q to mod k1(128) and mod k2(8).
	unsigned char c_t[LWE_N*2];
	for (i=0; i< LWE_N; ++i) {
		c_t[i] = ((c1[i] + 0x01) & 0xfe);
		c_t[LWE_N + i] = ((c2[i] + 0x10) & 0xe0);// 4=0x20/0xc0 8=0x10/0xe0, 16=0x08/0xf0, 32=0x04/0xf8 64=0x02/0xfc, 128= 0x01 0xfe
	}
	// compress unsigned char*LWE_N*2 bit => LOG_K_1*LWE_N + LOG_K_2*LWE_N bit
    for (i=0; i< LWE_N/8; i++){compress7to8(&c[LOG_K_1*i], &c_t[8*i]);}
	for (i=0; i< LWE_N/8; i++){compress3to8(&c[(LOG_K_1*LWE_N/8)+3*i], &c_t[LWE_N+8*i]);    }

	return 0;
}


int Decryption(unsigned char *Message, const unsigned char *c, const unsigned char *sk){
	int i, j;

	unsigned char c1_hat[LWE_N*2] = { 0, };
	unsigned char decomp_delta[LWE_N*2]={0,};

//// Step1 : (1) Parsing c and (2) Gen poly s. ////
	// recover LOG_K_1*LWE_N + LOG_K_2*LWE_N bit =>unsigned char*LWE_N*2 bit
    for (i=0; i< LWE_N/8; i++){
        recover8to7(&c1_hat[8*i], &c[7*i]);
    }
    for (i=0; i< LWE_N/8; i++){
        recover8to3(&decomp_delta[8*i], &c[(LOG_K_1*LWE_N/8)+3*i]);
    }
	
// (2) Gen s_idx
	uint16_t sk_s[HS]={0,};
    for (i=0; i< (int)(117 / 9); i++){
        recover8to9(&sk_s[8*i], &sk[9*i]);
    }
	int neg_start = sk[117];

	// Step2 : Compute Message_hat_prime = c2 - c1 * s.
	for(i = 0; i < HS; ++i){
		uint16_t deg = sk_s[i];
		int branch = (2 * ((i - neg_start) >> sft & 0x1) - 1);

		// Using bitwise operations to ensure constant time operations
		int subMaskPos = (branch == 0x0001);
		int subMaskNeg = ~subMaskPos & 0x01;

		for(j = 0; j < LWE_N; ++j){
			decomp_delta[deg + j] -= (c1_hat[j] & -subMaskPos) + ((~c1_hat[j] + 0x01) & -subMaskNeg);
		}
	}

	for(j = 0; j < LWE_N; ++j){
		decomp_delta[j] -= decomp_delta[LWE_N + j];
	}

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
	xef_compute(delta_hat, size_of_delta, 3);
	xef_fixerr(delta_hat, size_of_delta, 3);
	memcpy(Message, delta_hat, size_of_delta);
	
	return 0;
}
