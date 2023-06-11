#include <stdint.h>
#include "params.h"
#include "reduce.h"

#if NIMS_MODE == 2
#define MONT 83081 // 2^32 % Q
#define QINV 1554761515 // -q^(-1) mod 2^32
#define R2 5619074

#elif NIMS_MODE == 3
#define MONT 6123008 // 2^32 % Q
#define QINV 3813543687 // -q^(-1) mod 2^32
#define R2 1441797

#elif NIMS_MODE == 5
#define MONT 6424230 // 2^32 % Q
#define QINV 578884571 // -q^(-1) mod 2^32
#define R2 3645001

#endif

uint32_t montgomery_reduce(uint64_t a) {
  __uint128_t t;

  int64_t b;
  
  t = a * QINV;
  t &= 0xFFFFFFFF;
  t *= Q;
  t = a + t;
  b = t >> 32;
  b-= ((Q-b) >> 63) & Q;

  return (uint32_t)b;
}

static inline int32_t montgomery_reduced(int64_t a) {
  int32_t t;

  t = (int64_t)(int32_t)a*QINV;
  t = (a - (int64_t)t*Q) >> 32;

  return t;
}

uint32_t to_mont(int32_t a) {
  uint64_t t;
  uint32_t a0=a;
  
  a0+= (a >> 31) & Q;
  t = (uint64_t)a0 * (uint64_t)R2;

  return (uint32_t)montgomery_reduce(t);
}

uint32_t from_mont(int32_t a){
  return (int32_t)montgomery_reduced(a);
}

uint32_t reduce32(int64_t a) {
  int64_t t;
  uint32_t res;

  t = a%Q;
  t+= (t >> 63) & Q;
  res = (uint32_t)t;

  return res;
}

int32_t caddq(int32_t a) {
  a += (a >> 31) & Q; 
  return a;
}

int32_t csubq(int32_t a) {
  a -= Q;
  a += ((int32_t)a >> 31) & Q;
  return a;
}

int32_t freeze(int32_t a) {
  a = reduce32(a);
  a = caddq(a);
  return a;
}

uint32_t mod_add(uint32_t a, uint32_t b)
{
    int32_t t;
    t=(a+b);
    t=t-Q;
    t += (t >> 31) & Q;

    return (uint32_t)t;
}

uint32_t mod_sub(uint32_t a, uint32_t b)
{
    int32_t t;
    t=a-b;
    t += (t >> 31) & Q; 

    return (uint32_t)t;
}
