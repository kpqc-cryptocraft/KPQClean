#include "param.h"
#include "poly.h"
#include <stdint.h>

#if SOLMAE_D == 512
#include "precomp_data512.h"
#elif SOLMAE_D == 1024
#include "precomp_data1024.h"
#endif

static void load_fg(secret_key* sk) {
  for(int i=0; i < SOLMAE_D; ++i){
    sk->f[i] = precomp_f[i];
    sk->g[i] = precomp_g[i];
    sk->F[i] = precomp_F[i];
    sk->G[i] = precomp_G[i];

    (sk->b10).coeffs[i].v = (double)sk->f[i];
    (sk->b11).coeffs[i].v = (double)sk->g[i];
    (sk->b20).coeffs[i].v = (double)sk->F[i];
    (sk->b21).coeffs[i].v = (double)sk->G[i];
  }
  FFT(&(sk->b10));
  FFT(&(sk->b11));
  FFT(&(sk->b20));
  FFT(&(sk->b21));
}


void precomp_GSO(secret_key* sk){

  poly temp1, temp2, temp3;

  for(int i=0; i < SOLMAE_D; ++i){
    (sk->GSO_b10).coeffs[i].v = (sk->b10).coeffs[i].v;
    (sk->GSO_b11).coeffs[i].v = (sk->b11).coeffs[i].v;
    (sk->GSO_b20).coeffs[i].v = (sk->b20).coeffs[i].v;
    (sk->GSO_b21).coeffs[i].v = (sk->b21).coeffs[i].v;
  }

  
  set_poly(&temp1, &(sk->b20)); set_poly(&temp2, &(sk->b21));

  FFT_mul_adj(&temp1, &(sk->b10)); FFT_mul_adj(&temp2, &(sk->b11));
  poly_add(&temp1, &temp2); // temp1 = <b1, b2> 
  
  set_poly(&temp2, &(sk->b10)); set_poly(&temp3, &(sk->b11));

  FFT_mul_selfadj(&temp2); FFT_mul_selfadj(&temp3);
  poly_add(&temp2, &temp3); // temp2 = <b1, b1>
  
  poly_div_FFT(&temp1, &temp2);
  set_poly(&temp2, &temp1); // temp2 = temp1 = <b1, b2>/<b1, b1>

  pointwise_mul(&temp1, &(sk->GSO_b10));
  pointwise_mul(&temp2, &(sk->GSO_b11)); // [temp1,temp2] = (<b1, b2>/<b1, b1>) * ~b1



  poly_sub(&(sk->GSO_b20), &temp1); poly_sub(&(sk->GSO_b21), &temp2);
}

void precomp_sigma(secret_key* sk){



  poly temp1, temp2, poly_r_square;

  for(int i=0; i < SOLMAE_D/2; ++i) {
    (sk->sigma1).coeffs[i].v = SIGMA_SQUARE;
    (sk->sigma2).coeffs[i].v = SIGMA_SQUARE;
    poly_r_square.coeffs[i].v = R_SQUARE;

  }
  for(int i=SOLMAE_D/2; i < SOLMAE_D; ++i) {
    poly_r_square.coeffs[i].v = 0;
    (sk->sigma1).coeffs[i].v = 0;
    (sk->sigma2).coeffs[i].v = 0;
  }


  set_poly(&temp1, &(sk->GSO_b10)); set_poly(&temp2, &(sk->GSO_b11));

  FFT_mul_selfadj(&temp1); FFT_mul_selfadj(&temp2);
  poly_add(&temp1, &temp2);

  poly_div_FFT(&(sk->sigma1), &temp1);
  poly_sub(&(sk->sigma1), &poly_r_square);

  for(int i=0; i < SOLMAE_D; ++i) (sk->sigma1).coeffs[i].v = sqrt((sk->sigma1).coeffs[i].v);


  set_poly(&temp1, &(sk->GSO_b20)); set_poly(&temp2, &(sk->GSO_b21));

  FFT_mul_selfadj(&temp1); FFT_mul_selfadj(&temp2);
  poly_add(&temp1, &temp2);
  poly_div_FFT(&(sk->sigma2), &temp1);
  poly_sub(&(sk->sigma2), &poly_r_square);
  for(int i=0; i < SOLMAE_D; ++i) (sk->sigma2).coeffs[i].v = sqrt((sk->sigma2).coeffs[i].v);

}

void precomp_beta_hat(secret_key* sk){

  poly temp1, temp2;
  // beta1
  set_poly(&temp1, &(sk->GSO_b10)); set_poly(&temp2, &(sk->GSO_b11));

  FFT_mul_selfadj(&temp1); FFT_mul_selfadj(&temp2);
  poly_add(&temp1, &temp2);

  set_poly(&(sk->beta10), &(sk->GSO_b10));
  set_poly(&(sk->beta11), &(sk->GSO_b11));
  FFT_adj(&(sk->beta10)); FFT_adj(&(sk->beta11));
  poly_div_FFT(&(sk->beta10), &temp1); poly_div_FFT(&(sk->beta11), &temp1);



  // beta2
  set_poly(&temp1, &(sk->GSO_b20)); set_poly(&temp2, &(sk->GSO_b21));

  FFT_mul_selfadj(&temp1); FFT_mul_selfadj(&temp2);
  poly_add(&temp1, &temp2);

  set_poly(&(sk->beta20), &(sk->GSO_b20));
  set_poly(&(sk->beta21), &(sk->GSO_b21));
  FFT_adj(&(sk->beta20)); FFT_adj(&(sk->beta21));
  poly_div_FFT(&(sk->beta20), &temp1); poly_div_FFT(&(sk->beta21), &temp1);

}

void precomp(secret_key* sk){
  load_fg(sk);
  precomp_GSO(sk);
  precomp_sigma(sk);
  precomp_beta_hat(sk); 
  //print_sk(sk);
}

void load_pk(public_key* pk){
  for(int i=0; i < SOLMAE_D; ++i){
    pk->hint[i] = precomp_hint[i];
    pk->h.coeffs[i].v = (double)pk->hint[i];
  }
  FFT(&(pk->h));
}
