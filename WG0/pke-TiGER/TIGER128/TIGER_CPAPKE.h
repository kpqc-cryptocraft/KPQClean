#include <stdint.h>
#include "params.h"


int Keygen(unsigned char *pk, unsigned char *sk);

int Encryption(unsigned char *c, const unsigned char *pk, unsigned char *Message, unsigned char *coin);

int Decryption(unsigned char *Message, const unsigned char *c, const unsigned char *sk);


