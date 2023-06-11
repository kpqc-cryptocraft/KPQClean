#ifndef api_h
#define api_h

#include "params.h"

#define CRYPTO_PUBLICKEYBYTES IPCC_PUBLICKEYBYTES // pk
#define CRYPTO_PUBLICKEYBYTES_ROW IPCC_PUBLICKEYBYTES_ROW  // pk_row
#define CRYPTO_PUBLICKEYBYTES_COL IPCC_PUBLICKEYBYTES_COL  // pk_col
#define CRYPTO_SECRETKEYBYTES IPCC_SECRETKEYBYTES // sk
#define CRYPTO_CIPHERTEXTBYTES IPCC_CIPHERTEXTBYTES // ct
#define CRYPTO_CIPHERTEXTBYTES_ROW IPCC_CIPHERTEXTBYTES_ROW // ct_row
#define CRYPTO_CIPHERTEXTBYTES_COL IPCC_CIPHERTEXTBYTES_COL // ct_col

#if   (IPCC_f == 1)
#define CRYPTO_ALGNAME "IPCC_f1"
#elif (IPCC_f == 2)
#define CRYPTO_ALGNAME "IPCC_f2"
#elif (IPCC_f == 3)
#define CRYPTO_ALGNAME "IPCC_f3"
#elif (IPCC_f == 4)
#define CRYPTO_ALGNAME "IPCC_f4"
#else
#error "IPCC_f must be in {1,2,3,4}"
#endif

int crypto_encrypt_keypair(unsigned int (*pk1)[2], unsigned int (*pk2)[2], unsigned int *sk);
int crypto_encrypt(unsigned int (*c)[CIPHER_DIM + 2], unsigned int *clen, const unsigned int *m, unsigned long long mlen, const unsigned int (*pk1)[2], const unsigned int (*pk2)[2]);
int crypto_encrypt_open(unsigned int *m, unsigned long long *mlen, const unsigned int (*c)[CIPHER_DIM + 2], unsigned int *clen, const unsigned int *sk);

#endif /* api_h */