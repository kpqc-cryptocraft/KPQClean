/**
 * \file api.h
 * \brief NIST KEM API used by the biiy_KEM IND-CPA scheme
 */


#ifndef BIIY_KEM_H
#define BIIY_KEM_H

int biiy_keygen(uint8_t* pk, uint8_t* sk);
int biiy_encaps(uint8_t* ct, uint8_t* ss, const uint8_t* pk);
int biiy_decaps(uint8_t* ss, const uint8_t* ct, const uint8_t* sk);

#endif

