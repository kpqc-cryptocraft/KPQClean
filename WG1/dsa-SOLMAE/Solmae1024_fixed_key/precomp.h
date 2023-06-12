#include "param.h"

extern double precomp_b10[SOLMAE_D];
extern double precomp_b11[SOLMAE_D];
extern double precomp_b20[SOLMAE_D];
extern double precomp_b21[SOLMAE_D];
extern double precomp_h[SOLMAE_D];


void load_B(secret_key* sk);
void precomp_GSO(secret_key* sk);
void precomp_sigma(secret_key* sk);
void precomp_beta_hat(secret_key* sk);
void precomp(secret_key* sk);
void load_pk(public_key* pk);
