
/*
 * PQClean compatible AES implementation using aes-ni instructions.
 *
 * Based on "Intel Advanced Encryption Standard (AES) New Instructions Set" by Shay Gueron.
 */

#include <stdint.h>
#include <string.h>
#include <immintrin.h>

#include "aes.h"


inline __m128i aes_128_assist (__m128i temp1, __m128i temp2) {
  __m128i temp3;

  temp2 = _mm_shuffle_epi32 (temp2 ,0xff);
  temp3 = _mm_slli_si128 (temp1, 0x4);

  temp1 = _mm_xor_si128 (temp1, temp3);
  temp3 = _mm_slli_si128 (temp3, 0x4);

  temp1 = _mm_xor_si128 (temp1, temp3);
  temp3 = _mm_slli_si128 (temp3, 0x4);

  temp1 = _mm_xor_si128 (temp1, temp3);
  temp1 = _mm_xor_si128 (temp1, temp2);

  return temp1;
}



void aes128_keyexp(aes128ctx *ctx, const unsigned char *key) {
  __m128i temp1, temp2;

  temp1 = _mm_loadu_si128((__m128i*) key);
  ctx->key_schedule[0] = temp1;

  temp2 = _mm_aeskeygenassist_si128 (temp1, 0x1);
  temp1 = aes_128_assist(temp1, temp2);
  ctx->key_schedule[1] = temp1;

  temp2 = _mm_aeskeygenassist_si128 (temp1, 0x2);
  temp1 = aes_128_assist(temp1, temp2);
  ctx->key_schedule[2] = temp1;

  temp2 = _mm_aeskeygenassist_si128 (temp1, 0x4);
  temp1 = aes_128_assist(temp1, temp2);
  ctx->key_schedule[3] = temp1;

  temp2 = _mm_aeskeygenassist_si128 (temp1, 0x8);
  temp1 = aes_128_assist(temp1, temp2);
  ctx->key_schedule[4] = temp1;

  temp2 = _mm_aeskeygenassist_si128 (temp1, 0x10);
  temp1 = aes_128_assist(temp1, temp2);
  ctx->key_schedule[5] = temp1;

  temp2 = _mm_aeskeygenassist_si128 (temp1, 0x20);
  temp1 = aes_128_assist(temp1, temp2);
  ctx->key_schedule[6] = temp1;

  temp2 = _mm_aeskeygenassist_si128 (temp1, 0x40);
  temp1 = aes_128_assist(temp1, temp2);
  ctx->key_schedule[7] = temp1;

  temp2 = _mm_aeskeygenassist_si128 (temp1, 0x80);
  temp1 = aes_128_assist(temp1, temp2);
  ctx->key_schedule[8] = temp1;

  temp2 = _mm_aeskeygenassist_si128 (temp1, 0x1b);
  temp1 = aes_128_assist(temp1, temp2);
  ctx->key_schedule[9] = temp1;

  temp2 = _mm_aeskeygenassist_si128 (temp1, 0x36);
  temp1 = aes_128_assist(temp1, temp2);
  ctx->key_schedule[10] = temp1;
}



inline void aes_256_assist_1(__m128i* temp1, __m128i * temp2) {
  __m128i temp4;

  *temp2 = _mm_shuffle_epi32(*temp2, 0xff);
  temp4 = _mm_slli_si128(*temp1, 0x4);

  *temp1 = _mm_xor_si128(*temp1, temp4);
  temp4 = _mm_slli_si128(temp4, 0x4);

  *temp1 = _mm_xor_si128(*temp1, temp4);
  temp4 = _mm_slli_si128(temp4, 0x4);

  *temp1 = _mm_xor_si128(*temp1, temp4);
  *temp1 = _mm_xor_si128(*temp1, *temp2);
}



inline void aes_256_assist_2(__m128i* temp1, __m128i * temp3) {
  __m128i temp2, temp4;

  temp4 = _mm_aeskeygenassist_si128(*temp1, 0x0);
  temp2 = _mm_shuffle_epi32(temp4, 0xaa);
  temp4 = _mm_slli_si128(*temp3, 0x4);

  *temp3 = _mm_xor_si128(*temp3, temp4);
  temp4 = _mm_slli_si128(temp4, 0x4);

  *temp3 = _mm_xor_si128(*temp3, temp4);
  temp4 = _mm_slli_si128(temp4, 0x4);

  *temp3 = _mm_xor_si128(*temp3, temp4);
  *temp3 = _mm_xor_si128(*temp3, temp2);
}



void aes256_keyexp(aes256ctx *ctx, const unsigned char *key) {
  __m128i temp1, temp2, temp3;

  temp1 = _mm_loadu_si128((__m128i*) key);
  temp3 = _mm_loadu_si128((__m128i*) (key+16));
  ctx->key_schedule[0] = temp1;
  ctx->key_schedule[1] = temp3;

  temp2 = _mm_aeskeygenassist_si128(temp3, 0x01);
  aes_256_assist_1(&temp1, &temp2); 
  ctx->key_schedule[2] = temp1;

  aes_256_assist_2(&temp1, &temp3);
  ctx->key_schedule[3] = temp3;

  temp2 = _mm_aeskeygenassist_si128(temp3, 0x02);
  aes_256_assist_1(&temp1, &temp2);
  ctx->key_schedule[4] = temp1;

  aes_256_assist_2(&temp1, &temp3);
  ctx->key_schedule[5] = temp3;

  temp2 = _mm_aeskeygenassist_si128(temp3, 0x04);
  aes_256_assist_1(&temp1, &temp2);
  ctx->key_schedule[6] = temp1;

  aes_256_assist_2(&temp1, &temp3);
  ctx->key_schedule[7] = temp3;

  temp2 = _mm_aeskeygenassist_si128(temp3, 0x08);
  aes_256_assist_1(&temp1, &temp2);
  ctx->key_schedule[8] = temp1;

  aes_256_assist_2(&temp1, &temp3);
  ctx->key_schedule[9] = temp3;

  temp2 = _mm_aeskeygenassist_si128(temp3, 0x10);
  aes_256_assist_1(&temp1, &temp2);
  ctx->key_schedule[10] = temp1;

  aes_256_assist_2(&temp1, &temp3);
  ctx->key_schedule[11] = temp3;

  temp2 = _mm_aeskeygenassist_si128(temp3, 0x20);
  aes_256_assist_1(&temp1, &temp2);
  ctx->key_schedule[12] = temp1;

  aes_256_assist_2(&temp1, &temp3);
  ctx->key_schedule[13] = temp3;

  temp2 = _mm_aeskeygenassist_si128(temp3, 0x40);
  aes_256_assist_1(&temp1, &temp2);
  ctx->key_schedule[14] = temp1;
} 



void aes128_ecb(unsigned char *out, const unsigned char *in, size_t nblocks, const aes128ctx *ctx) {

  unsigned int number_of_rounds = 10;

  __m128i tmp;
  size_t i, j;

  for(i = 0 ; i < nblocks ; i++) { 
    tmp = _mm_loadu_si128(&((__m128i*) in)[i]);
    tmp = _mm_xor_si128(tmp, ctx->key_schedule[0]);

    for(j = 1 ; j < number_of_rounds ; j++) {
      tmp = _mm_aesenc_si128(tmp, ctx->key_schedule[j]);
    }

    tmp = _mm_aesenclast_si128(tmp, ctx->key_schedule[j]);
    _mm_storeu_si128(&((__m128i*) out)[i],tmp);
  }
}



void aes256_ecb(unsigned char *out, const unsigned char *in, size_t nblocks, const aes256ctx *ctx) {

  unsigned int number_of_rounds = 14;

  __m128i tmp;
  size_t i, j;

  for(i = 0 ; i < nblocks ; i++) { 
    tmp = _mm_loadu_si128(&((__m128i*) in)[i]);
    tmp = _mm_xor_si128(tmp, ctx->key_schedule[0]);

    for(j = 1 ; j < number_of_rounds ; j++) {
      tmp = _mm_aesenc_si128(tmp, ctx->key_schedule[j]);
    }

    tmp = _mm_aesenclast_si128(tmp, ctx->key_schedule[j]);
    _mm_storeu_si128(&((__m128i*) out)[i], tmp);
  }
}



void aes128_ctx_release(aes128ctx *ctx) {
    (void) ctx;
}



void aes256_ctx_release(aes256ctx *ctx) {
    (void) ctx;
}

