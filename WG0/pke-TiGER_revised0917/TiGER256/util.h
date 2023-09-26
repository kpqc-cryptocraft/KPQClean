// FrodoKEM: Learning with Errors Key Encapsulation
#include <string.h>
#include <stdint.h>

unsigned char ct_verify(const unsigned char *a, const unsigned char *b, size_t len);

void ct_select(unsigned char *r, const unsigned char *a, const unsigned char *b, size_t len, unsigned char selector);


