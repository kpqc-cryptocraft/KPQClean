/**
 * \file api.h
 * \brief NIST KEM API
 */

#ifndef API_H
#define API_H

#include <stdint.h>
#include {SCHEME_INCLUDE}

#define CRYPTO_ALGNAME {SCHEME_ALGNAME}

#define CRYPTO_SECRETKEYBYTES {SCHEME_SECRETKEYBYTES}
#define CRYPTO_PUBLICKEYBYTES {SCHEME_PUBLICKEYBYTES}
#define CRYPTO_CIPHERTEXTBYTES {SCHEME_CIPHERTEXTBYTES}
#define CRYPTO_BYTES {SCHEME_CRYPTOBYTES}

int crypto_kem_keypair(unsigned char* pk, unsigned char* sk) {return {SCHEME_KEYGEN}(pk, sk);}
int crypto_kem_enc(unsigned char* ct, unsigned char* ss, const unsigned char* pk) {return {SCHEME_ENC}(ct, ss, pk);}
int crypto_kem_dec(unsigned char* ss, const unsigned char* ct, const unsigned char* sk) {return {SCHEME_DEC}(ss, ct, sk);}

#endif
