#ifndef REDUCE_H
#define REDUCE_H

#include <stdint.h>
#include "params.h"

uint32_t montgomery_reduce(uint64_t a);
uint32_t to_mont(int32_t a);
uint32_t from_mont(int32_t a);

uint32_t reduce32(int64_t a);


int32_t caddq(int32_t a);
int32_t csubq(int32_t a) ;


int32_t freeze(int32_t a);
uint32_t mod_add(uint32_t a, uint32_t b);
uint32_t mod_sub(uint32_t a, uint32_t b);

#endif
