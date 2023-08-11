
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
  printf("***** RQC-%d *****\n", RQC_SECURITY);
  printf("*******************\n");

  printf("\n");
  printf("Q: %d   ", RQC_PARAM_Q);
  printf("M: %d   ", RQC_PARAM_M);
  printf("K: %d   ", RQC_PARAM_K);
  printf("N: %d   ", RQC_PARAM_N);
  printf("W: %d   ", RQC_PARAM_W);
  printf("W_R: %d   ", RQC_PARAM_W_R);
  printf("W_E: %d   ", RQC_PARAM_W_E);
  printf("DFR: 0   ");
  printf("Sec: %d bits", RQC_SECURITY);


  unsigned char seed[48];
  syscall(SYS_getrandom, seed, 48, 0);
  randombytes_init(seed, NULL, 256);


  unsigned char pk[RQC_PUBLIC_KEY_BYTES] = {0};
  unsigned char sk[RQC_SECRET_KEY_BYTES] = {0};
  unsigned char ct[RQC_CIPHERTEXT_BYTES] = {0};
  unsigned char ss1[RQC_SHARED_SECRET_BYTES] = {0};
  unsigned char ss2[RQC_SHARED_SECRET_BYTES] = {0};


  crypto_kem_keypair(pk, sk);
  crypto_kem_enc(ct, ss1, pk);
  crypto_kem_dec(ss2, ct, sk);


  printf("\n\nsecret1: ");
  for(int i = 0 ; i < RQC_SHARED_SECRET_BYTES ; ++i) printf("%02x", ss1[i]);

  printf("\nsecret2: ");
  for(int i = 0 ; i < RQC_SHARED_SECRET_BYTES ; ++i) printf("%02x", ss2[i]);
  printf("\n\n");

}

