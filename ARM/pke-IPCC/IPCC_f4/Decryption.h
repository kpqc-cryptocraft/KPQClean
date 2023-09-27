#ifndef DECRYPTION_H
#define DECRYPTION_H

#pragma once
#include "params.h"



/*   Bob   */

type NewDecryption(const type cipher[CIPHER_TERMS][CIPHER_DIM + 2],
	const type DigitCipher[CIPHER_TERMS][CIPHER_BOX], const type DigitPDS[CIPHER_BOX], type numTERMS);

void CipherDigitValue(type(*digit)[CIPHER_BOX],
	const type cipher[CIPHER_TERMS][CIPHER_DIM + 2], type numTERMS);
void PDSdigitValue(type(*digit), type PDS[CIPHER_PDS]);

#endif