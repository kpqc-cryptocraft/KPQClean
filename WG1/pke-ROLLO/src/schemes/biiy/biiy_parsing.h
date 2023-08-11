/** 
 * \file parsing.h
 * \brief Functions to parse secret key, public key and ciphertext of the ROLLO scheme
 */

#ifndef PARSING_H
#define PARSING_H

#include "biiy_types.h"

void biiy_secret_key_to_string(uint8_t* skString, const uint8_t* seed);
void biiy_secret_key_from_string(biiy_secretKey* sk, const uint8_t* skString);


void biiy_public_key_to_string(uint8_t* pkString, biiy_publicKey* pk);
void biiy_public_key_from_string(biiy_publicKey* pk, const uint8_t* pkString);


void biiy_biiy_ciphertext_to_string(uint8_t* ctString, biiy_ciphertext* ct);
void biiy_biiy_ciphertext_from_string(biiy_ciphertext* ct, const uint8_t* ctString);

#endif
