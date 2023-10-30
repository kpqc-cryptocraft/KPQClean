/*
 * Unit-tests 
 */
#include <unistd.h>
#include <stdlib.h>
#include "minunit.h"
#include <NTL/GF2E.h>
#include <NTL/GF2X.h>
#include <NTL/GF2EX.h>
#include <NTL/GF2XFactoring.h>
#include <stdio.h>
extern "C" {
  #include "rbc.h"
}
#include "utils.h"

// #define VERBOSE 1


/*
 * Variables
 */
int INSTANCES = 100;

#define ADIM 5
#define BDIM 5

rbc_vspace a;
rbc_vspace b;
rbc_vspace product;
rbc_vspace inter;
rbc_vspace invProd[ADIM];



/*
 * Initialization and destruction
 */
void test_setup() {

  // Initialization
  rbc_field_init();
  rbc_vspace_init(&a, ADIM);
  rbc_vspace_init(&b, BDIM);
  rbc_vspace_init(&product, ADIM * BDIM);
  rbc_vspace_init(&inter, ADIM * BDIM);
  for(int i = 0 ; i < ADIM ; i++) rbc_vspace_init(&invProd[i], ADIM * BDIM);
}

void test_teardown() {};

void MU_SUITE_CLEAR() {
  rbc_vspace_clear(a);
  rbc_vspace_clear(b);
  rbc_vspace_clear(product);
  rbc_vspace_clear(inter);
  for(int i = 0 ; i < ADIM ; i++) rbc_vspace_clear(invProd[i]);
}



/*
 * Test functions
 */
MU_TEST(test_vspace) {

  random_source prng;
  random_init(&prng, 0);

  // Get random vector space for A and B
  rbc_vspace_set_random_full_rank(&prng, a, ADIM);
  rbc_vspace_set_random_full_rank(&prng, b, BDIM);

  // Compute product vector space
  rbc_vspace_product(product, a, b, ADIM, BDIM);

  // Compute A[i]^-1 * product
  rbc_elt inverse;
  for(int i = 0 ; i < ADIM ; i++) {
    rbc_elt_inv(inverse, a[i]);
    rbc_vec_scalar_mul(invProd[i], product, inverse, ADIM*BDIM);
  }

  unsigned int interDim = 0;

  // Intersect every invProd. The resulting intersection should be at least of dimension BDIM (equal with high probability if m is high enough)
  interDim = rbc_vspace_intersection(inter, invProd[0], invProd[1], ADIM * BDIM, ADIM * BDIM);
  for(int i = 2 ; i < ADIM ; i++) {
    interDim = rbc_vspace_intersection(inter, inter, invProd[i], interDim, ADIM * BDIM);
  }

  mu_assert(interDim >= BDIM, "Failure: test_vspace");
}



/*
 * Test execution
 */
MU_TEST_SUITE(test_suite) {
  MU_SUITE_CONFIGURE(&test_setup, &test_teardown);
  MU_RUN_TEST(test_vspace);
  MU_SUITE_CLEAR();
}

int main() {
  for(int i = 0 ; i < INSTANCES ; ++i) {
    MU_RUN_SUITE(test_suite);
  }

  MU_REPORT();
  return minunit_fail;
}

