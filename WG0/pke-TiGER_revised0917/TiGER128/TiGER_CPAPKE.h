#include <stdint.h>
#include "params.h"


int Keygen(unsigned char *pk, unsigned char *sk);

int Encryption(unsigned char *c, const unsigned char *pk, unsigned char *Message, unsigned char *coin);

int Decryption(unsigned char *Message, const unsigned char *c, const unsigned char *sk);

//for compress sk//
void compress9to8(uint16_t* t, unsigned char* s);
void recover8to9(uint16_t* t, const unsigned char* s);
//for compress pk//
void compress7to8(unsigned char* t, unsigned char* s);
void recover8to7(unsigned char* t, const unsigned char* s);
//for compress c1, c2 to c//
void compress6to8(unsigned char* t, unsigned char* s);
void recover8to6(unsigned char* t, const unsigned char* s);
void compress4to8(unsigned char* t, unsigned char* s);
void recover8to4(unsigned char* t, const unsigned char* s);
void compress3to8(unsigned char* t, unsigned char* s);
void recover8to3(unsigned char* t, const unsigned char* s);
