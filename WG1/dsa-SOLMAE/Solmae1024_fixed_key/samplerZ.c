#include "randombytes.h"
#include "param.h"
#include "samplerZ.h"
#include <stdint.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#define TABLE_SIZE 13

uint64_t CDT[TABLE_SIZE] = {8562458705743934607LLU,
                            14988938141546119862LLU,
                            17705984313312429518LLU,
                            18353082494776078532LLU,
                            18439897061947435901LLU,
                            18446457975170112665LLU,
                            18446737284374178633LLU,
                            18446743982533372247LLU,
                            18446744073018029834LLU,
                            18446744073706592852LLU,
                            18446744073709544480LLU,
                            18446744073709551607LLU,
                            18446744073709551615LLU};

int base_sampler()
{
  uint64_t r = get64();
  int res = 0;
  for (int i = 0; i < TABLE_SIZE; ++i)
    res += (r >= CDT[i]);
  return res;
}

int samplerZ(double u)
{
  int z0, b, z, uf;
  double x, p, r;
  uint8_t entropy;
  uf = floor(u);
  while (1)
  {
    entropy = get8();
    for (int i = 0; i < 8; ++i)
    {
      z0 = base_sampler();
      b = (entropy >> i) & 1;
      z = (2 * b - 1) * z0 + b + uf; // add floor u here because we subtract u at the next step
      x = ((double)(z0 * z0) - ((double)(z - u) * (z - u))) / (2 * R * R);
      p = exp(x);

      /*
      r = (double)get64()/(1LLU<<63);
      r /= 2;
      */
      r = (double)(get64() & 0x1FFFFFFFFFFFFFul) * pow(2, -53);
      if (r < p)
        return z;
    }
  }
}

void sample_discrete_gauss(poly *p)
{
#if 0
  int j = 0;
  uint64_t rbase[SOLMAE_D], rcomp[SOLMAE_D];
  uint8_t bs[SOLMAE_D/8];
  int z0, b;
  double xp, z, z0d, r, us[SOLMAE_D], ufs[SOLMAE_D];

  for(int i=0; i<SOLMAE_D; i++) {
      ufs[i] = floor(p->coeffs[i].v);
      us[i]  = p->coeffs[i].v - ufs[i];
  }

  while(1) {
      randombytes((uint8_t*)rbase, sizeof rbase);
      randombytes((uint8_t*)rcomp, sizeof rcomp);
      randombytes(bs, sizeof bs);

      for(int i=0; i<SOLMAE_D; i++) {
	  z0 = 0;
	  for(int k=0; k < TABLE_SIZE; k++)
	      z0 += (rbase[i] >= CDT[k]);
	  b  = (bs[i>>3]>>(i&7))&1;
	  z0d= (double)z0;
	  z  = (2*b-1)*z0d + b;
	  r  = (double)(rcomp[i] & 0x1FFFFFFFFFFFFFul) * pow(2,-53);
	  z -= us[j];
          xp = exp( (z0d*z0d-z*z)/(2*R*R) );
	  if(r < xp) {
	      p->coeffs[j].v = z + ufs[j];
	      j++;
	      if(j >= SOLMAE_D)
		  return;
	  }
      }
  }
#else
  for (int i = 0; i < SOLMAE_D; i++)
  {
    p->coeffs[i].v = samplerZ(p->coeffs[i].v);
  }
#endif
}
