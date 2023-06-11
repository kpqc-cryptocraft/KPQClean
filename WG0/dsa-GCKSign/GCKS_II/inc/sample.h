#ifndef __SAMPLE_H__
#define __SAMPLE_H__

#include "poly.h"

int sample_a(int64_t out[][_N], uint8_t* seed, uint16_t nonce);
void sample_s(int64_t out[][_N], const uint8_t seed[32], uint16_t nonce);
int sample_y(int64_t out[][_N], uint8_t* seed, uint16_t nonce);
void sample_c(int64_t* c, const uint8_t seed[32]);
#endif