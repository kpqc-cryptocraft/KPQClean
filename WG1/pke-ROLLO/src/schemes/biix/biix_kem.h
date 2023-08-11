/**
 * \file api.h
 * \brief NIST KEM API used by the BIIX_KEM IND-CPA scheme
 */


#ifndef BIIX_KEM_H
#define BIIX_KEM_H

int biix_keygen(uint8_t* pk, uint8_t* sk);
int biix_encaps(uint8_t* ct, uint8_t* ss, const uint8_t* pk);
int biix_decaps(uint8_t* ss, const uint8_t* ct, const uint8_t* sk);

#endif

