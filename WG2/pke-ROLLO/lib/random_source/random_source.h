#ifndef RANDOM_H
#define RANDOM_H

#include "randombytes.h"
#include "seedexpander.h"

#define RANDOM_SOURCE_PRNG    0
#define RANDOM_SOURCE_SEEDEXP 1

typedef struct {
    /*
     * type 0 -> PRNG
     * type 1 -> seedexpander
    */
    int type;
    AES_XOF_struct *ctx;
} random_source;

void random_init(random_source *o, int type);
void random_seed(random_source *o, unsigned char *seed, unsigned char *diversifier, size_t maxlen);
void random_get_bytes(random_source *e, unsigned char *buffer, size_t len);
void random_clear(random_source *o);

#endif

