#include <stdint.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include "api.h"
#include "poly.h"
#include "param.h"
#include "fips202.h"
#include "randombytes.h"
#include "normaldist.h"
#include "samplerZ.h"

void H(const uint8_t *m_r, poly *c1)
{

  unsigned int ctr = 0;
  uint16_t val;
  uint64_t state[25];
  uint8_t buf[SHAKE128_RATE];
  int i;

  ctr = 0;
  shake128_absorb(state, m_r, MSG_BYTES + SOLMAE_K / 8);
  while (ctr < SOLMAE_D)
  {
    shake128_squeezeblocks(buf, 1, state);
    for (i = 0; i < SHAKE128_RATE && ctr < SOLMAE_D; i += 2)
    {
      val = (buf[i] | ((uint16_t)buf[i + 1] << 8));
      if (val < 5 * SOLMAE_Q)
      {
        c1->coeffs[ctr].v = (double)(val % SOLMAE_Q);
        ctr++;
      }
    }
  }
}

int check_norm(const poly *p1, const poly *p2)
{
  double s = 0.0;
  for (int i = 0; i < SOLMAE_D; ++i)
  {
    s += (p1->coeffs[i].v) * (p1->coeffs[i].v) + (p2->coeffs[i].v) * (p2->coeffs[i].v);
  }
  return s <= GAMMA_SQUARE;
}

void sampler(const secret_key *sk, const poly *c1, const poly *c2, poly *v0, poly *v1)
{

  // offline
  poly y1, y2;
  normaldist(&y1);
  normaldist(&y2);
  // FFT(&y1); FFT(&y2); /* FFT unnecessary: just a scaling in normaldist */
  pointwise_mul(&y1, &(sk->sigma1));
  pointwise_mul(&y2, &(sk->sigma2));

  poly d, temp, temp_c1, temp_c2;
  poly *x;

  // online
  // first nearest plane
  set_poly(&temp_c1, c1);
  set_poly(&temp_c2, c2);
  FFT(&temp_c1); // skip because c1 is 0
  FFT(&temp_c2);
  set_poly(&d, &temp_c1);
  set_poly(&temp, &temp_c2);
  pointwise_mul(&d, &(sk->beta20));
  pointwise_mul(&temp, &(sk->beta21)); // d2 fft form
  poly_add(&d, &temp);

  x = &d;           // x2
  poly_sub(x, &y2); // x2 = d2 - y2

  invFFT(x);
  sample_discrete_gauss(x);
  // second nearest plane

  FFT(x);
  set_poly(v0, x);
  set_poly(v1, x);
  pointwise_mul(v0, &(sk->b20));
  pointwise_mul(v1, &(sk->b21));
  poly_sub(&temp_c1, v0);
  poly_sub(&temp_c2, v1); // Alg 13 line 14 c1 in the paper is [temp_c1,temp_c2] in the code

  set_poly(&d, &temp_c1);
  set_poly(&temp, &temp_c2);
  pointwise_mul(&d, &(sk->beta10));
  pointwise_mul(&temp, &(sk->beta11));
  poly_add(&d, &temp); // d1 fft form

  x = &d;           // x1
  poly_sub(x, &y1); // x1 = d1 - y1
  invFFT(x);
  sample_discrete_gauss(x);
  FFT(x);

  set_poly(&temp, x);
  pointwise_mul(&temp, &(sk->b10));
  pointwise_mul(x, &(sk->b11));
  poly_add(v0, &temp);
  poly_add(v1, x); // v0 = v1 + x1*b1

  invFFT(v0);
  invFFT(v1);
}

void sign(const uint8_t *m, const secret_key *sk, signature *s)
{
  int trials = 0;
  poly c1, c2, v0, v1;
  uint8_t m_r[MSG_BYTES + SOLMAE_K / 8];
  for (int i = 0; i < MSG_BYTES; ++i)
    m_r[i] = m[i];

  do
  {
    trials++;
    randombytes(m_r + MSG_BYTES, SOLMAE_K / 8);
    H(m_r, &c2);
    for (int i = 0; i < SOLMAE_D; ++i)
      c1.coeffs[i].v = 0;
    // printf("c1:");print_poly(&c1); printf("c2:");print_poly(&c2);
    sampler(sk, &c1, &c2, &v0, &v1);
    // printf("v1:");print_poly(&v0); printf("v2:");print_poly(&v1);

    poly_sub(&c1, &v0);
    poly_sub(&c2, &v1);
    poly_recenter(&c1);
    poly_recenter(&c2);

    // printf("s1:");print_poly(&c1); printf("s2:");print_poly(&c2);

  } while (!check_norm(&c1, &c2));

  // printf("# Signature generated after %d trials\n\n", trials);
  set_poly(&(s->s1), &c1);
  set_poly(&(s->s2), &c2);
  for (int i = 0; i < SOLMAE_K / 8; ++i)
    (s->r)[i] = m_r[i + MSG_BYTES];
}

int verify(uint8_t *m, public_key *pk, signature *s)
{
  poly c1, t;
  uint8_t m_r[MSG_BYTES + SOLMAE_K / 8];
  for (int i = 0; i < MSG_BYTES; ++i)
    m_r[i] = m[i];
  for (int i = 0; i < SOLMAE_K / 8; ++i)
    m_r[i + MSG_BYTES] = (s->r)[i];
  H(m_r, &c1);

  set_poly(&t, &(s->s1));
  FFT(&t);
  pointwise_mul(&t, &(pk->h));
  invFFT(&t);
  poly_add(&t, &c1);
  poly_recenter(&t);
 // printf("s1*h+c:");
 // print_poly(&t);
 // printf("s2:");
 // print_poly(&(s->s2));
  return check_norm(&(s->s1), &t);
}
