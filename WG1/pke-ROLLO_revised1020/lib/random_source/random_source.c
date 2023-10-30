#include "random_source.h"
#include <stdlib.h>

void random_init(random_source *o, int type) {
    o->type = type;
    if(type == RANDOM_SOURCE_PRNG) { //PRNG
        o->ctx = NULL;
    } else { //Seedexpander
        o->ctx = (AES_XOF_struct*) malloc(sizeof(AES_XOF_struct));
    }
}

void random_seed(random_source *o, unsigned char *seed, unsigned char *diversifier, size_t maxlen) {
    if(o->type == RANDOM_SOURCE_SEEDEXP) {
        seedexpander_init(o->ctx, seed, diversifier, maxlen);
    }
}

void random_get_bytes(random_source *e, unsigned char *buffer, size_t len) {
    if (e->type == RANDOM_SOURCE_PRNG) {
        randombytes(buffer, len);
    } else {
        seedexpander(e->ctx, buffer, len);
    }
}

void random_clear(random_source *o) {
    if(o->type == RANDOM_SOURCE_SEEDEXP) {
        free(o->ctx);
    }
}
