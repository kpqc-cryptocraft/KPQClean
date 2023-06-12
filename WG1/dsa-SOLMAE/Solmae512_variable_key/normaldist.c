#include "normaldist.h"
#include "api.h"
#include "randombytes.h"
#include <math.h>
#include <x86intrin.h>
#include <string.h>

/* Store a double-precision centered normal vector in vec.
 * Computation using Box-Muller. SOLMAE_D is assumed to be even.
 * Standard deviation is SOLMAE_D/2, so that the output vector has
 * the same distribution as the FFT of a standard normal vector.
 */
void normaldist(poly *vec)
{
    uint64_t u[SOLMAE_D/2], v[SOLMAE_D/2], e[SOLMAE_D];
    double uf[SOLMAE_D/2], vf[SOLMAE_D/2];
    int geom[SOLMAE_D/2];

    randombytes((uint8_t*)u, sizeof u);
    randombytes((uint8_t*)v, sizeof v);
    randombytes((uint8_t*)e, sizeof e);

    /*
    for(int i=0; i < SOLMAE_D/2; ++i){
      u[i] = get64();
      v[i] = get64();
      e[i] = get64();
      e[i+SOLMAE_D/2] = get64();
    }
    */

    for(int i=0; i < SOLMAE_D/2; i++) {
      uf[i] = 2*M_PI*(double)(u[i] & 0x1FFFFFFFFFFFFFul) * pow(2,-53);
      vf[i] = 0.5 + (double)(v[i] & 0x1FFFFFFFFFFFFFul) * pow(2,-54);

      geom[i] = CMUX(63 + ffsll(e[2*i+1]), ffsll(e[2*i]) - 1, 
            CZERO64(e[2*i]));

      vf[i] = sqrt(SOLMAE_D*(M_LN2*geom[i]-log(vf[i])));
    }

    for(int i=0; i < SOLMAE_D/2; i++) {
      vec->coeffs[2*i].v   = vf[i] * cos(uf[i]);
      vec->coeffs[2*i+1].v = vf[i] * sin(uf[i]);
    }
}
