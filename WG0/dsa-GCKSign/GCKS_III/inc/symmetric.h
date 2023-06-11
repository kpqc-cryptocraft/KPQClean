#include <stdint.h>
#include "params.h"

#include "fips202.h"

typedef keccak_state stream128_state;
typedef keccak_state stream256_state;


void stream128_init(keccak_state *state,
                                    const uint8_t seed[32],
                                    uint16_t nonce);



#define STREAM128_BLOCKBYTES SHAKE128_RATE
