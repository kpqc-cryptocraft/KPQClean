#include <stdint.h>
#include "params.h"
#include "TiGER_CPAPKE.h"


int KEM_Keygen(unsigned char *pk, unsigned char *sk);

int KEM_Enc(unsigned char *c, unsigned char *shared_k, const unsigned char *pk);

int KEM_dec(unsigned char *shared_k, const unsigned char *c, const unsigned char *sk, const unsigned char *pk);


