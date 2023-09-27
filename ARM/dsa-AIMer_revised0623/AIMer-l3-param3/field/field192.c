// -----------------------------------------------------------------------------
// File Name   : field192.c
// Description : 
// SPDX-License-Identifier: MIT
// -----------------------------------------------------------------------------

#include "field192.h"
#include "portable_endian.h"

// square the lower 32-bit of the input
#define SQR_LOW(x) \
  sqr_table[((x) >> 28) & 0xf] << 56 | sqr_table[((x) >> 24) & 0xf] << 48 | \
  sqr_table[((x) >> 20) & 0xf] << 40 | sqr_table[((x) >> 16) & 0xf] << 32 | \
  sqr_table[((x) >> 12) & 0xf] << 24 | sqr_table[((x) >>  8) & 0xf] << 16 | \
  sqr_table[((x) >>  4) & 0xf] <<  8 | sqr_table[((x)      ) & 0xf]

// square the upper 32-bit of the input
#define SQR_HIGH(x) \
  sqr_table[((x) >> 60)      ] << 56 | sqr_table[((x) >> 56) & 0xf] << 48 | \
  sqr_table[((x) >> 52) & 0xf] << 40 | sqr_table[((x) >> 48) & 0xf] << 32 | \
  sqr_table[((x) >> 44) & 0xf] << 24 | sqr_table[((x) >> 40) & 0xf] << 16 | \
  sqr_table[((x) >> 36) & 0xf] <<  8 | sqr_table[((x) >> 32) & 0xf]

const uint64_t sqr_table[16] = {0x00, 0x01, 0x04, 0x05, 0x10, 0x11, 0x14, 0x15,
                                0x40, 0x41, 0x44, 0x45, 0x50, 0x51, 0x54, 0x55};

void poly192_sqr(const GF2_192 a, uint64_t *c);
void poly64_mul(const uint64_t a, const uint64_t b, uint64_t *c1, uint64_t *c0);
void poly192_mul(const GF2_192 a, const GF2_192 b, uint64_t *c);
void GF2_192_rdc(const uint64_t *a, GF2_192 c);

void GF2_192_set0(GF2_192 a)
{
  a[0] = 0;
  a[1] = 0;
  a[2] = 0;
}

void GF2_192_to_bytes(const GF2_192 in, uint8_t* out)
{
  uint64_t temp = htole64(in[0]);
  size_t num_bytes = sizeof(uint64_t);

  memcpy(out, (uint8_t*)(&temp), num_bytes);
  temp = htole64(in[1]);
  memcpy(out + num_bytes, (uint8_t*)(&temp), num_bytes);
  temp = htole64(in[2]);
  memcpy(out + 2 * num_bytes, (uint8_t*)(&temp), num_bytes);
}

void GF2_192_from_bytes(const uint8_t* in, GF2_192 out)
{
  uint64_t temp;
  size_t num_bytes = sizeof(uint64_t);

  memcpy((uint8_t*)(&temp), in, num_bytes);
  out[0] = le64toh(temp);
  memcpy((uint8_t*)(&temp), in + num_bytes, num_bytes);
  out[1] = le64toh(temp);
  memcpy((uint8_t*)(&temp), in + 2 * num_bytes, num_bytes);
  out[2] = le64toh(temp);
}

void GF2_192_copy(const GF2_192 in, GF2_192 out)
{
  out[0] = in[0];
  out[1] = in[1];
  out[2] = in[2];
}

void GF2_192_add(const GF2_192 a, const GF2_192 b, GF2_192 c)
{
  c[0] = a[0] ^ b[0];
  c[1] = a[1] ^ b[1];
  c[2] = a[2] ^ b[2];
}

void poly192_sqr(const GF2_192 a, uint64_t *c)
{
  c[0] = SQR_LOW(a[0]);
  c[1] = SQR_HIGH(a[0]);

  c[2] = SQR_LOW(a[1]);
  c[3] = SQR_HIGH(a[1]);

  c[4] = SQR_LOW(a[2]);
  c[5] = SQR_HIGH(a[2]);
}

void poly64_mul(const uint64_t a, const uint64_t b, uint64_t *c1, uint64_t *c0)
{
  uint64_t table[16];
  uint64_t temp, mask, high, low;
  uint64_t top3 = a >> 61;

  table[0] = 0;
  table[1] = a & 0x1fffffffffffffffULL;
  table[2] = table[1] << 1;
  table[4] = table[2] << 1;
  table[8] = table[4] << 1;

  table[3] = table[1] ^ table[2];

  table[5] = table[1] ^ table[4];
  table[6] = table[2] ^ table[4];
  table[7] = table[1] ^ table[6];

  table[9] = table[1] ^ table[8];
  table[10] = table[2] ^ table[8];
  table[11] = table[3] ^ table[8];
  table[12] = table[4] ^ table[8];
  table[13] = table[5] ^ table[8];
  table[14] = table[6] ^ table[8];
  table[15] = table[7] ^ table[8];

  low = table[b & 0xf];
  temp = table[(b >> 4) & 0xf];
  low ^= temp << 4;
  high = temp >> 60;
  temp = table[(b >> 8) & 0xf];
  low ^= temp << 8;
  high ^= temp >> 56;
  temp = table[(b >> 12) & 0xf];
  low ^= temp << 12;
  high ^= temp >> 52;
  temp = table[(b >> 16) & 0xf];
  low ^= temp << 16;
  high ^= temp >> 48;
  temp = table[(b >> 20) & 0xf];
  low ^= temp << 20;
  high ^= temp >> 44;
  temp = table[(b >> 24) & 0xf];
  low ^= temp << 24;
  high ^= temp >> 40;
  temp = table[(b >> 28) & 0xf];
  low ^= temp << 28;
  high ^= temp >> 36;
  temp = table[(b >> 32) & 0xf];
  low ^= temp << 32;
  high ^= temp >> 32;
  temp = table[(b >> 36) & 0xf];
  low ^= temp << 36;
  high ^= temp >> 28;
  temp = table[(b >> 40) & 0xf];
  low ^= temp << 40;
  high ^= temp >> 24;
  temp = table[(b >> 44) & 0xf];
  low ^= temp << 44;
  high ^= temp >> 20;
  temp = table[(b >> 48) & 0xf];
  low ^= temp << 48;
  high ^= temp >> 16;
  temp = table[(b >> 52) & 0xf];
  low ^= temp << 52;
  high ^= temp >> 12;
  temp = table[(b >> 56) & 0xf];
  low ^= temp << 56;
  high ^= temp >> 8;
  temp = table[b >> 60];
  low ^= temp << 60;
  high ^= temp >> 4;

  mask = -(int64_t)(top3 & 0x1);
  low ^= mask & (b << 61);
  high ^= mask & (b >> 3);
  mask = -(int64_t)((top3 >> 1) & 0x1);
  low ^= mask & (b << 62);
  high ^= mask & (b >> 2);
  mask = -(int64_t)((top3 >> 2) & 0x1);
  low ^= mask & (b << 63);
  high ^= mask & (b >> 1);

  *c0 = low;
  *c1 = high;
}

void poly192_mul(const GF2_192 a, const GF2_192 b, uint64_t *c)
{
  uint64_t t[3] = {0,};

  poly64_mul(a[0], b[0], &t[0], &c[0]);
  poly64_mul(a[1], b[1], &t[2], &t[1]);
  t[0] ^= t[1];

  poly64_mul(a[2], b[2], &c[5], &t[1]);
  t[1] ^= t[2];    

  c[1] = t[0] ^ c[0];
  c[2] = t[1] ^ c[1];
  c[4] = c[5] ^ t[1];
  c[3] = c[4] ^ t[0];

  poly64_mul((a[0] ^ a[1]), (b[0] ^ b[1]), &t[1], &t[0]);
  c[1] ^= t[0];
  c[2] ^= t[1];

  poly64_mul((a[0] ^ a[2]), (b[0] ^ b[2]), &t[1], &t[0]);
  c[2] ^= t[0];
  c[3] ^= t[1];

  poly64_mul((a[1] ^ a[2]), (b[1] ^ b[2]), &t[1], &t[0]);
  c[3] ^= t[0];
  c[4] ^= t[1];
}

void GF2_192_rdc(const uint64_t *a, GF2_192 c)
{
  uint64_t temp;

  // irreducible polynomial f(x) = x^192 + x^7 + x^2 + x + 1
  temp = a[3] ^ ((a[5] >> 57) ^ (a[5] >> 62) ^ (a[5] >> 63));

  c[2] = a[2] ^ a[5];
  c[2] ^= (a[5] << 7) | (a[4] >> 57);
  c[2] ^= (a[5] << 2) | (a[4] >> 62);
  c[2] ^= (a[5] << 1) | (a[4] >> 63);

  c[1] = a[1] ^ a[4];
  c[1] ^= (a[4] << 7) | (temp >> 57);
  c[1] ^= (a[4] << 2) | (temp >> 62);
  c[1] ^= (a[4] << 1) | (temp >> 63);

  c[0] = a[0] ^ temp;
  c[0] ^= (temp << 7);
  c[0] ^= (temp << 2);
  c[0] ^= (temp << 1);
}

void GF2_192_mul(const GF2_192 a, const GF2_192 b, GF2_192 c)
{
  uint64_t temp[6] = {0,};

  poly192_mul(a, b, temp);
  GF2_192_rdc(temp, c);
}

void GF2_192_sqr(const GF2_192 a, GF2_192 c)
{
  uint64_t temp[6] = {0,};

  poly192_sqr(a, temp);
  GF2_192_rdc(temp, c);
}

void GF2_192_transposed_matmul(const GF2_192 a, const GF2_192* b, GF2_192 c)
{
  unsigned int i, j;
  const uint64_t* a_ptr = a;
  const GF2_192* b_ptr = b;

  uint64_t temp0[3] = {0,};
  uint64_t temp1[3] = {0,};
  uint64_t mask;
  for (i = NUMWORDS_FIELD; i; --i, ++a_ptr)
  {
    uint64_t index = *a_ptr;
    for (j = NUMBITS_WORD; j; j -= 4, index >>= 4, b_ptr += 4)
    {
      mask = -(index & 1);
      temp0[0] ^= (b_ptr[0][0] & mask);
      temp0[1] ^= (b_ptr[0][1] & mask);
      temp0[2] ^= (b_ptr[0][2] & mask);

      mask = -((index >> 1) & 1);
      temp1[0] ^= (b_ptr[1][0] & mask);
      temp1[1] ^= (b_ptr[1][1] & mask);
      temp1[2] ^= (b_ptr[1][2] & mask);

      mask = -((index >> 2) & 1);
      temp0[0] ^= (b_ptr[2][0] & mask);
      temp0[1] ^= (b_ptr[2][1] & mask);
      temp0[2] ^= (b_ptr[2][2] & mask);

      mask = -((index >> 3) & 1);
      temp1[0] ^= (b_ptr[3][0] & mask);
      temp1[1] ^= (b_ptr[3][1] & mask);
      temp1[2] ^= (b_ptr[3][2] & mask);
    }
  }
  c[0] = temp0[0] ^ temp1[0];
  c[1] = temp0[1] ^ temp1[1];
  c[2] = temp0[2] ^ temp1[2];
}
