#include "poly.h"
#include "inner.h"
#include "samplerZ.h"
#include <stdio.h>

void print_poly(const poly* p){
  int PRINT_SIZE = 10;//SOLMAE_D-1;
  for(int i=0; i < PRINT_SIZE; ++i) printf("%f, ", p->coeffs[i].v);
  printf("%f \n", p->coeffs[PRINT_SIZE].v);

}

void naive_mul(poly* c, const poly* a, const poly* b){
  
  int i, j;
  int N = SOLMAE_D;

  for(j=0; j < N; j++) c->coeffs[j].v = a->coeffs[0].v*b->coeffs[j].v;
  for(i=1; i < N; i++){
    for(j=0; j < N; j++){
      if (i+j < N) c->coeffs[i+j  ].v = c->coeffs[i+j  ].v + a->coeffs[i].v*b->coeffs[j].v;
      else         c->coeffs[i+j-N].v = c->coeffs[i+j-N].v - a->coeffs[i].v*b->coeffs[j].v;
    }
  }
}


void FFT(poly* p){
  Zf(FFT)(p->coeffs,SOLMAE_LOG_D);

}

void invFFT(poly* p){
  Zf(iFFT)(p->coeffs,SOLMAE_LOG_D);
}


void pointwise_mul(poly* p1, const poly* p2){
  /* Multiplication in FFT form, result in p1*/
  Zf(poly_mul_fft)(p1->coeffs, p2->coeffs, SOLMAE_LOG_D);

}

void poly_add(poly* p1, const poly* p2){
  /* Addition result in p1*/
  Zf(poly_add)(p1->coeffs, p2->coeffs, SOLMAE_LOG_D);

}

void poly_sub(poly* p1, const poly* p2){
  /* Subtraction p1-p2 result in p1*/
  Zf(poly_sub)(p1->coeffs, p2->coeffs, SOLMAE_LOG_D);
}

void poly_div_FFT(poly* p1, const poly* p2){
  Zf(poly_div_fft)(p1->coeffs, p2->coeffs, SOLMAE_LOG_D);
}

void FFT_adj(poly* p){
  Zf(poly_adj_fft)(p->coeffs, SOLMAE_LOG_D);
}

void FFT_mul_adj(poly* p1, const poly* p2){
  Zf(poly_muladj_fft)(p1->coeffs, p2->coeffs, SOLMAE_LOG_D);
}

void FFT_mul_selfadj(poly *p1){
  Zf(poly_mulselfadj_fft)(p1->coeffs, SOLMAE_LOG_D);
}



void set_poly(poly* p1, const poly* p2){
  for(int i=0; i < SOLMAE_D; ++i)
    p1->coeffs[i].v = p2->coeffs[i].v;
}

void scalar_mul_FFT_form(double sigma2, poly* temp){
for(int i=0; i < SOLMAE_D; ++i)
    temp->coeffs[i].v = sigma2*(temp->coeffs[i].v);
}

void poly_recenter(poly* p) {
  for(int i=0; i < SOLMAE_D; ++i) {
    p->coeffs[i].v = fmod(p->coeffs[i].v, (double) SOLMAE_Q);
    if(p->coeffs[i].v > SOLMAE_Q/2)
	p->coeffs[i].v -= (double) SOLMAE_Q;
    else if(p->coeffs[i].v < -SOLMAE_Q/2)
	p->coeffs[i].v += (double) SOLMAE_Q;
  }
}


