#pragma once

#include "params.h"
#include "Keygen.h"
#include "Encryption.h"
#include "MultiEnc.h"
#include "Decryption.h"
#include "verify.h"
#include "rng.h"
#include <time.h>

type DigitCipher[CIPHER_TERMS][CIPHER_BOX];
/*************************************************
* Name:        crypto_encrypt_keypair
*
* Description: Generates public and private key
*              for IPCC encryption, decryption
*
* Arguments:   - unsigned int *pk1[2]: pointer to output public key of graph 1
*			   - unsigned int *pk1[2]: pointer to output public key of graph 2
*              - unsigned int *sk: pointer to output private key of graph 1 and graph 2
*
* Returns 0 (success)
**************************************************/
int crypto_encrypt_keypair(unsigned int (*pk1)[2], unsigned int (*pk2)[2], unsigned int *sk)
{

	for (int i=0; i<IPCC_PUBLICKEYBYTES_ROW; i++) {
			pk1[i][0]=0;
			pk1[i][1]=0;
			pk2[i][0]=0;
			pk2[i][1]=0;
	}
	for (int i=0; i<IPCC_SECRETKEYBYTES; i++)
		sk[i]=0;
	
		
	type PDS1[G1numPDS] = { 0, };
	type PDS2[G2numPDS] = { 0, };

	Keygen(pk1, PDS1, G1numVertex, 1); 				// pk1 == GraphEdge1
	Keygen(pk2, PDS2, G2numVertex, 1+G1numVertex); 	// pk2 == GraphEdge2

	setPDS(sk, PDS1, PDS2); // sk == PDS

	return 0;
}


/*************************************************
* Name:        crypto_encrypt
*
* Description: Generates cipher text
*
* Arguments:   - unsigned int (*c)[CRYPTO_CIPHERTEXTBYTES_COL]: pointer to output cipher text
*              - const unsigned int (*pk1)[2]: pointer to input public key of graph 1
*              - const unsigned int (*pk1)[2]: pointer to input public key of graph 1
*
* Returns 0 (success)
**************************************************/
int crypto_encrypt(unsigned int (*c)[IPCC_CIPHERTEXTBYTES_COL], unsigned long long *clen, const unsigned int *m, unsigned long long mlen, const unsigned int (*pk1)[2], const unsigned int (*pk2)[2]) {


	// [PRE-COMPUTATION]
	type GraphNeighbor1[maxVERTEX][4] = { 0, };
	type GraphNeighbor2[maxVERTEX][4] = { 0, };
	type v_nieg_1[G1numVertex][maxBOX] = { 0, };
	type v_nieg_2[G2numVertex][maxBOX] = { 0, };

	// First Graph Pre-computation
	Neighbor(GraphNeighbor1, pk1, G1numVertex, Graph1Start); // pk1 == graphedge1
	GetNeighborDigit(v_nieg_1, GraphNeighbor1, G1numVertex, Graph1Start);

	// Second Graph Pre-computation
	Neighbor(GraphNeighbor2, pk2, G2numVertex, Graph2Start); // pk1 == graphedge2
	GetNeighborDigit(v_nieg_2, GraphNeighbor2, G2numVertex, Graph2Start);

	// [ENCRYPT]
	type MESSAGE = m[0];
	type message1;
	

	type isprime = -1;
#if (IPCC_f == 1)
	while (1) {
		message1 = randomint() % MOD_N;
		if ( ((message1 != 0) && ((MESSAGE + 1) != (message1 + 1)) && ( ((MESSAGE + 1) % (message1 + 1) == 0) ))) {
			isprime=0;
			break;
		}
		if ( ((message1 != 0) && ((MESSAGE + 1) != (message1 + 1)) && ( ((MESSAGE + MOD_N + 1) % (message1 + 1) == 0) ))) {
			isprime=1;
			break;
		}
	}
	type message2;
	if (isprime == 0) {
		message2 = (((MESSAGE + 1) / (message1 + 1)) - 1) % MOD_N;
	}
	if (isprime == 1) {
		message2 = (((MESSAGE + MOD_N + 1) / (message1 + 1)) - 1) % MOD_N;
	}

#elif (IPCC_f == 2)
	type message3 = 0;
	while ((message3 != 0) && (message3 != MESSAGE))	message3 = randomint() % MOD_N;
	while (1) {
		message1 = randomint() % MOD_N;
		if ((message1 != 0) && ((MESSAGE - message3 + 1) != (message1 + 1)) && ((MESSAGE - message3 + 1) % (message1 + 1) == 0)) {
			isprime=0;
			break;
		}
		if ((message1 != 0) && ((MESSAGE - message3 + 1) != (message1 + 1)) && ((MESSAGE+MOD_N - message3 + 1) % (message1 + 1) == 0)) {
			isprime=1;
			break;
		} 
	}
	type message2;
	if (isprime == 0) {
		message2 = (((MESSAGE - message3 + 1) / (message1 + 1)) - 1) % MOD_N;
	}
	if (isprime == 1) {
		message2 = (((MESSAGE+MOD_N - message3 + 1) / (message1 + 1)) - 1) % MOD_N;
	} 

#elif (IPCC_f == 3)
	type message3 = 0;
	while (message3 != 0)	message3 = randomint() % MOD_N;

	while (1) {
		message1 = randomint() % MOD_N;
		if ((message1 != 0) && (message1 != MESSAGE) && ((MESSAGE - message3) % (message1 + 1) == 0))	break;
	}
	type message2 = ((MESSAGE - message3) / (message1 + 1)) % MOD_N;
#elif (IPCC_f == 4)
	type message3 = 0;
	while (message3 > 0)	message3 = randomint() % MOD_N;
	type message4 = 0;
	while (message4 > 0)	message4 = randomint() % MOD_N;

	while (1) {
		message1 = randomint() % MOD_N;
		if ((message1 > 0) && (message1 != MESSAGE) && ((MESSAGE - message3 - message4) % message1 == 0))	break;
	}
	type message2 = ((MESSAGE - message3 - message4) / message1) % MOD_N;
#elif ((IPCC_f > 4) || (IPCC_f < 1))
	type numTerms = 0;
	return -1;
#endif

#if (IPCC_f == 1)
	type numTerms = EncType1(c, GraphNeighbor1, v_nieg_1, message1,
		GraphNeighbor2, v_nieg_2, message2);
#elif (IPCC_f == 2)
	type numTerms = EncType2(c, GraphNeighbor1, v_nieg_1, message1,
		GraphNeighbor2, v_nieg_2, message2, message3);
#elif (IPCC_f == 3)
	type numTerms = EncType3(c, GraphNeighbor1, v_nieg_1, message1,
		GraphNeighbor2, v_nieg_2, message2, message3);
#elif (IPCC_f == 4)
	type numTerms = EncType4(c, GraphNeighbor1, v_nieg_1, message1,
		GraphNeighbor2, v_nieg_2, message2, message3, message4);
#endif

	clen[0] = numTerms;


	return 0;
	
}


/*************************************************
* Name:        crypto_encrypt_open
*
* Description: Generates decrypted text
*
* Arguments:   - unsigned int *m:       pointer to output plain text
*              - const unsigned int (*c)[IPCC_CIPHERTEXTBYTES_COL]: pointer to input cipher text
*              - const unsigned int *sk: pointer to input private key
*
* Returns 0 for sucess or -1 for failure
*
**************************************************/
int crypto_encrypt_open(unsigned int *m, unsigned long long *mlen, const unsigned int (*c)[IPCC_CIPHERTEXTBYTES_COL], unsigned int *clen, const unsigned int *sk)
{
	int fail;
	unsigned int pt;

	pt = m[0];
	m[0] = 0;
	type DigitPDS[CIPHER_BOX] = { 0, };

	// digitCipher -> static Array 
	for (int i=0; i<11500;i++) {
		for (int j=0; j<307;j++)
			DigitCipher[i][j]=0;
	}

	// Can be precomputation
	PDSdigitValue(DigitPDS, sk);
	CipherDigitValue(DigitCipher, c, clen[0]);

	// DEC
	m[0] = NewDecryption(c, DigitCipher, DigitPDS, clen[0]);
	fail = verify(m, pt);
#if UI
	if (fail == 0)   		printf("verifiation success!\n(message = %d) == (decrypted message = %d)\n", pt, m[0]);
	else if (fail == 1) 	printf("verifiation failed.\n(message = %d) == (decrypted message = %d)\n", pt, m[0]);
	else 					{
		printf("verification Error!\n exit program...");
		exit(1);
	}
#endif

	return -fail;
}