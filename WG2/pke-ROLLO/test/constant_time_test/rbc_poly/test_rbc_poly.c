/*
 * Constant-time test
 */
#include <unistd.h>
#include <stdlib.h>

#include <stdio.h>

#include "rbc.h"

#include "random_source.h"
#include "poison.h"


/*
 * Variables
 */
int INSTANCES = 10;
int DEGREE;


rbc_poly a;
rbc_poly b;
rbc_poly c;
rbc_poly d;

rbc_poly modulo;
rbc_poly_sparse sparse_modulo;

rbc_poly divisor;
rbc_poly q;
rbc_poly r;

uint8_t done = 0;

#define DIVISOR_SIZE 10


/*
 * Initialization and destruction
 */
void test_setup() {
  random_source prng;
  random_init(&prng, RANDOM_SOURCE_PRNG);

  // RBC initialization
  rbc_field_init();

  rbc_poly_init(&a, DEGREE-1);
  rbc_poly_init(&b, DEGREE-1);
  rbc_poly_init(&c, DEGREE-1);
  rbc_poly_init(&d, 2*DEGREE-1);
  rbc_poly_set_random(&prng,a,DEGREE-1);
  rbc_poly_set_random(&prng,b,DEGREE-1);
  rbc_poly_set_random(&prng,c,DEGREE-1);
  rbc_poly_set_random(&prng,d,2*DEGREE-1);

  rbc_poly_init(&modulo, DEGREE);
  rbc_poly_set_random(&prng,modulo,DEGREE);

  sparse_modulo = rbc_qre_get_modulus(DEGREE);

  rbc_poly_init(&divisor, DIVISOR_SIZE-1);
  rbc_poly_init(&q, DEGREE-1);
  rbc_poly_init(&r, DEGREE-1);
  rbc_poly_set_random(&prng,divisor,DIVISOR_SIZE-1);
  rbc_poly_set_random(&prng,q,DEGREE-1);
  rbc_poly_set_random(&prng,r,DEGREE-1);
}



void clear() {

  rbc_poly_clear(a);
  rbc_poly_clear(b);
  rbc_poly_clear(c);
  rbc_poly_clear(d);
  rbc_poly_clear(modulo);
  rbc_poly_sparse_clear(sparse_modulo);
  rbc_poly_clear(divisor);
  rbc_poly_clear(q);
  rbc_poly_clear(r);
}



/*
 * Test functions
 */
void test_poly_add(){

  poison(c->v,sizeof(rbc_elt)*DEGREE);
  poison(&(c->degree),sizeof(int32_t));
 // poison(&(c->max_degree),sizeof(int32_t));
  poison(a->v,sizeof(rbc_elt)*DEGREE);
  poison(&(a->degree),sizeof(int32_t));
 // poison(&(a->max_degree),sizeof(int32_t));
  poison(b->v,sizeof(rbc_elt)*DEGREE);
  poison(&(b->degree),sizeof(int32_t));
 // poison(&(b->max_degree),sizeof(int32_t));
 
  rbc_poly_add(c, a, b);
}

void test_poly_mul(){

  poison(d->v,sizeof(rbc_elt)*DEGREE*2);
  poison(&(d->degree),sizeof(int32_t));
 // poison(&(c->max_degree),sizeof(int32_t));
  poison(a->v,sizeof(rbc_elt)*DEGREE);
  poison(&(a->degree),sizeof(int32_t));
 // poison(&(a->max_degree),sizeof(int32_t));
  poison(b->v,sizeof(rbc_elt)*DEGREE);
  poison(&(b->degree),sizeof(int32_t));

  rbc_poly_mul(d, a, b);

}


void test_poly_mulmod_sparse(){
  poison(c->v,sizeof(rbc_elt)*DEGREE);
  poison(&(c->degree),sizeof(int32_t));
 // poison(&(c->max_degree),sizeof(int32_t));
  poison(a->v,sizeof(rbc_elt)*DEGREE);
  poison(&(a->degree),sizeof(int32_t));
 // poison(&(a->max_degree),sizeof(int32_t));
  poison(b->v,sizeof(rbc_elt)*DEGREE);
  poison(&(b->degree),sizeof(int32_t));


  unpoison(sparse_modulo->coeffs,sizeof(int32_t)*(sparse_modulo->coeffs_nb));
  unpoison(&(sparse_modulo->coeffs_nb),sizeof(int32_t));

  rbc_poly_mulmod_sparse(c, a, b, sparse_modulo);

}


void test_poly_inv(){
  poison(c->v,sizeof(rbc_elt)*DEGREE);
  poison(&(c->degree),sizeof(int32_t));
 // poison(&(c->max_degree),sizeof(int32_t));
  poison(a->v,sizeof(rbc_elt)*DEGREE);
  poison(&(a->degree),sizeof(int32_t));
 // poison(&(a->max_degree),sizeof(int32_t));
 
  unpoison(modulo->v,sizeof(rbc_elt)*(DEGREE+1));
  unpoison(&(modulo->degree),sizeof(int32_t));

  rbc_poly_inv(c, a, modulo);

}


/*
 * Test execution
 */
void test_suite(){
  test_setup();
  test_poly_add();
  test_poly_mul();
  test_poly_mulmod_sparse();
  test_poly_inv();
  clear();
}

int main() {
  for(int i = 0 ; i < INSTANCES ; ++i) {
    test_suite();
  }
}

