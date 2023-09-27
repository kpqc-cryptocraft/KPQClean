#ifndef __SIGN_H__
#define __SIGN_H__

#include "params.h"
#include "poly.h"

#define Z_BYTES ((_log_B + 1) * _hhhN * _M)
#define S_BYTES ((_N * _M) >> 2)


#define CRYPTO_PUBLICKEYBYTES (32 + POLY_BYTES)

#ifdef Raw_s
#define CRYPTO_SECRETKEYBYTES (N + 32)
#endif

#ifdef NTT_s
#define CRYPTO_SECRETKEYBYTES (4*POLY_BYTES + 32) // Poly_bytes * 4 + 32
#endif


#define CRYPTO_BYTES (Z_BYTES + 32)

int64_t cpucycles(void);

int 
crypto_sign_keypair(unsigned char *pk, unsigned char *sk);

int
crypto_sign(unsigned char *sm, unsigned long long *smlen,
            unsigned char *m, unsigned long long mlen,
            unsigned char *sk);

int
crypto_sign_open(unsigned char *m, unsigned long long *mlen,
                 unsigned char *sm, unsigned long long smlen,
                 unsigned char *pk);

#endif
