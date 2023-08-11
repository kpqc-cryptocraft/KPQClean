/** 
 * \file biix_types.h
 * \brief File defining publicKey, secretKey and cipherText types for ROLLO-I
 */

#ifndef BIIX_TYPES_H
#define BIIX_TYPES_H

#include "rbc_qre.h"
#include "rbc_vspace.h"

// Additional part for permutation matrix
// Later, substituted for the better performance

//#include <vector> // Not supported as in C

typedef struct biix_secretKey {
	rbc_vspace F;
	rbc_qre x;
	rbc_qre y;
	// Additional part
	// std::vector<int> P
	
} biix_secretKey;

// C: Acutally, Not a PK, rather, indeterminated set of sub-PKs for string-like values 

typedef struct biix_publicKey {
	rbc_qre h;
} biix_publicKey;

typedef struct biix_ciphertext{
	rbc_qre syndrom;
} biix_ciphertext;

//

typedef struct biix_errgen{
	rbc_qre Err;
	rbc_vspace E;
} biix_errgen;


#endif
