#include <stdint.h>
#include "params.h"


int Keygen(unsigned char *pk, unsigned char *sk);

int Encryption(unsigned char *c, const unsigned char *pk, unsigned char *Message, unsigned char *coin);

int Decryption(unsigned char *Message, const unsigned char *c, const unsigned char *sk);

////////sk
void compress10to8(uint16_t* t, unsigned char* s);
void recover8to10(uint16_t* t, const unsigned char* s);

/////////////pk/////////////
void compress7to8(unsigned char* t, unsigned char* s);
void recover8to7(unsigned char* t, const unsigned char* s);

/////////ctx
void compress6to8(unsigned char* t, unsigned char* s);
void recover8to6(unsigned char* t, const unsigned char* s);

void compress2to8(unsigned char* t, unsigned char* s);
void recover8to2(unsigned char* t, const unsigned char* s);

void compress3to8(unsigned char* t, unsigned char* s);
void recover8to3(unsigned char* t, const unsigned char* s);
