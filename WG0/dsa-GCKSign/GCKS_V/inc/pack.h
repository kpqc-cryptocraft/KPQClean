#ifndef __PACK_H__
#define __PACK_H__

#include <stdint.h>
#include "poly.h"

void pack_poly(uint8_t* out, int64_t* in);
void pack_s(uint8_t* out, int64_t in[][_N]);
void pack_z(uint8_t* out, int64_t in[][_N]);

void unpack_poly(int64_t* out, uint8_t* in);
void unpack_s(int64_t out[][_N], uint8_t* in);
void unpack_z(int64_t out[][_N], uint8_t* in);

#endif