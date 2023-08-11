/** 
 * \file biiy_types.h
 * \brief File defining publicKey, secretKey and cipherText types for ROLLO-I
 */

#ifndef BIIY_TYPES_H
#define BIIY_TYPES_H

#include "rbc_qre.h"
#include "rbc_vspace.h"

typedef struct biiy_secretKey {
	rbc_vspace F;
	rbc_qre x;
	rbc_qre y;
} biiy_secretKey;

typedef struct biiy_publicKey {
	rbc_qre h;
} biiy_publicKey;

typedef struct biiy_ciphertext{
	rbc_qre syndrom;
} biiy_ciphertext;

#endif
