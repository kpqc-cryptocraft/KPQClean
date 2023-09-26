#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

#include "TiGER_CPAPKE.h"
#include "rng.h"
#include "fips202.h"
#include "xef.h"
#include "util.h"


int KEM_Keygen(unsigned char *pk, unsigned char *sk){
	int res;
	unsigned char u[MESSAGE_LEN];

	res=Keygen(pk, sk);

	randombytes(u, MESSAGE_LEN);
	memcpy(sk+SECRETKEYSIZE-MESSAGE_LEN, u, MESSAGE_LEN);//***
	
	return 0;
}

int KEM_Enc(unsigned char *c, unsigned char *shared_k, const unsigned char *pk){
	int res;
	unsigned char delta_kem[size_of_delta] = {0, };
	unsigned char coin[32];
	unsigned char hash_t[CIPHERTEXTSIZE+size_of_delta];//***

	randombytes(delta_kem, size_of_delta);

	unsigned char temp_coin[size_of_delta+32];
	memcpy(temp_coin, delta_kem, size_of_delta);
	sha3_256(temp_coin+size_of_delta, pk, PUBLICKEYSIZE);  // H function
	sha3_256(coin, temp_coin, size_of_delta+32);		// G function

	res=Encryption(c, pk, delta_kem, coin);
	
	unsigned char hash_c[32];
	sha3_256(hash_c, c, CIPHERTEXTSIZE);  // H(ctxt) //***

	memcpy(hash_t, delta_kem, size_of_delta);
	memcpy(hash_t+size_of_delta, hash_c, 32);

	shake256(shared_k, KK_LEN, hash_t, size_of_delta+32);
	
	return 0;
}

int KEM_dec(unsigned char *shared_k, const unsigned char *c, const unsigned char *sk, unsigned char *pk){
	int i, res_enc, res_dec;
	unsigned char u[MESSAGE_LEN];
	unsigned char sk_CPA[SECRETKEYSIZE-MESSAGE_LEN];//***
	unsigned char delta_hat[size_of_delta]={0,};	
	unsigned char c_hat[CIPHERTEXTSIZE];//***
	unsigned char coin[32];
	unsigned char hash_t[CIPHERTEXTSIZE+size_of_delta];//***

	memcpy(sk_CPA, sk, SECRETKEYSIZE-MESSAGE_LEN);//***
	memcpy(u, sk+(SECRETKEYSIZE-MESSAGE_LEN), MESSAGE_LEN);//***

	res_dec=Decryption(delta_hat, c, sk_CPA);
	
	unsigned char temp_coin[size_of_delta+32];
	memcpy(temp_coin, delta_hat, size_of_delta);
	sha3_256(temp_coin+size_of_delta, pk, PUBLICKEYSIZE);//***
	sha3_256(coin, temp_coin, size_of_delta+32);	

	res_enc=Encryption(c_hat, pk, delta_hat, coin);
	
	
	unsigned char hash_c[32];
	sha3_256(hash_c, c, CIPHERTEXTSIZE);//***

	int8_t selector = ct_verify(c, c_hat, CIPHERTEXTSIZE);//***
    ct_select((unsigned char*)hash_t, (unsigned char*)delta_hat, (unsigned char*)u, size_of_delta, selector);

    memcpy(hash_t+size_of_delta, hash_c, 32);
	shake256(shared_k, KK_LEN, hash_t, size_of_delta+32);

	return 0;
}


