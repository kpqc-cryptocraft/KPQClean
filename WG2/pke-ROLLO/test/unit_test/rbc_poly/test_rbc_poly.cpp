/*
 * Unit-tests against NTL
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

//#define VERBOSE 1

using namespace NTL;
using namespace std;


/*
 * Variables
 */
int INSTANCES = 25;
int DEGREE;

GF2EX a1;
GF2EX b1;
GF2EX c1;
GF2EX d1;

GF2EX c1_tmp;
GF2EX modulo1;
GF2EX sparse_modulo1;

GF2EX divisor1;
GF2EX q1;
GF2EX r1;

rbc_poly a2;
rbc_poly b2;
rbc_poly c2;
rbc_poly d2;

rbc_poly modulo2;
rbc_poly_sparse sparse_modulo2;

rbc_poly divisor2;
rbc_poly q2;
rbc_poly r2;

uint8_t done = 0;

#define DIVISOR_SIZE 10


/*
 * Initialization and destruction
 */
void test_setup() {

  // NTL initialization
  if(!done) {
    GF2X P = BuildSparseIrred_GF2X(RBC_FIELD_M);
    GF2E::init(P);
    done = 1;
  }

  random(a1, DEGREE);
  random(b1, DEGREE);
  random(c1, DEGREE);

  random(modulo1, DEGREE+1);

  sparse_modulo1.SetLength(DEGREE+1);
  GF2X sparse_modulo_F2 = BuildSparseIrred_GF2X(DEGREE);

  for(int i=0 ; i<DEGREE+1 ; i++) {
    if(sparse_modulo_F2[i] == 1) SetCoeff(sparse_modulo1, i, 1);
    else SetCoeff(sparse_modulo1, i, 0);
  }
  
  sparse_modulo1.normalize();

  random(divisor1, DIVISOR_SIZE+1);

  // RBC initialization
  rbc_field_init();

  rbc_poly_init(&a2, DEGREE-1);
  rbc_poly_init(&b2, DEGREE-1);
  rbc_poly_init(&c2, DEGREE-1);
  rbc_poly_init(&d2, 2*DEGREE-1);
  rbc_poly_set_ntl(a2, a1, DEGREE-1);
  rbc_poly_set_ntl(b2, b1, DEGREE-1);
  rbc_poly_set_ntl(c2, c1, DEGREE-1);
  rbc_poly_set_ntl(d2, d1, 2*DEGREE-1);

  rbc_poly_init(&modulo2, DEGREE);
  rbc_poly_set_ntl(modulo2, modulo1, DEGREE);

  sparse_modulo2 = rbc_qre_get_modulus(DEGREE);

  rbc_poly_init(&divisor2, DIVISOR_SIZE-1);
  rbc_poly_init(&q2, DEGREE-1);
  rbc_poly_init(&r2, DEGREE-1);
  rbc_poly_set_ntl(divisor2, divisor1, DIVISOR_SIZE-1);
  rbc_poly_set_ntl(q2, q1, DEGREE-1);
  rbc_poly_set_ntl(r2, r1, DEGREE-1);

  #if VERBOSE
    printf("\na1:"); ntl_poly_print_uint64(a1);
    printf("\na2:"); rbc_poly_print(a2);
    printf("\n");
    printf("\nb1:"); ntl_poly_print_uint64(b1);
    printf("\nb2:"); rbc_poly_print(b2);
    printf("\n");
  #endif
}

void test_teardown() {};

void MU_SUITE_CLEAR() {
  rbc_poly_clear(a2);
  rbc_poly_clear(b2);
  rbc_poly_clear(c2);
  rbc_poly_clear(d2);
  rbc_poly_clear(modulo2);
  rbc_poly_sparse_clear(sparse_modulo2);
  rbc_poly_clear(divisor2);
  rbc_poly_clear(q2);
  rbc_poly_clear(r2);
}



/*
 * Test functions
 */
MU_TEST(test_poly_add) {
  c1 = a1 + b1;
  rbc_poly_add(c2, a2, b2);

  #if VERBOSE
    printf("\nc1:"); ntl_poly_print_uint64(c1);
    printf("\nc2:"); rbc_poly_print(c2);
    printf("\n");
  #endif

  mu_assert(rbc_poly_equal_to_ntl(c1, c2) == 1, "Failure: test_poly_add");
}


MU_TEST(test_poly_mul) {
  d1 = b1 * a1;
  rbc_poly_mul(d2, a2, b2);

  #if VERBOSE
    printf("\nd1:"); ntl_poly_print_uint64(d1);
    printf("\nd2:"); rbc_poly_print(d2);
    printf("\n");
  #endif

  mu_assert(rbc_poly_equal_to_ntl(d1, d2) == 1, "Failure: test_poly_mul");
}


MU_TEST(test_poly_mulmod_sparse) {
  GF2EX quotient;
  c1_tmp = b1 * a1;
  DivRem(quotient, c1, c1_tmp, sparse_modulo1);

  rbc_poly_mulmod_sparse(c2, a2, b2, sparse_modulo2);

  #if VERBOSE
    printf("\nc1:"); ntl_poly_print_uint64(c1);
    printf("\nc2:"); rbc_poly_print(c2);
    printf("\n");
  #endif

  mu_assert(rbc_poly_equal_to_ntl(c1, c2) == 1, "Failure: test_poly_mulmod_sparse");
}


MU_TEST(test_poly_inv) {
  InvMod(c1, a1, modulo1);

  rbc_poly_inv(c2, a2, modulo2);

  #if VERBOSE
    printf("\nc1:"); ntl_poly_print_uint64(c1);
    printf("\nc2:"); rbc_poly_print(c2);
    printf("\n");
  #endif

  mu_assert(rbc_poly_equal_to_ntl(c1, c2) == 1, "Failure: test_poly_inv");
}


/*
 * Test execution
 */
MU_TEST_SUITE(test_suite) {
  MU_SUITE_CONFIGURE(&test_setup, &test_teardown);
  MU_RUN_TEST(test_poly_add);
  MU_RUN_TEST(test_poly_mul);
  MU_RUN_TEST(test_poly_mulmod_sparse);
  MU_RUN_TEST(test_poly_inv);
  MU_SUITE_CLEAR();
}

int main() {
  for(int i = 0 ; i < INSTANCES ; ++i) {
    MU_RUN_SUITE(test_suite);
  }

  MU_REPORT();
  return minunit_fail;
}

