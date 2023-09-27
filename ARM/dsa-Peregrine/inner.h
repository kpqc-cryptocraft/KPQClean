#ifndef PEREGRINE_INNER_H__
#define PEREGRINE_INNER_H__

/*
 * Peregrine Related Functions. Many functions are adopted from FALCON Implementation
 * and some new functions are implemented for Peregrine operations. 
 *
 * ==========================(LICENSE BEGIN)============================
 *
 * Copyright (c) 2017-2019  Falcon Project
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * ===========================(LICENSE END)=============================
 *
 * @author   Thomas Pornin <thomas.pornin@nccgroup.com>
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/*
 * "Naming" macro used to apply a consistent prefix over all global
 * symbols.
 */

#define ALGO_PREFIX   algo_inner
#define Zf(name)             Zf_(ALGO_PREFIX, name)
#define Zf_(prefix, name)    Zf__(prefix, name)
#define Zf__(prefix, name)   prefix ## _ ## name  

static inline unsigned
set_fpu_cw(unsigned x)
{
	return x;
}
#pragma STDC FP_CONTRACT OFF

/* ==================================================================== */
/*
 * SHAKE256 implementation (shake.c).
 *
 * API is defined to be easily replaced with the fips202.h API defined
 * as part of PQClean.
 */

typedef struct {
	union {
		uint64_t A[25];
		uint8_t dbuf[200];
	} st;
	uint64_t dptr;
} inner_shake256_context;

#define inner_shake256_init      Zf(i_shake256_init)
#define inner_shake256_inject    Zf(i_shake256_inject)
#define inner_shake256_flip      Zf(i_shake256_flip)
#define inner_shake256_extract   Zf(i_shake256_extract)

void Zf(i_shake256_init)(
	inner_shake256_context *sc);
void Zf(i_shake256_inject)(
	inner_shake256_context *sc, const uint8_t *in, size_t len);
void Zf(i_shake256_flip)(
	inner_shake256_context *sc);
void Zf(i_shake256_extract)(
	inner_shake256_context *sc, uint8_t *out, size_t len);

// EYSEO moved this part from keygen.c---------------------

#define RNG_CONTEXT   inner_shake256_context

/*
 * Get a random 8-byte integer from a SHAKE-based RNG. This function
 * ensures consistent interpretation of the SHAKE output so that
 * the same values will be obtained over different platforms, in case
 * a known seed is used.
 */
static inline uint64_t
get_rng_u64(inner_shake256_context *rng)
{
    uint64_t r;
    inner_shake256_extract(rng, (uint8_t *)&r, sizeof r);
    return r;
}


size_t Zf(modq_encode)(void *out, size_t max_out_len,
	const uint16_t *x, unsigned logn);
size_t Zf(trim_i16_encode)(void *out, size_t max_out_len,
	const int16_t *x, unsigned logn, unsigned bits);
size_t Zf(trim_i8_encode)(void *out, size_t max_out_len,
	const int8_t *x, unsigned logn, unsigned bits);
size_t Zf(comp_encode)(void *out, size_t max_out_len,
	const int16_t *x, unsigned logn);

size_t Zf(modq_decode)(uint16_t *x, unsigned logn,
	const void *in, size_t max_in_len);
size_t Zf(trim_i16_decode)(int16_t *x, unsigned logn, unsigned bits,
	const void *in, size_t max_in_len);
size_t Zf(trim_i8_decode)(int8_t *x, unsigned logn, unsigned bits,
	const void *in, size_t max_in_len);
size_t Zf(comp_decode)(int16_t *x, unsigned logn,
	const void *in, size_t max_in_len);

/*
 * Number of bits for key elements, indexed by logn (1 to 10). This
 * is at most 8 bits for all degrees, but some degrees may have shorter
 * elements.
 */
extern const uint8_t Zf(max_fg_bits)[];
extern const uint8_t Zf(max_FG_bits)[];

/*
 * Maximum size, in bits, of elements in a signature, indexed by logn
 * (1 to 10). The size includes the sign bit.
 */
extern const uint8_t Zf(max_sig_bits)[];

/* ==================================================================== */
/*
 * Support functions used for both signature generation and signature
 * verification (common.c).
 */

/*
 * From a SHAKE256 context (must be already flipped), produce a new
 * point. This is the non-constant-time version, which may leak enough
 * information to serve as a stop condition on a brute force attack on
 * the hashed message (provided that the nonce value is known).
 */
void Zf(hash_to_point_vartime)(inner_shake256_context *sc,
	uint16_t *x, unsigned logn);

/*
 * From a SHAKE256 context (must be already flipped), produce a new
 * point. The temporary buffer (tmp) must have room for 2*2^logn bytes.
 * This function is constant-time but is typically more expensive than
 * Zf(hash_to_point_vartime)().
 *
 * tmp[] must have 16-bit alignment.
 */
void Zf(hash_to_point_ct)(inner_shake256_context *sc,
	uint16_t *x, unsigned logn, uint8_t *tmp);

/*
 * Tell whether a given vector (2N coordinates, in two halves) is
 * acceptable as a signature. This compares the appropriate norm of the
 * vector with the acceptance bound. Returned value is 1 on success
 * (vector is short enough to be acceptable), 0 otherwise.
 */
int Zf(is_short)(const int16_t *s1, const int16_t *s2, unsigned logn);

/*
 * Tell whether a given vector (2N coordinates, in two halves) is
 * acceptable as a signature. Instead of the first half s1, this
 * function receives the "saturated squared norm" of s1, i.e. the
 * sum of the squares of the coordinates of s1 (saturated at 2^32-1
 * if the sum exceeds 2^31-1).
 *
 * Returned value is 1 on success (vector is short enough to be
 * acceptable), 0 otherwise.
 */
int Zf(is_short_half)(uint32_t sqn, const int16_t *s2, unsigned logn);

/* ==================================================================== */
/*
 * Signature verification functions (vrfy.c).
 */

/*
 * Convert a public key to NTT + Montgomery format. Conversion is done
 * in place.
 */
void Zf(to_ntt_monty)(uint16_t *h, unsigned logn);

/*
 * Internal signature verification code:
 *   c0[]      contains the hashed nonce+message
 *   s2[]      is the decoded signature
 *   h[]       contains the public key, in NTT + Montgomery format
 *   logn      is the degree log
 *   tmp[]     temporary, must have at least 2*2^logn bytes
 * Returned value is 1 on success, 0 on error.
 *
 * tmp[] must have 16-bit alignment.
 */
int Zf(verify_raw)(const uint16_t *c0, const int16_t *s2,
	const uint16_t *h, unsigned logn, uint8_t *tmp);

/*
 * Compute the public key h[], given the private key elements f[] and
 * g[]. This computes h = g/f mod phi mod q, where phi is the polynomial
 * modulus. This function returns 1 on success, 0 on error (an error is
 * reported if f is not invertible mod phi mod q).
 *
 * The tmp[] array must have room for at least 2*2^logn elements.
 * tmp[] must have 16-bit alignment.
 */
int Zf(compute_public)(uint16_t *h,
	const int8_t *f, const int8_t *g, unsigned logn, uint8_t *tmp);

/*
 * Recompute the fourth private key element. Private key consists in
 * four polynomials with small coefficients f, g, F and G, which are
 * such that fG - gF = q mod phi; furthermore, f is invertible modulo
 * phi and modulo q. This function recomputes G from f, g and F.
 *
 * The tmp[] array must have room for at least 4*2^logn bytes.
 *
 * Returned value is 1 in success, 0 on error (f not invertible).
 * tmp[] must have 16-bit alignment.
 */
int Zf(complete_private)(int8_t *G,
	const int8_t *f, const int8_t *g, const int8_t *F,
	unsigned logn, uint8_t *tmp);

/*
 * Test whether a given polynomial is invertible modulo phi and q.
 * Polynomial coefficients are small integers.
 *
 * tmp[] must have 16-bit alignment.
 */
int Zf(is_invertible)(
	const int16_t *s2, unsigned logn, uint8_t *tmp);

/*
 * Count the number of elements of value zero in the NTT representation
 * of the given polynomial: this is the number of primitive 2n-th roots
 * of unity (modulo q = 12289) that are roots of the provided polynomial
 * (taken modulo q).
 *
 * tmp[] must have 16-bit alignment.
 */
int Zf(count_nttzero)(const int16_t *sig, unsigned logn, uint8_t *tmp);

/*
 * Internal signature verification with public key recovery:
 *   h[]       receives the public key (NOT in NTT/Montgomery format)
 *   c0[]      contains the hashed nonce+message
 *   s1[]      is the first signature half
 *   s2[]      is the second signature half
 *   logn      is the degree log
 *   tmp[]     temporary, must have at least 2*2^logn bytes
 * Returned value is 1 on success, 0 on error. Success is returned if
 * the signature is a short enough vector; in that case, the public
 * key has been written to h[]. However, the caller must still
 * verify that h[] is the correct value (e.g. with regards to a known
 * hash of the public key).
 *
 * h[] may not overlap with any of the other arrays.
 *
 * tmp[] must have 16-bit alignment.
 */
int Zf(verify_recover)(uint16_t *h,
	const uint16_t *c0, const int16_t *s1, const int16_t *s2,
	unsigned logn, uint8_t *tmp);


/* ==================================================================== */
/*
 * Implementation of floating-point real numbers (fpr.h, fpr.c).
 */

/*
 * Real numbers are implemented by an extra header file, included below.
 * This is meant to support pluggable implementations. The default
 * implementation relies on the C type 'double'.
 *
 * The included file must define the following types, functions and
 * constants:
 *
 *   fpr
 *         type for a real number
 *
 *   fpr fpr_of(int64_t i)
 *         cast an integer into a real number; source must be in the
 *         -(2^63-1)..+(2^63-1) range
 *
 *   fpr fpr_scaled(int64_t i, int sc)
 *         compute i*2^sc as a real number; source 'i' must be in the
 *         -(2^63-1)..+(2^63-1) range
 *
 *   fpr fpr_ldexp(fpr x, int e)
 *         compute x*2^e
 *
 *   int64_t fpr_rint(fpr x)
 *         round x to the nearest integer; x must be in the -(2^63-1)
 *         to +(2^63-1) range
 *
 *   int64_t fpr_trunc(fpr x)
 *         round to an integer; this rounds towards zero; value must
 *         be in the -(2^63-1) to +(2^63-1) range
 *
 *   fpr fpr_add(fpr x, fpr y)
 *         compute x + y
 *
 *   fpr fpr_sub(fpr x, fpr y)
 *         compute x - y
 *
 *   fpr fpr_neg(fpr x)
 *         compute -x
 *
 *   fpr fpr_half(fpr x)
 *         compute x/2
 *
 *   fpr fpr_double(fpr x)
 *         compute x*2
 *
 *   fpr fpr_mul(fpr x, fpr y)
 *         compute x * y
 *
 *   fpr fpr_sqr(fpr x)
 *         compute x * x
 *
 *   fpr fpr_inv(fpr x)
 *         compute 1/x
 *
 *   fpr fpr_div(fpr x, fpr y)
 *         compute x/y
 *
 *   fpr fpr_sqrt(fpr x)
 *         compute the square root of x
 *
 *   int fpr_lt(fpr x, fpr y)
 *         return 1 if x < y, 0 otherwise
 *
 *   uint64_t fpr_expm_p63(fpr x)
 *         return exp(x), assuming that 0 <= x < log(2). Returned value
 *         is scaled to 63 bits (i.e. it really returns 2^63*exp(-x),
 *         rounded to the nearest integer). Computation should have a
 *         precision of at least 45 bits.
 *
 *   const fpr fpr_gm_tab[]
 *         array of constants for FFT / iFFT
 *
 *   const fpr fpr_p2_tab[]
 *         precomputed powers of 2 (by index, 0 to 10)
 *
 * Constants of type 'fpr':
 *
 *   fpr fpr_q                 12289
 *   fpr fpr_inverse_of_q      1/12289
 *   fpr fpr_inv_2sqrsigma0    1/(2*(1.8205^2))
 *   fpr fpr_inv_sigma[]       1/sigma (indexed by logn, 1 to 10)
 *   fpr fpr_sigma_min[]       1/sigma_min (indexed by logn, 1 to 10)
 *   fpr fpr_log2              log(2)
 *   fpr fpr_inv_log2          1/log(2)
 *   fpr fpr_bnorm_max         16822.4121
 *   fpr fpr_zero              0
 *   fpr fpr_one               1
 *   fpr fpr_two               2
 *   fpr fpr_onehalf           0.5
 *   fpr fpr_ptwo31            2^31
 *   fpr fpr_ptwo31m1          2^31-1
 *   fpr fpr_mtwo31m1          -(2^31-1)
 *   fpr fpr_ptwo63m1          2^63-1
 *   fpr fpr_mtwo63m1          -(2^63-1)
 *   fpr fpr_ptwo63            2^63
 */
#include "fpr.h"

/* ==================================================================== */
/*
 * RNG (rng.c).
 *
 * A PRNG based on ChaCha20 is implemented; it is seeded from a SHAKE256
 * context (flipped) and is used for bulk pseudorandom generation.
 * A system-dependent seed generator is also provided.
 */

/*
 * Obtain a random seed from the system RNG.
 *
 * Returned value is 1 on success, 0 on error.
 */
int Zf(get_seed)(void *seed, size_t seed_len);

/*
 * Structure for a PRNG. This includes a large buffer so that values
 * get generated in advance. The 'state' is used to keep the current
 * PRNG algorithm state (contents depend on the selected algorithm).
 *
 * The unions with 'dummy_u64' are there to ensure proper alignment for
 * 64-bit direct access.
 */
typedef struct {
	union {
		uint8_t d[512]; /* MUST be 512, exactly */
		uint64_t dummy_u64;
	} buf;
	size_t ptr;
	union {
		uint8_t d[256];
		uint64_t dummy_u64;
	} state;
	int type;
} prng;

/*
 * Instantiate a PRNG. That PRNG will feed over the provided SHAKE256
 * context (in "flipped" state) to obtain its initial state.
 */
void Zf(prng_init)(prng *p, inner_shake256_context *src);

/*
 * Refill the PRNG buffer. This is normally invoked automatically, and
 * is declared here only so that prng_get_u64() may be inlined.
 */
void Zf(prng_refill)(prng *p);

/*
 * Get some bytes from a PRNG.
 */
void Zf(prng_get_bytes)(prng *p, void *dst, size_t len);

/*
 * Get a 64-bit random value from a PRNG.
 */
static inline uint64_t
prng_get_u64(prng *p)
{
	size_t u;

	/*
	 * If there are less than 9 bytes in the buffer, we refill it.
	 * This means that we may drop the last few bytes, but this allows
	 * for faster extraction code. Also, it means that we never leave
	 * an empty buffer.
	 */
	u = p->ptr;
	if (u >= (sizeof p->buf.d) - 9) {
		Zf(prng_refill)(p);
		u = 0;
	}
	p->ptr = u + 8;

	/*
	 * On systems that use little-endian encoding and allow
	 * unaligned accesses, we can simply read the data where it is.
	 */
	return (uint64_t)p->buf.d[u + 0]
		| ((uint64_t)p->buf.d[u + 1] << 8)
		| ((uint64_t)p->buf.d[u + 2] << 16)
		| ((uint64_t)p->buf.d[u + 3] << 24)
		| ((uint64_t)p->buf.d[u + 4] << 32)
		| ((uint64_t)p->buf.d[u + 5] << 40)
		| ((uint64_t)p->buf.d[u + 6] << 48)
		| ((uint64_t)p->buf.d[u + 7] << 56);
}

/*
 * Get an 8-bit random value from a PRNG.
 */
static inline unsigned
prng_get_u8(prng *p)
{
	unsigned v;

	v = p->buf.d[p->ptr ++];
	if (p->ptr == sizeof p->buf.d) {
		Zf(prng_refill)(p);
	}
	return v;
}

/* ==================================================================== */
/*
 * FFT (falcon-fft.c).
 *
 * A real polynomial is represented as an array of N 'fpr' elements.
 * The FFT representation of a real polynomial contains N/2 complex
 * elements; each is stored as two real numbers, for the real and
 * imaginary parts, respectively. See falcon-fft.c for details on the
 * internal representation.
 */

/*
 * Compute FFT in-place: the source array should contain a real
 * polynomial (N coefficients); its storage area is reused to store
 * the FFT representation of that polynomial (N/2 complex numbers).
 *
 * 'logn' MUST lie between 1 and 10 (inclusive).
 */
void Zf(FFT)(fpr *f, unsigned logn);

/*
 * Compute the inverse FFT in-place: the source array should contain the
 * FFT representation of a real polynomial (N/2 elements); the resulting
 * real polynomial (N coefficients of type 'fpr') is written over the
 * array.
 *
 * 'logn' MUST lie between 1 and 10 (inclusive).
 */
void Zf(iFFT)(fpr *f, unsigned logn);

/*
 * Add polynomial b to polynomial a. a and b MUST NOT overlap. This
 * function works in both normal and FFT representations.
 */
void Zf(poly_add)(fpr *restrict a, const fpr *restrict b, unsigned logn);

/*
 * Subtract polynomial b from polynomial a. a and b MUST NOT overlap. This
 * function works in both normal and FFT representations.
 */
void Zf(poly_sub)(fpr *restrict a, const fpr *restrict b, unsigned logn);

/*
 * Negate polynomial a. This function works in both normal and FFT
 * representations.
 */
void Zf(poly_neg)(fpr *a, unsigned logn);

/*
 * Compute adjoint of polynomial a. This function works only in FFT
 * representation.
 */
void Zf(poly_adj_fft)(fpr *a, unsigned logn);

/*
 * Multiply polynomial a with polynomial b. a and b MUST NOT overlap.
 * This function works only in FFT representation.
 */
void Zf(poly_mul_fft)(fpr *restrict a, const fpr *restrict b, unsigned logn);

/*
 * Multiply polynomial a with the adjoint of polynomial b. a and b MUST NOT
 * overlap. This function works only in FFT representation.
 */
void Zf(poly_muladj_fft)(fpr *restrict a, const fpr *restrict b, unsigned logn);

/*
 * Multiply polynomial with its own adjoint. This function works only in FFT
 * representation.
 */
void Zf(poly_mulselfadj_fft)(fpr *a, unsigned logn);

/*
 * Multiply polynomial with a real constant. This function works in both
 * normal and FFT representations.
 */
void Zf(poly_mulconst)(fpr *a, fpr x, unsigned logn);

/*
 * Divide polynomial a by polynomial b, modulo X^N+1 (FFT representation).
 * a and b MUST NOT overlap.
 */
void Zf(poly_div_fft)(fpr *restrict a, const fpr *restrict b, unsigned logn);

/*
 * Given f and g (in FFT representation), compute 1/(f*adj(f)+g*adj(g))
 * (also in FFT representation). Since the result is auto-adjoint, all its
 * coordinates in FFT representation are real; as such, only the first N/2
 * values of d[] are filled (the imaginary parts are skipped).
 *
 * Array d MUST NOT overlap with either a or b.
 */
void Zf(poly_invnorm2_fft)(fpr *restrict d,
	const fpr *restrict a, const fpr *restrict b, unsigned logn);

/*
 * Given F, G, f and g (in FFT representation), compute F*adj(f)+G*adj(g)
 * (also in FFT representation). Destination d MUST NOT overlap with
 * any of the source arrays.
 */
void Zf(poly_add_muladj_fft)(fpr *restrict d,
	const fpr *restrict F, const fpr *restrict G,
	const fpr *restrict f, const fpr *restrict g, unsigned logn);

/*
 * Multiply polynomial a by polynomial b, where b is autoadjoint. Both
 * a and b are in FFT representation. Since b is autoadjoint, all its
 * FFT coefficients are real, and the array b contains only N/2 elements.
 * a and b MUST NOT overlap.
 */
void Zf(poly_mul_autoadj_fft)(fpr *restrict a,
	const fpr *restrict b, unsigned logn);

/*
 * Divide polynomial a by polynomial b, where b is autoadjoint. Both
 * a and b are in FFT representation. Since b is autoadjoint, all its
 * FFT coefficients are real, and the array b contains only N/2 elements.
 * a and b MUST NOT overlap.
 */
void Zf(poly_div_autoadj_fft)(fpr *restrict a,
	const fpr *restrict b, unsigned logn);

/*
 * Perform an LDL decomposition of an auto-adjoint matrix G, in FFT
 * representation. On input, g00, g01 and g11 are provided (where the
 * matrix G = [[g00, g01], [adj(g01), g11]]). On output, the d00, l10
 * and d11 values are written in g00, g01 and g11, respectively
 * (with D = [[d00, 0], [0, d11]] and L = [[1, 0], [l10, 1]]).
 * (In fact, d00 = g00, so the g00 operand is left unmodified.)
 */
void Zf(poly_LDL_fft)(const fpr *restrict g00,
	fpr *restrict g01, fpr *restrict g11, unsigned logn);

/*
 * Perform an LDL decomposition of an auto-adjoint matrix G, in FFT
 * representation. This is identical to poly_LDL_fft() except that
 * g00, g01 and g11 are unmodified; the outputs d11 and l10 are written
 * in two other separate buffers provided as extra parameters.
 */
void Zf(poly_LDLmv_fft)(fpr *restrict d11, fpr *restrict l10,
	const fpr *restrict g00, const fpr *restrict g01,
	const fpr *restrict g11, unsigned logn);

/*
 * Apply "split" operation on a polynomial in FFT representation:
 * f = f0(x^2) + x*f1(x^2), for half-size polynomials f0 and f1
 * (polynomials modulo X^(N/2)+1). f0, f1 and f MUST NOT overlap.
 */
void Zf(poly_split_fft)(fpr *restrict f0, fpr *restrict f1,
	const fpr *restrict f, unsigned logn);

/*
 * Apply "merge" operation on two polynomials in FFT representation:
 * given f0 and f1, polynomials moduo X^(N/2)+1, this function computes
 * f = f0(x^2) + x*f1(x^2), in FFT representation modulo X^N+1.
 * f MUST NOT overlap with either f0 or f1.
 */
void Zf(poly_merge_fft)(fpr *restrict f,
	const fpr *restrict f0, const fpr *restrict f1, unsigned logn);

/* ==================================================================== */
/*
 * Key pair generation.
 */
/*
 * Generate a new key pair. Randomness is extracted from the provided
 * SHAKE256 context, which must have already been seeded and flipped.
 * The tmp[] array must have suitable size (see FALCON_KEYGEN_TEMP_*
 * macros) and be aligned for the uint32_t, uint64_t and fpr types.
 *
 * The private key elements are written in f, g, F and G, and the
 * public key is written in h. Either or both of G and h may be NULL,
 * in which case the corresponding element is not returned (they can
 * be recomputed from f, g and F).
 *
 * tmp[] must have 64-bit alignment.
 * This function uses floating-point rounding (see set_fpu_cw()).
 */
void Zf(keygen)(inner_shake256_context *rng,
	int8_t *f, int8_t *g, int8_t *F, int8_t *G, uint16_t *h,
	unsigned logn, uint8_t *tmp);
//EYSEO
/*void Zf(NTT_RNS)(inner_shake256_context *rng,
    int8_t *f, unsigned logn);*/
//EYSEO
void Zf(CBD)(inner_shake256_context *rng, int8_t *f, unsigned logn);
//EYSEO
void Zf(ModDown)(const uint16_t *input1, const int8_t *restrict input2, int16_t *ouput, unsigned logn, int16_t *tmp);
/* ==================================================================== */
/*
 * Signature generation.
 */

/*
 * Expand a private key into the B0 matrix in FFT representation and
 * the LDL tree. All the values are written in 'expanded_key', for
 * a total of (8*logn+40)*2^logn bytes.
 *
 * The tmp[] array must have room for at least 48*2^logn bytes.
 *
 * tmp[] must have 64-bit alignment.
 * This function uses floating-point rounding (see set_fpu_cw()).
 */
/*void Zf(expand_privkey)(fpr *restrict expanded_key,
	const int8_t *f, const int8_t *g, const int8_t *F, const int8_t *G,
	unsigned logn, uint8_t *restrict tmp);*/

/*
 * Compute a signature over the provided hashed message (hm); the
 * signature value is one short vector. This function uses an
 * expanded key (as generated by Zf(expand_privkey)()).
 *
 * The sig[] and hm[] buffers may overlap.
 *
 * On successful output, the start of the tmp[] buffer contains the s1
 * vector (as int16_t elements).
 *
 * The minimal size (in bytes) of tmp[] is 48*2^logn bytes.
 *
 * tmp[] must have 64-bit alignment.
 * This function uses floating-point rounding (see set_fpu_cw()).
 */
/*void Zf(sign_tree)(int16_t *sig, inner_shake256_context *rng,
	const fpr *restrict expanded_key,
	const uint16_t *hm, unsigned logn, uint8_t *tmp);*/

/*
 * Compute a signature over the provided hashed message (hm); the
 * signature value is one short vector. This function uses a raw
 * key and dynamically recompute the B0 matrix and LDL tree; this
 * saves RAM since there is no needed for an expanded key, but
 * increases the signature cost.
 *
 * The sig[] and hm[] buffers may overlap.
 *
 * On successful output, the start of the tmp[] buffer contains the s1
 * vector (as int16_t elements).
 *
 * The minimal size (in bytes) of tmp[] is 72*2^logn bytes.
 *
 * tmp[] must have 64-bit alignment.
 * This function uses floating-point rounding (see set_fpu_cw()).
 */
/*void Zf(sign_dyn)(int16_t *sig, inner_shake256_context *rng,
	const int8_t *restrict f, const int8_t *restrict g,
	const int8_t *restrict F, const int8_t *restrict G,
	const uint16_t *hm, unsigned logn, uint8_t *tmp);*/
void Zf(sign_dyn_peregrine)(int16_t *sig, inner_shake256_context *rng,
    const int8_t *restrict f, const int8_t *restrict g,
    const int8_t *restrict F, const int8_t *restrict G,
    const uint16_t *hm, const uint16_t *h_src, unsigned logn, uint8_t *tmp);//EYSEO

typedef struct {
    uint32_t p;
    uint32_t g;
    uint32_t s;
} small_prime;

static const small_prime PRIMES[] = {
    { 2147473409,  383167813,      10239 },//10239는 2^31 mod 2147473409
    { 2147389441,  211808905,  471403745 },
    { 2147387393,   37672282, 1329335065 },
    { 2147377153, 1977035326,  968223422 },
    { 2147358721, 1067163706,  132460015 },
    { 2147352577, 1606082042,  598693809 },
    { 2147346433, 2033915641, 1056257184 },
    { 2147338241, 1653770625,  421286710 },
    { 2147309569,  631200819, 1111201074 },
    { 2147297281, 2038364663, 1042003613 },
    { 2147295233, 1962540515,   19440033 },
    { 2147239937, 2100082663,  353296760 },
    { 2147235841, 1991153006, 1703918027 },
    { 2147217409,  516405114, 1258919613 },
    { 2147205121,  409347988, 1089726929 },
    { 2147196929,  927788991, 1946238668 },
    { 2147178497, 1136922411, 1347028164 },
    { 2147100673,  868626236,  701164723 },
    { 2147082241, 1897279176,  617820870 },
    { 2147074049, 1888819123,  158382189 },
    { 2147051521,   25006327,  522758543 },
    { 2147043329,  327546255,   37227845 },
    { 2147039233,  766324424, 1133356428 },
    { 2146988033, 1862817362,   73861329 },
    { 2146963457,  404622040,  653019435 },
    { 2146959361, 1936581214,  995143093 },
    { 2146938881, 1559770096,  634921513 },
    { 2146908161,  422623708, 1985060172 },
    { 2146885633, 1751189170,  298238186 },
    { 2146871297,  578919515,  291810829 },
    { 2146846721, 1114060353,  915902322 },
    { 2146834433, 2069565474,   47859524 },
    { 2146818049, 1552824584,  646281055 },
    { 2146775041, 1906267847, 1597832891 },
    { 2146756609, 1847414714, 1228090888 },
    { 2146744321, 1818792070, 1176377637 },
    { 2146738177, 1118066398, 1054971214 },
    { 2146736129,   52057278,  933422153 },
    { 2146713601,  592259376, 1406621510 },
    { 2146695169,  263161877, 1514178701 },
    { 2146656257,  685363115,  384505091 },
    { 2146650113,  927727032,  537575289 },
    { 2146646017,   52575506, 1799464037 },
    { 2146643969, 1276803876, 1348954416 },
    { 2146603009,  814028633, 1521547704 },
    { 2146572289, 1846678872, 1310832121 },
    { 2146547713,  919368090, 1019041349 },
    { 2146508801,  671847612,   38582496 },
    { 2146492417,  283911680,  532424562 },
    { 2146490369, 1780044827,  896447978 },
    { 2146459649,  327980850, 1327906900 },
    { 2146447361, 1310561493,  958645253 },
    { 2146441217,  412148926,  287271128 },
    { 2146437121,  293186449, 2009822534 },
    { 2146430977,  179034356, 1359155584 },
    { 2146418689, 1517345488, 1790248672 },
    { 2146406401, 1615820390, 1584833571 },
    { 2146404353,  826651445,  607120498 },
    { 2146379777,    3816988, 1897049071 },
    { 2146363393, 1221409784, 1986921567 },
    { 2146355201, 1388081168,  849968120 },
    { 2146336769, 1803473237, 1655544036 },
    { 2146312193, 1023484977,  273671831 },
    { 2146293761, 1074591448,  467406983 },
    { 2146283521,  831604668, 1523950494 },
    { 2146203649,  712865423, 1170834574 },
    { 2146154497, 1764991362, 1064856763 },
    { 2146142209,  627386213, 1406840151 },
    { 2146127873, 1638674429, 2088393537 },
    { 2146099201, 1516001018,  690673370 },
    { 2146093057, 1294931393,  315136610 },
    { 2146091009, 1942399533,  973539425 },
    { 2146078721, 1843461814, 2132275436 },
    { 2146060289, 1098740778,  360423481 },
    { 2146048001, 1617213232, 1951981294 },
    { 2146041857, 1805783169, 2075683489 },
    { 2146019329,  272027909, 1753219918 },
    { 2145986561, 1206530344, 2034028118 },
    { 2145976321, 1243769360, 1173377644 },
    { 2145964033,  887200839, 1281344586 },
    { 2145906689, 1651026455,  906178216 },
    { 2145875969, 1673238256, 1043521212 },
    { 2145871873, 1226591210, 1399796492 },
    { 2145841153, 1465353397, 1324527802 },
    { 2145832961, 1150638905,  554084759 },
    { 2145816577,  221601706,  427340863 },
    { 2145785857,  608896761,  316590738 },
    { 2145755137, 1712054942, 1684294304 },
    { 2145742849, 1302302867,  724873116 },
    { 2145728513,  516717693,  431671476 },
    { 2145699841,  524575579, 1619722537 },
    { 2145691649, 1925625239,  982974435 },
    { 2145687553,  463795662, 1293154300 },
    { 2145673217,  771716636,  881778029 },
    { 2145630209, 1509556977,  837364988 },
    { 2145595393,  229091856,  851648427 },
    { 2145587201, 1796903241,  635342424 },
    { 2145525761,  715310882, 1677228081 },
    { 2145495041, 1040930522,  200685896 },
    { 2145466369,  949804237, 1809146322 },
    { 2145445889, 1673903706,   95316881 },
    { 2145390593,  806941852, 1428671135 },
    { 2145372161, 1402525292,  159350694 },
    { 2145361921, 2124760298, 1589134749 },
    { 2145359873, 1217503067, 1561543010 },
    { 2145355777,  338341402,   83865711 },
    { 2145343489, 1381532164,  641430002 },
    { 2145325057, 1883895478, 1528469895 },
    { 2145318913, 1335370424,   65809740 },
    { 2145312769, 2000008042, 1919775760 },
    { 2145300481,  961450962, 1229540578 },
    { 2145282049,  910466767, 1964062701 },
    { 2145232897,  816527501,  450152063 },
    { 2145218561, 1435128058, 1794509700 },
    { 2145187841,   33505311, 1272467582 },
    { 2145181697,  269767433, 1380363849 },
    { 2145175553,   56386299, 1316870546 },
    { 2145079297, 2106880293, 1391797340 },
    { 2145021953, 1347906152,  720510798 },
    { 2145015809,  206769262, 1651459955 },
    { 2145003521, 1885513236, 1393381284 },
    { 2144960513, 1810381315,   31937275 },
    { 2144944129, 1306487838, 2019419520 },
    { 2144935937,   37304730, 1841489054 },
    { 2144894977, 1601434616,  157985831 },
    { 2144888833,   98749330, 2128592228 },
    { 2144880641, 1772327002, 2076128344 },
    { 2144864257, 1404514762, 2029969964 },
    { 2144827393,  801236594,  406627220 },
    { 2144806913,  349217443, 1501080290 },
    { 2144796673, 1542656776, 2084736519 },
    { 2144778241, 1210734884, 1746416203 },
    { 2144759809, 1146598851,  716464489 },
    { 2144757761,  286328400, 1823728177 },
    { 2144729089, 1347555695, 1836644881 },
    { 2144727041, 1795703790,  520296412 },
    { 2144696321, 1302475157,  852964281 },
    { 2144667649, 1075877614,  504992927 },
    { 2144573441,  198765808, 1617144982 },
    { 2144555009,  321528767,  155821259 },
    { 2144550913,  814139516, 1819937644 },
    { 2144536577,  571143206,  962942255 },
    { 2144524289, 1746733766,    2471321 },
    { 2144512001, 1821415077,  124190939 },
    { 2144468993,  917871546, 1260072806 },
    { 2144458753,  378417981, 1569240563 },
    { 2144421889,  175229668, 1825620763 },
    { 2144409601, 1699216963,  351648117 },
    { 2144370689, 1071885991,  958186029 },
    { 2144348161, 1763151227,  540353574 },
    { 2144335873, 1060214804,  919598847 },
    { 2144329729,  663515846, 1448552668 },
    { 2144327681, 1057776305,  590222840 },
    { 2144309249, 1705149168, 1459294624 },
    { 2144296961,  325823721, 1649016934 },
    { 2144290817,  738775789,  447427206 },
    { 2144243713,  962347618,  893050215 },
    { 2144237569, 1655257077,  900860862 },
    { 2144161793,  242206694, 1567868672 },
    { 2144155649,  769415308, 1247993134 },
    { 2144137217,  320492023,  515841070 },
    { 2144120833, 1639388522,  770877302 },
    { 2144071681, 1761785233,  964296120 },
    { 2144065537,  419817825,  204564472 },
    { 2144028673,  666050597, 2091019760 },
    { 2144010241, 1413657615, 1518702610 },
    { 2143952897, 1238327946,  475672271 },
    { 2143940609,  307063413, 1176750846 },
    { 2143918081, 2062905559,  786785803 },
    { 2143899649, 1338112849, 1562292083 },
    { 2143891457,   68149545,   87166451 },
    { 2143885313,  921750778,  394460854 },
    { 2143854593,  719766593,  133877196 },
    { 2143836161, 1149399850, 1861591875 },
    { 2143762433, 1848739366, 1335934145 },
    { 2143756289, 1326674710,  102999236 },
    { 2143713281,  808061791, 1156900308 },
    { 2143690753,  388399459, 1926468019 },
    { 2143670273, 1427891374, 1756689401 },
    { 2143666177, 1912173949,  986629565 },
    { 2143645697, 2041160111,  371842865 },
    { 2143641601, 1279906897, 2023974350 },
    { 2143635457,  720473174, 1389027526 },
    { 2143621121, 1298309455, 1732632006 },
    { 2143598593, 1548762216, 1825417506 },
    { 2143567873,  620475784, 1073787233 },
    { 2143561729, 1932954575,  949167309 },
    { 2143553537,  354315656, 1652037534 },
    { 2143541249,  577424288, 1097027618 },
    { 2143531009,  357862822,  478640055 },
    { 2143522817, 2017706025, 1550531668 },
    { 2143506433, 2078127419, 1824320165 },
    { 2143488001,  613475285, 1604011510 },
    { 2143469569, 1466594987,  502095196 },
    { 2143426561, 1115430331, 1044637111 },
    { 2143383553,    9778045, 1902463734 },
    { 2143377409, 1557401276, 2056861771 },
    { 2143363073,  652036455, 1965915971 },
    { 2143260673, 1464581171, 1523257541 },
    { 2143246337, 1876119649,  764541916 },
    { 2143209473, 1614992673, 1920672844 },
    { 2143203329,  981052047, 2049774209 },
    { 2143160321, 1847355533,  728535665 },
    { 2143129601,  965558457,  603052992 },
    { 2143123457, 2140817191,    8348679 },
    { 2143100929, 1547263683,  694209023 },
    { 2143092737,  643459066, 1979934533 },
    { 2143082497,  188603778, 2026175670 },
    { 2143062017, 1657329695,  377451099 },
    { 2143051777,  114967950,  979255473 },
    { 2143025153, 1698431342, 1449196896 },
    { 2143006721, 1862741675, 1739650365 },
    { 2142996481,  756660457,  996160050 },
    { 2142976001,  927864010, 1166847574 },
    { 2142965761,  905070557,  661974566 },
    { 2142916609,   40932754, 1787161127 },
    { 2142892033, 1987985648,  675335382 },
    { 2142885889,  797497211, 1323096997 },
    { 2142871553, 2068025830, 1411877159 },
    { 2142861313, 1217177090, 1438410687 },
    { 2142830593,  409906375, 1767860634 },
    { 2142803969, 1197788993,  359782919 },
    { 2142785537,  643817365,  513932862 },
    { 2142779393, 1717046338,  218943121 },
    { 2142724097,   89336830,  416687049 },
    { 2142707713,    5944581, 1356813523 },
    { 2142658561,  887942135, 2074011722 },
    { 2142638081,  151851972, 1647339939 },
    { 2142564353, 1691505537, 1483107336 },
    { 2142533633, 1989920200, 1135938817 },
    { 2142529537,  959263126, 1531961857 },
    { 2142527489,  453251129, 1725566162 },
    { 2142502913, 1536028102,  182053257 },
    { 2142498817,  570138730,  701443447 },
    { 2142416897,  326965800,  411931819 },
    { 2142363649, 1675665410, 1517191733 },
    { 2142351361,  968529566, 1575712703 },
    { 2142330881, 1384953238, 1769087884 },
    { 2142314497, 1977173242, 1833745524 },
    { 2142289921,   95082313, 1714775493 },
    { 2142283777,  109377615, 1070584533 },
    { 2142277633,   16960510,  702157145 },
    { 2142263297,  553850819,  431364395 },
    { 2142208001,  241466367, 2053967982 },
    { 2142164993, 1795661326, 1031836848 },
    { 2142097409, 1212530046,  712772031 },
    { 2142087169, 1763869720,  822276067 },
    { 2142078977,  644065713, 1765268066 },
    { 2142074881,  112671944,  643204925 },
    { 2142044161, 1387785471, 1297890174 },
    { 2142025729,  783885537, 1000425730 },
    { 2142011393,  905662232, 1679401033 },
    { 2141974529,  799788433,  468119557 },
    { 2141943809, 1932544124,  449305555 },
    { 2141933569, 1527403256,  841867925 },
    { 2141931521, 1247076451,  743823916 },
    { 2141902849, 1199660531,  401687910 },
    { 2141890561,  150132350, 1720336972 },
    { 2141857793, 1287438162,  663880489 },
    { 2141833217,  618017731, 1819208266 },
    { 2141820929,  999578638, 1403090096 },
    { 2141786113,   81834325, 1523542501 },
    { 2141771777,  120001928,  463556492 },
    { 2141759489,  122455485, 2124928282 },
    { 2141749249,  141986041,  940339153 },
    { 2141685761,  889088734,  477141499 },
    { 2141673473,  324212681, 1122558298 },
    { 2141669377, 1175806187, 1373818177 },
    { 2141655041, 1113654822,  296887082 },
    { 2141587457,  991103258, 1585913875 },
    { 2141583361, 1401451409, 1802457360 },
    { 2141575169, 1571977166,  712760980 },
    { 2141546497, 1107849376, 1250270109 },
    { 2141515777,  196544219,  356001130 },
    { 2141495297, 1733571506, 1060744866 },
    { 2141483009,  321552363, 1168297026 },
    { 2141458433,  505818251,  733225819 },
    { 2141360129, 1026840098,  948342276 },
    { 2141325313,  945133744, 2129965998 },
    { 2141317121, 1871100260, 1843844634 },
    { 2141286401, 1790639498, 1750465696 },
    { 2141267969, 1376858592,  186160720 },
    { 2141255681, 2129698296, 1876677959 },
    { 2141243393, 2138900688, 1340009628 },
    { 2141214721, 1933049835, 1087819477 },
    { 2141212673, 1898664939, 1786328049 },
    { 2141202433,  990234828,  940682169 },
    { 2141175809, 1406392421,  993089586 },
    { 2141165569, 1263518371,  289019479 },
    { 2141073409, 1485624211,  507864514 },
    { 2141052929, 1885134788,  311252465 },
    { 2141040641, 1285021247,  280941862 },
    { 2141028353, 1527610374,  375035110 },
    { 2141011969, 1400626168,  164696620 },
    { 2140999681,  632959608,  966175067 },
    { 2140997633, 2045628978, 1290889438 },
    { 2140993537, 1412755491,  375366253 },
    { 2140942337,  719477232,  785367828 },
    { 2140925953,   45224252,  836552317 },
    { 2140917761, 1157376588, 1001839569 },
    { 2140887041,  278480752, 2098732796 },
    { 2140837889, 1663139953,  924094810 },
    { 2140788737,  802501511, 2045368990 },
    { 2140766209, 1820083885, 1800295504 },
    { 2140764161, 1169561905, 2106792035 },
    { 2140696577,  127781498, 1885987531 },
    { 2140684289,   16014477, 1098116827 },
    { 2140653569,  665960598, 1796728247 },
    { 2140594177, 1043085491,  377310938 },
    { 2140579841, 1732838211, 1504505945 },
    { 2140569601,  302071939,  358291016 },
    { 2140567553,  192393733, 1909137143 },
    { 2140557313,  406595731, 1175330270 },
    { 2140549121, 1748850918,  525007007 },
    { 2140477441,  499436566, 1031159814 },
    { 2140469249, 1886004401, 1029951320 },
    { 2140426241, 1483168100, 1676273461 },
    { 2140420097, 1779917297,  846024476 },
    { 2140413953,  522948893, 1816354149 },
    { 2140383233, 1931364473, 1296921241 },
    { 2140366849, 1917356555,  147196204 },
    { 2140354561,   16466177, 1349052107 },
    { 2140348417, 1875366972, 1860485634 },
    { 2140323841,  456498717, 1790256483 },
    { 2140321793, 1629493973,  150031888 },
    { 2140315649, 1904063898,  395510935 },
    { 2140280833, 1784104328,  831417909 },
    { 2140250113,  256087139,  697349101 },
    { 2140229633,  388553070,  243875754 },
    { 2140223489,  747459608, 1396270850 },
    { 2140200961,  507423743, 1895572209 },
    { 2140162049,  580106016, 2045297469 },
    { 2140149761,  712426444,  785217995 },
    { 2140137473, 1441607584,  536866543 },
    { 2140119041,  346538902, 1740434653 },
    { 2140090369,  282642885,   21051094 },
    { 2140076033, 1407456228,  319910029 },
    { 2140047361, 1619330500, 1488632070 },
    { 2140041217, 2089408064, 2012026134 },
    { 2140008449, 1705524800, 1613440760 },
    { 2139924481, 1846208233, 1280649481 },
    { 2139906049,  989438755, 1185646076 },
    { 2139867137, 1522314850,  372783595 },
    { 2139842561, 1681587377,  216848235 },
    { 2139826177, 2066284988, 1784999464 },
    { 2139824129,  480888214, 1513323027 },
    { 2139789313,  847937200,  858192859 },
    { 2139783169, 1642000434, 1583261448 },
    { 2139770881,  940699589,  179702100 },
    { 2139768833,  315623242,  964612676 },
    { 2139666433,  331649203,  764666914 },
    { 2139641857, 2118730799, 1313764644 },
    { 2139635713,  519149027,  519212449 },
    { 2139598849, 1526413634, 1769667104 },
    { 2139574273,  551148610,  820739925 },
    { 2139568129, 1386800242,  472447405 },
    { 2139549697,  813760130, 1412328531 },
    { 2139537409, 1615286260, 1609362979 },
    { 2139475969, 1352559299, 1696720421 },
    { 2139455489, 1048691649, 1584935400 },
    { 2139432961,  836025845,  950121150 },
    { 2139424769, 1558281165, 1635486858 },
    { 2139406337, 1728402143, 1674423301 },
    { 2139396097, 1727715782, 1483470544 },
    { 2139383809, 1092853491, 1741699084 },
    { 2139369473,  690776899, 1242798709 },
    { 2139351041, 1768782380, 2120712049 },
    { 2139334657, 1739968247, 1427249225 },
    { 2139332609, 1547189119,  623011170 },
    { 2139310081, 1346827917, 1605466350 },
    { 2139303937,  369317948,  828392831 },
    { 2139301889, 1560417239, 1788073219 },
    { 2139283457, 1303121623,  595079358 },
    { 2139248641, 1354555286,  573424177 },
    { 2139240449,   60974056,  885781403 },
    { 2139222017,  355573421, 1221054839 },
    { 2139215873,  566477826, 1724006500 },
    { 2139150337,  871437673, 1609133294 },
    { 2139144193, 1478130914, 1137491905 },
    { 2139117569, 1854880922,  964728507 },
    { 2139076609,  202405335,  756508944 },
    { 2139062273, 1399715741,  884826059 },
    { 2139045889, 1051045798, 1202295476 },
    { 2139033601, 1707715206,  632234634 },
    { 2139006977, 2035853139,  231626690 },
    { 2138951681,  183867876,  838350879 },
    { 2138945537, 1403254661,  404460202 },
    { 2138920961,  310865011, 1282911681 },
    { 2138910721, 1328496553,  103472415 },
    { 2138904577,   78831681,  993513549 },
    { 2138902529, 1319697451, 1055904361 },
    { 2138816513,  384338872, 1706202469 },
    { 2138810369, 1084868275,  405677177 },
    { 2138787841,  401181788, 1964773901 },
    { 2138775553, 1850532988, 1247087473 },
    { 2138767361,  874261901, 1576073565 },
    { 2138757121, 1187474742,  993541415 },
    { 2138748929, 1782458888, 1043206483 },
    { 2138744833, 1221500487,  800141243 },
    { 2138738689,  413465368, 1450660558 },
    { 2138695681,  739045140,  342611472 },
    { 2138658817, 1355845756,  672674190 },
    { 2138644481,  608379162, 1538874380 },
    { 2138632193, 1444914034,  686911254 },
    { 2138607617,  484707818, 1435142134 },
    { 2138591233,  539460669, 1290458549 },
    { 2138572801, 2093538990, 2011138646 },
    { 2138552321, 1149786988, 1076414907 },
    { 2138546177,  840688206, 2108985273 },
    { 2138533889,  209669619,  198172413 },
    { 2138523649, 1975879426, 1277003968 },
    { 2138490881, 1351891144, 1976858109 },
    { 2138460161, 1817321013, 1979278293 },
    { 2138429441, 1950077177,  203441928 },
    { 2138400769,  908970113,  628395069 },
    { 2138398721,  219890864,  758486760 },
    { 2138376193, 1306654379,  977554090 },
    { 2138351617,  298822498, 2004708503 },
    { 2138337281,  441457816, 1049002108 },
    { 2138320897, 1517731724, 1442269609 },
    { 2138290177, 1355911197, 1647139103 },
    { 2138234881,  531313247, 1746591962 },
    { 2138214401, 1899410930,  781416444 },
    { 2138202113, 1813477173, 1622508515 },
    { 2138191873, 1086458299, 1025408615 },
    { 2138183681, 1998800427,  827063290 },
    { 2138173441, 1921308898,  749670117 },
    { 2138103809, 1620902804, 2126787647 },
    { 2138099713,  828647069, 1892961817 },
    { 2138085377,  179405355, 1525506535 },
    { 2138060801,  615683235, 1259580138 },
    { 2138044417, 2030277840, 1731266562 },
    { 2138042369, 2087222316, 1627902259 },
    { 2138032129,  126388712, 1108640984 },
    { 2138011649,  715026550, 1017980050 },
    { 2137993217, 1693714349, 1351778704 },
    { 2137888769, 1289762259, 1053090405 },
    { 2137853953,  199991890, 1254192789 },
    { 2137833473,  941421685,  896995556 },
    { 2137817089,  750416446, 1251031181 },
    { 2137792513,  798075119,  368077456 },
    { 2137786369,  878543495, 1035375025 },
    { 2137767937,    9351178, 1156563902 },
    { 2137755649, 1382297614, 1686559583 },
    { 2137724929, 1345472850, 1681096331 },
    { 2137704449,  834666929,  630551727 },
    { 2137673729, 1646165729, 1892091571 },
    { 2137620481,  778943821,   48456461 },
    { 2137618433, 1730837875, 1713336725 },
    { 2137581569,  805610339, 1378891359 },
    { 2137538561,  204342388, 1950165220 },
    { 2137526273, 1947629754, 1500789441 },
    { 2137516033,  719902645, 1499525372 },
    { 2137491457,  230451261,  556382829 },
    { 2137440257,  979573541,  412760291 },
    { 2137374721,  927841248, 1954137185 },
    { 2137362433, 1243778559,  861024672 },
    { 2137313281, 1341338501,  980638386 },
    { 2137311233,  937415182, 1793212117 },
    { 2137255937,  795331324, 1410253405 },
    { 2137243649,  150756339, 1966999887 },
    { 2137182209,  163346914, 1939301431 },
    { 2137171969, 1952552395,  758913141 },
    { 2137159681,  570788721,  218668666 },
    { 2137147393, 1896656810, 2045670345 },
    { 2137141249,  358493842,  518199643 },
    { 2137139201, 1505023029,  674695848 },
    { 2137133057,   27911103,  830956306 },
    { 2137122817,  439771337, 1555268614 },
    { 2137116673,  790988579, 1871449599 },
    { 2137110529,  432109234,  811805080 },
    { 2137102337, 1357900653, 1184997641 },
    { 2137098241,  515119035, 1715693095 },
    { 2137090049,  408575203, 2085660657 },
    { 2137085953, 2097793407, 1349626963 },
    { 2137055233, 1556739954, 1449960883 },
    { 2137030657, 1545758650, 1369303716 },
    { 2136987649,  332602570,  103875114 },
    { 2136969217, 1499989506, 1662964115 },
    { 2136924161,  857040753,    4738842 },
    { 2136895489, 1948872712,  570436091 },
    { 2136893441,   58969960, 1568349634 },
    { 2136887297, 2127193379,  273612548 },
    { 2136850433,  111208983, 1181257116 },
    { 2136809473, 1627275942, 1680317971 },
    { 2136764417, 1574888217,   14011331 },
    { 2136741889,   14011055, 1129154251 },
    { 2136727553,   35862563, 1838555253 },
    { 2136721409,  310235666, 1363928244 },
    { 2136698881, 1612429202, 1560383828 },
    { 2136649729, 1138540131,  800014364 },
    { 2136606721,  602323503, 1433096652 },
    { 2136563713,  182209265, 1919611038 },
    { 2136555521,  324156477,  165591039 },
    { 2136549377,  195513113,  217165345 },
    { 2136526849, 1050768046,  939647887 },
    { 2136508417, 1886286237, 1619926572 },
    { 2136477697,  609647664,   35065157 },
    { 2136471553,  679352216, 1452259468 },
    { 2136457217,  128630031,  824816521 },
    { 2136422401,   19787464, 1526049830 },
    { 2136420353,  698316836, 1530623527 },
    { 2136371201, 1651862373, 1804812805 },
    { 2136334337,  326596005,  336977082 },
    { 2136322049,   63253370, 1904972151 },
    { 2136297473,  312176076,  172182411 },
    { 2136248321,  381261841,  369032670 },
    { 2136242177,  358688773, 1640007994 },
    { 2136229889,  512677188,   75585225 },
    { 2136219649, 2095003250, 1970086149 },
    { 2136207361, 1909650722,  537760675 },
    { 2136176641, 1334616195, 1533487619 },
    { 2136158209, 2096285632, 1793285210 },
    { 2136143873, 1897347517,  293843959 },
    { 2136133633,  923586222, 1022655978 },
    { 2136096769, 1464868191, 1515074410 },
    { 2136094721, 2020679520, 2061636104 },
    { 2136076289,  290798503, 1814726809 },
    { 2136041473,  156415894, 1250757633 },
    { 2135996417,  297459940, 1132158924 },
    { 2135955457,  538755304, 1688831340 },
    { 0, 0, 0 }
};
/*
 * Bit-reversal index table.
 */
static const uint16_t REV10[] = {
       0,  512,  256,  768,  128,  640,  384,  896,   64,  576,  320,  832,
     192,  704,  448,  960,   32,  544,  288,  800,  160,  672,  416,  928,
      96,  608,  352,  864,  224,  736,  480,  992,   16,  528,  272,  784,
     144,  656,  400,  912,   80,  592,  336,  848,  208,  720,  464,  976,
      48,  560,  304,  816,  176,  688,  432,  944,  112,  624,  368,  880,
     240,  752,  496, 1008,    8,  520,  264,  776,  136,  648,  392,  904,
      72,  584,  328,  840,  200,  712,  456,  968,   40,  552,  296,  808,
     168,  680,  424,  936,  104,  616,  360,  872,  232,  744,  488, 1000,
      24,  536,  280,  792,  152,  664,  408,  920,   88,  600,  344,  856,
     216,  728,  472,  984,   56,  568,  312,  824,  184,  696,  440,  952,
     120,  632,  376,  888,  248,  760,  504, 1016,    4,  516,  260,  772,
     132,  644,  388,  900,   68,  580,  324,  836,  196,  708,  452,  964,
      36,  548,  292,  804,  164,  676,  420,  932,  100,  612,  356,  868,
     228,  740,  484,  996,   20,  532,  276,  788,  148,  660,  404,  916,
      84,  596,  340,  852,  212,  724,  468,  980,   52,  564,  308,  820,
     180,  692,  436,  948,  116,  628,  372,  884,  244,  756,  500, 1012,
      12,  524,  268,  780,  140,  652,  396,  908,   76,  588,  332,  844,
     204,  716,  460,  972,   44,  556,  300,  812,  172,  684,  428,  940,
     108,  620,  364,  876,  236,  748,  492, 1004,   28,  540,  284,  796,
     156,  668,  412,  924,   92,  604,  348,  860,  220,  732,  476,  988,
      60,  572,  316,  828,  188,  700,  444,  956,  124,  636,  380,  892,
     252,  764,  508, 1020,    2,  514,  258,  770,  130,  642,  386,  898,
      66,  578,  322,  834,  194,  706,  450,  962,   34,  546,  290,  802,
     162,  674,  418,  930,   98,  610,  354,  866,  226,  738,  482,  994,
      18,  530,  274,  786,  146,  658,  402,  914,   82,  594,  338,  850,
     210,  722,  466,  978,   50,  562,  306,  818,  178,  690,  434,  946,
     114,  626,  370,  882,  242,  754,  498, 1010,   10,  522,  266,  778,
     138,  650,  394,  906,   74,  586,  330,  842,  202,  714,  458,  970,
      42,  554,  298,  810,  170,  682,  426,  938,  106,  618,  362,  874,
     234,  746,  490, 1002,   26,  538,  282,  794,  154,  666,  410,  922,
      90,  602,  346,  858,  218,  730,  474,  986,   58,  570,  314,  826,
     186,  698,  442,  954,  122,  634,  378,  890,  250,  762,  506, 1018,
       6,  518,  262,  774,  134,  646,  390,  902,   70,  582,  326,  838,
     198,  710,  454,  966,   38,  550,  294,  806,  166,  678,  422,  934,
     102,  614,  358,  870,  230,  742,  486,  998,   22,  534,  278,  790,
     150,  662,  406,  918,   86,  598,  342,  854,  214,  726,  470,  982,
      54,  566,  310,  822,  182,  694,  438,  950,  118,  630,  374,  886,
     246,  758,  502, 1014,   14,  526,  270,  782,  142,  654,  398,  910,
      78,  590,  334,  846,  206,  718,  462,  974,   46,  558,  302,  814,
     174,  686,  430,  942,  110,  622,  366,  878,  238,  750,  494, 1006,
      30,  542,  286,  798,  158,  670,  414,  926,   94,  606,  350,  862,
     222,  734,  478,  990,   62,  574,  318,  830,  190,  702,  446,  958,
     126,  638,  382,  894,  254,  766,  510, 1022,    1,  513,  257,  769,
     129,  641,  385,  897,   65,  577,  321,  833,  193,  705,  449,  961,
      33,  545,  289,  801,  161,  673,  417,  929,   97,  609,  353,  865,
     225,  737,  481,  993,   17,  529,  273,  785,  145,  657,  401,  913,
      81,  593,  337,  849,  209,  721,  465,  977,   49,  561,  305,  817,
     177,  689,  433,  945,  113,  625,  369,  881,  241,  753,  497, 1009,
       9,  521,  265,  777,  137,  649,  393,  905,   73,  585,  329,  841,
     201,  713,  457,  969,   41,  553,  297,  809,  169,  681,  425,  937,
     105,  617,  361,  873,  233,  745,  489, 1001,   25,  537,  281,  793,
     153,  665,  409,  921,   89,  601,  345,  857,  217,  729,  473,  985,
      57,  569,  313,  825,  185,  697,  441,  953,  121,  633,  377,  889,
     249,  761,  505, 1017,    5,  517,  261,  773,  133,  645,  389,  901,
      69,  581,  325,  837,  197,  709,  453,  965,   37,  549,  293,  805,
     165,  677,  421,  933,  101,  613,  357,  869,  229,  741,  485,  997,
      21,  533,  277,  789,  149,  661,  405,  917,   85,  597,  341,  853,
     213,  725,  469,  981,   53,  565,  309,  821,  181,  693,  437,  949,
     117,  629,  373,  885,  245,  757,  501, 1013,   13,  525,  269,  781,
     141,  653,  397,  909,   77,  589,  333,  845,  205,  717,  461,  973,
      45,  557,  301,  813,  173,  685,  429,  941,  109,  621,  365,  877,
     237,  749,  493, 1005,   29,  541,  285,  797,  157,  669,  413,  925,
      93,  605,  349,  861,  221,  733,  477,  989,   61,  573,  317,  829,
     189,  701,  445,  957,  125,  637,  381,  893,  253,  765,  509, 1021,
       3,  515,  259,  771,  131,  643,  387,  899,   67,  579,  323,  835,
     195,  707,  451,  963,   35,  547,  291,  803,  163,  675,  419,  931,
      99,  611,  355,  867,  227,  739,  483,  995,   19,  531,  275,  787,
     147,  659,  403,  915,   83,  595,  339,  851,  211,  723,  467,  979,
      51,  563,  307,  819,  179,  691,  435,  947,  115,  627,  371,  883,
     243,  755,  499, 1011,   11,  523,  267,  779,  139,  651,  395,  907,
      75,  587,  331,  843,  203,  715,  459,  971,   43,  555,  299,  811,
     171,  683,  427,  939,  107,  619,  363,  875,  235,  747,  491, 1003,
      27,  539,  283,  795,  155,  667,  411,  923,   91,  603,  347,  859,
     219,  731,  475,  987,   59,  571,  315,  827,  187,  699,  443,  955,
     123,  635,  379,  891,  251,  763,  507, 1019,    7,  519,  263,  775,
     135,  647,  391,  903,   71,  583,  327,  839,  199,  711,  455,  967,
      39,  551,  295,  807,  167,  679,  423,  935,  103,  615,  359,  871,
     231,  743,  487,  999,   23,  535,  279,  791,  151,  663,  407,  919,
      87,  599,  343,  855,  215,  727,  471,  983,   55,  567,  311,  823,
     183,  695,  439,  951,  119,  631,  375,  887,  247,  759,  503, 1015,
      15,  527,  271,  783,  143,  655,  399,  911,   79,  591,  335,  847,
     207,  719,  463,  975,   47,  559,  303,  815,  175,  687,  431,  943,
     111,  623,  367,  879,  239,  751,  495, 1007,   31,  543,  287,  799,
     159,  671,  415,  927,   95,  607,  351,  863,  223,  735,  479,  991,
      63,  575,  319,  831,  191,  703,  447,  959,  127,  639,  383,  895,
     255,  767,  511, 1023
};

/*
 * Reduce a small signed integer modulo a small prime. The source
 * value x MUST be such that -p < x < p.
 */
//-p<x<p 사이에 있는 x를 0=<x<p 사이에 있는 값으로 바꾸겠다는 것임 return 값이 unsigned EYSEO
static inline uint32_t
modp_set(int32_t x, uint32_t p)
{
    uint32_t w;

    w = (uint32_t)x;
    w += p & -(w >> 31);
    return w;
}
/*
 * Normalize a modular integer around 0.
 */
static inline int32_t
modp_norm(uint32_t x, uint32_t p)
{
    return (int32_t)(x - (p & (((x - ((p + 1) >> 1)) >> 31) - 1)));
}

/*
 * Compute -1/p mod 2^31. This works for all odd integers p that fit
 * on 31 bits.
 */
//p*p'=-1 mod R=2^{31} 을 만족시키는 p'=-1/p mod 2^31을 구하는 과정 EYSEO
static uint32_t
modp_ninv31(uint32_t p)
{
    uint32_t y;

    y = 2 - p;
    y *= 2 - p * y;
    y *= 2 - p * y;
    y *= 2 - p * y;
    y *= 2 - p * y;
    return (uint32_t)0x7FFFFFFF & -y;
}
/*
 * Compute R = 2^31 mod p.
 */
//이것은 1에 대한 Montgomery form임 EYSEO
static inline uint32_t
modp_R(uint32_t p)
{
    /*
     * Since 2^30 < p < 2^31, we know that 2^31 mod p is simply
     * 2^31 - p.
     */
    return ((uint32_t)1 << 31) - p;
}

/*
 * Addition modulo p.
 */
static inline uint32_t
modp_add(uint32_t a, uint32_t b, uint32_t p)
{
    uint32_t d;

    d = a + b - p;
    d += p & -(d >> 31);
    return d;
}

/*
 * Subtraction modulo p.
 */
static inline uint32_t
modp_sub(uint32_t a, uint32_t b, uint32_t p)
{
    uint32_t d;

    d = a - b;
    d += p & -(d >> 31);
    return d;
}
/*
 * Montgomery multiplication modulo p. The 'p0i' value is -1/p mod 2^31.
 * It is required that p is an odd integer.
 */
static inline uint32_t
modp_montymul(uint32_t a, uint32_t b, uint32_t p, uint32_t p0i)
{
    uint64_t z, w;
    uint32_t d;

    z = (uint64_t)a * (uint64_t)b;
    w = ((z * p0i) & (uint64_t)0x7FFFFFFF) * p;
    d = (uint32_t)((z + w) >> 31) - p;
    d += p & -(d >> 31);
    return d;
}
/*
 * Compute R2 = 2^62 mod p.
 */
static uint32_t
modp_R2(uint32_t p, uint32_t p0i)
{
    uint32_t z;

    /*
     * Compute z = 2^31 mod p (this is the value 1 in Montgomery
     * representation), then double it with an addition.
     */
    z = modp_R(p);
    z = modp_add(z, z, p);

    /*
     * Square it five times to obtain 2^32 in Montgomery representation
     * (i.e. 2^63 mod p).
     */
    z = modp_montymul(z, z, p, p0i);
    z = modp_montymul(z, z, p, p0i);
    z = modp_montymul(z, z, p, p0i);
    z = modp_montymul(z, z, p, p0i);
    z = modp_montymul(z, z, p, p0i);

    /*
     * Halve the value mod p to get 2^62.
     */
    z = (z + (p & -(z & 1))) >> 1;
    return z;
}
/*
 * Division modulo p. If the divisor (b) is 0, then 0 is returned.
 * This function computes proper results only when p is prime.
 * Parameters:
 *   a     dividend
 *   b     divisor
 *   p     odd prime modulus
 *   p0i   -1/p mod 2^31
 *   R     2^31 mod R
 */
static uint32_t
modp_div(uint32_t a, uint32_t b, uint32_t p, uint32_t p0i, uint32_t R)
{
    uint32_t z, e;
    int i;

    e = p - 2;
    z = R;
    for (i = 30; i >= 0; i --) {
        uint32_t z2;

        z = modp_montymul(z, z, p, p0i);
        z2 = modp_montymul(z, b, p, p0i);
        z ^= (z ^ z2) & -(uint32_t)((e >> i) & 1);
    }

    /*
     * The loop above just assumed that b was in Montgomery
     * representation, i.e. really contained b*R; under that
     * assumption, it returns 1/b in Montgomery representation,
     * which is R/b. But we gave it b in normal representation,
     * so the loop really returned R/(b/R) = R^2/b.
     *
     * We want a/b, so we need one Montgomery multiplication with a,
     * which also remove one of the R factors, and another such
     * multiplication to remove the second R factor.
     */
    //printf("\nmodp_div:z[%d]\n",z);//EYSEO
    z = modp_montymul(z, 1, p, p0i);
    return modp_montymul(a, z, p, p0i);
}


/*
 * Compute the roots for NTT and inverse NTT (binary case). Input
 * parameter g is a primitive 2048-th root of 1 modulo p (i.e. g^1024 =
 * -1 mod p). This fills gm[] and igm[] with powers of g and 1/g:
 *   gm[rev(i)] = g^i mod p
 *   igm[rev(i)] = (1/g)^i mod p
 * where rev() is the "bit reversal" function over 10 bits. It fills
 * the arrays only up to N = 2^logn values.
 *
 * The values stored in gm[] and igm[] are in Montgomery representation.
 *
 * p must be a prime such that p = 1 mod 2048.
 */
//이 함수를 부르기 직전에 R2를 계산하는데, 굳이 이 함수 안에서 또 R2를 따로 계산할 필요가 있나 EYSEO
static void
modp_mkgm2(uint32_t *restrict gm, uint32_t *restrict igm, unsigned logn,
    uint32_t g, uint32_t p, uint32_t p0i)
{
    size_t u, n;
    unsigned k;
    uint32_t ig, x1, x2, R2;

    n = (size_t)1 << logn;

    /*
     * We want g such that g^(2N) = 1 mod p, but the provided
     * generator has order 2048. We must square it a few times.
     */
    R2 = modp_R2(p, p0i);
    g = modp_montymul(g, R2, p, p0i);//g의 montgomery form EYSEO
    for (k = logn; k < 10; k ++) {
        g = modp_montymul(g, g, p, p0i);
    }
    //printf("\nmkgm2: logn[%d], g[%d],p[%d],p0i[%d]\n", logn,g,p,p0i);//EYSEO
    ig = modp_div(R2, g, p, p0i, modp_R(p));
    //printf("modp_mkgm2:ig[%d]\n",ig);//EYSEO
    k = 10 - logn;
    x1 = x2 = modp_R(p);
    for (u = 0; u < n; u ++) {
        size_t v;

        v = REV10[u << k];
        gm[v] = x1;
        igm[v] = x2;
        x1 = modp_montymul(x1, g, p, p0i);
        x2 = modp_montymul(x2, ig, p, p0i);
    }
}

/*
 * Compute the NTT over a polynomial (binary case). Polynomial elements
 * are a[0], a[stride], a[2 * stride]...
 */
//아래의 NTT함수의 결과는 montgomery form이 아니다. 왜냐하면 modp_montymul(a,gm)에서
//a는 montgomery가 아니고, gm은 montgomery이기 때문이다.
static void
modp_NTT2_ext(uint32_t *a, size_t stride, const uint32_t *gm, unsigned logn,
    uint32_t p, uint32_t p0i)
{
    size_t t, m, n;

    if (logn == 0) {
        return;
    }
    n = (size_t)1 << logn;
    t = n;
    for (m = 1; m < n; m <<= 1) {
        size_t ht, u, v1;

        ht = t >> 1;
        for (u = 0, v1 = 0; u < m; u ++, v1 += t) {
            uint32_t s;
            size_t v;
            uint32_t *r1, *r2;

            s = gm[m + u];
            r1 = a + v1 * stride;
            r2 = r1 + ht * stride;
            for (v = 0; v < ht; v ++, r1 += stride, r2 += stride) {
                uint32_t x, y;

                x = *r1;
                y = modp_montymul(*r2, s, p, p0i);
                *r1 = modp_add(x, y, p);
                *r2 = modp_sub(x, y, p);
            }
        }
        t = ht;
    }
}

/*
 * Compute the inverse NTT over a polynomial (binary case).
 */
static void
modp_iNTT2_ext(uint32_t *a, size_t stride, const uint32_t *igm, unsigned logn,
    uint32_t p, uint32_t p0i)
{
    size_t t, m, n, k;
    uint32_t ni;
    uint32_t *r;

    if (logn == 0) {
        return;
    }
    n = (size_t)1 << logn;
    t = 1;
    for (m = n; m > 1; m >>= 1) {
        size_t hm, dt, u, v1;

        hm = m >> 1;
        dt = t << 1;
        for (u = 0, v1 = 0; u < hm; u ++, v1 += dt) {
            uint32_t s;
            size_t v;
            uint32_t *r1, *r2;

            s = igm[hm + u];
            r1 = a + v1 * stride;
            r2 = r1 + t * stride;
            for (v = 0; v < t; v ++, r1 += stride, r2 += stride) {
                uint32_t x, y;

                x = *r1;
                y = *r2;
                *r1 = modp_add(x, y, p);
                *r2 = modp_montymul(
                    modp_sub(x, y, p), s, p, p0i);;
            }
        }
        t = dt;
    }

    /*
     * We need 1/n in Montgomery representation, i.e. R/n. Since
     * 1 <= logn <= 10, R/n is an integer; morever, R/n <= 2^30 < p,
     * thus a simple shift will do.
     */
    ni = (uint32_t)1 << (31 - logn);
    for (k = 0, r = a; k < n; k ++, r += stride) {
        *r = modp_montymul(*r, ni, p, p0i);
    }
}


/* ==================================================================== */
/* ===================================================================== */
/*
 * Constants for NTT.
 *
 *   n = 2^logn  (2 <= n <= 1024)
 *   phi = X^n + 1
 *   q = 12289
 *   q0i = -1/q mod 2^16
 *   QR = 2^16 mod q
 *   QR2 = 2^32 mod q
 */

#define Q     12289
#define Q0I   12287
#define QR      4091
#define QR2    10952

/*
 * Table for NTT, binary case:
 *   GMb[x] = R*(g^rev(x)) mod q
 * where g = 7 (it is a 2048-th primitive root of 1 modulo q)
 * and rev() is the bit-reversal function over 10 bits.
 */
static const uint16_t GMb[] = {
     4091,  7888, 11060, 11208,  6960,  4342,  6275,  9759,
     1591,  6399,  9477,  5266,   586,  5825,  7538,  9710,
     1134,  6407,  1711,   965,  7099,  7674,  3743,  6442,
    10414,  8100,  1885,  1688,  1364, 10329, 10164,  9180,
    12210,  6240,   997,   117,  4783,  4407,  1549,  7072,
     2829,  6458,  4431,  8877,  7144,  2564,  5664,  4042,
    12189,   432, 10751,  1237,  7610,  1534,  3983,  7863,
     2181,  6308,  8720,  6570,  4843,  1690,    14,  3872,
     5569,  9368, 12163,  2019,  7543,  2315,  4673,  7340,
     1553,  1156,  8401, 11389,  1020,  2967, 10772,  7045,
     3316, 11236,  5285, 11578, 10637, 10086,  9493,  6180,
     9277,  6130,  3323,   883, 10469,   489,  1502,  2851,
    11061,  9729,  2742, 12241,  4970, 10481, 10078,  1195,
      730,  1762,  3854,  2030,  5892, 10922,  9020,  5274,
     9179,  3604,  3782, 10206,  3180,  3467,  4668,  2446,
     7613,  9386,   834,  7703,  6836,  3403,  5351, 12276,
     3580,  1739, 10820,  9787, 10209,  4070, 12250,  8525,
    10401,  2749,  7338, 10574,  6040,   943,  9330,  1477,
     6865,  9668,  3585,  6633, 12145,  4063,  3684,  7680,
     8188,  6902,  3533,  9807,  6090,   727, 10099,  7003,
     6945,  1949,  9731, 10559,  6057,   378,  7871,  8763,
     8901,  9229,  8846,  4551,  9589, 11664,  7630,  8821,
     5680,  4956,  6251,  8388, 10156,  8723,  2341,  3159,
     1467,  5460,  8553,  7783,  2649,  2320,  9036,  6188,
      737,  3698,  4699,  5753,  9046,  3687,    16,   914,
     5186, 10531,  4552,  1964,  3509,  8436,  7516,  5381,
    10733,  3281,  7037,  1060,  2895,  7156,  8887,  5357,
     6409,  8197,  2962,  6375,  5064,  6634,  5625,   278,
      932, 10229,  8927,  7642,   351,  9298,   237,  5858,
     7692,  3146, 12126,  7586,  2053, 11285,  3802,  5204,
     4602,  1748, 11300,   340,  3711,  4614,   300, 10993,
     5070, 10049, 11616, 12247,  7421, 10707,  5746,  5654,
     3835,  5553,  1224,  8476,  9237,  3845,   250, 11209,
     4225,  6326,  9680, 12254,  4136,  2778,   692,  8808,
     6410,  6718, 10105, 10418,  3759,  7356, 11361,  8433,
     6437,  3652,  6342,  8978,  5391,  2272,  6476,  7416,
     8418, 10824, 11986,  5733,   876,  7030,  2167,  2436,
     3442,  9217,  8206,  4858,  5964,  2746,  7178,  1434,
     7389,  8879, 10661, 11457,  4220,  1432, 10832,  4328,
     8557,  1867,  9454,  2416,  3816,  9076,   686,  5393,
     2523,  4339,  6115,   619,   937,  2834,  7775,  3279,
     2363,  7488,  6112,  5056,   824, 10204, 11690,  1113,
     2727,  9848,   896,  2028,  5075,  2654, 10464,  7884,
    12169,  5434,  3070,  6400,  9132, 11672, 12153,  4520,
     1273,  9739, 11468,  9937, 10039,  9720,  2262,  9399,
    11192,   315,  4511,  1158,  6061,  6751, 11865,   357,
     7367,  4550,   983,  8534,  8352, 10126,  7530,  9253,
     4367,  5221,  3999,  8777,  3161,  6990,  4130, 11652,
     3374, 11477,  1753,   292,  8681,  2806, 10378, 12188,
     5800, 11811,  3181,  1988,  1024,  9340,  2477, 10928,
     4582,  6750,  3619,  5503,  5233,  2463,  8470,  7650,
     7964,  6395,  1071,  1272,  3474, 11045,  3291, 11344,
     8502,  9478,  9837,  1253,  1857,  6233,  4720, 11561,
     6034,  9817,  3339,  1797,  2879,  6242,  5200,  2114,
     7962,  9353, 11363,  5475,  6084,  9601,  4108,  7323,
    10438,  9471,  1271,   408,  6911,  3079,   360,  8276,
    11535,  9156,  9049, 11539,   850,  8617,   784,  7919,
     8334, 12170,  1846, 10213, 12184,  7827, 11903,  5600,
     9779,  1012,   721,  2784,  6676,  6552,  5348,  4424,
     6816,  8405,  9959,  5150,  2356,  5552,  5267,  1333,
     8801,  9661,  7308,  5788,  4910,   909, 11613,  4395,
     8238,  6686,  4302,  3044,  2285, 12249,  1963,  9216,
     4296, 11918,   695,  4371,  9793,  4884,  2411, 10230,
     2650,   841,  3890, 10231,  7248,  8505, 11196,  6688,
     4059,  6060,  3686,  4722, 11853,  5816,  7058,  6868,
    11137,  7926,  4894, 12284,  4102,  3908,  3610,  6525,
     7938,  7982, 11977,  6755,   537,  4562,  1623,  8227,
    11453,  7544,   906, 11816,  9548, 10858,  9703,  2815,
    11736,  6813,  6979,   819,  8903,  6271, 10843,   348,
     7514,  8339,  6439,   694,   852,  5659,  2781,  3716,
    11589,  3024,  1523,  8659,  4114, 10738,  3303,  5885,
     2978,  7289, 11884,  9123,  9323, 11830,    98,  2526,
     2116,  4131, 11407,  1844,  3645,  3916,  8133,  2224,
    10871,  8092,  9651,  5989,  7140,  8480,  1670,   159,
    10923,  4918,   128,  7312,   725,  9157,  5006,  6393,
     3494,  6043, 10972,  6181, 11838,  3423, 10514,  7668,
     3693,  6658,  6905, 11953, 10212, 11922,  9101,  8365,
     5110,    45,  2400,  1921,  4377,  2720,  1695,    51,
     2808,   650,  1896,  9997,  9971, 11980,  8098,  4833,
     4135,  4257,  5838,  4765, 10985, 11532,   590, 12198,
      482, 12173,  2006,  7064, 10018,  3912, 12016, 10519,
    11362,  6954,  2210,   284,  5413,  6601,  3865, 10339,
    11188,  6231,   517,  9564, 11281,  3863,  1210,  4604,
     8160, 11447,   153,  7204,  5763,  5089,  9248, 12154,
    11748,  1354,  6672,   179,  5532,  2646,  5941, 12185,
      862,  3158,   477,  7279,  5678,  7914,  4254,   302,
     2893, 10114,  6890,  9560,  9647, 11905,  4098,  9824,
    10269,  1353, 10715,  5325,  6254,  3951,  1807,  6449,
     5159,  1308,  8315,  3404,  1877,  1231,   112,  6398,
    11724, 12272,  7286,  1459, 12274,  9896,  3456,   800,
     1397, 10678,   103,  7420,  7976,   936,   764,   632,
     7996,  8223,  8445,  7758, 10870,  9571,  2508,  1946,
     6524, 10158,  1044,  4338,  2457,  3641,  1659,  4139,
     4688,  9733, 11148,  3946,  2082,  5261,  2036, 11850,
     7636, 12236,  5366,  2380,  1399,  7720,  2100,  3217,
    10912,  8898,  7578, 11995,  2791,  1215,  3355,  2711,
     2267,  2004,  8568, 10176,  3214,  2337,  1750,  4729,
     4997,  7415,  6315, 12044,  4374,  7157,  4844,   211,
     8003, 10159,  9290, 11481,  1735,  2336,  5793,  9875,
     8192,   986,  7527,  1401,   870,  3615,  8465,  2756,
     9770,  2034, 10168,  3264,  6132,    54,  2880,  4763,
    11805,  3074,  8286,  9428,  4881,  6933,  1090, 10038,
     2567,   708,   893,  6465,  4962, 10024,  2090,  5718,
    10743,   780,  4733,  4623,  2134,  2087,  4802,   884,
     5372,  5795,  5938,  4333,  6559,  7549,  5269, 10664,
     4252,  3260,  5917, 10814,  5768,  9983,  8096,  7791,
     6800,  7491,  6272,  1907, 10947,  6289, 11803,  6032,
    11449,  1171,  9201,  7933,  2479,  7970, 11337,  7062,
     8911,  6728,  6542,  8114,  8828,  6595,  3545,  4348,
     4610,  2205,  6999,  8106,  5560, 10390,  9321,  2499,
     2413,  7272,  6881, 10582,  9308,  9437,  3554,  3326,
     5991, 11969,  3415, 12283,  9838, 12063,  4332,  7830,
    11329,  6605, 12271,  2044, 11611,  7353, 11201, 11582,
     3733,  8943,  9978,  1627,  7168,  3935,  5050,  2762,
     7496, 10383,   755,  1654, 12053,  4952, 10134,  4394,
     6592,  7898,  7497,  8904, 12029,  3581, 10748,  5674,
    10358,  4901,  7414,  8771,   710,  6764,  8462,  7193,
     5371,  7274, 11084,   290,  7864,  6827, 11822,  2509,
     6578,  4026,  5807,  1458,  5721,  5762,  4178,  2105,
    11621,  4852,  8897,  2856, 11510,  9264,  2520,  8776,
     7011,  2647,  1898,  7039,  5950, 11163,  5488,  6277,
     9182, 11456,   633, 10046, 11554,  5633,  9587,  2333,
     7008,  7084,  5047,  7199,  9865,  8997,   569,  6390,
    10845,  9679,  8268, 11472,  4203,  1997,     2,  9331,
      162,  6182,  2000,  3649,  9792,  6363,  7557,  6187,
     8510,  9935,  5536,  9019,  3706, 12009,  1452,  3067,
     5494,  9692,  4865,  6019,  7106,  9610,  4588, 10165,
     6261,  5887,  2652, 10172,  1580, 10379,  4638,  9949
};

/*
 * Table for inverse NTT, binary case:
 *   iGMb[x] = R*((1/g)^rev(x)) mod q
 * Since g = 7, 1/g = 8778 mod 12289.
 */
static const uint16_t iGMb[] = {
     4091,  4401,  1081,  1229,  2530,  6014,  7947,  5329,
     2579,  4751,  6464, 11703,  7023,  2812,  5890, 10698,
     3109,  2125,  1960, 10925, 10601, 10404,  4189,  1875,
     5847,  8546,  4615,  5190, 11324, 10578,  5882, 11155,
     8417, 12275, 10599,  7446,  5719,  3569,  5981, 10108,
     4426,  8306, 10755,  4679, 11052,  1538, 11857,   100,
     8247,  6625,  9725,  5145,  3412,  7858,  5831,  9460,
     5217, 10740,  7882,  7506, 12172, 11292,  6049,    79,
       13,  6938,  8886,  5453,  4586, 11455,  2903,  4676,
     9843,  7621,  8822,  9109,  2083,  8507,  8685,  3110,
     7015,  3269,  1367,  6397, 10259,  8435, 10527, 11559,
    11094,  2211,  1808,  7319,    48,  9547,  2560,  1228,
     9438, 10787, 11800,  1820, 11406,  8966,  6159,  3012,
     6109,  2796,  2203,  1652,   711,  7004,  1053,  8973,
     5244,  1517,  9322, 11269,   900,  3888, 11133, 10736,
     4949,  7616,  9974,  4746, 10270,   126,  2921,  6720,
     6635,  6543,  1582,  4868,    42,   673,  2240,  7219,
     1296, 11989,  7675,  8578, 11949,   989, 10541,  7687,
     7085,  8487,  1004, 10236,  4703,   163,  9143,  4597,
     6431, 12052,  2991, 11938,  4647,  3362,  2060, 11357,
    12011,  6664,  5655,  7225,  5914,  9327,  4092,  5880,
     6932,  3402,  5133,  9394, 11229,  5252,  9008,  1556,
     6908,  4773,  3853,  8780, 10325,  7737,  1758,  7103,
    11375, 12273,  8602,  3243,  6536,  7590,  8591, 11552,
     6101,  3253,  9969,  9640,  4506,  3736,  6829, 10822,
     9130,  9948,  3566,  2133,  3901,  6038,  7333,  6609,
     3468,  4659,   625,  2700,  7738,  3443,  3060,  3388,
     3526,  4418, 11911,  6232,  1730,  2558, 10340,  5344,
     5286,  2190, 11562,  6199,  2482,  8756,  5387,  4101,
     4609,  8605,  8226,   144,  5656,  8704,  2621,  5424,
    10812,  2959, 11346,  6249,  1715,  4951,  9540,  1888,
     3764,    39,  8219,  2080,  2502,  1469, 10550,  8709,
     5601,  1093,  3784,  5041,  2058,  8399, 11448,  9639,
     2059,  9878,  7405,  2496,  7918, 11594,   371,  7993,
     3073, 10326,    40, 10004,  9245,  7987,  5603,  4051,
     7894,   676, 11380,  7379,  6501,  4981,  2628,  3488,
    10956,  7022,  6737,  9933,  7139,  2330,  3884,  5473,
     7865,  6941,  5737,  5613,  9505, 11568, 11277,  2510,
     6689,   386,  4462,   105,  2076, 10443,   119,  3955,
     4370, 11505,  3672, 11439,   750,  3240,  3133,   754,
     4013, 11929,  9210,  5378, 11881, 11018,  2818,  1851,
     4966,  8181,  2688,  6205,  6814,   926,  2936,  4327,
    10175,  7089,  6047,  9410, 10492,  8950,  2472,  6255,
      728,  7569,  6056, 10432, 11036,  2452,  2811,  3787,
      945,  8998,  1244,  8815, 11017, 11218,  5894,  4325,
     4639,  3819,  9826,  7056,  6786,  8670,  5539,  7707,
     1361,  9812,  2949, 11265, 10301,  9108,   478,  6489,
      101,  1911,  9483,  3608, 11997, 10536,   812,  8915,
      637,  8159,  5299,  9128,  3512,  8290,  7068,  7922,
     3036,  4759,  2163,  3937,  3755, 11306,  7739,  4922,
    11932,   424,  5538,  6228, 11131,  7778, 11974,  1097,
     2890, 10027,  2569,  2250,  2352,   821,  2550, 11016,
     7769,   136,   617,  3157,  5889,  9219,  6855,   120,
     4405,  1825,  9635,  7214, 10261, 11393,  2441,  9562,
    11176,   599,  2085, 11465,  7233,  6177,  4801,  9926,
     9010,  4514,  9455, 11352, 11670,  6174,  7950,  9766,
     6896, 11603,  3213,  8473,  9873,  2835, 10422,  3732,
     7961,  1457, 10857,  8069,   832,  1628,  3410,  4900,
    10855,  5111,  9543,  6325,  7431,  4083,  3072,  8847,
     9853, 10122,  5259, 11413,  6556,   303,  1465,  3871,
     4873,  5813, 10017,  6898,  3311,  5947,  8637,  5852,
     3856,   928,  4933,  8530,  1871,  2184,  5571,  5879,
     3481, 11597,  9511,  8153,    35,  2609,  5963,  8064,
     1080, 12039,  8444,  3052,  3813, 11065,  6736,  8454,
     2340,  7651,  1910, 10709,  2117,  9637,  6402,  6028,
     2124,  7701,  2679,  5183,  6270,  7424,  2597,  6795,
     9222, 10837,   280,  8583,  3270,  6753,  2354,  3779,
     6102,  4732,  5926,  2497,  8640, 10289,  6107, 12127,
     2958, 12287, 10292,  8086,   817,  4021,  2610,  1444,
     5899, 11720,  3292,  2424,  5090,  7242,  5205,  5281,
     9956,  2702,  6656,   735,  2243, 11656,   833,  3107,
     6012,  6801,  1126,  6339,  5250, 10391,  9642,  5278,
     3513,  9769,  3025,   779,  9433,  3392,  7437,   668,
    10184,  8111,  6527,  6568, 10831,  6482,  8263,  5711,
     9780,   467,  5462,  4425, 11999,  1205,  5015,  6918,
     5096,  3827,  5525, 11579,  3518,  4875,  7388,  1931,
     6615,  1541,  8708,   260,  3385,  4792,  4391,  5697,
     7895,  2155,  7337,   236, 10635, 11534,  1906,  4793,
     9527,  7239,  8354,  5121, 10662,  2311,  3346,  8556,
      707,  1088,  4936,   678, 10245,    18,  5684,   960,
     4459,  7957,   226,  2451,     6,  8874,   320,  6298,
     8963,  8735,  2852,  2981,  1707,  5408,  5017,  9876,
     9790,  2968,  1899,  6729,  4183,  5290, 10084,  7679,
     7941,  8744,  5694,  3461,  4175,  5747,  5561,  3378,
     5227,   952,  4319,  9810,  4356,  3088, 11118,   840,
     6257,   486,  6000,  1342, 10382,  6017,  4798,  5489,
     4498,  4193,  2306,  6521,  1475,  6372,  9029,  8037,
     1625,  7020,  4740,  5730,  7956,  6351,  6494,  6917,
    11405,  7487, 10202, 10155,  7666,  7556, 11509,  1546,
     6571, 10199,  2265,  7327,  5824, 11396, 11581,  9722,
     2251, 11199,  5356,  7408,  2861,  4003,  9215,   484,
     7526,  9409, 12235,  6157,  9025,  2121, 10255,  2519,
     9533,  3824,  8674, 11419, 10888,  4762, 11303,  4097,
     2414,  6496,  9953, 10554,   808,  2999,  2130,  4286,
    12078,  7445,  5132,  7915,   245,  5974,  4874,  7292,
     7560, 10539,  9952,  9075,  2113,  3721, 10285, 10022,
     9578,  8934, 11074,  9498,   294,  4711,  3391,  1377,
     9072, 10189,  4569, 10890,  9909,  6923,    53,  4653,
      439, 10253,  7028, 10207,  8343,  1141,  2556,  7601,
     8150, 10630,  8648,  9832,  7951, 11245,  2131,  5765,
    10343,  9781,  2718,  1419,  4531,  3844,  4066,  4293,
    11657, 11525, 11353,  4313,  4869, 12186,  1611, 10892,
    11489,  8833,  2393,    15, 10830,  5003,    17,   565,
     5891, 12177, 11058, 10412,  8885,  3974, 10981,  7130,
     5840, 10482,  8338,  6035,  6964,  1574, 10936,  2020,
     2465,  8191,   384,  2642,  2729,  5399,  2175,  9396,
    11987,  8035,  4375,  6611,  5010, 11812,  9131, 11427,
      104,  6348,  9643,  6757, 12110,  5617, 10935,   541,
      135,  3041,  7200,  6526,  5085, 12136,   842,  4129,
     7685, 11079,  8426,  1008,  2725, 11772,  6058,  1101,
     1950,  8424,  5688,  6876, 12005, 10079,  5335,   927,
     1770,   273,  8377,  2271,  5225, 10283,   116, 11807,
       91, 11699,   757,  1304,  7524,  6451,  8032,  8154,
     7456,  4191,   309,  2318,  2292, 10393, 11639,  9481,
    12238, 10594,  9569,  7912, 10368,  9889, 12244,  7179,
     3924,  3188,   367,  2077,   336,  5384,  5631,  8596,
     4621,  1775,  8866,   451,  6108,  1317,  6246,  8795,
     5896,  7283,  3132, 11564,  4977, 12161,  7371,  1366,
    12130, 10619,  3809,  5149,  6300,  2638,  4197,  1418,
    10065,  4156,  8373,  8644, 10445,   882,  8158, 10173,
     9763, 12191,   459,  2966,  3166,   405,  5000,  9311,
     6404,  8986,  1551,  8175,  3630, 10766,  9265,   700,
     8573,  9508,  6630, 11437, 11595,  5850,  3950,  4775,
    11941,  1446,  6018,  3386, 11470,  5310,  5476,   553,
     9474,  2586,  1431,  2741,   473, 11383,  4745,   836,
     4062, 10666,  7727, 11752,  5534,   312,  4307,  4351,
     5764,  8679,  8381,  8187,     5,  7395,  4363,  1152,
     5421,  5231,  6473,   436,  7567,  8603,  6229,  8230
};

/*
 * Reduce a small signed integer modulo q. The source integer MUST
 * be between -q/2 and +q/2.
 */
//x가 양수이면 그냥 x를 return하고 x가 음수이면 x+12289의 값을 return EYSEO
//mod 12289 이니깐 필요한 연산, 그런데 return type이 uint16_t여도 상관이 없을것 같은데..
//이런 부분도 SCA에 취약할 것 같은데..
static inline uint32_t
mq_conv_small(int x)
{
    /*
     * If x < 0, the cast to uint32_t will set the high bit to 1.
     */
    uint32_t y;

    y = (uint32_t)x;
    y += Q & -(y >> 31);
    return y;
}
/*
 * Reduce a small signed integer modulo q. The source integer MUST
 * be between -q/2 and +q/2.
 */
//x가 양수이면 그냥 x를 return하고 x가 음수이면 x+12289의 값을 return EYSEO
//mod 12289 이니깐 필요한 연산, 그런데 return type이 uint16_t여도 상관이 없을것 같은데..
//이런 부분도 SCA에 취약할 것 같은데..
static inline uint16_t
mq_conv_to_positive(int16_t x)
{
    /*
     * If x < 0, the cast to uint32_t will set the high bit to 1.
     */
    uint32_t y;

    y = (uint32_t)x;
    y += Q & -(y >> 31);
    return (uint16_t)y;
}
static inline int16_t
mq_conv_to_center(uint16_t x)
{
    return (int16_t)(x - (12289&(((x-6145)>>15)^0xffff)));//modified by EYSEO
   //return (int16_t)(x - (12289 & (((x - ((12289 + 1) >> 1)) >> 31) - 1)));
}

/*
 * Addition modulo q. Operands must be in the 0..q-1 range.
 */
//mq_add와 mq_sub도 uint16_t를 받아서 그것을 return해도 될 것 같은데 EYSEO
static inline uint32_t
mq_add(uint32_t x, uint32_t y)
{
    /*
     * We compute x + y - q. If the result is negative, then the
     * high bit will be set, and 'd >> 31' will be equal to 1;
     * thus '-(d >> 31)' will be an all-one pattern. Otherwise,
     * it will be an all-zero pattern. In other words, this
     * implements a conditional addition of q.
     */
    uint32_t d;

    d = x + y - Q;
    d += Q & -(d >> 31);
    return d;
}

static inline uint16_t
mq_add_16(uint16_t x, uint16_t y)
{
    /*
     * We compute x + y - q. If the result is negative, then the
     * high bit will be set, and 'd >> 31' will be equal to 1;
     * thus '-(d >> 31)' will be an all-one pattern. Otherwise,
     * it will be an all-zero pattern. In other words, this
     * implements a conditional addition of q.
     */
    uint16_t d;

    d = x + y - Q;
    d += Q & -(d >> 15);
    return d;
}

/*
 * Subtraction modulo q. Operands must be in the 0..q-1 range.
 */
static inline uint32_t
mq_sub(uint32_t x, uint32_t y)
{
    /*
     * As in mq_add(), we use a conditional addition to ensure the
     * result is in the 0..q-1 range.
     */
    uint32_t d;

    d = x - y;
    d += Q & -(d >> 31);
    return d;
}
/*
 * Subtraction modulo q. Operands must be in the 0..q-1 range.
 */
static inline uint16_t
mq_sub_16(uint16_t x, uint16_t y)
{
    /*
     * As in mq_add(), we use a conditional addition to ensure the
     * result is in the 0..q-1 range.
     */
    uint16_t d;

    d = x - y;
    d += Q & -(d >> 15);
    return d;
}

/*
 * Division by 2 modulo q. Operand must be in the 0..q-1 range.
 */
static inline uint32_t
mq_rshift1(uint32_t x)
{
    x += Q & -(x & 1);
    return (x >> 1);
}

/*
 * Montgomery multiplication modulo q. If we set R = 2^16 mod q, then
 * this function computes: x * y / R mod q
 * Operands must be in the 0..q-1 range.
 */
//original uint32_t input/output version
static inline uint32_t
mq_montymul(uint32_t x, uint32_t y)
{
    uint32_t z, w;

    /*
     * We compute x*y + k*q with a value of k chosen so that the 16
     * low bits of the result are 0. We can then shift the value.
     * After the shift, result may still be larger than q, but it
     * will be lower than 2*q, so a conditional subtraction works.
     */

    z = x * y;
    w = ((z * Q0I) & 0xFFFF) * Q;

    /*
     * When adding z and w, the result will have its low 16 bits
     * equal to 0. Since x, y and z are lower than q, the sum will
     * be no more than (2^15 - 1) * q + (q - 1)^2, which will
     * fit on 29 bits.
     */
    z = (z + w) >> 16;

    /*
     * After the shift, analysis shows that the value will be less
     * than 2q. We do a subtraction then conditional subtraction to
     * ensure the result is in the expected range.
     */
    z -= Q;
    z += Q & -(z >> 31);
    return z;
}

/*
 * Montgomery multiplication modulo q. If we set R = 2^16 mod q, then
 * this function computes: x * y / R mod q
 * Operands must be in the 0..q-1 range.
 */
static inline uint16_t
mq_montymul_16(uint16_t x, uint16_t y)
{
    uint32_t z, w;

    /*
     * We compute x*y + k*q with a value of k chosen so that the 16
     * low bits of the result are 0. We can then shift the value.
     * After the shift, result may still be larger than q, but it
     * will be lower than 2*q, so a conditional subtraction works.
     */

    z = ((uint32_t)x) * ((uint32_t)y);
    w = ((z * Q0I) & 0xFFFF) * Q;

    /*
     * When adding z and w, the result will have its low 16 bits
     * equal to 0. Since x, y and z are lower than q, the sum will
     * be no more than (2^15 - 1) * q + (q - 1)^2, which will
     * fit on 29 bits.
     */
    z = (z + w) >> 16;

    /*
     * After the shift, analysis shows that the value will be less
     * than 2q. We do a subtraction then conditional subtraction to
     * ensure the result is in the expected range.
     */
    z -= Q;
    z += Q & -(z >> 31);
    return (uint16_t)z;
}

/*
 * Montgomery squaring (computes (x^2)/R).
 */
static inline uint32_t
mq_montysqr(uint32_t x)
{
    return mq_montymul(x, x);
}

/*
 * Divide x by y modulo q = 12289.
 */
static inline uint32_t
mq_div_12289(uint32_t x, uint32_t y)
{
    /*
     * We invert y by computing y^(q-2) mod q.
     *
     * We use the following addition chain for exponent e = 12287:
     *
     *   e0 = 1
     *   e1 = 2 * e0 = 2
     *   e2 = e1 + e0 = 3
     *   e3 = e2 + e1 = 5
     *   e4 = 2 * e3 = 10
     *   e5 = 2 * e4 = 20
     *   e6 = 2 * e5 = 40
     *   e7 = 2 * e6 = 80
     *   e8 = 2 * e7 = 160
     *   e9 = e8 + e2 = 163
     *   e10 = e9 + e8 = 323
     *   e11 = 2 * e10 = 646
     *   e12 = 2 * e11 = 1292
     *   e13 = e12 + e9 = 1455
     *   e14 = 2 * e13 = 2910
     *   e15 = 2 * e14 = 5820
     *   e16 = e15 + e10 = 6143
     *   e17 = 2 * e16 = 12286
     *   e18 = e17 + e0 = 12287
     *
     * Additions on exponents are converted to Montgomery
     * multiplications. We define all intermediate results as so
     * many local variables, and let the C compiler work out which
     * must be kept around.
     */
    uint32_t y0, y1, y2, y3, y4, y5, y6, y7, y8, y9;
    uint32_t y10, y11, y12, y13, y14, y15, y16, y17, y18;

    y0 = mq_montymul(y, QR2);
    y1 = mq_montysqr(y0);
    y2 = mq_montymul(y1, y0);
    y3 = mq_montymul(y2, y1);
    y4 = mq_montysqr(y3);
    y5 = mq_montysqr(y4);
    y6 = mq_montysqr(y5);
    y7 = mq_montysqr(y6);
    y8 = mq_montysqr(y7);
    y9 = mq_montymul(y8, y2);
    y10 = mq_montymul(y9, y8);
    y11 = mq_montysqr(y10);
    y12 = mq_montysqr(y11);
    y13 = mq_montymul(y12, y9);
    y14 = mq_montysqr(y13);
    y15 = mq_montysqr(y14);
    y16 = mq_montymul(y15, y10);
    y17 = mq_montysqr(y16);
    y18 = mq_montymul(y17, y0);

    /*
     * Final multiplication with x, which is not in Montgomery
     * representation, computes the correct division result.
     */
    return mq_montymul(y18, x);
}

/*
 * Compute NTT on a ring element.
 */
static void
mq_NTT(uint16_t *a, unsigned logn)
{
    size_t n, t, m;

    n = (size_t)1 << logn;
    t = n;
    for (m = 1; m < n; m <<= 1) {
        size_t ht, i, j1;

        ht = t >> 1;
        for (i = 0, j1 = 0; i < m; i ++, j1 += t) {
            size_t j, j2;
            uint32_t s;

            s = GMb[m + i];
            j2 = j1 + ht;
            for (j = j1; j < j2; j ++) {
                uint32_t u, v;

                u = a[j];
                v = mq_montymul(a[j + ht], s);
                a[j] = (uint16_t)mq_add(u, v);
                a[j + ht] = (uint16_t)mq_sub(u, v);
            }
        }
        t = ht;
    }
}

/*
 * Compute the inverse NTT on a ring element, binary case.
 */
static void
mq_iNTT(uint16_t *a, unsigned logn)
{
    size_t n, t, m;
    uint32_t ni;

    n = (size_t)1 << logn;
    t = 1;
    m = n;
    while (m > 1) {
        size_t hm, dt, i, j1;

        hm = m >> 1;
        dt = t << 1;
        for (i = 0, j1 = 0; i < hm; i ++, j1 += dt) {
            size_t j, j2;
            uint32_t s;

            j2 = j1 + t;
            s = iGMb[hm + i];
            for (j = j1; j < j2; j ++) {
                uint32_t u, v, w;

                u = a[j];
                v = a[j + t];
                a[j] = (uint16_t)mq_add(u, v);
                w = mq_sub(u, v);
                a[j + t] = (uint16_t)
                    mq_montymul(w, s);
            }
        }
        t = dt;
        m = hm;
    }

    /*
     * To complete the inverse NTT, we must now divide all values by
     * n (the vector size). We thus need the inverse of n, i.e. we
     * need to divide 1 by 2 logn times. But we also want it in
     * Montgomery representation, i.e. we also want to multiply it
     * by R = 2^16. In the common case, this should be a simple right
     * shift. The loop below is generic and works also in corner cases;
     * its computation time is negligible.
     */
    ni = QR;
    for (m = n; m > 1; m >>= 1) {
        ni = mq_rshift1(ni);
    }
    for (m = 0; m < n; m ++) {
        a[m] = (uint16_t)mq_montymul(a[m], ni);
    }
}

/*
 * Convert a polynomial (mod q) to Montgomery representation.
 */
static void
mq_poly_tomonty(uint16_t *f, unsigned logn)
{
    size_t u, n;

    n = (size_t)1 << logn;
    for (u = 0; u < n; u ++) {
        f[u] = (uint16_t)mq_montymul(f[u], QR2);
    }
}

/*
 * Multiply two polynomials together (NTT representation, and using
 * a Montgomery multiplication). Result f*g is written over f.
 */
static void
mq_poly_montymul_ntt(uint16_t *f, const uint16_t *g, unsigned logn)
{
    size_t u, n;

    n = (size_t)1 << logn;
    for (u = 0; u < n; u ++) {
        f[u] = (uint16_t)mq_montymul(f[u], g[u]);
    }
}

/*
 * Subtract polynomial g from polynomial f.
 */
static void
mq_poly_sub(uint16_t *f, const uint16_t *g, unsigned logn)
{
    size_t u, n;

    n = (size_t)1 << logn;
    for (u = 0; u < n; u ++) {
        f[u] = (uint16_t)mq_sub(f[u], g[u]);
    }
}

/* ===================================================================== */


#endif
