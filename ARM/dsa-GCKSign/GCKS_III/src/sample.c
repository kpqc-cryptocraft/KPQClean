#include "sample.h"
#include "NIST/rng.h"
#include "fips202.h"
#include "symmetric.h"

#define POLY_UNIFORM_NBLOCKS ((768 + STREAM128_BLOCKBYTES - 1)/STREAM128_BLOCKBYTES)


#ifdef PARAM_1
static unsigned int rej_uniform(int64_t *a, unsigned int len,
                                const uint8_t *buf,
                                unsigned int buflen)
{
  unsigned int ctr, pos;
  uint64_t t;
  
  ctr = pos = 0;
  while(ctr < len && pos + 7 <= buflen) {
    t  = buf[pos++];
    t |= (uint64_t)buf[pos++] << 8;
    t |= (uint64_t)buf[pos++] << 16;
    t |= (uint64_t)buf[pos++] << 24;
    t |= (uint64_t)buf[pos++] << 32;
    t |= (uint64_t)buf[pos++] << 40;
    t |= (uint64_t)buf[pos++] << 48;
    
    t &= ((1L << _log_P) - 1);

    if(t < Q)
      a[ctr++] = t;
  }
  return ctr;
}
#endif

#ifdef PARAM_2
static unsigned int rej_uniform(int64_t *a, unsigned int len,
                                const uint8_t *buf,
                                unsigned int buflen)
{
  unsigned int ctr, pos;
  uint64_t t;
  
  ctr = pos = 0;
  while(ctr < len && pos + 8 <= buflen) {
    t  = buf[pos++];
    t |= (uint64_t)buf[pos++] << 8;
    t |= (uint64_t)buf[pos++] << 16;
    t |= (uint64_t)buf[pos++] << 24;
    t |= (uint64_t)buf[pos++] << 32;
    t |= (uint64_t)buf[pos++] << 40;
    t |= (uint64_t)buf[pos++] << 48;
    t |= (uint64_t)buf[pos++] << 54; // PARAM2
    

    t &= ((1L << _log_P) - 1);

    if(t < Q)
      a[ctr++] = t;
  }
  return ctr;
}
#endif

#ifdef PARAM_3
static unsigned int rej_uniform(int64_t *a, unsigned int len,
                                const uint8_t *buf,
                                unsigned int buflen)
{
  unsigned int ctr, pos;
  uint64_t t;
  
  ctr = pos = 0;
  while(ctr < len && pos + 6 <= buflen) {
    t  = buf[pos++];
    t |= (uint64_t)buf[pos++] << 8;
    t |= (uint64_t)buf[pos++] << 16;
    t |= (uint64_t)buf[pos++] << 24;
    t |= (uint64_t)buf[pos++] << 32;
    t |= (uint64_t)buf[pos++] << 40; // PARAM3
    

    t &= ((1L << _log_P) - 1);

    if(t < Q)
      a[ctr++] = t;
  }
  return ctr;
}
#endif

#ifdef PARAM_1
int sample_a(int64_t out[][_N], uint8_t* seed, uint16_t nonce)
{
	unsigned int i, ctr;
    unsigned int buflen = POLY_UNIFORM_NBLOCKS*SHAKE128_RATE;
    uint8_t buf[POLY_UNIFORM_NBLOCKS*SHAKE128_RATE + 2];
    stream128_state state;

    stream128_init(&state, seed, nonce);
    shake128_squeezeblocks(buf, POLY_UNIFORM_NBLOCKS, &state);

    int64_t* tmp =(int64_t*)out;

  	ctr = rej_uniform(tmp, _N * _M, buf, buflen);

	while(ctr < _N * _M) {
	shake128_squeezeblocks(buf, 1, &state);
    buflen = SHAKE128_RATE;
    ctr += rej_uniform(tmp + ctr, _N * _M - ctr, buf, buflen);
	}
}
#endif

#ifdef PARAM_2
int sample_a(int64_t out[][_N], uint8_t* seed, uint16_t nonce)
{
	unsigned int i, ctr, off;
    unsigned int buflen = POLY_UNIFORM_NBLOCKS*SHAKE128_RATE;
    uint8_t buf[POLY_UNIFORM_NBLOCKS*SHAKE128_RATE + 2];
    stream128_state state;

    stream128_init(&state, seed, nonce);
    shake128_squeezeblocks(buf, POLY_UNIFORM_NBLOCKS, &state);

    int64_t* tmp =(int64_t*)out;

  	ctr = rej_uniform(tmp, _N * _M, buf, buflen);

	while(ctr < _N * _M) {
    off = buflen % 8;
    for(i = 0; i < off; ++i)
      buf[i] = buf[buflen - off + i];

    shake128_squeezeblocks(buf + off, 1, &state);
    buflen = STREAM128_BLOCKBYTES + off;
    ctr += rej_uniform(tmp + ctr, _N * _M - ctr, buf, buflen);
	}
}
#endif

#ifdef PARAM_3
int sample_a(int64_t out[][_N], uint8_t* seed, uint16_t nonce)
{
	unsigned int i, ctr, off;
    unsigned int buflen = POLY_UNIFORM_NBLOCKS*SHAKE128_RATE;
    uint8_t buf[POLY_UNIFORM_NBLOCKS*SHAKE128_RATE + 2];
    stream128_state state;

    stream128_init(&state, seed, nonce);
    shake128_squeezeblocks(buf, POLY_UNIFORM_NBLOCKS, &state);

    int64_t* tmp =(int64_t*)out;

  	ctr = rej_uniform(tmp, _N * _M, buf, buflen);

	while(ctr < _N * _M) {
    off = buflen % 8;
    for(i = 0; i < off; ++i)
      buf[i] = buf[buflen - off + i];

    shake128_squeezeblocks(buf + off, 1, &state);
    buflen = STREAM128_BLOCKBYTES + off;
    ctr += rej_uniform(tmp + ctr, _N * _M - ctr, buf, buflen);
	}
}
#endif

static unsigned int rej_eta(int64_t *a, unsigned int len, const uint8_t *buf, unsigned int buflen)
{
  unsigned int ctr, pos;
  int64_t t0, t1, t2, t3;

  ctr = pos = 0;
  while(ctr < len && pos < buflen) {
    t0 = buf[pos] & 0x03;
    t1 = (buf[pos] >> 2) & 0x03;
    t2 = (buf[pos] >> 4) & 0x03;
    t3 = (buf[pos++] >> 6) & 0x03;


    if(t0 < 3) 
      a[ctr++] = 1 - t0;
    if(t1 < 3 && ctr < len)
      a[ctr++] = 1 - t1;      
    if(t2 < 3 && ctr < len)
      a[ctr++] = 1 - t2;      
    if(t3 < 3 && ctr < len)
      a[ctr++] = 1 - t3;         
  }
  return ctr;
}

#define POLY_UNIFORM_ETA_NBLOCKS ((136 + STREAM128_BLOCKBYTES - 1)/STREAM128_BLOCKBYTES)
void sample_s(int64_t out[][_N], const uint8_t seed[32], uint16_t nonce)
{
  unsigned int ctr;
  unsigned int buflen = POLY_UNIFORM_ETA_NBLOCKS*STREAM128_BLOCKBYTES;
  uint8_t buf[POLY_UNIFORM_ETA_NBLOCKS*STREAM128_BLOCKBYTES];
  stream128_state state;

  stream128_init(&state, seed, nonce);
  shake128_squeezeblocks(buf, POLY_UNIFORM_ETA_NBLOCKS, &state);

  int64_t* tmp = (int64_t*)out;

  ctr = rej_eta(tmp, _N * _M, buf, buflen);

  while(ctr < _N * _M) {
    shake128_squeezeblocks(buf, 1, &state);
    ctr += rej_eta(tmp + ctr, _N * _M - ctr, buf, STREAM128_BLOCKBYTES);
  }
}

#define POLY_B_NBLOCKS ((320 + STREAM128_BLOCKBYTES - 1)/STREAM128_BLOCKBYTES)
static unsigned int rej_B(int64_t *a, unsigned int len,
                                const uint8_t *buf,
                                unsigned int buflen)
{
  unsigned int ctr, pos;
  uint64_t t;
  
  ctr = pos = 0;
  while(ctr < len && pos + 2 <= buflen) {
    t  = buf[pos++];
    t |= (uint64_t)buf[pos++] << 8;
   
    #ifdef PARAM_1			
			t = t & ((1L << (_log_B + 1)) - 1);
	#endif
		
			if(t < _B * 2 + 1)
				a[ctr++] = t - _B;

  }
  return ctr;
}

int sample_y(int64_t out[][_N], uint8_t* seed, uint16_t nonce)
{
	unsigned int i, ctr, off;
    unsigned int buflen = POLY_B_NBLOCKS*SHAKE128_RATE;
    uint8_t buf[POLY_B_NBLOCKS*SHAKE128_RATE + 2];
    stream128_state state;

    stream128_init(&state, seed, nonce);
    shake128_squeezeblocks(buf, POLY_B_NBLOCKS, &state);

    int64_t* tmp =(int64_t*)out;

  	ctr = rej_B(tmp, _N * _M, buf, buflen);

	while(ctr < _N * _M) {
	shake128_squeezeblocks(buf, 1, &state);
    buflen = STREAM128_BLOCKBYTES;
    ctr += rej_B(tmp + ctr, _N * _M - ctr, buf, buflen);
	}
}

void sample_c(int64_t* c, const uint8_t seed[32]) {
  unsigned int i, b, pos;
  uint64_t signs;
  uint8_t buf[SHAKE256_RATE];
  keccak_state state;

  shake256_init(&state);
  shake256_absorb(&state, seed, 32);
  shake256_finalize(&state);
  shake256_squeezeblocks(buf, 1, &state);

  signs = 0;
  for(i = 0; i < 8; ++i)
    signs |= (uint64_t)buf[i] << 8*i;
  pos = 8;

  for(i = 0; i < _N; ++i)
    c[i]=0;
  for(i = _N -_H; i < _N; ++i) {
    do {
      if(pos >= SHAKE256_RATE) {
        shake256_squeezeblocks(buf, 1, &state);
        pos = 0;
      }

      b = buf[pos++];
    } while(b > i);

    c[i] = c[b];
    c[b] = 1 - 2*(signs & 1);
    signs >>= 1;
  }
}