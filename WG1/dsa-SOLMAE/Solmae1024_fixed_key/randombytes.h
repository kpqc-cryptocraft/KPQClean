#ifndef RNG_H
#define RNG_H
#include <stdlib.h>
#include <stdint.h>
#include "inner.h"



void seed_rng();

int randombytes(uint8_t *buf, size_t n);
uint64_t get64();
uint8_t get8();


#endif
