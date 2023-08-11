
#include <stdio.h>
#include "rbc.h"

#define VERBOSE

static long long cpucycles() {
  unsigned long long result;
  __asm__ volatile(".byte 15;.byte 49;shlq $32,%%rdx;orq %%rdx,%%rax" : "=a" (result) ::  "%rdx");
  return result;
}

int main() {

  unsigned char pk[BIIX_PUBLIC_KEY_BYTES] = {0};
  unsigned char sk[BIIX_SECRET_KEY_BYTES] = {0};
  unsigned char ct[BIIX_CIPHERTEXT_BYTES] = {0};
  unsigned char ss1[BIIX_SHARED_SECRET_BYTES] = {0};
  unsigned char ss2[BIIX_SHARED_SECRET_BYTES] = {0};

  long long t1 = cpucycles();
  biix_keygen(pk, sk);
  long long t2 = cpucycles();


  long long t3 = cpucycles();
  biix_encaps(ct, ss1, pk);
  long long t4 = cpucycles();
 
  long long t5 = cpucycles();
  biix_decaps(ss2, ct, sk);
  long long t6 = cpucycles();
  

  printf("\nKeygen: %lld CPU cycles", t2 - t1);
  
  printf("\nEncaps: %lld CPU cycles", t4 - t3);
  
  printf("\nDecaps: %lld CPU cycles", t6 - t5);

  printf("\n\nsecret1: ");
  for(int i = 0 ; i < BIIX_SHARED_SECRET_BYTES ; ++i) printf("%02x", ss1[i]);

  printf("\nsecret2: ");
  for(int i = 0 ; i < BIIX_SHARED_SECRET_BYTES ; ++i) printf("%02x", ss2[i]);
  printf("\n\n");
  
  
  
}

