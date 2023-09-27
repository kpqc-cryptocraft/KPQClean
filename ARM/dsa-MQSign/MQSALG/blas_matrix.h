#ifndef _BLAS_MATRIX_H_
#define _BLAS_MATRIX_H_

#include <stdint.h>

#ifdef  __cplusplus
extern  "C" {
#endif

void gf256mat_prod(uint8_t *c, const uint8_t *matA, unsigned n_A_vec_byte, unsigned n_A_width, const uint8_t *b);
unsigned gf256mat_inv(uint8_t* inv_a, const uint8_t* mat_a, const unsigned h);
unsigned gf256mat_solve_linear_eq(uint8_t* output, const uint8_t* mat, const unsigned H, const uint8_t* vec, uint8_t depth);

#ifdef  __cplusplus
}
#endif

#endif

