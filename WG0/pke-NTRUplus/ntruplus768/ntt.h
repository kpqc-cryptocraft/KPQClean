#ifndef NTT_H
#define NTT_H

#include <stdint.h>
#include "params.h"

extern int16_t zetas[383];
extern int16_t zetas_inv[383];

void ntt(int16_t r[NTRUPLUS_N], const int16_t a[NTRUPLUS_N]);
void invntt(int16_t r[NTRUPLUS_N], const int16_t a[NTRUPLUS_N]);

void basemul(int16_t r[2], const int16_t a[2], const int16_t b[2], int16_t zeta);
int baseinv(int16_t r[2], const int16_t a[2], int16_t zeta);

void ntt_pack(int16_t b[NTRUPLUS_N], const int16_t a[NTRUPLUS_N]);
void ntt_unpack(int16_t b[NTRUPLUS_N], const int16_t a[NTRUPLUS_N]);

#endif