/*
 * Unit-tests against NTL
 */
#include <unistd.h>
#include <stdlib.h>
#include "minunit.h"
#include <NTL/GF2E.h>
#include <NTL/GF2X.h>
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

int INSTANCES = 50;

GF2E a1;
GF2E b1;
GF2E c1;
GF2X d1;
GF2E zero;
GF2E sparse;

rbc_elt a2;
rbc_elt b2;
rbc_elt c2;
rbc_elt_ur d2;
rbc_elt zero2;
rbc_elt sparse2;

uint8_t done = 0;



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

  random(a1);
  random(b1);
  clear(c1);
  random(d1, 2*RBC_FIELD_M-1);
  clear(zero);
  //Only one 1 to test is_zero
  clear(sparse);
  uint8_t pos;
  randombytes(&pos, 1);
  pos = pos % RBC_FIELD_M;
  SetCoeff(sparse._GF2E__rep, pos, 1);

  // RBC initialization
  rbc_field_init();
  rbc_elt_set_from_ntl(a2, rep(a1));
  rbc_elt_set_from_ntl(b2, rep(b1));
  rbc_elt_set_from_ntl(c2, rep(c1));
  rbc_elt_ur_set_from_ntl(d2, d1);
  rbc_elt_set_from_ntl(zero2, rep(zero));
  rbc_elt_set_from_ntl(sparse2, rep(sparse));

  #if VERBOSE
    printf("\na1: "); ntl_elt_print_uint64(rep(a1));
    printf("\na2: "); rbc_elt_print(a2);
    printf("\n");
    printf("\nb1: "); ntl_elt_print_uint64(rep(b1));
    printf("\nb2: "); rbc_elt_print(b2);
    printf("\n");
    printf("\nc1: "); ntl_elt_print_uint64(rep(c1));
    printf("\nc2: "); rbc_elt_print(c2);
    printf("\n");
    printf("\nd1: "); ntl_elt_ur_print_uint64(d1);
    printf("\nd2: "); rbc_elt_ur_print(d2);
    printf("\n");
    printf("\nzero1: "); ntl_elt_print_uint64(rep(zero));
    printf("\nzero2: "); rbc_elt_print(zero2);
    printf("\n");
    printf("\nsparse1: "); ntl_elt_print_uint64(rep(sparse));
    printf("\nsparse2: "); rbc_elt_print(sparse2);
    printf("\n");
  #endif
}

void test_teardown() {};

void MU_SUITE_CLEAR() {};



/*
 * Test functions
 */

MU_TEST(test_elt_is_zero) {
  mu_assert(rbc_elt_is_zero(zero2) && !rbc_elt_is_zero(sparse2), "Failure: test_elt_is_zero");
}


MU_TEST(test_elt_is_greater_than) {
  mu_assert((rbc_elt_is_greater_than(a2, b2) && ntl_is_greater_than(rep(a1), rep(b1))) || (!rbc_elt_is_greater_than(a2, b2) && !ntl_is_greater_than(rep(a1), rep(b1))), "Failure: test_elt_is_greater_than");
}

MU_TEST(test_elt_add) {
  c1 = b1 + a1;
  rbc_elt_add(c2, a2, b2);

  #if VERBOSE
    printf("\nc1: "); ntl_elt_print_uint64(rep(c1));
    printf("\nc2: "); rbc_elt_print(c2);
    printf("\n");
  #endif

  mu_assert(rbc_elt_equal_to_ntl(c1, c2) == 1, "Failure: test_elt_add");
}

MU_TEST(test_elt_mul) {
  c1 = a1 * b1;
  rbc_elt_mul(c2, a2, b2);

  #if VERBOSE
    printf("\nc1: "); ntl_elt_print_uint64(rep(c1));
    printf("\nc2: "); rbc_elt_print(c2);
    printf("\n");
  #endif

  mu_assert(rbc_elt_equal_to_ntl(c1, c2) == 1, "Failure: test_elt_mul");
}



MU_TEST(test_elt_sqr) {
  c1 = sqr(a1);
  rbc_elt_sqr(c2, a2);

  #if VERBOSE
    printf("\na1: "); ntl_elt_print_uint64(rep(a1));
    printf("\na2: "); rbc_elt_print(a2);
    printf("\n");
    printf("\nc1: "); ntl_elt_print_uint64(rep(c1));
    printf("\nc2: "); rbc_elt_print(c2);
    printf("\n");
  #endif

  mu_assert(rbc_elt_equal_to_ntl(c1, c2) == 1, "Failure: test_elt_sqr");
}



MU_TEST(test_elt_inv) {
  c1 = inv(a1);
  rbc_elt_inv(c2, a2);

  #if VERBOSE
    printf("\na1: "); ntl_elt_print_uint64(rep(a1));
    printf("\na2: "); rbc_elt_print(a2);
    printf("\n");
    printf("\nc1: "); ntl_elt_print_uint64(rep(c1));
    printf("\nc2: "); rbc_elt_print(c2);
    printf("\n");
  #endif

  mu_assert(rbc_elt_equal_to_ntl(c1, c2) == 1, "Failure: test_elt_inv");
}



MU_TEST(test_elt_red) {
  GF2X q, rem;
  DivRem(q, rem, d1, GF2E::modulus());
  c1._GF2E__rep = rem;
  rbc_elt_reduce(c2, d2);

  #if VERBOSE
    printf("\nd1: "); ntl_elt_ur_print_uint64(d1);
    printf("\nd2: "); rbc_elt_ur_print(d2);
    printf("\n");
    printf("\nc1: "); ntl_elt_print_uint64(rep(c1));
    printf("\nc2: "); rbc_elt_print(c2);
    printf("\n");
  #endif

  mu_assert(rbc_elt_equal_to_ntl(c1, c2) == 1, "Failure: test_elt_red");
}

/*
 * Test execution
 */
MU_TEST_SUITE(test_suite) {
  MU_SUITE_CONFIGURE(&test_setup, &test_teardown);
  MU_RUN_TEST(test_elt_is_zero);
  MU_RUN_TEST(test_elt_is_greater_than);
  MU_RUN_TEST(test_elt_add);
  MU_RUN_TEST(test_elt_mul);
  MU_RUN_TEST(test_elt_sqr);
  MU_RUN_TEST(test_elt_inv);
  MU_RUN_TEST(test_elt_red);
  MU_SUITE_CLEAR();
}

int main() {
  for(int i = 0 ; i < INSTANCES ; ++i) {
    MU_RUN_SUITE(test_suite);
  }

  MU_REPORT();
  return minunit_fail;
}

