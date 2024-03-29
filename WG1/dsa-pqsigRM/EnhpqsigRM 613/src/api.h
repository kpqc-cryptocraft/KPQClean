//
//  api.h
//
// #ifndef api_h
// #define api_h

// #define CRYPTO_PUBLICKEYBYTES (2510*4096)/8
// #define CRYPTO_SECRETKEYBYTES (4096*16 + 1024*16 + 1024*16)/8 + (2 * (1+(32-1)/64)*64)/8

// #define CRYPTO_BYTES (64+4096+64)/8 

// #define CRYPTO_ALGNAME "pqsigRM-6-12"

// int
// crypto_sign_keypair(unsigned char *pk, unsigned char *sk);

// int
// crypto_sign(unsigned char *sm, unsigned long long *smlen,
//             const unsigned char *m, unsigned long long mlen,
//             const unsigned char *sk);

// int
// crypto_sign_open(unsigned char *m, unsigned long long *mlen,
//                  const unsigned char *sm, unsigned long long smlen,
//                  const unsigned char *pk);

// #endif /* api_h */


#ifndef api_h
#define api_h

#define CRYPTO_PUBLICKEYBYTES (4096*8192)/8
#define CRYPTO_SECRETKEYBYTES (8192*16 + 2048*16 + 2048*16)/8 + (2 * (1+(32-1)/64)*64)/8

#define CRYPTO_BYTES (64+8192+64)/8 

#define CRYPTO_ALGNAME "pqsigRM-6-13"

int
crypto_sign_keypair(unsigned char *pk, unsigned char *sk);

int
crypto_sign(unsigned char *sm, unsigned long long *smlen,
            const unsigned char *m, unsigned long long mlen,
            const unsigned char *sk);

int
crypto_sign_open(unsigned char *m, unsigned long long *mlen,
                 const unsigned char *sm, unsigned long long smlen,
                 const unsigned char *pk);

#endif /* api_h */