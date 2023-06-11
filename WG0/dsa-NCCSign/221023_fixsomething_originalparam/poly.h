#ifndef POLY_H
#define POLY_H

#include <stdint.h>
#include "params.h"

typedef struct {
  int32_t coeffs[NR<<1];
} poly;

void poly_modadd(poly *c, const poly *a, const poly *b);
void poly_modsub(poly *c, const poly *a, const poly *b);

void poly_tomont(poly *a_mont, poly *a);
void poly_frommont(poly *a, poly *a_mont);
void karatsuba_simple(const uint32_t* a_1,const uint32_t* b_1, uint32_t* result_final);
void toom_cook_4way (const uint32_t* a1,const uint32_t* b1, uint32_t* result);

void poly_reduce(poly *a);

void poly_caddq(poly *a);

void poly_freeze(poly *a);


void poly_add(poly *c, const poly *a, const poly *b);

void poly_sub(poly *c, const poly *a, const poly *b);

void poly_shiftl(poly *a);

void poly_pointwise_montgomery(poly *c, const poly *a, const poly *b);


void poly_power2round(poly *a1, poly *a0, const poly *a);

void poly_decompose(poly *a1, poly *a0, const poly *a);

unsigned int poly_make_hint(poly *h, const poly *a0, const poly *a1);

void poly_use_hint(poly *b, const poly *a, const poly *h);


int poly_chknorm(poly *a, int32_t B);

void poly_uniform(poly *a,
                  const uint8_t seed[SEEDBYTES],
                  uint16_t nonce);

void poly_uniform_eta(poly *a,
                      const uint8_t seed[SEEDBYTES],
                      uint16_t nonce);

void poly_uniform_gamma1(poly *a,
                         const uint8_t seed[CRHBYTES],
                         uint16_t nonce);

void poly_challenge(poly *c, const uint8_t seed[SEEDBYTES]);


void poly_mul(poly* res, poly* a, poly* b);
#endif
