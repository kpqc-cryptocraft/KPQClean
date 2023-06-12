// Assuming No Encoding/Decoding in Solmae-(512 or 1024)
#define CRYPTO_SECRETKEYBYTES 16385 // 16384 bytes + header 1 byte
#define CRYPTO_PUBLICKEYBYTES 4097  // 4096 bytes + header 1 byte
#define CRYPTO_BYTES 8273           // nonce 40 bytes + header 1 bytes + signature 4096 * 2 bytes
#define CRYPTO_ALGNAME "Solmae-512"

int crypto_sign_keypair(unsigned char *pk, unsigned char *sk);

int crypto_sign(unsigned char *sm, unsigned long long *smlen,
                const unsigned char *m, unsigned long long mlen,
                const unsigned char *sk);

int crypto_sign_open(unsigned char *m, unsigned long long *mlen,
                     const unsigned char *sm, unsigned long long smlen,
                     const unsigned char *pk);

#ifndef API_H
#define API_H

#include <stdint.h>
#include "poly.h"

int keygen_full(secret_key *sk, public_key *pk);
void sign(const uint8_t *m, const secret_key *sk, signature *s);
int verify(uint8_t *m, public_key *pk, signature *s);
void sampler(const secret_key *sk, const poly *c1, const poly *c2, poly *v0, poly *v1);

/* Constant-time macros */
#define LSBMASK(c) (-((c)&1))
#define CMUX(x, y, c) (((x) & (LSBMASK(c))) ^ ((y) & (~LSBMASK(c))))
#define CFLIP(x, c) CMUX(x, -(x), c)
#define CZERO64(x) ((~(x) & ((x)-1)) >> 63)

#endif
