#pragma once
#include "Decryption.h"



/*************************************************
* Name:         NewDecryption
* 
* Description:  Decrypt the ciphertext consisting of a combination of vertices 
*			    by adding a value of 1 to the vertex corresponding to sk.
*
* Arguments:    const type cipher[CIPHER_TERMS][CIPHER_DIM + 2]   pointer to ciphertext       
*               const type DigitCipher[CIPHER_TERMS][CIPHER_BOX]  pointer to ciphertext converted to digit
*               const type DigitPDS[CIPHER_BOX], type numTERMS)	 pointer to secret key
**************************************************/
type NewDecryption(const type cipher[CIPHER_TERMS][CIPHER_DIM + 2],
	const type DigitCipher[CIPHER_TERMS][CIPHER_BOX], const type DigitPDS[CIPHER_BOX], type numTERMS)
{
	type plaintext = 0;
	
	for (type terms = 0; terms < numTERMS; terms++) {
		if ((cipher[terms][0] > 0) && (cipher[terms][1] > 0)) {
			//* Check that all variables in the term belong to the PDS set
			for (type box = 0; box < CIPHER_BOX; box++)
				if ((DigitCipher[terms][box] & DigitPDS[box]) != DigitCipher[terms][box])	// PASS term
					goto NEXT;

			plaintext = (plaintext + cipher[terms][1]) % MOD_N;
		}
	NEXT:;
	}


	return plaintext;
}





/*************************************************
* Name:         CipherDigitValue
* 
* Description:  Converts ciphers to ciphers in the form of digits
*
* Arguments:    type(*digit)[CIPHER_BOX]   						 pointer to ciphertext in the form of digits
*               const type cipher[CIPHER_TERMS][CIPHER_DIM + 2]  pointer to ciphertext
*               type numTERMS									 size of ciphertext
**************************************************/
void CipherDigitValue(type(*digit)[CIPHER_BOX],
	const type cipher[CIPHER_TERMS][CIPHER_DIM + 2], type numTERMS)
{
	for (type terms = 0; terms < numTERMS; terms++)
		if ((cipher[terms][0] > 0) && (cipher[terms][1] > 0))
			for (type vertex = 0; vertex < CIPHER_DIM; vertex++)
				if (cipher[terms][vertex + 2] > 0) {
					type box = Box(cipher[terms][vertex + 2]);
					type val = Val(cipher[terms][vertex + 2]);
					digit[terms][box] |= val;
				}
}



/*************************************************
* Name:         PDSdigitValue
* 
* Description:  Converts secret key to secret key in the form of digits
*
* Arguments:    type(*digit)   		 pointer to secret key in the form of digits
*               type PDS[CIPHER_PDS]  pointer to secret key
**************************************************/
void PDSdigitValue(type(*digit), type PDS[CIPHER_PDS])
{
	for (type i = 0; i < CIPHER_PDS; i++) {
		if (PDS[i] == 0) {
			printf("   PDS ERROR\n");
			break;
		}
		type box = Box(PDS[i]);
		type val = Val(PDS[i]);
		digit[box] |= val;
	}
}