
#define _GNU_SOURCE
#include <stdio.h>
#include <unistd.h>
#include <syscall.h>
#include "randombytes.h"
#include "api.h"
#include "parameters.h"

int main() {

  printf("\n");
  printf("*******************\n");
  printf("*** ROLLOII-%d ***\n", ROLLOII_PARAM_SECURITY);
  printf("*******************\n");

  printf("\n");
  printf("M: %d   ", ROLLOII_PARAM_M);
  printf("N: %d   ", ROLLOII_PARAM_N);
  printf("R: %d   ", ROLLOII_PARAM_R);
  printf("D: %d   ", ROLLOII_PARAM_D);
  printf("DFR: %d   ", ROLLOII_PARAM_DFR);
  printf("Sec: %d bits", ROLLOII_PARAM_SECURITY);


  unsigned char seed[48];
  syscall(SYS_getrandom, seed, 48, 0);
  randombytes_init(seed, NULL, 256);


  unsigned char pk[ROLLOII_PUBLIC_KEY_BYTES] = {0};
  unsigned char sk[ROLLOII_SECRET_KEY_BYTES] = {0};
  unsigned char ct[ROLLOII_CIPHERTEXT_BYTES] = {0};
  unsigned char ss1[ROLLOII_SHARED_SECRET_BYTES] = {0};
  unsigned char ss2[ROLLOII_SHARED_SECRET_BYTES] = {0};


  crypto_kem_keypair(pk, sk);
  crypto_kem_enc(ct, ss1, pk);
  crypto_kem_dec(ss2, ct, sk);


  printf("\n\nsecret1: ");
  for(int i = 0 ; i < ROLLOII_SHARED_SECRET_BYTES ; ++i) printf("%02x", ss1[i]);

  printf("\nsecret2: ");
  for(int i = 0 ; i < ROLLOII_SHARED_SECRET_BYTES ; ++i) printf("%02x", ss2[i]);
  printf("\n\n");

}

