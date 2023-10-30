/**
 * \file hash.h
 * \brief A generic declaration of SHA512
 */

#ifndef HASH_H
#define HASH_H

#include <stdlib.h>

void sha512(unsigned char* output, unsigned char* input, size_t size);

#endif
