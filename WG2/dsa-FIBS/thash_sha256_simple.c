#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include "thash.h"
#include "address.h"
#include "params.h"
#include "sha256.h"
#include "./cglhash/P610/api.h"

/**
 * Takes an array of inblocks concatenated arrays of SPX_N bytes.
 */
void thash(unsigned char *out, const unsigned char *in, unsigned int inblocks,
           const unsigned char *pub_seed, uint32_t addr[8])
{
    unsigned char buf[SPX_SHA256_ADDR_BYTES + inblocks*SPX_N];
    unsigned char outbuf[SPX_SHA256_OUTPUT_BYTES];
    uint8_t sha2_state[40];

    (void)pub_seed; /* Suppress an 'unused parameter' warning. */

    /* Retrieve precomputed state containing pub_seed */
    memcpy(sha2_state, state_seeded, 40 * sizeof(uint8_t));

    memcpy(buf, addr, SPX_SHA256_ADDR_BYTES);
    memcpy(buf + SPX_SHA256_ADDR_BYTES, in, inblocks * SPX_N);

    sha256_inc_finalize(outbuf, sha2_state, buf, SPX_SHA256_ADDR_BYTES + inblocks*SPX_N);
    memcpy(out, outbuf, SPX_N);
}


void thash_ori(unsigned char *out, const unsigned char *in, unsigned int inblocks,
           const unsigned char *pub_seed, uint32_t addr[8])
{
    unsigned char buf[SPX_SHA256_ADDR_BYTES + inblocks*SPX_N];
    unsigned char outbuf[SPX_SHA256_OUTPUT_BYTES];
    uint8_t sha2_state[40];

    (void)pub_seed; /* Suppress an 'unused parameter' warning. */

    /* Retrieve precomputed state containing pub_seed */
    memcpy(sha2_state, state_seeded, 40 * sizeof(uint8_t));

    memcpy(buf, addr, SPX_SHA256_ADDR_BYTES);
    memcpy(buf + SPX_SHA256_ADDR_BYTES, in, inblocks * SPX_N);

    printf("len : %d",SPX_SHA256_ADDR_BYTES + inblocks*SPX_N );
    sha256_inc_finalize(outbuf, sha2_state, buf, SPX_SHA256_ADDR_BYTES + inblocks*SPX_N);
    memcpy(out, outbuf, SPX_N);
}


void thash_wots(unsigned char *out, const unsigned char *in, unsigned int inblocks,
           const unsigned char *pub_seed, uint32_t addr[8])
{
    unsigned char buf[SPX_SHA256_ADDR_BYTES + inblocks*SPX_N];
    unsigned char outbuf[SPX_SHA256_OUTPUT_BYTES];
    uint8_t sha2_state[40];

    (void)pub_seed; /* Suppress an 'unused parameter' warning. */

    /* Retrieve precomputed state containing pub_seed */
    memcpy(sha2_state, state_seeded, 40 * sizeof(uint8_t));

    memcpy(buf, addr, SPX_SHA256_ADDR_BYTES);
    memcpy(buf + SPX_SHA256_ADDR_BYTES, in, inblocks * SPX_N);

        //isogeny hash here....
    unsigned char DigestA[SPX_CGL]={0,};
    CGLhash(buf, DigestA, SPX_SHA256_ADDR_BYTES + inblocks*SPX_N);
  
    /*
        for (size_t i = 0; i < 32; ++i) {
        outbuf[i] = DigestA[i];
    }  
    */
    sha256_inc_finalize(outbuf, sha2_state, DigestA, SPX_CGL);
    memcpy(out, outbuf, SPX_N);
    //memcpy(out, outbuf, SPX_CGL);
}