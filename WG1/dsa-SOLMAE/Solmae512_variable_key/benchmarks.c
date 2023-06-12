#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "benchmarks.h"
#include "samplerZ.h"
#include "normaldist.h"
#include "api.h"
#include "poly.h"
#include "param.h"
#include "cpucycles.h"

#define BENCH_ITER 1000000
#define BENCH_ITER2 10000

static void random_poly(poly *p)
{
  for (int i = 0; i < SOLMAE_D; ++i)
    p->coeffs[i].v = (double)(rand() % SOLMAE_Q);
}

void benchmark_FFT()
{

  uint64_t start = cpucycles();

  poly p;
  random_poly(&p);

  for (int i = 0; i < BENCH_ITER; ++i)
  {
    FFT(&p);
  }
  uint64_t stop = cpucycles();
  double delta = (double)(stop - start) / BENCH_ITER;
  printf("Average cycle count for FFT: %f\n", delta);
}

void benchmark_poly_add()
{

  uint64_t start = cpucycles();

  poly p1, p2;
  random_poly(&p1);
  random_poly(&p2);

  for (int i = 0; i < BENCH_ITER; ++i)
  {
    poly_add(&p1, &p2);
  }
  uint64_t stop = cpucycles();
  double delta = (double)(stop - start) / BENCH_ITER;
  printf("Average cycle count for poly_add: %f\n", delta);
}

void benchmark_pointwise_mul()
{

  uint64_t start = cpucycles();

  poly p1, p2;
  random_poly(&p1);
  random_poly(&p2);

  for (int i = 0; i < BENCH_ITER; ++i)
  {
    pointwise_mul(&p1, &p2);
  }
  uint64_t stop = cpucycles();
  double delta = (double)(stop - start) / BENCH_ITER;
  printf("Average cycle count for pointwise_mul: %f\n", delta);
}

void benchmark_FFT_mul_adj()
{

  uint64_t start = cpucycles();

  poly p1, p2;
  random_poly(&p1);
  random_poly(&p2);

  for (int i = 0; i < BENCH_ITER; ++i)
  {
    FFT_mul_adj(&p1, &p2);
  }
  uint64_t stop = cpucycles();
  double delta = (double)(stop - start) / BENCH_ITER;
  printf("Average cycle count for FFT_mul_adj: %f\n", delta);
}

void benchmark_poly_div_FFT()
{

  uint64_t start = cpucycles();

  poly p1, p2;
  random_poly(&p1);
  random_poly(&p2);

  for (int i = 0; i < BENCH_ITER; ++i)
  {
    poly_div_FFT(&p1, &p2);
  }
  uint64_t stop = cpucycles();
  double delta = (double)(stop - start) / BENCH_ITER;
  printf("Average cycle count for poly_div_FFT: %f\n", delta);
}

void benchmark_sample_discrete_gauss()
{

  uint64_t start = cpucycles();

  poly p;
  for (int i = 0; i < SOLMAE_D; ++i)
    p.coeffs[i].v = 0;
  for (int i = 0; i < BENCH_ITER2; ++i)
  {
    sample_discrete_gauss(&p);
  }
  uint64_t stop = cpucycles();
  double delta = (double)(stop - start) / BENCH_ITER2;
  printf("Average cycle count for sample_discrete_gauss: %f\n", delta);
}

void benchmark_normaldist()
{

  uint64_t start = cpucycles();

  poly p;
  for (int i = 0; i < SOLMAE_D; ++i)
    p.coeffs[i].v = 0;

  for (int i = 0; i < BENCH_ITER2; ++i)
  {
    normaldist(&p);
  }
  uint64_t stop = cpucycles();
  double delta = (double)(stop - start) / BENCH_ITER2;
  printf("Average cycle count for continuous gaussian sampling: %f\n", delta);
}

void run_benchmarks()
{

  benchmark_FFT();
  benchmark_poly_add();
  benchmark_pointwise_mul();
  benchmark_FFT_mul_adj();
  benchmark_poly_div_FFT();
  benchmark_sample_discrete_gauss();
  benchmark_normaldist();
}
