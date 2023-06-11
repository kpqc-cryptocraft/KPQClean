#ifndef __REDUCE_H_
#define __REDUCE_H_

#include "params.h"
#include <stdint.h>
#include "poly.h"

int64_t caddp(int64_t in);
int64_t csubp(int64_t in);
int64_t center(int64_t in);
int64_t madd(int64_t a, int64_t b);
int64_t msub(int64_t a, int64_t b);
/*
int64_t mont_cvt(int64_t a);
int64_t mont_ivt(__int128_t t);
int64_t mont_mul(int64_t a, int64_t b);
*/
int64_t csubp2(int64_t in);
int64_t mont_cvt2(int64_t a);
int64_t mont_ivt2(__int128_t t);
int64_t mont_mul2(int64_t a, int64_t b);

#endif