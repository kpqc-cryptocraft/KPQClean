// FrodoKEM: Learning with Errors Key Encapsulation

#include <string.h>
#include <stdint.h>

unsigned char ct_verify(const unsigned char *a, const unsigned char *b, size_t len) 
{ // Compare two arrays in constant time.
  // Returns 0 if the byte arrays are equal, -1 otherwise.
    unsigned char r = 0;

    for (size_t i = 0; i < len; i++) {
        r |= a[i] ^ b[i];
    }

    r = (-(unsigned char)(r >> 1) | -(unsigned char)(r & 1)) >> (8*sizeof(unsigned char)-1);
    return (unsigned char)r;
}


void ct_select(unsigned char *r, const unsigned char *a, const unsigned char *b, size_t len, unsigned char selector) 
{ // Select one of the two input arrays to be moved to r
  // If (selector == 0) then load r with a, else if (selector == -1) load r with b

    for (size_t i = 0; i < len; i++) {
        r[i] = (~selector & a[i]) | (selector & b[i]);
    }
}

void compress10to8(uint16_t* t, unsigned char* s){
	s[0] = 						   (t[0] >> 2) & 0xFF; 
	s[1] = ((t[0] & 0x03) << 6) | ((t[1] >> 4) & 0x3F); 
	s[2] = ((t[1] & 0x0F) << 4) | ((t[2] >> 6) & 0x0F); 
	s[3] = ((t[2] & 0x3F) << 2) | ((t[3] >> 8) & 0x03); 
    s[4] = ((t[3] & 0xFF) << 0); 
}

void recover8to10(uint16_t* t, const unsigned char* s){
	t[0] = ((s[0] & 0xFF) << 2) | ((s[1] >> 6) & 0x03); 
	t[1] = ((s[1] & 0x3F) << 4) | ((s[2] >> 4) & 0x0F);
	t[2] = ((s[2] & 0x0F) << 6) | ((s[3] >> 2) & 0x3F);
	t[3] = ((s[3] & 0x03) << 8) | ((s[4] >> 0) & 0xFF);
}

void compress7to8(unsigned char* s, unsigned char* t){
   s[0] =  t[0]              | (t[1] >> 7);            
   s[1] = (t[1] & 0x7E) << 1 | (t[2] >> 6);    
   s[2] = (t[2] & 0x3E) << 2 | (t[3] >> 5);     
   s[3] = (t[3] & 0x1E) << 3 | (t[4] >> 4);    
   s[4] = (t[4] & 0x0E) << 4 | (t[5] >> 3);   
   s[5] = (t[5] & 0x06) << 5 | (t[6] >> 2);   
   s[6] = (t[6] & 0x02) << 6 | (t[7] >> 1);    
}

void recover8to7(unsigned char* t, const unsigned char* s){
   t[0] =                       s[0] & 0xFE  >> 0;
   t[1] = (s[0] & 0x01) << 7 | (s[1] & 0xFC) >> 1; 
   t[2] = (s[1] & 0x03) << 6 | (s[2] & 0xF8) >> 2;
   t[3] = (s[2] & 0x07) << 5 | (s[3] & 0xF0) >> 3;
   t[4] = (s[3] & 0x0F) << 4 | (s[4] & 0xE0) >> 4;
   t[5] = (s[4] & 0x1F) << 3 | (s[5] & 0xC0) >> 5; 
   t[6] = (s[5] & 0x3F) << 2 | (s[6] & 0x80) >> 6;
   t[7] = (s[6] & 0x7F) << 1                     ;
}

void compress6to8(unsigned char* s, unsigned char* t){
   s[0] =  t[0]              | (t[1] >> 6);
   s[1] = (t[1] & 0x3C) << 2 | (t[2] >> 4);
   s[2] = (t[2] & 0x0C) << 4 | (t[3] >> 2);
}

void recover8to6(unsigned char* t, const unsigned char* s){
   t[0] =                       s[0] & 0xFC;
   t[1] = (s[0] & 0x03) << 6 | (s[1] & 0xF0) >> 2;
   t[2] = (s[1] & 0x0F) << 4 | (s[2] & 0xC0) >> 4;
   t[3] = (s[2] & 0x3F) << 2;
}

void compress2to8(unsigned char* s, unsigned char* t){
   s[0] = t[0] | (t[1] >> 2) | (t[2] >> 4) | (t[3] >> 6);
}

void recover8to2(unsigned char* t, const unsigned char* s){
   t[0] = s[0] & 0xC0;
   t[1] = (s[0] & 0x30) << 2;
   t[2] = (s[0] & 0x0C) << 4;
   t[3] = (s[0] & 0x03) << 6;
}

void compress3to8(unsigned char* s, unsigned char* t){
	s[0] = (t[0] & 0xE0) | (t[1] >> 3) | (t[2] >> 6);
	s[1] = ((t[2] << 2) & 0x80) | (t[3] >> 1) | (t[4] >> 4) | (t[5] >> 7);
	s[2] = ((t[5] << 1) & 0xC0) | (t[6] >> 2) | (t[7] >> 5);
}

void recover8to3(unsigned char* t, const unsigned char* s){
	t[0] = s[0]&0xE0;
	t[1] = (s[0]<<3)&0xE0;
	t[2] = ((s[0]<<6)&0xC0) | ((s[1]&0x80)>>2);
	t[3] = (s[1]<<1)&0xE0;
	t[4] = (s[1]<<4)&0xE0;
	t[5] = ((s[1]<<7)&0x80) | ((s[2]&0xC0)>>1);
	t[6] = (s[2]<<2)&0xE0;
	t[7] = (s[2]<<5)&0xE0;
}
