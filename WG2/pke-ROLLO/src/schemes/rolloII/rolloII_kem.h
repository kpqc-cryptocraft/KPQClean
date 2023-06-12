/**
 * \file api.h
 * \brief NIST KEM API used by the ROLLOI_KEM IND-CPA scheme
 */


#ifndef ROLLOII_KEM_H
#define ROLLOII_KEM_H

int rolloII_keygen(uint8_t* pk, uint8_t* sk);
int rolloII_encaps(uint8_t* ct, uint8_t* ss, const uint8_t* pk);
int rolloII_decaps(uint8_t* ss, const uint8_t* ct, const uint8_t* sk);

#endif

