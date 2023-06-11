#include "params.h"


/*************************************************
* We gratefully used the code created by 'NewHope'.
https://github.com/newhopecrypto/newhope.git
**************************************************/
static unsigned char coeff_freeze(unsigned char x)
{
  uint16_t m,r;
  int16_t c;
  r = x % Modulus_Q;

  m = r - Modulus_Q;
  c = m;
  c >>= 15;
  r = m ^ ((r^m)&c);

  return r;
}

static unsigned char flipabs(unsigned char x)
{
  int16_t r,m;
  r = coeff_freeze(x);

  r = r - Modulus_Q/2;
  m = r >> 15;
  return (r + m) ^ m;
}
