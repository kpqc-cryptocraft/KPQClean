#include "blas_comm.h"
#include "blas_matrix.h"
#include "blas.h"

#if defined( _BLAS_AVX2_ )

#include "blas_matrix_avx2.h"
#define gf256mat_prod_impl			gf256mat_prod_avx2
#define gf256mat_inv_impl			gf256mat_inv_avx2
#define gf256mat_solve_linear_impl	gf256mat_solve_linear_eq_avx2

#else

#include "blas_matrix_ref.h"
#define gf256mat_prod_impl            gf256mat_prod_ref
#define gf256mat_inv_impl			gf256mat_inv_ref
#define gf256mat_solve_linear_impl	gf256mat_solve_linear_eq_ref

#endif


void gf256mat_prod(uint8_t *c, const uint8_t *matA, unsigned n_A_vec_byte, unsigned n_A_width, const uint8_t *b)
{
    gf256mat_prod_impl( c, matA, n_A_vec_byte, n_A_width, b);
}

unsigned gf256mat_inv(uint8_t* inv_a, const uint8_t* mat_a, const unsigned h)
{
	return gf256mat_inv_impl(inv_a, mat_a, h);
}
unsigned gf256mat_solve_linear_eq(uint8_t* output, const uint8_t* mat, const unsigned H, const uint8_t* vec, uint8_t depth)
{
	return gf256mat_solve_linear_impl(output, mat, H, vec, depth);
}