#include "blas_comm.h"
#include "blas.h"
#include "blas_matrix_ref.h"

#include <stdint.h>
#include <string.h>

void gf256mat_prod_ref(uint8_t *c, const uint8_t *matA, unsigned n_A_vec_byte, unsigned n_A_width, const uint8_t *b) {
    gf256v_set_zero(c, n_A_vec_byte);
    for (unsigned i = 0; i < n_A_width; i++) {
        gf256v_madd(c, matA, b[i], n_A_vec_byte);
        matA += n_A_vec_byte;
    }
}

void gf256mat_mul(uint8_t* c, const uint8_t* a, const uint8_t* b, unsigned len_vec) {
	unsigned n_vec_byte = len_vec;
	for (unsigned k = 0; k < len_vec; k++) {
		gf256v_set_zero(c, n_vec_byte);
		const uint8_t* bk = b + n_vec_byte * k;
		for (unsigned i = 0; i < len_vec; i++) {
			gf256v_madd(c, a + n_vec_byte * i, bk[i], n_vec_byte);
		}
		c += n_vec_byte;
	}
}

static
unsigned gf256mat_gauss_elim_ref( uint8_t * mat , unsigned h , unsigned w )
{
    unsigned r8 = 1;

    for(unsigned i=0;i<h;i++) {
        uint8_t * ai = mat + w*i;
        //unsigned i_start = i-(i&(_BLAS_UNIT_LEN_-1));
        unsigned i_start = i;

        for(unsigned j=i+1;j<h;j++) {
            uint8_t * aj = mat + w*j;
            gf256v_conditional_add( ai + i_start , !gf256_is_nonzero(ai[i]) , aj + i_start , w - i_start );
        }
        r8 &= gf256_is_nonzero(ai[i]);
        uint8_t pivot = ai[i];
        pivot = gf256_inv( pivot );
        gf256v_mul_scalar( ai + i_start  , pivot , w - i_start );
        for(unsigned j=0;j<h;j++) {
            if(i==j) continue;
            uint8_t * aj = mat + w*j;
            gf256v_madd( aj + i_start , ai+ i_start , aj[i] , w - i_start );
        }
    }

    return r8;
}

static inline
void gf256mat_submat(uint8_t* mat2, unsigned w2, unsigned st, const uint8_t* mat, unsigned w, unsigned h)
{
	for (unsigned i = 0; i < h; i++) {
		for (unsigned j = 0; j < w2; j++) mat2[i * w2 + j] = mat[i * w + st + j];
	}
}

unsigned gf256mat_inv_ref(uint8_t* inv_a, const uint8_t* a, unsigned h)
{
	const unsigned H = h;
	uint8_t mat[H * H * 2];
	// uint8_t* mat = (uint8_t*)malloc(H * H * 2);
	for (unsigned i = 0; i < H; i++) {
		uint8_t* ai = mat + i * 2 * H;
		gf256v_set_zero(ai, 2 * H);
		gf256v_add(ai, a + i * H, H);
		ai[H + i] = 1;
	}
	unsigned char r8 = gf256mat_gauss_elim_ref(mat, H, 2 * H);
	gf256mat_submat(inv_a, H, H, mat, 2 * H, H);
	gf256v_set_zero(mat, H * 2 * H);
	return r8;
}

unsigned gf256mat_solve_linear_CORE_ref(uint8_t* sol, const uint8_t* inp_mat, const uint8_t* c_terms, const unsigned h)
{
	unsigned vec_len = h + _BLAS_UNIT_LEN_;

	uint8_t mat[h * vec_len];
	gf256v_set_zero(mat, h * vec_len);
	for (unsigned i = 0; i < h; i++) {
		uint8_t* mi = mat + i * vec_len;
		for (unsigned j = 0; j < h; j++) mi[j] = inp_mat[j * h + i];
		mi[h] = c_terms[i];
	}
	unsigned r8 = gf256mat_gauss_elim_ref(mat, h, vec_len);
	for (unsigned i = 0; i < h; i++) sol[i] = mat[i * vec_len + h];
	gf256v_set_zero(mat, h * vec_len); // clean
	return r8;
}

unsigned gf256mat_solve_linear_eq_ref(uint8_t* output, const uint8_t* mat, const unsigned H, const uint8_t* vec, uint8_t depth) {
	unsigned rr = 1;
	uint8_t temp_mat[H * H];
	if (depth == 0) {
		return gf256mat_solve_linear_CORE_ref(output, mat, vec, H);
	}

	unsigned H_half = H >> 1;
	unsigned H_hh = H_half * H_half;

	uint8_t A[H_hh];
	uint8_t B[H_hh];
	uint8_t C[H_hh];
	uint8_t D[H_hh];
	uint8_t A_inv[H_hh];
	uint8_t CA_inv[H_hh];

	uint8_t temp_vec[H];
	uint8_t temp_vec2[H];

	for (unsigned i = 0; i < H_half; i++) {
		gf256v_set_zero(A + i * H_half, H_half);
		gf256v_set_zero(C + i * H_half, H_half);
		gf256v_add(A + i * H_half, mat + i * H, H_half);
		gf256v_add(C + i * H_half, mat + H_half + i * H, H_half);
	} // up to down & left to right
	for (unsigned i = 0; i < H_half; i++) {
		gf256v_set_zero(B + i * H_half, H_half);
		gf256v_set_zero(D + i * H_half, H_half);
		gf256v_add(B + i * H_half, mat + H_half * H + i * H, H_half);
		gf256v_add(D + i * H_half, mat + H_half * H + H_half + i * H, H_half);
	}
	rr &= gf256mat_inv_ref(A_inv, A, H_half);

	gf256mat_mul(CA_inv, C, A_inv, H_half);
	gf256mat_prod(temp_vec + H_half, CA_inv, H_half, H_half, vec);
	gf256v_set_zero(temp_vec, H_half);

	gf256v_add(temp_vec, vec, H);


	gf256mat_mul(temp_mat, CA_inv, B, H_half);
	gf256v_add(temp_mat, D, H_half * H_half);
	//rr &= gf256mat_inv(DCA_invB_inv , temp_mat , H_half,buffer);

	gf256mat_prod(temp_vec2, A_inv, H_half, H_half, temp_vec);

	rr &= gf256mat_solve_linear_eq_ref(temp_vec2 + H_half, temp_mat, H_half, temp_vec + H_half, depth - 1);
	//rr &=gf256mat_inv_avx2(DCA_invB_inv , temp_mat , H_half);
	//gf256mat_prod_avx2(temp_vec2+H_half,DCA_invB_inv,H_half,H_half,temp_vec+H_half);

	gf256mat_prod(temp_vec, B, H_half, H_half, temp_vec2 + H_half);

	gf256mat_prod(output, A_inv, H_half, H_half, temp_vec);

	gf256v_set_zero(output + H_half, H_half);
	gf256v_add(output, temp_vec2, H);

	return rr;
}