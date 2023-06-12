#include <stdio.h>

#include "test_dist.h"
#include "precomp.h"
#include "samplerZ.h"
#include "normaldist.h"
#include "api.h"
#include "poly.h"
#include "randombytes.h"

#define ITER 128

void test_normal()
{
  poly normal;
  /* FILE *fp; */
  double mean = 0., stddev = 0.;

  /* fp = fopen("normal.csv", "w"); */
  for (int j = 0; j < ITER; j++)
  {
    normaldist(&normal);

    /*
    for(int i=0; i<SOLMAE_D; i++)
      fprintf(fp, "%f%c", normal.coeffs[i].v, '\n');//(i==SOLMAE_D-1)?'\n':',');
    */

    for (int i = 0; i < SOLMAE_D; i++)
    {
      mean += normal.coeffs[i].v;
      stddev += normal.coeffs[i].v * normal.coeffs[i].v;
    }
  }
  /* fclose(fp); */

  mean /= (SOLMAE_D * ITER);
  stddev = sqrt(stddev / (SOLMAE_D * ITER));
  printf("mean   = %f (expecting 0)\nstddev = %f (expecting %f)\n",
         mean, stddev, sqrt((double)SOLMAE_D / 2));
}

void test_discrete_gauss()
{
  double mean = 0., stddev = 0.;
  double center = 46.4;
  int sample;
  int NBR_SAMPLES = 50000;

  for (int j = 0; j < NBR_SAMPLES; j++)
  {
    sample = samplerZ(center);
    mean += sample;
    stddev += sample * sample;
  }

  mean /= (NBR_SAMPLES);
  stddev = sqrt(((stddev) / (NBR_SAMPLES)) - (mean * mean));
  printf("mean   = %f (expecting %f)\nstddev = %f (expecting 1.32)\n",
         mean, center, stddev);
}

void test_sampler(secret_key *sk)
{
  poly c1, c2, v1, v2;
  uint16_t buf[SOLMAE_D];
  randombytes((uint8_t *)buf, sizeof buf);
  for (int i = 0; i < SOLMAE_D; i++)
  {
    c1.coeffs[i].v = buf[i] % SOLMAE_Q;
    c2.coeffs[i].v = 0.;
  }
  sampler(sk, &c1, &c2, &v1, &v2);
  poly_sub(&v1, &c1);
  poly_recenter(&v1);
  printf("v1:");
  print_poly(&v1);
  printf("v2:");
  print_poly(&v2);

  double mean = 0., stddev = 0.;
  for (int i = 0; i < SOLMAE_D; i++)
  {
    mean += v1.coeffs[i].v;
    stddev += v1.coeffs[i].v * v1.coeffs[i].v;
  }
  mean /= SOLMAE_D;
  stddev = sqrt(((stddev) / (SOLMAE_D)) - (mean * mean));
  printf("mean   = %f (expecting %f)\nstddev = %f (expecting %f)\n",
         mean, 0., stddev, sqrt(SIGMA_SQUARE));
}

void run_tests()
{

  secret_key sk;
  precomp(&sk);

  test_normal();
  test_discrete_gauss();
  test_sampler(&sk);
}
