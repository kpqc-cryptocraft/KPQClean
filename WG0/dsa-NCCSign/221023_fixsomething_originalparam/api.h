#ifndef API_H
#define API_H
#include "params.h"

#include "config.h"

#if NIMS_MODE == 2
#define CRYPTO_PUBLICKEYBYTES 1564
#define CRYPTO_SECRETKEYBYTES 2266
#define CRYPTO_BYTES 2458

#elif NIMS_MODE == 3
#define CRYPTO_PUBLICKEYBYTES 1997
#define CRYPTO_SECRETKEYBYTES 3312
#define CRYPTO_BYTES 3605

#elif NIMS_MODE == 5
#define CRYPTO_PUBLICKEYBYTES 2663
#define CRYPTO_SECRETKEYBYTES 4402
#define CRYPTO_BYTES 5055
#endif

int crypto_sign_keypair(unsigned char *pk, unsigned char *sk);

int crypto_sign(unsigned char *sm, unsigned long long *smlen,
                const unsigned char *msg, unsigned long long len,
                const unsigned char *sk);

int crypto_sign_open(unsigned char *m, unsigned long long *mlen,
                     const unsigned char *sm, unsigned long long smlen,
                     const unsigned char *pk);

#endif
