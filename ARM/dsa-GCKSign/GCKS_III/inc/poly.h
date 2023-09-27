#ifndef __POLY_H__
#define __POLY_H__

#include "params.h"
#include <stdint.h>
#include <stdio.h>
#include <stdint.h>

#define POLY_BYTES ((_log_P * _N) >> 3)
#define POLYVEC_BYTES (POLY_BYTES * _M)

void comput_w();
void ntt(int64_t* in);
void invntt(int64_t* in);

void poly_cadd(int64_t* in);
void polyvec_cadd(int64_t in[][_N]);

int64_t mont_cvt(int64_t in);
int64_t mont_mul(int64_t a, int64_t b);
int64_t mont_ivt(__int128_t in);

void poly_init(int64_t* in);
void poly_add(int64_t* c, int64_t* a, int64_t* b);
void poly_sub(int64_t* c, int64_t* a, int64_t* b);
void polyvec_add(int64_t c[][_N], int64_t a[][_N], int64_t b[][_N]);
void polyvec_mul(int64_t* c, int64_t a[][_N], int64_t b[][_N]);
void poly_mul_NTT(int64_t* c, int64_t* a, int64_t* b);

void poly_mul_NTT_wocvt(int64_t* c, int64_t* a, int64_t* b);
void polyvec_mul_NTT_wocvt(int64_t* c, int64_t a[][_N], int64_t b[][_N]);

int poly_reject_cs(int64_t in[][_N]);
int poly_reject_z(int64_t in[][_N]);

void schoolbook_poly_mul(int64_t* c, int64_t* a, int64_t* b);
void schoolbook_polyvec_mul(int64_t* c, int64_t a[][_N], int64_t b[][_N]);



#endif