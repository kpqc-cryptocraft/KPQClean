#ifndef API_H
#define API_H
#include "params.h"

#include "config.h"

#if NIMS_MODE == 2
#define CRYPTO_PUBLICKEYBYTES 1984
#define CRYPTO_SECRETKEYBYTES 2800
#define CRYPTO_BYTES 3186

#elif NIMS_MODE == 3
#define CRYPTO_PUBLICKEYBYTES 2443
#define CRYPTO_SECRETKEYBYTES 3914
#define CRYPTO_BYTES 4251

#elif NIMS_MODE == 5
#define CRYPTO_PUBLICKEYBYTES 3091
#define CRYPTO_SECRETKEYBYTES 4940
#define CRYPTO_BYTES 5385
#endif

int crypto_sign_keypair(unsigned char *pk, unsigned char *sk);

int crypto_sign(unsigned char *sm, unsigned long long *smlen,
                const unsigned char *msg, unsigned long long len,
                const unsigned char *sk);

int crypto_sign_open(unsigned char *m, unsigned long long *mlen,
                     const unsigned char *sm, unsigned long long smlen,
                     const unsigned char *pk);

#endif
