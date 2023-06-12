#ifndef AES_H
#define AES_H

#include <stdint.h>
#include <stdlib.h>
#include <immintrin.h>


#define AES128_KEYBYTES 16
#define AES256_KEYBYTES 32
#define AES_BLOCKBYTES 16


typedef struct {
    __m128i key_schedule[11];
} aes128ctx;

typedef struct {
    __m128i key_schedule[15];
} aes256ctx;



void aes128_keyexp(aes128ctx *ctx, const unsigned char *key);
void aes128_ecb(unsigned char *out, const unsigned char *in, size_t nblocks, const aes128ctx *ctx);
void aes128_ctx_release(aes128ctx *ctx);


void aes256_keyexp(aes256ctx *r, const unsigned char *key);
void aes256_ecb(unsigned char *out, const unsigned char *in, size_t nblocks, const aes256ctx *ctx);
void aes256_ctx_release(aes256ctx *r);

#endif

