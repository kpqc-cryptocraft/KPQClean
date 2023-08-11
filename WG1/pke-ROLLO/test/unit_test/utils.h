
#ifndef TEST_UTILS_H
#define TEST_UTILS_H

#include <NTL/GF2E.h>
#include <NTL/GF2X.h>
#include <NTL/GF2EX.h>

using namespace NTL;

void rbc_elt_set_from_ntl(rbc_elt o, const GF2X &target_value) {
  unsigned char tmp[RBC_ELT_UINT8_SIZE];
  BytesFromGF2X(tmp, target_value, RBC_ELT_UINT8_SIZE);
  memcpy(o, tmp, RBC_ELT_UINT8_SIZE);
}

void rbc_elt_ur_set_from_ntl(rbc_elt_ur o, const GF2X &target_value) {
  unsigned char tmp[RBC_ELT_UR_UINT8_SIZE];
  BytesFromGF2X(tmp, target_value, RBC_ELT_UR_UINT8_SIZE);
  memcpy(o, tmp, RBC_ELT_UR_UINT8_SIZE);
}

void ntl_elt_print_uint64(const GF2X &e) {
  unsigned char tmp[RBC_ELT_UINT8_SIZE] = {0};
  BytesFromGF2X(tmp, e, RBC_ELT_UINT8_SIZE);
  rbc_elt_print((rbc_elt_ptr) tmp);
}

void ntl_elt_ur_print_uint64(const GF2X &e) {
  unsigned char tmp[RBC_ELT_UR_UINT8_SIZE] = {0};
  BytesFromGF2X(tmp, e, RBC_ELT_UR_UINT8_SIZE);
  rbc_elt_ur_print((rbc_elt_ptr) tmp);
}

int rbc_elt_equal_to_ntl(const GF2E &expected, rbc_elt result) {
  for(int i = 0 ; i < RBC_FIELD_M ; i++) {
  	if(deg(rep(expected)) < i && rbc_elt_get_coefficient_vartime(result, i) == 0) continue;
    if(rep(expected)[i] != rbc_elt_get_coefficient_vartime(result, i)) {
       printf("%d\n", i);
       std::cout << expected << std::endl;
       return 0;
    }
  }

  return 1;
}

int ntl_is_greater_than(const GF2X &a, const GF2X &b) {
	for(int i = RBC_FIELD_M - 1 ; i >= 0; i--) {
		if(a[i] == 1 && b[i] == 0) return 1;
		if(b[i] == 1 && a[i] == 0) return 0;
	}

	return 0;
}

void rbc_poly_set_ntl(rbc_poly o, const GF2EX &e, int degree) {
	for(int i=0 ; i<=degree ; i++) {
		if(deg(e)<i) rbc_elt_set_zero(o->v[i]);
		else rbc_elt_set_from_ntl(o->v[i], rep(e[i]));
	}
}

void ntl_poly_print_uint64(const GF2EX &e) {
	for(int i=0 ; i<=deg(e) ; i++) {
		if (i < 10) printf("\n%i   - ", i);
    	else if (i < 100) printf("\n%i  - ", i);
    	else printf("\n%i - ", i);

    	ntl_elt_print_uint64(rep(e[i]));
	}
}

int rbc_poly_equal_to_ntl(const GF2EX &expected, rbc_poly result) {
	if(deg(expected) > result->max_degree) return 0;

	for(int i=0 ; i<=deg(expected) ; i++) {
		if(!rbc_elt_equal_to_ntl(expected[i], result->v[i])) return 0;
	}

	for(int i=deg(expected) + 1 ; i<= result->max_degree ; i++) {
		if(!rbc_elt_is_zero(result->v[i])) return 0;
	}

	return 1;
}

#endif
