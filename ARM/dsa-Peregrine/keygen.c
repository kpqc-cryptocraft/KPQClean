/*
 * Peregrine Key Generation
 * Many functions are adopted from FALCON Implementation
 * and some new functions are implemented for Peregrine operations. 
 * by Eun-Young Seo
 * Version: Nov. 16, 2022
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

#include "inner.h"
#include <stdio.h>//EYSEO
#include <stdlib.h>//EYSEO
#define MKN(logn)   ((size_t)1 << (logn))

/* ==================================================================== */
/*
 * Modular arithmetics.
 *
 * We implement a few functions for computing modulo a small integer p.
 *
 * All functions require that 2^30 < p < 2^31. Moreover, operands must
 * be in the 0..p-1 range.
 *
 * Modular addition and subtraction work for all such p.
 *
 * Montgomery multiplication requires that p is odd, and must be provided
 * with an additional value p0i = -1/p mod 2^31. See below for some basics
 * on Montgomery multiplication.
 *
 * Division computes an inverse modulo p by an exponentiation (with
 * exponent p-2): this works only if p is prime. Multiplication
 * requirements also apply, i.e. p must be odd and p0i must be provided.
 *
 * The NTT and inverse NTT need all of the above, and also that
 * p = 1 mod 2048.
 *
 * -----------------------------------------------------------------------
 *
 * We use Montgomery representation with 31-bit values:
 *
 *   Let R = 2^31 mod p. When 2^30 < p < 2^31, R = 2^31 - p.
 *   Montgomery representation of an integer x modulo p is x*R mod p.
 *
 *   Montgomery multiplication computes (x*y)/R mod p for
 *   operands x and y. Therefore:
 *
 *    - if operands are x*R and y*R (Montgomery representations of x and
 *      y), then Montgomery multiplication computes (x*R*y*R)/R = (x*y)*R
 *      mod p, which is the Montgomery representation of the product x*y;
 *
 *    - if operands are x*R and y (or x and y*R), then Montgomery
 *      multiplication returns x*y mod p: mixed-representation
 *      multiplications yield results in normal representation.
 *
 * To convert to Montgomery representation, we multiply by R, which is done
 * by Montgomery-multiplying by R^2. Stand-alone conversion back from
 * Montgomery representation is Montgomery-multiplication by 1.
 */

/*
 * Precomputed small primes. Each element contains the following:
 *
 *  p   The prime itself.
 *
 *  g   A primitive root of phi = X^N+1 (in field Z_p).
 *
 *  s   The inverse of the product of all previous primes in the array,
 *      computed modulo p and in Montgomery representation.
 *
 * All primes are such that p = 1 mod 2048, and are lower than 2^31. They
 * are listed in decreasing order.
 */

/*
 * Compute 2^(31*x) modulo p. This works for integers x up to 2^11.
 * p must be prime such that 2^30 < p < 2^31; p0i must be equal to
 * -1/p mod 2^31; R2 must be equal to 2^62 mod p.
 */
static inline uint32_t
modp_Rx(unsigned x, uint32_t p, uint32_t p0i, uint32_t R2)
{
	int i;
	uint32_t r, z;

	/*
	 * 2^(31*x) = (2^31)*(2^(31*(x-1))); i.e. we want the Montgomery
	 * representation of (2^31)^e mod p, where e = x-1.
	 * R2 is 2^31 in Montgomery representation.
	 */
	x --;
	r = R2;
	z = modp_R(p);
	for (i = 0; (1U << i) <= x; i ++) {
		if ((x & (1U << i)) != 0) {
			z = modp_montymul(z, r, p, p0i);
		}
		r = modp_montymul(r, r, p, p0i);
	}
	return z;
}

/*
 * Simplified macros for NTT and iNTT (binary case) when the elements
 * are consecutive in RAM.
 */
#define modp_NTT2(a, gm, logn, p, p0i)   modp_NTT2_ext(a, 1, gm, logn, p, p0i)
#define modp_iNTT2(a, igm, logn, p, p0i) modp_iNTT2_ext(a, 1, igm, logn, p, p0i)

/*
 * Given polynomial f in NTT representation modulo p, compute f' of degree
 * less than N/2 such that f' = f0^2 - X*f1^2, where f0 and f1 are
 * polynomials of degree less than N/2 such that f = f0(X^2) + X*f1(X^2).
 *
 * The new polynomial is written "in place" over the first N/2 elements
 * of f.
 *
 * If applied logn times successively on a given polynomial, the resulting
 * degree-0 polynomial is the resultant of f and X^N+1 modulo p.
 *
 * This function applies only to the binary case; it is invoked from
 * solve_NTRU_binary_depth1().
 */
static void
modp_poly_rec_res(uint32_t *f, unsigned logn,
	uint32_t p, uint32_t p0i, uint32_t R2)
{
	size_t hn, u;

	hn = (size_t)1 << (logn - 1);
	for (u = 0; u < hn; u ++) {
		uint32_t w0, w1;

		w0 = f[(u << 1) + 0];
		w1 = f[(u << 1) + 1];
		f[u] = modp_montymul(modp_montymul(w0, w1, p, p0i), R2, p, p0i);
	}
}

/* ==================================================================== */
/*
 * Custom bignum implementation.
 *
 * This is a very reduced set of functionalities. We need to do the
 * following operations:
 *
 *  - Rebuild the resultant and the polynomial coefficients from their
 *    values modulo small primes (of length 31 bits each).
 *
 *  - Compute an extended GCD between the two computed resultants.
 *
 *  - Extract top bits and add scaled values during the successive steps
 *    of Babai rounding.
 *
 * When rebuilding values using CRT, we must also recompute the product
 * of the small prime factors. We always do it one small factor at a
 * time, so the "complicated" operations can be done modulo the small
 * prime with the modp_* functions. CRT coefficients (inverses) are
 * precomputed.
 *
 * All values are positive until the last step: when the polynomial
 * coefficients have been rebuilt, we normalize them around 0. But then,
 * only additions and subtractions on the upper few bits are needed
 * afterwards.
 *
 * We keep big integers as arrays of 31-bit words (in uint32_t values);
 * the top bit of each uint32_t is kept equal to 0. Using 31-bit words
 * makes it easier to keep track of carries. When negative values are
 * used, two's complement is used.
 */

/*
 * Subtract integer b from integer a. Both integers are supposed to have
 * the same size. The carry (0 or 1) is returned. Source arrays a and b
 * MUST be distinct.
 *
 * The operation is performed as described above if ctr = 1. If
 * ctl = 0, the value a[] is unmodified, but all memory accesses are
 * still performed, and the carry is computed and returned.
 */
static uint32_t
zint_sub(uint32_t *restrict a, const uint32_t *restrict b, size_t len,
	uint32_t ctl)
{
	size_t u;
	uint32_t cc, m;

	cc = 0;
	m = -ctl;
	for (u = 0; u < len; u ++) {
		uint32_t aw, w;

		aw = a[u];
		w = aw - b[u] - cc;
		cc = w >> 31;
		aw ^= ((w & 0x7FFFFFFF) ^ aw) & m;
		a[u] = aw;
	}
	return cc;
}

/*
 * Mutiply the provided big integer m with a small value x.
 * This function assumes that x < 2^31. The carry word is returned.
 */
static uint32_t
zint_mul_small(uint32_t *m, size_t mlen, uint32_t x)
{
	size_t u;
	uint32_t cc;

	cc = 0;
	for (u = 0; u < mlen; u ++) {
		uint64_t z;

		z = (uint64_t)m[u] * (uint64_t)x + cc;
		m[u] = (uint32_t)z & 0x7FFFFFFF;
		cc = (uint32_t)(z >> 31);
	}
	return cc;
}

/*
 * Reduce a big integer d modulo a small integer p.
 * Rules:
 *  d is unsigned
 *  p is prime
 *  2^30 < p < 2^31
 *  p0i = -(1/p) mod 2^31
 *  R2 = 2^62 mod p
 */
//이 함수에서는 while문이 있지만 various time은 아닌것 같음. EYSEO
static uint32_t
zint_mod_small_unsigned(const uint32_t *d, size_t dlen,
	uint32_t p, uint32_t p0i, uint32_t R2)
{
	uint32_t x;
	size_t u;

	/*
	 * Algorithm: we inject words one by one, starting with the high
	 * word. Each step is:
	 *  - multiply x by 2^31
	 *  - add new word
	 */
	x = 0;
	u = dlen;
    //printf("\nzint_mod_small_unsigned dlen:[%d]", dlen);
	while (u -- > 0) {
		uint32_t w;

		x = modp_montymul(x, R2, p, p0i);
		w = d[u] - p;
		w += p & -(w >> 31);
		x = modp_add(x, w, p);
        //printf("w:[%d]",w);
	}
	return x;
}

/*
 * Similar to zint_mod_small_unsigned(), except that d may be signed.
 * Extra parameter is Rx = 2^(31*dlen) mod p.
 */
static uint32_t
zint_mod_small_signed(const uint32_t *d, size_t dlen,
	uint32_t p, uint32_t p0i, uint32_t R2, uint32_t Rx)
{
	uint32_t z;

	if (dlen == 0) {
		return 0;
	}
	z = zint_mod_small_unsigned(d, dlen, p, p0i, R2);
	z = modp_sub(z, Rx & -(d[dlen - 1] >> 30), p);
	return z;
}

/*
 * Add y*s to x. x and y initially have length 'len' words; the new x
 * has length 'len+1' words. 's' must fit on 31 bits. x[] and y[] must
 * not overlap.
 */
static void
zint_add_mul_small(uint32_t *restrict x,
	const uint32_t *restrict y, size_t len, uint32_t s)
{
	size_t u;
	uint32_t cc;

	cc = 0;
	for (u = 0; u < len; u ++) {
		uint32_t xw, yw;
		uint64_t z;

		xw = x[u];
		yw = y[u];
		z = (uint64_t)yw * (uint64_t)s + (uint64_t)xw + (uint64_t)cc;
		x[u] = (uint32_t)z & 0x7FFFFFFF;
		cc = (uint32_t)(z >> 31);
	}
	x[len] = cc;
}

/*
 * Normalize a modular integer around 0: if x > p/2, then x is replaced
 * with x - p (signed encoding with two's complement); otherwise, x is
 * untouched. The two integers x and p are encoded over the same length.
 */
static void
zint_norm_zero(uint32_t *restrict x, const uint32_t *restrict p, size_t len)
{
	size_t u;
	uint32_t r, bb;

	/*
	 * Compare x with p/2. We use the shifted version of p, and p
	 * is odd, so we really compare with (p-1)/2; we want to perform
	 * the subtraction if and only if x > (p-1)/2.
	 */
	r = 0;
	bb = 0;
	u = len;
	while (u -- > 0) {
		uint32_t wx, wp, cc;

		/*
		 * Get the two words to compare in wx and wp (both over
		 * 31 bits exactly).
		 */
		wx = x[u];
		wp = (p[u] >> 1) | (bb << 30);
		bb = p[u] & 1;

		/*
		 * We set cc to -1, 0 or 1, depending on whether wp is
		 * lower than, equal to, or greater than wx.
		 */
		cc = wp - wx;
		cc = ((-cc) >> 31) | -(cc >> 31);

		/*
		 * If r != 0 then it is either 1 or -1, and we keep its
		 * value. Otherwise, if r = 0, then we replace it with cc.
		 */
		r |= cc & ((r & 1) - 1);
	}

	/*
	 * At this point, r = -1, 0 or 1, depending on whether (p-1)/2
	 * is lower than, equal to, or greater than x. We thus want to
	 * do the subtraction only if r = -1.
	 */
	zint_sub(x, p, len, r >> 31);
}

/*
 * Rebuild integers from their RNS representation. There are 'num'
 * integers, and each consists in 'xlen' words. 'xx' points at that
 * first word of the first integer; subsequent integers are accessed
 * by adding 'xstride' repeatedly.
 *
 * The words of an integer are the RNS representation of that integer,
 * using the provided 'primes' are moduli. This function replaces
 * each integer with its multi-word value (little-endian order).
 *
 * If "normalize_signed" is non-zero, then the returned value is
 * normalized to the -m/2..m/2 interval (where m is the product of all
 * small prime moduli); two's complement is used for negative values.
 */
static void
zint_rebuild_CRT(uint32_t *restrict xx, size_t xlen, size_t xstride,
	size_t num, const small_prime *primes, int normalize_signed,
	uint32_t *restrict tmp)
{
	size_t u;
	uint32_t *x;

	tmp[0] = primes[0].p;
	for (u = 1; u < xlen; u ++) {
		/*
		 * At the entry of each loop iteration:
		 *  - the first u words of each array have been
		 *    reassembled;
		 *  - the first u words of tmp[] contains the
		 * product of the prime moduli processed so far.
		 *
		 * We call 'q' the product of all previous primes.
		 */
		uint32_t p, p0i, s, R2;
		size_t v;

		p = primes[u].p;
		s = primes[u].s;
		p0i = modp_ninv31(p);
		R2 = modp_R2(p, p0i);

		for (v = 0, x = xx; v < num; v ++, x += xstride) {
			uint32_t xp, xq, xr;
			/*
			 * xp = the integer x modulo the prime p for this
			 *      iteration
			 * xq = (x mod q) mod p
			 */
			xp = x[u];
            //xq가 한 word로 표현이 안될 수도 있으니, EYSEO
            //이 경우 여러 word로 된 숫자로 mod p를 취해서 한 word로 저장
			xq = zint_mod_small_unsigned(x, u, p, p0i, R2);

			/*
			 * New value is (x mod q) + q * (s * (xp - xq) mod p)
			 */
            //s*(xp-xq) mod p 계산 EYSEO
            //여기서 s, xp, xq 는 모두 한 word로 저장되어 있음
            //s는 montgomery form 이고, xr은 montgomery form이 아님
			xr = modp_montymul(s, modp_sub(xp, xq, p), p, p0i);
            //x mod q + q * xr 를 계산하는데
            //x mod q 와 q 는 여러 word로 저장되어 있기 때문에 아래와 같은 함수 정의해서 사용
			zint_add_mul_small(x, tmp, u, xr);
		}

		/*
		 * Update product of primes in tmp[].
		 */
        //CRT에서 p1*p2*..pi 를 계산하는 과정 EYSEO
        //이것도 미리 저장해 놓으면 되지 않나? 뭐 계산 속도에 별 차이 없으니깐..
		tmp[u] = zint_mul_small(tmp, u, p);
	}

	/*
	 * Normalize the reconstructed values around 0.
	 */
	if (normalize_signed) {
		for (u = 0, x = xx; u < num; u ++, x += xstride) {
			zint_norm_zero(x, tmp, xlen);
		}
	}
}

/*
 * Negate a big integer conditionally: value a is replaced with -a if
 * and only if ctl = 1. Control value ctl must be 0 or 1.
 */
static void
zint_negate(uint32_t *a, size_t len, uint32_t ctl)
{
	size_t u;
	uint32_t cc, m;

	/*
	 * If ctl = 1 then we flip the bits of a by XORing with
	 * 0x7FFFFFFF, and we add 1 to the value. If ctl = 0 then we XOR
	 * with 0 and add 0, which leaves the value unchanged.
	 */
	cc = ctl;
	m = -ctl >> 1;
	for (u = 0; u < len; u ++) {
		uint32_t aw;

		aw = a[u];
		aw = (aw ^ m) + cc;
		a[u] = aw & 0x7FFFFFFF;
		cc = aw >> 31;
	}
}

/*
 * Replace a with (a*xa+b*xb)/(2^31) and b with (a*ya+b*yb)/(2^31).
 * The low bits are dropped (the caller should compute the coefficients
 * such that these dropped bits are all zeros). If either or both
 * yields a negative value, then the value is negated.
 *
 * Returned value is:
 *  0  both values were positive
 *  1  new a had to be negated
 *  2  new b had to be negated
 *  3  both new a and new b had to be negated
 *
 * Coefficients xa, xb, ya and yb may use the full signed 32-bit range.
 */
static uint32_t
zint_co_reduce(uint32_t *a, uint32_t *b, size_t len,
	int64_t xa, int64_t xb, int64_t ya, int64_t yb)
{
	size_t u;
	int64_t cca, ccb;
	uint32_t nega, negb;

	cca = 0;
	ccb = 0;
	for (u = 0; u < len; u ++) {
		uint32_t wa, wb;
		uint64_t za, zb;

		wa = a[u];
		wb = b[u];
		za = wa * (uint64_t)xa + wb * (uint64_t)xb + (uint64_t)cca;
		zb = wa * (uint64_t)ya + wb * (uint64_t)yb + (uint64_t)ccb;
		if (u > 0) {
			a[u - 1] = (uint32_t)za & 0x7FFFFFFF;
			b[u - 1] = (uint32_t)zb & 0x7FFFFFFF;
		}
		cca = *(int64_t *)&za >> 31;
		ccb = *(int64_t *)&zb >> 31;
	}
	a[len - 1] = (uint32_t)cca;
	b[len - 1] = (uint32_t)ccb;

	nega = (uint32_t)((uint64_t)cca >> 63);
	negb = (uint32_t)((uint64_t)ccb >> 63);
	zint_negate(a, len, nega);
	zint_negate(b, len, negb);
	return nega | (negb << 1);
}

/*
 * Finish modular reduction. Rules on input parameters:
 *
 *   if neg = 1, then -m <= a < 0
 *   if neg = 0, then 0 <= a < 2*m
 *
 * If neg = 0, then the top word of a[] is allowed to use 32 bits.
 *
 * Modulus m must be odd.
 */
static void
zint_finish_mod(uint32_t *a, size_t len, const uint32_t *m, uint32_t neg)
{
	size_t u;
	uint32_t cc, xm, ym;

	/*
	 * First pass: compare a (assumed nonnegative) with m. Note that
	 * if the top word uses 32 bits, subtracting m must yield a
	 * value less than 2^31 since a < 2*m.
	 */
	cc = 0;
	for (u = 0; u < len; u ++) {
		cc = (a[u] - m[u] - cc) >> 31;
	}

	/*
	 * If neg = 1 then we must add m (regardless of cc)
	 * If neg = 0 and cc = 0 then we must subtract m
	 * If neg = 0 and cc = 1 then we must do nothing
	 *
	 * In the loop below, we conditionally subtract either m or -m
	 * from a. Word xm is a word of m (if neg = 0) or -m (if neg = 1);
	 * but if neg = 0 and cc = 1, then ym = 0 and it forces mw to 0.
	 */
	xm = -neg >> 1;
	ym = -(neg | (1 - cc));
	cc = neg;
	for (u = 0; u < len; u ++) {
		uint32_t aw, mw;

		aw = a[u];
		mw = (m[u] ^ xm) & ym;
		aw = aw - mw - cc;
		a[u] = aw & 0x7FFFFFFF;
		cc = aw >> 31;
	}
}

/*
 * Replace a with (a*xa+b*xb)/(2^31) mod m, and b with
 * (a*ya+b*yb)/(2^31) mod m. Modulus m must be odd; m0i = -1/m[0] mod 2^31.
 */
static void
zint_co_reduce_mod(uint32_t *a, uint32_t *b, const uint32_t *m, size_t len,
	uint32_t m0i, int64_t xa, int64_t xb, int64_t ya, int64_t yb)
{
	size_t u;
	int64_t cca, ccb;
	uint32_t fa, fb;

	/*
	 * These are actually four combined Montgomery multiplications.
	 */
	cca = 0;
	ccb = 0;
	fa = ((a[0] * (uint32_t)xa + b[0] * (uint32_t)xb) * m0i) & 0x7FFFFFFF;
	fb = ((a[0] * (uint32_t)ya + b[0] * (uint32_t)yb) * m0i) & 0x7FFFFFFF;
	for (u = 0; u < len; u ++) {
		uint32_t wa, wb;
		uint64_t za, zb;

		wa = a[u];
		wb = b[u];
		za = wa * (uint64_t)xa + wb * (uint64_t)xb
			+ m[u] * (uint64_t)fa + (uint64_t)cca;
		zb = wa * (uint64_t)ya + wb * (uint64_t)yb
			+ m[u] * (uint64_t)fb + (uint64_t)ccb;
		if (u > 0) {
			a[u - 1] = (uint32_t)za & 0x7FFFFFFF;
			b[u - 1] = (uint32_t)zb & 0x7FFFFFFF;
		}
		cca = *(int64_t *)&za >> 31;
		ccb = *(int64_t *)&zb >> 31;
	}
	a[len - 1] = (uint32_t)cca;
	b[len - 1] = (uint32_t)ccb;

	/*
	 * At this point:
	 *   -m <= a < 2*m
	 *   -m <= b < 2*m
	 * (this is a case of Montgomery reduction)
	 * The top words of 'a' and 'b' may have a 32-th bit set.
	 * We want to add or subtract the modulus, as required.
	 */
	zint_finish_mod(a, len, m, (uint32_t)((uint64_t)cca >> 63));
	zint_finish_mod(b, len, m, (uint32_t)((uint64_t)ccb >> 63));
}

/*
 * Compute a GCD between two positive big integers x and y. The two
 * integers must be odd. Returned value is 1 if the GCD is 1, 0
 * otherwise. When 1 is returned, arrays u and v are filled with values
 * such that:
 *   0 <= u <= y
 *   0 <= v <= x
 *   x*u - y*v = 1
 * x[] and y[] are unmodified. Both input values must have the same
 * encoded length. Temporary array must be large enough to accommodate 4
 * extra values of that length. Arrays u, v and tmp may not overlap with
 * each other, or with either x or y.
 */
static int
zint_bezout(uint32_t *restrict u, uint32_t *restrict v,
	const uint32_t *restrict x, const uint32_t *restrict y,
	size_t len, uint32_t *restrict tmp)
{
	/*
	 * Algorithm is an extended binary GCD. We maintain 6 values
	 * a, b, u0, u1, v0 and v1 with the following invariants:
	 *
	 *  a = x*u0 - y*v0
	 *  b = x*u1 - y*v1
	 *  0 <= a <= x
	 *  0 <= b <= y
	 *  0 <= u0 < y
	 *  0 <= v0 < x
	 *  0 <= u1 <= y
	 *  0 <= v1 < x
	 *
	 * Initial values are:
	 *
	 *  a = x   u0 = 1   v0 = 0
	 *  b = y   u1 = y   v1 = x-1
	 *
	 * Each iteration reduces either a or b, and maintains the
	 * invariants. Algorithm stops when a = b, at which point their
	 * common value is GCD(a,b) and (u0,v0) (or (u1,v1)) contains
	 * the values (u,v) we want to return.
	 *
	 * The formal definition of the algorithm is a sequence of steps:
	 *
	 *  - If a is even, then:
	 *        a <- a/2
	 *        u0 <- u0/2 mod y
	 *        v0 <- v0/2 mod x
	 *
	 *  - Otherwise, if b is even, then:
	 *        b <- b/2
	 *        u1 <- u1/2 mod y
	 *        v1 <- v1/2 mod x
	 *
	 *  - Otherwise, if a > b, then:
	 *        a <- (a-b)/2
	 *        u0 <- (u0-u1)/2 mod y
	 *        v0 <- (v0-v1)/2 mod x
	 *
	 *  - Otherwise:
	 *        b <- (b-a)/2
	 *        u1 <- (u1-u0)/2 mod y
	 *        v1 <- (v1-v0)/2 mod y
	 *
	 * We can show that the operations above preserve the invariants:
	 *
	 *  - If a is even, then u0 and v0 are either both even or both
	 *    odd (since a = x*u0 - y*v0, and x and y are both odd).
	 *    If u0 and v0 are both even, then (u0,v0) <- (u0/2,v0/2).
	 *    Otherwise, (u0,v0) <- ((u0+y)/2,(v0+x)/2). Either way,
	 *    the a = x*u0 - y*v0 invariant is preserved.
	 *
	 *  - The same holds for the case where b is even.
	 *
	 *  - If a and b are odd, and a > b, then:
	 *
	 *      a-b = x*(u0-u1) - y*(v0-v1)
	 *
	 *    In that situation, if u0 < u1, then x*(u0-u1) < 0, but
	 *    a-b > 0; therefore, it must be that v0 < v1, and the
	 *    first part of the update is: (u0,v0) <- (u0-u1+y,v0-v1+x),
	 *    which preserves the invariants. Otherwise, if u0 > u1,
	 *    then u0-u1 >= 1, thus x*(u0-u1) >= x. But a <= x and
	 *    b >= 0, hence a-b <= x. It follows that, in that case,
	 *    v0-v1 >= 0. The first part of the update is then:
	 *    (u0,v0) <- (u0-u1,v0-v1), which again preserves the
	 *    invariants.
	 *
	 *    Either way, once the subtraction is done, the new value of
	 *    a, which is the difference of two odd values, is even,
	 *    and the remaining of this step is a subcase of the
	 *    first algorithm case (i.e. when a is even).
	 *
	 *  - If a and b are odd, and b > a, then the a similar
	 *    argument holds.
	 *
	 * The values a and b start at x and y, respectively. Since x
	 * and y are odd, their GCD is odd, and it is easily seen that
	 * all steps conserve the GCD (GCD(a-b,b) = GCD(a, b);
	 * GCD(a/2,b) = GCD(a,b) if GCD(a,b) is odd). Moreover, either a
	 * or b is reduced by at least one bit at each iteration, so
	 * the algorithm necessarily converges on the case a = b, at
	 * which point the common value is the GCD.
	 *
	 * In the algorithm expressed above, when a = b, the fourth case
	 * applies, and sets b = 0. Since a contains the GCD of x and y,
	 * which are both odd, a must be odd, and subsequent iterations
	 * (if any) will simply divide b by 2 repeatedly, which has no
	 * consequence. Thus, the algorithm can run for more iterations
	 * than necessary; the final GCD will be in a, and the (u,v)
	 * coefficients will be (u0,v0).
	 *
	 *
	 * The presentation above is bit-by-bit. It can be sped up by
	 * noticing that all decisions are taken based on the low bits
	 * and high bits of a and b. We can extract the two top words
	 * and low word of each of a and b, and compute reduction
	 * parameters pa, pb, qa and qb such that the new values for
	 * a and b are:
	 *    a' = (a*pa + b*pb) / (2^31)
	 *    b' = (a*qa + b*qb) / (2^31)
	 * the two divisions being exact. The coefficients are obtained
	 * just from the extracted words, and may be slightly off, requiring
	 * an optional correction: if a' < 0, then we replace pa with -pa
	 * and pb with -pb. Each such step will reduce the total length
	 * (sum of lengths of a and b) by at least 30 bits at each
	 * iteration.
	 */
	uint32_t *u0, *u1, *v0, *v1, *a, *b;
	uint32_t x0i, y0i;
	uint32_t num, rc;
	size_t j;

	if (len == 0) {
		return 0;
	}

	/*
	 * u0 and v0 are the u and v result buffers; the four other
	 * values (u1, v1, a and b) are taken from tmp[].
	 */
	u0 = u;
	v0 = v;
	u1 = tmp;
	v1 = u1 + len;
	a = v1 + len;
	b = a + len;

	/*
	 * We'll need the Montgomery reduction coefficients.
	 */
	x0i = modp_ninv31(x[0]);
	y0i = modp_ninv31(y[0]);

	/*
	 * Initialize a, b, u0, u1, v0 and v1.
	 *  a = x   u0 = 1   v0 = 0
	 *  b = y   u1 = y   v1 = x-1
	 * Note that x is odd, so computing x-1 is easy.
	 */
	memcpy(a, x, len * sizeof *x);
	memcpy(b, y, len * sizeof *y);
	u0[0] = 1;
	memset(u0 + 1, 0, (len - 1) * sizeof *u0);
	memset(v0, 0, len * sizeof *v0);
	memcpy(u1, y, len * sizeof *u1);
	memcpy(v1, x, len * sizeof *v1);
	v1[0] --;

	/*
	 * Each input operand may be as large as 31*len bits, and we
	 * reduce the total length by at least 30 bits at each iteration.
	 */
	for (num = 62 * (uint32_t)len + 30; num >= 30; num -= 30) {
		uint32_t c0, c1;
		uint32_t a0, a1, b0, b1;
		uint64_t a_hi, b_hi;
		uint32_t a_lo, b_lo;
		int64_t pa, pb, qa, qb;
		int i;
		uint32_t r;
        //printf("\nbezout:num[%d]",num);
		/*
		 * Extract the top words of a and b. If j is the highest
		 * index >= 1 such that a[j] != 0 or b[j] != 0, then we
		 * want (a[j] << 31) + a[j-1] and (b[j] << 31) + b[j-1].
		 * If a and b are down to one word each, then we use
		 * a[0] and b[0].
		 */
		c0 = (uint32_t)-1;
		c1 = (uint32_t)-1;
		a0 = 0;
		a1 = 0;
		b0 = 0;
		b1 = 0;
		j = len;
		while (j -- > 0) {
			uint32_t aw, bw;

			aw = a[j];
			bw = b[j];
			a0 ^= (a0 ^ aw) & c0;
			a1 ^= (a1 ^ aw) & c1;
			b0 ^= (b0 ^ bw) & c0;
			b1 ^= (b1 ^ bw) & c1;
			c1 = c0;
			c0 &= (((aw | bw) + 0x7FFFFFFF) >> 31) - (uint32_t)1;
		}

		/*
		 * If c1 = 0, then we grabbed two words for a and b.
		 * If c1 != 0 but c0 = 0, then we grabbed one word. It
		 * is not possible that c1 != 0 and c0 != 0, because that
		 * would mean that both integers are zero.
		 */
		a1 |= a0 & c1;
		a0 &= ~c1;
		b1 |= b0 & c1;
		b0 &= ~c1;
		a_hi = ((uint64_t)a0 << 31) + a1;
		b_hi = ((uint64_t)b0 << 31) + b1;
		a_lo = a[0];
		b_lo = b[0];

		/*
		 * Compute reduction factors:
		 *
		 *   a' = a*pa + b*pb
		 *   b' = a*qa + b*qb
		 *
		 * such that a' and b' are both multiple of 2^31, but are
		 * only marginally larger than a and b.
		 */
		pa = 1;
		pb = 0;
		qa = 0;
		qb = 1;
		for (i = 0; i < 31; i ++) {
			/*
			 * At each iteration:
			 *
			 *   a <- (a-b)/2 if: a is odd, b is odd, a_hi > b_hi
			 *   b <- (b-a)/2 if: a is odd, b is odd, a_hi <= b_hi
			 *   a <- a/2 if: a is even
			 *   b <- b/2 if: a is odd, b is even
			 *
			 * We multiply a_lo and b_lo by 2 at each
			 * iteration, thus a division by 2 really is a
			 * non-multiplication by 2.
			 */
			uint32_t rt, oa, ob, cAB, cBA, cA;
			uint64_t rz;

			/*
			 * rt = 1 if a_hi > b_hi, 0 otherwise.
			 */
			rz = b_hi - a_hi;
			rt = (uint32_t)((rz ^ ((a_hi ^ b_hi)
				& (a_hi ^ rz))) >> 63);

			/*
			 * cAB = 1 if b must be subtracted from a
			 * cBA = 1 if a must be subtracted from b
			 * cA = 1 if a must be divided by 2
			 *
			 * Rules:
			 *
			 *   cAB and cBA cannot both be 1.
			 *   If a is not divided by 2, b is.
			 */
			oa = (a_lo >> i) & 1;
			ob = (b_lo >> i) & 1;
			cAB = oa & ob & rt;
			cBA = oa & ob & ~rt;
			cA = cAB | (oa ^ 1);

			/*
			 * Conditional subtractions.
			 */
			a_lo -= b_lo & -cAB;
			a_hi -= b_hi & -(uint64_t)cAB;
			pa -= qa & -(int64_t)cAB;
			pb -= qb & -(int64_t)cAB;
			b_lo -= a_lo & -cBA;
			b_hi -= a_hi & -(uint64_t)cBA;
			qa -= pa & -(int64_t)cBA;
			qb -= pb & -(int64_t)cBA;

			/*
			 * Shifting.
			 */
			a_lo += a_lo & (cA - 1);
			pa += pa & ((int64_t)cA - 1);
			pb += pb & ((int64_t)cA - 1);
			a_hi ^= (a_hi ^ (a_hi >> 1)) & -(uint64_t)cA;
			b_lo += b_lo & -cA;
			qa += qa & -(int64_t)cA;
			qb += qb & -(int64_t)cA;
			b_hi ^= (b_hi ^ (b_hi >> 1)) & ((uint64_t)cA - 1);
		}

		/*
		 * Apply the computed parameters to our values. We
		 * may have to correct pa and pb depending on the
		 * returned value of zint_co_reduce() (when a and/or b
		 * had to be negated).
		 */
		r = zint_co_reduce(a, b, len, pa, pb, qa, qb);
		pa -= (pa + pa) & -(int64_t)(r & 1);
		pb -= (pb + pb) & -(int64_t)(r & 1);
		qa -= (qa + qa) & -(int64_t)(r >> 1);
		qb -= (qb + qb) & -(int64_t)(r >> 1);
		zint_co_reduce_mod(u0, u1, y, len, y0i, pa, pb, qa, qb);
		zint_co_reduce_mod(v0, v1, x, len, x0i, pa, pb, qa, qb);
	}

	/*
	 * At that point, array a[] should contain the GCD, and the
	 * results (u,v) should already be set. We check that the GCD
	 * is indeed 1. We also check that the two operands x and y
	 * are odd.
	 */
	rc = a[0] ^ 1;
	for (j = 1; j < len; j ++) {
		rc |= a[j];
	}
	return (int)((1 - ((rc | -rc) >> 31)) & x[0] & y[0]);
}

/*
 * Add k*y*2^sc to x. The result is assumed to fit in the array of
 * size xlen (truncation is applied if necessary).
 * Scale factor 'sc' is provided as sch and scl, such that:
 *   sch = sc / 31
 *   scl = sc % 31
 * xlen MUST NOT be lower than ylen.
 *
 * x[] and y[] are both signed integers, using two's complement for
 * negative values.
 */
static void
zint_add_scaled_mul_small(uint32_t *restrict x, size_t xlen,
	const uint32_t *restrict y, size_t ylen, int32_t k,
	uint32_t sch, uint32_t scl)
{
	size_t u;
	uint32_t ysign, tw;
	int32_t cc;

	if (ylen == 0) {
		return;
	}

	ysign = -(y[ylen - 1] >> 30) >> 1;
	tw = 0;
	cc = 0;
	for (u = sch; u < xlen; u ++) {
		size_t v;
		uint32_t wy, wys, ccu;
		uint64_t z;

		/*
		 * Get the next word of y (scaled).
		 */
		v = u - sch;
		wy = v < ylen ? y[v] : ysign;
		wys = ((wy << scl) & 0x7FFFFFFF) | tw;
		tw = wy >> (31 - scl);

		/*
		 * The expression below does not overflow.
		 */
		z = (uint64_t)((int64_t)wys * (int64_t)k + (int64_t)x[u] + cc);
		x[u] = (uint32_t)z & 0x7FFFFFFF;

		/*
		 * Right-shifting the signed value z would yield
		 * implementation-defined results (arithmetic shift is
		 * not guaranteed). However, we can cast to unsigned,
		 * and get the next carry as an unsigned word. We can
		 * then convert it back to signed by using the guaranteed
		 * fact that 'int32_t' uses two's complement with no
		 * trap representation or padding bit, and with a layout
		 * compatible with that of 'uint32_t'.
		 */
		ccu = (uint32_t)(z >> 31);
		cc = *(int32_t *)&ccu;
	}
}

/*
 * Subtract y*2^sc from x. The result is assumed to fit in the array of
 * size xlen (truncation is applied if necessary).
 * Scale factor 'sc' is provided as sch and scl, such that:
 *   sch = sc / 31
 *   scl = sc % 31
 * xlen MUST NOT be lower than ylen.
 *
 * x[] and y[] are both signed integers, using two's complement for
 * negative values.
 */
static void
zint_sub_scaled(uint32_t *restrict x, size_t xlen,
	const uint32_t *restrict y, size_t ylen, uint32_t sch, uint32_t scl)
{
	size_t u;
	uint32_t ysign, tw;
	uint32_t cc;

	if (ylen == 0) {
		return;
	}

	ysign = -(y[ylen - 1] >> 30) >> 1;
	tw = 0;
	cc = 0;
	for (u = sch; u < xlen; u ++) {
		size_t v;
		uint32_t w, wy, wys;

		/*
		 * Get the next word of y (scaled).
		 */
		v = u - sch;
		wy = v < ylen ? y[v] : ysign;
		wys = ((wy << scl) & 0x7FFFFFFF) | tw;
		tw = wy >> (31 - scl);

		w = x[u] - wys - cc;
		x[u] = w & 0x7FFFFFFF;
		cc = w >> 31;
	}
}

/*
 * Convert a one-word signed big integer into a signed value.
 */
static inline int32_t
zint_one_to_plain(const uint32_t *x)
{
	uint32_t w;

	w = x[0];
	w |= (w & 0x40000000) << 1;
	return *(int32_t *)&w;
}

/* ==================================================================== */

/*
 * Convert a polynomial to floating-point values.
 *
 * Each coefficient has length flen words, and starts fstride words after
 * the previous.
 *
 * IEEE-754 binary64 values can represent values in a finite range,
 * roughly 2^(-1023) to 2^(+1023); thus, if coefficients are too large,
 * they should be "trimmed" by pointing not to the lowest word of each,
 * but upper.
 */
static void
poly_big_to_fp(fpr *d, const uint32_t *f, size_t flen, size_t fstride,
	unsigned logn)
{
	size_t n, u;

	n = MKN(logn);
	if (flen == 0) {
		for (u = 0; u < n; u ++) {
			d[u] = fpr_zero;
		}
		return;
	}
	for (u = 0; u < n; u ++, f += fstride) {
		size_t v;
		uint32_t neg, cc, xm;
		fpr x, fsc;

		/*
		 * Get sign of the integer; if it is negative, then we
		 * will load its absolute value instead, and negate the
		 * result.
		 */
		neg = -(f[flen - 1] >> 30);
		xm = neg >> 1;
		cc = neg & 1;
		x = fpr_zero;
		fsc = fpr_one;
		for (v = 0; v < flen; v ++, fsc = fpr_mul(fsc, fpr_ptwo31)) {
			uint32_t w;

			w = (f[v] ^ xm) + cc;
			cc = w >> 31;
			w &= 0x7FFFFFFF;
			w -= (w << 1) & neg;
			x = fpr_add(x, fpr_mul(fpr_of(*(int32_t *)&w), fsc));
		}
		d[u] = x;
	}
}

/*
 * Convert a polynomial to small integers. Source values are supposed
 * to be one-word integers, signed over 31 bits. Returned value is 0
 * if any of the coefficients exceeds the provided limit (in absolute
 * value), or 1 on success.
 *
 * This is not constant-time; this is not a problem here, because on
 * any failure, the NTRU-solving process will be deemed to have failed
 * and the (f,g) polynomials will be discarded.
 */
static int
poly_big_to_small(int8_t *d, const uint32_t *s, int lim, unsigned logn)
{
	size_t n, u;

	n = MKN(logn);
	for (u = 0; u < n; u ++) {
		int32_t z;

		z = zint_one_to_plain(s + u);
		if (z < -lim || z > lim) {
			return 0;
		}
		d[u] = (int8_t)z;
	}
	return 1;
}

/*
 * Subtract k*f from F, where F, f and k are polynomials modulo X^N+1.
 * Coefficients of polynomial k are small integers (signed values in the
 * -2^31..2^31 range) scaled by 2^sc. Value sc is provided as sch = sc / 31
 * and scl = sc % 31.
 *
 * This function implements the basic quadratic multiplication algorithm,
 * which is efficient in space (no extra buffer needed) but slow at
 * high degree.
 */
static void
poly_sub_scaled(uint32_t *restrict F, size_t Flen, size_t Fstride,
	const uint32_t *restrict f, size_t flen, size_t fstride,
	const int32_t *restrict k, uint32_t sch, uint32_t scl, unsigned logn)
{
	size_t n, u;

	n = MKN(logn);
	for (u = 0; u < n; u ++) {
		int32_t kf;
		size_t v;
		uint32_t *x;
		const uint32_t *y;

		kf = -k[u];
		x = F + u * Fstride;
		y = f;
		for (v = 0; v < n; v ++) {
			zint_add_scaled_mul_small(
				x, Flen, y, flen, kf, sch, scl);
			if (u + v == n - 1) {
				x = F;
				kf = -kf;
			} else {
				x += Fstride;
			}
			y += fstride;
		}
	}
}

/*
 * Subtract k*f from F. Coefficients of polynomial k are small integers
 * (signed values in the -2^31..2^31 range) scaled by 2^sc. This function
 * assumes that the degree is large, and integers relatively small.
 * The value sc is provided as sch = sc / 31 and scl = sc % 31.
 */
static void
poly_sub_scaled_ntt(uint32_t *restrict F, size_t Flen, size_t Fstride,
	const uint32_t *restrict f, size_t flen, size_t fstride,
	const int32_t *restrict k, uint32_t sch, uint32_t scl, unsigned logn,
	uint32_t *restrict tmp)
{
	uint32_t *gm, *igm, *fk, *t1, *x;
	const uint32_t *y;
	size_t n, u, tlen;
	const small_prime *primes;

	n = MKN(logn);
	tlen = flen + 1;
	gm = tmp;
	igm = gm + MKN(logn);
	fk = igm + MKN(logn);
	t1 = fk + n * tlen;

	primes = PRIMES;

	/*
	 * Compute k*f in fk[], in RNS notation.
	 */
	for (u = 0; u < tlen; u ++) {
		uint32_t p, p0i, R2, Rx;
		size_t v;

		p = primes[u].p;
		p0i = modp_ninv31(p);
		R2 = modp_R2(p, p0i);
		Rx = modp_Rx((unsigned)flen, p, p0i, R2);
		modp_mkgm2(gm, igm, logn, primes[u].g, p, p0i);

		for (v = 0; v < n; v ++) {
			t1[v] = modp_set(k[v], p);
		}
		modp_NTT2(t1, gm, logn, p, p0i);
		for (v = 0, y = f, x = fk + u;
			v < n; v ++, y += fstride, x += tlen)
		{
			*x = zint_mod_small_signed(y, flen, p, p0i, R2, Rx);
		}
		modp_NTT2_ext(fk + u, tlen, gm, logn, p, p0i);
		for (v = 0, x = fk + u; v < n; v ++, x += tlen) {
			*x = modp_montymul(
				modp_montymul(t1[v], *x, p, p0i), R2, p, p0i);
		}
		modp_iNTT2_ext(fk + u, tlen, igm, logn, p, p0i);
	}

	/*
	 * Rebuild k*f.
	 */
	zint_rebuild_CRT(fk, tlen, tlen, n, primes, 1, t1);

	/*
	 * Subtract k*f, scaled, from F.
	 */
	for (u = 0, x = F, y = fk; u < n; u ++, x += Fstride, y += tlen) {
		zint_sub_scaled(x, Flen, y, tlen, sch, scl);
	}
}

/* ==================================================================== */





/*
 * The MAX_BL_SMALL[] and MAX_BL_LARGE[] contain the lengths, in 31-bit
 * words, of intermediate values in the computation:
 *
 *   MAX_BL_SMALL[depth]: length for the input f and g at that depth
 *   MAX_BL_LARGE[depth]: length for the unreduced F and G at that depth
 *
 * Rules:
 *
 *  - Within an array, values grow.
 *
 *  - The 'SMALL' array must have an entry for maximum depth, corresponding
 *    to the size of values used in the binary GCD. There is no such value
 *    for the 'LARGE' array (the binary GCD yields already reduced
 *    coefficients).
 *
 *  - MAX_BL_LARGE[depth] >= MAX_BL_SMALL[depth + 1].
 *
 *  - Values must be large enough to handle the common cases, with some
 *    margins.
 *
 *  - Values must not be "too large" either because we will convert some
 *    integers into floating-point values by considering the top 10 words,
 *    i.e. 310 bits; hence, for values of length more than 10 words, we
 *    should take care to have the length centered on the expected size.
 *
 * The following average lengths, in bits, have been measured on thousands
 * of random keys (fg = max length of the absolute value of coefficients
 * of f and g at that depth; FG = idem for the unreduced F and G; for the
 * maximum depth, F and G are the output of binary GCD, multiplied by q;
 * for each value, the average and standard deviation are provided).
 *
 * Binary case:
 *    depth: 10    fg: 6307.52 (24.48)    FG: 6319.66 (24.51)
 *    depth:  9    fg: 3138.35 (12.25)    FG: 9403.29 (27.55)
 *    depth:  8    fg: 1576.87 ( 7.49)    FG: 4703.30 (14.77)
 *    depth:  7    fg:  794.17 ( 4.98)    FG: 2361.84 ( 9.31)
 *    depth:  6    fg:  400.67 ( 3.10)    FG: 1188.68 ( 6.04)
 *    depth:  5    fg:  202.22 ( 1.87)    FG:  599.81 ( 3.87)
 *    depth:  4    fg:  101.62 ( 1.02)    FG:  303.49 ( 2.38)
 *    depth:  3    fg:   50.37 ( 0.53)    FG:  153.65 ( 1.39)
 *    depth:  2    fg:   24.07 ( 0.25)    FG:   78.20 ( 0.73)
 *    depth:  1    fg:   10.99 ( 0.08)    FG:   39.82 ( 0.41)
 *    depth:  0    fg:    4.00 ( 0.00)    FG:   19.61 ( 0.49)
 *
 * Integers are actually represented either in binary notation over
 * 31-bit words (signed, using two's complement), or in RNS, modulo
 * many small primes. These small primes are close to, but slightly
 * lower than, 2^31. Use of RNS loses less than two bits, even for
 * the largest values.
 *
 * IMPORTANT: if these values are modified, then the temporary buffer
 * sizes (FALCON_KEYGEN_TEMP_*, in inner.h) must be recomputed
 * accordingly.
 */

static const size_t MAX_BL_SMALL[] = {
	1, 1, 2, 2, 4, 7, 14, 27, 53, 106, 209
};

static const size_t MAX_BL_LARGE[] = {
	2, 2, 5, 7, 12, 21, 40, 78, 157, 308
};

/*
 * Average and standard deviation for the maximum size (in bits) of
 * coefficients of (f,g), depending on depth. These values are used
 * to compute bounds for Babai's reduction.
 */
static const struct {
	int avg;
	int std;
} BITLENGTH[] = {
	{    4,  0 },
	{   11,  1 },
	{   24,  1 },
	{   50,  1 },
	{  102,  1 },
	{  202,  2 },
	{  401,  4 },
	{  794,  5 },
	{ 1577,  8 },
	{ 3138, 13 },
	{ 6308, 25 }
};

/*
 * Minimal recursion depth at which we rebuild intermediate values
 * when reconstructing f and g.
 */
#define DEPTH_INT_FG   4

/*
 * Compute squared norm of a short vector. Returned value is saturated to
 * 2^32-1 if it is not lower than 2^31.
 */
static uint32_t
poly_small_sqnorm(const int8_t *f, unsigned logn)
{
	size_t n, u;
	uint32_t s, ng;

	n = MKN(logn);
	s = 0;
	ng = 0;
	for (u = 0; u < n; u ++) {
		int32_t z;

		z = f[u];
		s += (uint32_t)(z * z);
		ng |= s;
	}
	return s | -(ng >> 31);
}

/*
 * Align (upwards) the provided 'data' pointer with regards to 'base'
 * so that the offset is a multiple of the size of 'fpr'.
 */
static fpr *
align_fpr(void *base, void *data)
{
	uint8_t *cb, *cd;
	size_t k, km;

	cb = base;
	cd = data;
	k = (size_t)(cd - cb);
	km = k % sizeof(fpr);
    //아래의 if문은 절대 안들어가네 EYSEO
	if (km) {
		k += (sizeof(fpr)) - km;
	}
	return (fpr *)(cb + k);
}

/*
 * Align (upwards) the provided 'data' pointer with regards to 'base'
 * so that the offset is a multiple of the size of 'uint32_t'.
 */
static uint32_t *
align_u32(void *base, void *data)
{
	uint8_t *cb, *cd;
	size_t k, km;

	cb = base;
	cd = data;
	k = (size_t)(cd - cb);
	km = k % sizeof(uint32_t);
	if (km) {
		k += (sizeof(uint32_t)) - km;
	}
	return (uint32_t *)(cb + k);
}

/*
 * Convert a small vector to floating point.
 */
static void
poly_small_to_fp(fpr *x, const int8_t *f, unsigned logn)
{
	size_t n, u;

	n = MKN(logn);
	for (u = 0; u < n; u ++) {
		x[u] = fpr_of(f[u]);
	}
}

/*
 * Input: f,g of degree N = 2^logn; 'depth' is used only to get their
 * individual length.
 *
 * Output: f',g' of degree N/2, with the length for 'depth+1'.
 *
 * Values are in RNS; input and/or output may also be in NTT.
 */
static void
make_fg_step(uint32_t *data, unsigned logn, unsigned depth,
	int in_ntt, int out_ntt)
{
	size_t n, hn, u;
	size_t slen, tlen;
	uint32_t *fd, *gd, *fs, *gs, *gm, *igm, *t1;
	const small_prime *primes;

    
	n = (size_t)1 << logn;
	hn = n >> 1;
	slen = MAX_BL_SMALL[depth];
	tlen = MAX_BL_SMALL[depth + 1];
	primes = PRIMES;
    
    //printf("\nmake_fg_step:logn[%d], depth[%d],in_ntt[%d],out_ntt[%d],tlen[%d],slen[%d]",logn,depth,in_ntt,out_ntt,tlen,slen);//EYSEO

	/*
	 * Prepare room for the result.
	 */
	fd = data;
	gd = fd + hn * tlen;
	fs = gd + hn * tlen;
	gs = fs + n * slen;
	gm = gs + n * slen;
	igm = gm + n;
	t1 = igm + n;
    //아 이 부분이 있었구나..여기서 fd의 값을 fs로 옮기는구나..
    //fs에 있던 값은 날라가겠네 더 이상 쓸모가 없으니
	memmove(fs, data, 2 * n * slen * sizeof *data);
	/*
	 * First slen words: we use the input values directly, and apply
	 * inverse NTT as we go.
	 */
	for (u = 0; u < slen; u ++) {
		uint32_t p, p0i, R2;
		size_t v;
		uint32_t *x;

		p = primes[u].p;
		p0i = modp_ninv31(p);
		R2 = modp_R2(p, p0i);
        modp_mkgm2(gm, igm, logn, primes[u].g, p, p0i);
        //printf("\nu=%d,prime=%d",u,p);
        //f에 대한 NTT를 구하고 subfield상의 f에 대한 NTT와 INTT를 구하는 과정의 시작
        //t1[v]에 fs에 들어있는 값을 집어 넣음
		for (v = 0, x = fs + u; v < n; v ++, x += slen) {
			t1[v] = *x;
            //printf("t1[%d]:%d,",v,t1[v]);
		}
        //depth 0인 경우에만 f에 대한 NTT구해서 t1에 저장함
		if (!in_ntt) {
			modp_NTT2(t1, gm, logn, p, p0i);
		}
        //printf("\nNTT of down f:");
        //t1에 있는 값이 montgomery 형태가 아니니깐 아래와 같이 multiplication을 두번해서
        //결과 역시 montgomery 형태가 아님. 아랫 단계 다항식의 NTT의 값을 계산해서 fd에 저장함.
		for (v = 0, x = fd + u; v < hn; v ++, x += tlen) {
			uint32_t w0, w1;

			w0 = t1[(v << 1) + 0];
			w1 = t1[(v << 1) + 1];
			*x = modp_montymul(
				modp_montymul(w0, w1, p, p0i), R2, p, p0i);
            //printf("x[%d]:%d,",v,*x);
		}
        //f에 대한 INTT를 구해서 원래 f자리에 대입함
        //depth 0인 경우에는 이 부분이 실행되지 않고 나머지는 다 실행됨
        //사실 이런 것도 slen이 1이면 할 필요가 없는 부분으로 생각됨
		if (in_ntt) {
			modp_iNTT2_ext(fs + u, slen, igm, logn, p, p0i);//slen이 stride 값 EYSEO
		}
        //f에 대해 했던 것을 동일하게 g에 대해서도 구함.
		for (v = 0, x = gs + u; v < n; v ++, x += slen) {
			t1[v] = *x;
		}
        //depth 0인 경우에만 g에 대한 NTT구하기
		if (!in_ntt) {
			modp_NTT2(t1, gm, logn, p, p0i);
		}
		for (v = 0, x = gd + u; v < hn; v ++, x += tlen) {
			uint32_t w0, w1;

			w0 = t1[(v << 1) + 0];
			w1 = t1[(v << 1) + 1];
			*x = modp_montymul(
				modp_montymul(w0, w1, p, p0i), R2, p, p0i);
		}
        //g에 대한 INTT
		if (in_ntt) {
			modp_iNTT2_ext(gs + u, slen, igm, logn, p, p0i);
		}
        //NTT형태로 저장되어 있는 subfield polynomial을 원래 형태로 바꿈.
        //이 부분은 depth가 8일때만 실행됨
        //그런데 이 경우 이미 logn-1=0 이므로 실질적으로 INTT가 실행되지는 아니함.
		if (!out_ntt) {
			modp_iNTT2_ext(fd + u, tlen, igm, logn - 1, p, p0i);
			modp_iNTT2_ext(gd + u, tlen, igm, logn - 1, p, p0i);
		}
	}
	/*
	 * Since the fs and gs words have been de-NTTized, we can use the
	 * CRT to rebuild the values.
	 */
    //사실상 slen이 1인 경우는 여기서 하는 일이 normalization 밖에 없음
	zint_rebuild_CRT(fs, slen, slen, n, primes, 1, gm);
	zint_rebuild_CRT(gs, slen, slen, n, primes, 1, gm);

	/*
	 * Remaining words: use modular reductions to extract the values.
	 */
	for (u = slen; u < tlen; u ++) {
		uint32_t p, p0i, R2, Rx;
		size_t v;
		uint32_t *x;

		p = primes[u].p;
		p0i = modp_ninv31(p);
		R2 = modp_R2(p, p0i);
		Rx = modp_Rx((unsigned)slen, p, p0i, R2);
		modp_mkgm2(gm, igm, logn, primes[u].g, p, p0i);
		for (v = 0, x = fs; v < n; v ++, x += slen) {
			t1[v] = zint_mod_small_signed(x, slen, p, p0i, R2, Rx);
		}
		modp_NTT2(t1, gm, logn, p, p0i);
        //아래에서 +u가 핵심
		for (v = 0, x = fd + u; v < hn; v ++, x += tlen) {
			uint32_t w0, w1;

			w0 = t1[(v << 1) + 0];
			w1 = t1[(v << 1) + 1];
			*x = modp_montymul(
				modp_montymul(w0, w1, p, p0i), R2, p, p0i);
		}
		for (v = 0, x = gs; v < n; v ++, x += slen) {
			t1[v] = zint_mod_small_signed(x, slen, p, p0i, R2, Rx);
		}
		modp_NTT2(t1, gm, logn, p, p0i);
		for (v = 0, x = gd + u; v < hn; v ++, x += tlen) {
			uint32_t w0, w1;

			w0 = t1[(v << 1) + 0];
			w1 = t1[(v << 1) + 1];
			*x = modp_montymul(
				modp_montymul(w0, w1, p, p0i), R2, p, p0i);
		}

		if (!out_ntt) {
			modp_iNTT2_ext(fd + u, tlen, igm, logn - 1, p, p0i);
			modp_iNTT2_ext(gd + u, tlen, igm, logn - 1, p, p0i);
		}
	}
}

/*
 * Compute f and g at a specific depth, in RNS notation.
 *
 * Returned values are stored in the data[] array, at slen words per integer.
 *
 * Conditions:
 *   0 <= depth <= logn
 *
 * Space use in data[]: enough room for any two successive values (f', g',
 * f and g).
 */
static void
make_fg(uint32_t *data, const int8_t *f, const int8_t *g,
	unsigned logn, unsigned depth, int out_ntt)
{
	size_t n, u;
	uint32_t *ft, *gt, p0;
	unsigned d;
	const small_prime *primes;

	n = MKN(logn);
	ft = data;
	gt = ft + n;
	primes = PRIMES;
	p0 = primes[0].p;
	for (u = 0; u < n; u ++) {
		ft[u] = modp_set(f[u], p0);
		gt[u] = modp_set(g[u], p0);
	}
    //ft[u] gt[u] print EYSEO
    /*printf("\nmake_fg");
    printf("\nft[u] : ");
    for (int i = 0; i < 10; i++)
    {
        printf("[%d]",ft[i]);
    }
    printf("\ngt[u] : ");
    for (int i = 0; i < 10; i++)
    {
        printf("[%d]",gt[i]);
    }*/

	if (depth == 0 && out_ntt) {
		uint32_t *gm, *igm;
		uint32_t p, p0i;

		p = primes[0].p;
		p0i = modp_ninv31(p);
		gm = gt + n;
		igm = gm + MKN(logn);
		modp_mkgm2(gm, igm, logn, primes[0].g, p, p0i);
		modp_NTT2(ft, gm, logn, p, p0i);
		modp_NTT2(gt, gm, logn, p, p0i);
		return;
	}

	for (d = 0; d < depth; d ++) {
		make_fg_step(data, logn - d, d,
			d != 0, (d + 1) < depth || out_ntt);
	}
}

/*
 * Solving the NTRU equation, deepest level: compute the resultants of
 * f and g with X^N+1, and use binary GCD. The F and G values are
 * returned in tmp[].
 *
 * Returned value: 1 on success, 0 on error.
 */
static int
solve_NTRU_deepest(unsigned logn_top,
	const int8_t *f, const int8_t *g, uint32_t *tmp)
{
	size_t len;
	uint32_t *Fp, *Gp, *fp, *gp, *t1, q;
	const small_prime *primes;

	len = MAX_BL_SMALL[logn_top];
	primes = PRIMES;
    //printf("\nsolve_NTRU_deepest");
    //printf("\nlength: [%d]\n",len);
	Fp = tmp;
	Gp = Fp + len;
	fp = Gp + len;
	gp = fp + len;
	t1 = gp + len;

	make_fg(fp, f, g, logn_top, logn_top, 0);

	/*
	 * We use the CRT to rebuild the resultants as big integers.
	 * There are two such big integers. The resultants are always
	 * nonnegative.
	 */
    //make_fg()의 output은 INTT까지 된 값이지 CRT를 rebuild 된 상태는 아님 EYSEO
    //make_fg()의 마지막 parameter가 0이기 때문에 INTT된 값이 나오는 것임.
    //이것도 make_fg_step()안에서 out_NTT 변수에 따라 INTT를 할때, 이부분도 하면 더 좋지 않나?
	zint_rebuild_CRT(fp, len, len, 2, primes, 0, t1);

	/*
	 * Apply the binary GCD. The zint_bezout() function works only
	 * if both inputs are odd.
	 *
	 * We can test on the result and return 0 because that would
	 * imply failure of the NTRU solving equation, and the (f,g)
	 * values will be abandoned in that case.
	 */
	if (!zint_bezout(Gp, Fp, fp, gp, len, t1)) {
		return 0;
	}

	/*
	 * Multiply the two values by the target value q. Values must
	 * fit in the destination arrays.
	 * We can again test on the returned words: a non-zero output
	 * of zint_mul_small() means that we exceeded our array
	 * capacity, and that implies failure and rejection of (f,g).
	 */
	q = 12289;
	if (zint_mul_small(Fp, len, q) != 0
		|| zint_mul_small(Gp, len, q) != 0)
	{
		return 0;
	}

	return 1;
}

/*
 * Solving the NTRU equation, intermediate level. Upon entry, the F and G
 * from the previous level should be in the tmp[] array.
 * This function MAY be invoked for the top-level (in which case depth = 0).
 *
 * Returned value: 1 on success, 0 on error.
 */
static int
solve_NTRU_intermediate(unsigned logn_top,
	const int8_t *f, const int8_t *g, unsigned depth, uint32_t *tmp)
{
	/*
	 * In this function, 'logn' is the log2 of the degree for
	 * this step. If N = 2^logn, then:
	 *  - the F and G values already in fk->tmp (from the deeper
	 *    levels) have degree N/2;
	 *  - this function should return F and G of degree N.
	 */
	unsigned logn;
	size_t n, hn, slen, dlen, llen, rlen, FGlen, u;
	uint32_t *Fd, *Gd, *Ft, *Gt, *ft, *gt, *t1;
	fpr *rt1, *rt2, *rt3, *rt4, *rt5;
	int scale_fg, minbl_fg, maxbl_fg, maxbl_FG, scale_k;
	uint32_t *x, *y;
	int32_t *k;
	const small_prime *primes;

	logn = logn_top - depth;
	n = (size_t)1 << logn;
	hn = n >> 1;

	/*
	 * slen = size for our input f and g; also size of the reduced
	 *        F and G we return (degree N)
	 *
	 * dlen = size of the F and G obtained from the deeper level
	 *        (degree N/2 or N/3)
	 *
	 * llen = size for intermediary F and G before reduction (degree N)
	 *
	 * We build our non-reduced F and G as two independent halves each,
	 * of degree N/2 (F = F0 + X*F1, G = G0 + X*G1).
	 */
	slen = MAX_BL_SMALL[depth];
	dlen = MAX_BL_SMALL[depth + 1];
	llen = MAX_BL_LARGE[depth];
	primes = PRIMES;
    
    /*printf("\nNTRU_intermediate: logn_top[%d],depth[%d],logn[%d],n[%d],hn[%d]",logn_top,depth,logn,n,hn);
    printf("\ndlen[%d],hn[%d],llen[%d],n[%d]",dlen,hn,llen,n);*/
    
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //아래 과정에서 가장 중요한 것은 dlen*hn < llen*n 이 항상 성립해서 겹치지 않음. EYSEO
    //Ft,Gt 이번 level에서 구할 polynomial
    //ft,gt 이번 level에서 곱해야 할 f와 g polynomial(NTT, RNS)
    //Fd,Gd 이번 level에서 곱해야 할 F와 G polynomial(no NTT, no RNS)
    //중간에 Fd,Gd의 값을 Ft,Gt에 옮겨 놓음
	/*
	 * Fd and Gd are the F and G from the deeper level.
	 */
	Fd = tmp;
	Gd = Fd + dlen * hn;

	/*
	 * Compute the input f and g for this level. Note that we get f
	 * and g in RNS + NTT representation.
	 */
	ft = Gd + dlen * hn;
    //1을 parameter로 넣는 것은 NTT형태의 output을 얻기 위함 EYSEO
	make_fg(ft, f, g, logn_top, depth, 1);

	/*
	 * Move the newly computed f and g to make room for our candidate
	 * F and G (unreduced).
	 */
	Ft = tmp;
	Gt = Ft + n * llen;
	t1 = Gt + n * llen;
	memmove(t1, ft, 2 * n * slen * sizeof *ft);
	ft = t1;
	gt = ft + slen * n;
	t1 = gt + slen * n;

	/*
	 * Move Fd and Gd _after_ f and g.
	 */
	memmove(t1, Fd, 2 * hn * dlen * sizeof *Fd);
	Fd = t1;
	Gd = Fd + hn * dlen;
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //윗 부분을 아래와 같이 하면 안되는 이유가 segment fault가 나는데, ft앞에 EYSEO
    //n*llen*2만큼 메모리를 미리 너무 많이 잡아놓으니
    //ft에서 사용할 만큼 충분한 양이 남지 않아서 문제가 발생하는 것으로 추정됨
    /*Ft = tmp;
    Gt = Ft + n*llen;
    ft = Gt + n*llen;
    make_fg(ft, f, g, logn_top, depth, 1);
    t1 = ft + 2*slen*n;
    memmove(t1, Ft, 2 * hn * dlen * sizeof *Ft);
    Fd = t1;
    Gd = Fd + hn*dlen;*/
    
	/*
	 * We reduce Fd and Gd modulo all the small primes we will need,
	 * and store the values in Ft and Gt (only n/2 values in each).
	 */
    //여기서 Fd,Gd를 RNS 형태로 바꿈
	for (u = 0; u < llen; u ++) {
		uint32_t p, p0i, R2, Rx;
		size_t v;
		uint32_t *xs, *ys, *xd, *yd;

		p = primes[u].p;
		p0i = modp_ninv31(p);
		R2 = modp_R2(p, p0i);
		Rx = modp_Rx((unsigned)dlen, p, p0i, R2);
		for (v = 0, xs = Fd, ys = Gd, xd = Ft + u, yd = Gt + u;
			v < hn;
			v ++, xs += dlen, ys += dlen, xd += llen, yd += llen)
		{
			*xd = zint_mod_small_signed(xs, dlen, p, p0i, R2, Rx);
			*yd = zint_mod_small_signed(ys, dlen, p, p0i, R2, Rx);
		}
	}

	/*
	 * We do not need Fd and Gd after that point.
	 */

	/*
	 * Compute our F and G modulo sufficiently many small primes.
	 */
	for (u = 0; u < llen; u ++) {
		uint32_t p, p0i, R2;
		uint32_t *gm, *igm, *fx, *gx, *Fp, *Gp;
		size_t v;

		/*
		 * All computations are done modulo p.
		 */
		p = primes[u].p;
		p0i = modp_ninv31(p);
		R2 = modp_R2(p, p0i);

		/*
		 * If we processed slen words, then f and g have been
		 * de-NTTized, and are in RNS; we can rebuild them.
		 */
        //이 부분의 아랫부분에서 INTT를 해서 slen개에 대해서는 no NTT 형태로 이미 만들어 놓음
        //그래서 이 부분을 통해 no RNS 형태로 저장
        //ft,gt에는 f,g가 각 계수가 slen개로 이루어진 no NTT, no RNS 형태로 저장됨
        //fx,gx에는 f,g의 각 계수가 llen개로 이루어진 NTT, RNS 형태로 저장됨 EYSEO
		if (u == slen) {
            //printf("\nu[%d],llen[%d],slen[%d]",u,llen,slen);
			zint_rebuild_CRT(ft, slen, slen, n, primes, 1, t1);
			zint_rebuild_CRT(gt, slen, slen, n, primes, 1, t1);
		}
        
        //t1의 자리에 Fd가 있었는데 더 이상 쓰지 않으니 이렇게 활용
        //fx에 slen을 넘어가는 prime에 대한 f의 추가적인 NTT 저장 EYSEO
		gm = t1;
		igm = gm + n;
		fx = igm + n;
		gx = fx + n;

		modp_mkgm2(gm, igm, logn, primes[u].g, p, p0i);
        
        
		if (u < slen) {
            //f와 g에 대해 INTT 실행 각 p에 대해서 실행 EYSEO
			for (v = 0, x = ft + u, y = gt + u;
				v < n; v ++, x += slen, y += slen)
			{
				fx[v] = *x;
				gx[v] = *y;
			}
			modp_iNTT2_ext(ft + u, slen, igm, logn, p, p0i);
			modp_iNTT2_ext(gt + u, slen, igm, logn, p, p0i);
		} else {
            //slen 개수보다 더 많은 prime 들에 대한 modulo와 NTT 실행 EYSEO
			uint32_t Rx;

			Rx = modp_Rx((unsigned)slen, p, p0i, R2);
			for (v = 0, x = ft, y = gt;
				v < n; v ++, x += slen, y += slen)
			{
				fx[v] = zint_mod_small_signed(x, slen,
					p, p0i, R2, Rx);
				gx[v] = zint_mod_small_signed(y, slen,
					p, p0i, R2, Rx);
			}
			modp_NTT2(fx, gm, logn, p, p0i);
			modp_NTT2(gx, gm, logn, p, p0i);
		}

		/*
		 * Get F' and G' modulo p and in NTT representation
		 * (they have degree n/2). These values were computed in
		 * a previous step, and stored in Ft and Gt.
		 */
        //F',G'는 no NTT, RNS 형태였으니깐 각 p에 대해 NTT로 변환 EYSEO
        //원래 no RNS 였는데 이 함수의 초반에 이미 RNS 형태로 바꿔 놓음
		Fp = gx + n;
		Gp = Fp + hn;
		for (v = 0, x = Ft + u, y = Gt + u;
			v < hn; v ++, x += llen, y += llen)
		{
			Fp[v] = *x;
			Gp[v] = *y;
		}
		modp_NTT2(Fp, gm, logn - 1, p, p0i);
		modp_NTT2(Gp, gm, logn - 1, p, p0i);

		/*
		 * Compute our F and G modulo p.
		 *
		 * General case:
		 *
		 *   we divide degree by d = 2 or 3
		 *   f'(x^d) = N(f)(x^d) = f * adj(f)
		 *   g'(x^d) = N(g)(x^d) = g * adj(g)
		 *   f'*G' - g'*F' = q
		 *   F = F'(x^d) * adj(g)
		 *   G = G'(x^d) * adj(f)
		 *
		 * We compute things in the NTT. We group roots of phi
		 * such that all roots x in a group share the same x^d.
		 * If the roots in a group are x_1, x_2... x_d, then:
		 *
		 *   N(f)(x_1^d) = f(x_1)*f(x_2)*...*f(x_d)
		 *
		 * Thus, we have:
		 *
		 *   G(x_1) = f(x_2)*f(x_3)*...*f(x_d)*G'(x_1^d)
		 *   G(x_2) = f(x_1)*f(x_3)*...*f(x_d)*G'(x_1^d)
		 *   ...
		 *   G(x_d) = f(x_1)*f(x_2)*...*f(x_{d-1})*G'(x_1^d)
		 *
		 * In all cases, we can thus compute F and G in NTT
		 * representation by a few simple multiplications.
		 * Moreover, in our chosen NTT representation, roots
		 * from the same group are consecutive in RAM.
		 */
        //F(a^i)=F'(a^2i)g(-a^i)=F'(a^2i)g(a^{{n/2}+i})
        //F(a^{{n/2}+i})=F'(a^2i)g(a^i)
        //위 두 식을 이용하여 F의 NTT를 구하는 것이 아래의 for문임 EYSEO
		for (v = 0, x = Ft + u, y = Gt + u; v < hn;
			v ++, x += (llen << 1), y += (llen << 1))
		{
			uint32_t ftA, ftB, gtA, gtB;
			uint32_t mFp, mGp;

			ftA = fx[(v << 1) + 0];
			ftB = fx[(v << 1) + 1];
			gtA = gx[(v << 1) + 0];
			gtB = gx[(v << 1) + 1];
			mFp = modp_montymul(Fp[v], R2, p, p0i);
			mGp = modp_montymul(Gp[v], R2, p, p0i);
			x[0] = modp_montymul(gtB, mFp, p, p0i);
			x[llen] = modp_montymul(gtA, mFp, p, p0i);
			y[0] = modp_montymul(ftB, mGp, p, p0i);
			y[llen] = modp_montymul(ftA, mGp, p, p0i);
		}
		modp_iNTT2_ext(Ft + u, llen, igm, logn, p, p0i);
		modp_iNTT2_ext(Gt + u, llen, igm, logn, p, p0i);
	}

	/*
	 * Rebuild F and G with the CRT.
	 */
	zint_rebuild_CRT(Ft, llen, llen, n, primes, 1, t1);
	zint_rebuild_CRT(Gt, llen, llen, n, primes, 1, t1);
    
    //그리하여 F,G는 각 계수가 slen의 길이로 Ft,Gt에 no NTT, no RNS 형태로 tmp에 저장됨
    //그리고  f,g는 각 계수가 llen의 길이로 ft,gt에 no NTT, no RNS 형태로 Gt다음에 저장됨
    //이것을 가지고 Babai redution을 어찌 하는가가 문제...EYSEO

	/*
	 * At that point, Ft, Gt, ft and gt are consecutive in RAM (in that
	 * order).
	 */

	/*
	 * Apply Babai reduction to bring back F and G to size slen.
	 *
	 * We use the FFT to compute successive approximations of the
	 * reduction coefficient. We first isolate the top bits of
	 * the coefficients of f and g, and convert them to floating
	 * point; with the FFT, we compute adj(f), adj(g), and
	 * 1/(f*adj(f)+g*adj(g)).
	 *
	 * Then, we repeatedly apply the following:
	 *
	 *   - Get the top bits of the coefficients of F and G into
	 *     floating point, and use the FFT to compute:
	 *        (F*adj(f)+G*adj(g))/(f*adj(f)+g*adj(g))
	 *
	 *   - Convert back that value into normal representation, and
	 *     round it to the nearest integers, yielding a polynomial k.
	 *     Proper scaling is applied to f, g, F and G so that the
	 *     coefficients fit on 32 bits (signed).
	 *
	 *   - Subtract k*f from F and k*g from G.
	 *
	 * Under normal conditions, this process reduces the size of F
	 * and G by some bits at each iteration. For constant-time
	 * operation, we do not want to measure the actual length of
	 * F and G; instead, we do the following:
	 *
	 *   - f and g are converted to floating-point, with some scaling
	 *     if necessary to keep values in the representable range.
	 *
	 *   - For each iteration, we _assume_ a maximum size for F and G,
	 *     and use the values at that size. If we overreach, then
	 *     we get zeros, which is harmless: the resulting coefficients
	 *     of k will be 0 and the value won't be reduced.
	 *
	 *   - We conservatively assume that F and G will be reduced by
	 *     at least 25 bits at each iteration.
	 *
	 * Even when reaching the bottom of the reduction, reduction
	 * coefficient will remain low. If it goes out-of-range, then
	 * something wrong occurred and the whole NTRU solving fails.
	 */

	/*
	 * Memory layout:
	 *  - We need to compute and keep adj(f), adj(g), and
	 *    1/(f*adj(f)+g*adj(g)) (sizes N, N and N/2 fp numbers,
	 *    respectively).
	 *  - At each iteration we need two extra fp buffer (N fp values),
	 *    and produce a k (N 32-bit words). k will be shared with one
	 *    of the fp buffers.
	 *  - To compute k*f and k*g efficiently (with the NTT), we need
	 *    some extra room; we reuse the space of the temporary buffers.
	 *
	 * Arrays of 'fpr' are obtained from the temporary array itself.
	 * We ensure that the base is at a properly aligned offset (the
	 * source array tmp[] is supposed to be already aligned).
	 */
    //t1의 위치는 F,G,f,g 다음임 EYSEO
    //rt3에는 t1의 위치가 저장됨
    //이미 align이 되어 있어서 사실은 align_fpr을 할 필요가 없음
	rt3 = align_fpr(tmp, t1);
	rt4 = rt3 + n;
	rt5 = rt4 + n;
	rt1 = rt5 + (n >> 1);
	k = (int32_t *)align_u32(tmp, rt1);
	rt2 = align_fpr(tmp, k + n);
	if (rt2 < (rt1 + n)) {
		rt2 = rt1 + n;
	}
	t1 = (uint32_t *)k + n;

	/*
	 * Get f and g into rt3 and rt4 as floating-point approximations.
	 *
	 * We need to "scale down" the floating-point representation of
	 * coefficients when they are too big. We want to keep the value
	 * below 2^310 or so. Thus, when values are larger than 10 words,
	 * we consider only the top 10 words. Array lengths have been
	 * computed so that average maximum length will fall in the
	 * middle or the upper half of these top 10 words.
	 */
    //slen이 10보다 크면 rlen의 값은 10, 아니면 slen의 값 EYSEO
	rlen = (slen > 10) ? 10 : slen;
	poly_big_to_fp(rt3, ft + slen - rlen, rlen, slen, logn);
	poly_big_to_fp(rt4, gt + slen - rlen, rlen, slen, logn);

	/*
	 * Values in rt3 and rt4 are downscaled by 2^(scale_fg).
	 */
    //위에서 잘라 넣을때 잘려나간 부분에 대한 scaling을 위한 변수 미리 계산 EYSEO
	scale_fg = 31 * (int)(slen - rlen);

	/*
	 * Estimated boundaries for the maximum size (in bits) of the
	 * coefficients of (f,g). We use the measured average, and
	 * allow for a deviation of at most six times the standard
	 * deviation.
	 */
	minbl_fg = BITLENGTH[depth].avg - 6 * BITLENGTH[depth].std;
	maxbl_fg = BITLENGTH[depth].avg + 6 * BITLENGTH[depth].std;

	/*
	 * Compute 1/(f*adj(f)+g*adj(g)) in rt5. We also keep adj(f)
	 * and adj(g) in rt3 and rt4, respectively.
	 */
	Zf(FFT)(rt3, logn);
	Zf(FFT)(rt4, logn);
	Zf(poly_invnorm2_fft)(rt5, rt3, rt4, logn);
	Zf(poly_adj_fft)(rt3, logn);
	Zf(poly_adj_fft)(rt4, logn);

	/*
	 * Reduce F and G repeatedly.
	 *
	 * The expected maximum bit length of coefficients of F and G
	 * is kept in maxbl_FG, with the corresponding word length in
	 * FGlen.
	 */
	FGlen = llen;
	maxbl_FG = 31 * (int)llen;

	/*
	 * Each reduction operation computes the reduction polynomial
	 * "k". We need that polynomial to have coefficients that fit
	 * on 32-bit signed integers, with some scaling; thus, we use
	 * a descending sequence of scaling values, down to zero.
	 *
	 * The size of the coefficients of k is (roughly) the difference
	 * between the size of the coefficients of (F,G) and the size
	 * of the coefficients of (f,g). Thus, the maximum size of the
	 * coefficients of k is, at the start, maxbl_FG - minbl_fg;
	 * this is our starting scale value for k.
	 *
	 * We need to estimate the size of (F,G) during the execution of
	 * the algorithm; we are allowed some overestimation but not too
	 * much (poly_big_to_fp() uses a 310-bit window). Generally
	 * speaking, after applying a reduction with k scaled to
	 * scale_k, the size of (F,G) will be size(f,g) + scale_k + dd,
	 * where 'dd' is a few bits to account for the fact that the
	 * reduction is never perfect (intuitively, dd is on the order
	 * of sqrt(N), so at most 5 bits; we here allow for 10 extra
	 * bits).
	 *
	 * The size of (f,g) is not known exactly, but maxbl_fg is an
	 * upper bound.
	 */
	scale_k = maxbl_FG - minbl_fg;

	for (;;) {
		int scale_FG, dc, new_maxbl_FG;
		uint32_t scl, sch;
		fpr pdc, pt;

		/*
		 * Convert current F and G into floating-point. We apply
		 * scaling if the current length is more than 10 words.
		 */
		rlen = (FGlen > 10) ? 10 : FGlen;
		scale_FG = 31 * (int)(FGlen - rlen);
		poly_big_to_fp(rt1, Ft + FGlen - rlen, rlen, llen, logn);
		poly_big_to_fp(rt2, Gt + FGlen - rlen, rlen, llen, logn);

		/*
		 * Compute (F*adj(f)+G*adj(g))/(f*adj(f)+g*adj(g)) in rt2.
		 */
		Zf(FFT)(rt1, logn);
		Zf(FFT)(rt2, logn);
		Zf(poly_mul_fft)(rt1, rt3, logn);
		Zf(poly_mul_fft)(rt2, rt4, logn);
		Zf(poly_add)(rt2, rt1, logn);
        //r2와 r5를 그냥 poly_mul_fft를 안하고 아래 함수를 쓰는 이유가 뭘까?EYSEO
		Zf(poly_mul_autoadj_fft)(rt2, rt5, logn);
		Zf(iFFT)(rt2, logn);

		/*
		 * (f,g) are scaled by 'scale_fg', meaning that the
		 * numbers in rt3/rt4 should be multiplied by 2^(scale_fg)
		 * to have their true mathematical value.
		 *
		 * (F,G) are similarly scaled by 'scale_FG'. Therefore,
		 * the value we computed in rt2 is scaled by
		 * 'scale_FG-scale_fg'.
		 *
		 * We want that value to be scaled by 'scale_k', hence we
		 * apply a corrective scaling. After scaling, the values
		 * should fit in -2^31-1..+2^31-1.
		 */
		dc = scale_k - scale_FG + scale_fg;

		/*
		 * We will need to multiply values by 2^(-dc). The value
		 * 'dc' is not secret, so we can compute 2^(-dc) with a
		 * non-constant-time process.
		 * (We could use ldexp(), but we prefer to avoid any
		 * dependency on libm. When using FP emulation, we could
		 * use our fpr_ldexp(), which is constant-time.)
		 */
		if (dc < 0) {
			dc = -dc;
			pt = fpr_two;
		} else {
			pt = fpr_onehalf;
		}
		pdc = fpr_one;
		while (dc != 0) {
			if ((dc & 1) != 0) {
				pdc = fpr_mul(pdc, pt);
			}
			dc >>= 1;
			pt = fpr_sqr(pt);
		}

		for (u = 0; u < n; u ++) {
			fpr xv;

			xv = fpr_mul(rt2[u], pdc);

			/*
			 * Sometimes the values can be out-of-bounds if
			 * the algorithm fails; we must not call
			 * fpr_rint() (and cast to int32_t) if the value
			 * is not in-bounds. Note that the test does not
			 * break constant-time discipline, since any
			 * failure here implies that we discard the current
			 * secret key (f,g).
			 */
			if (!fpr_lt(fpr_mtwo31m1, xv)
				|| !fpr_lt(xv, fpr_ptwo31m1))
			{
				return 0;
			}
			k[u] = (int32_t)fpr_rint(xv);
		}

		/*
		 * Values in k[] are integers. They really are scaled
		 * down by maxbl_FG - minbl_fg bits.
		 *
		 * If we are at low depth, then we use the NTT to
		 * compute k*f and k*g.
		 */
		sch = (uint32_t)(scale_k / 31);
		scl = (uint32_t)(scale_k % 31);
		if (depth <= DEPTH_INT_FG) {
			poly_sub_scaled_ntt(Ft, FGlen, llen, ft, slen, slen,
				k, sch, scl, logn, t1);
			poly_sub_scaled_ntt(Gt, FGlen, llen, gt, slen, slen,
				k, sch, scl, logn, t1);
		} else {
			poly_sub_scaled(Ft, FGlen, llen, ft, slen, slen,
				k, sch, scl, logn);
			poly_sub_scaled(Gt, FGlen, llen, gt, slen, slen,
				k, sch, scl, logn);
		}

		/*
		 * We compute the new maximum size of (F,G), assuming that
		 * (f,g) has _maximal_ length (i.e. that reduction is
		 * "late" instead of "early". We also adjust FGlen
		 * accordingly.
		 */
		new_maxbl_FG = scale_k + maxbl_fg + 10;
		if (new_maxbl_FG < maxbl_FG) {
			maxbl_FG = new_maxbl_FG;
			if ((int)FGlen * 31 >= maxbl_FG + 31) {
				FGlen --;
			}
		}

		/*
		 * We suppose that scaling down achieves a reduction by
		 * at least 25 bits per iteration. We stop when we have
		 * done the loop with an unscaled k.
		 */
		if (scale_k <= 0) {
			break;
		}
		scale_k -= 25;
		if (scale_k < 0) {
			scale_k = 0;
		}
	}

	/*
	 * If (F,G) length was lowered below 'slen', then we must take
	 * care to re-extend the sign.
	 */
	if (FGlen < slen) {
		for (u = 0; u < n; u ++, Ft += llen, Gt += llen) {
			size_t v;
			uint32_t sw;

			sw = -(Ft[FGlen - 1] >> 30) >> 1;
			for (v = FGlen; v < slen; v ++) {
				Ft[v] = sw;
			}
			sw = -(Gt[FGlen - 1] >> 30) >> 1;
			for (v = FGlen; v < slen; v ++) {
				Gt[v] = sw;
			}
		}
	}

	/*
	 * Compress encoding of all values to 'slen' words (this is the
	 * expected output format).
	 */
	for (u = 0, x = tmp, y = tmp;
		u < (n << 1); u ++, x += slen, y += llen)
	{
		memmove(x, y, slen * sizeof *y);
	}
	return 1;
}

/*
 * Solving the NTRU equation, binary case, depth = 1. Upon entry, the
 * F and G from the previous level should be in the tmp[] array.
 *
 * Returned value: 1 on success, 0 on error.
 */
static int
solve_NTRU_binary_depth1(unsigned logn_top,
	const int8_t *f, const int8_t *g, uint32_t *tmp)
{
	/*
	 * The first half of this function is a copy of the corresponding
	 * part in solve_NTRU_intermediate(), for the reconstruction of
	 * the unreduced F and G. The second half (Babai reduction) is
	 * done differently, because the unreduced F and G fit in 53 bits
	 * of precision, allowing a much simpler process with lower RAM
	 * usage.
	 */
	unsigned depth, logn;
	size_t n_top, n, hn, slen, dlen, llen, u;
	uint32_t *Fd, *Gd, *Ft, *Gt, *ft, *gt, *t1;
	fpr *rt1, *rt2, *rt3, *rt4, *rt5, *rt6;
	uint32_t *x, *y;

	depth = 1;
	n_top = (size_t)1 << logn_top;
	logn = logn_top - depth;
	n = (size_t)1 << logn;
	hn = n >> 1;

	/*
	 * Equations are:
	 *
	 *   f' = f0^2 - X^2*f1^2
	 *   g' = g0^2 - X^2*g1^2
	 *   F' and G' are a solution to f'G' - g'F' = q (from deeper levels)
	 *   F = F'*(g0 - X*g1)
	 *   G = G'*(f0 - X*f1)
	 *
	 * f0, f1, g0, g1, f', g', F' and G' are all "compressed" to
	 * degree N/2 (their odd-indexed coefficients are all zero).
	 */

	/*
	 * slen = size for our input f and g; also size of the reduced
	 *        F and G we return (degree N)
	 *
	 * dlen = size of the F and G obtained from the deeper level
	 *        (degree N/2)
	 *
	 * llen = size for intermediary F and G before reduction (degree N)
	 *
	 * We build our non-reduced F and G as two independent halves each,
	 * of degree N/2 (F = F0 + X*F1, G = G0 + X*G1).
	 */
	slen = MAX_BL_SMALL[depth];
	dlen = MAX_BL_SMALL[depth + 1];
	llen = MAX_BL_LARGE[depth];

	/*
	 * Fd and Gd are the F and G from the deeper level. Ft and Gt
	 * are the destination arrays for the unreduced F and G.
	 */
	Fd = tmp;
	Gd = Fd + dlen * hn;
	Ft = Gd + dlen * hn;
	Gt = Ft + llen * n;

	/*
	 * We reduce Fd and Gd modulo all the small primes we will need,
	 * and store the values in Ft and Gt.
	 */
	for (u = 0; u < llen; u ++) {
		uint32_t p, p0i, R2, Rx;
		size_t v;
		uint32_t *xs, *ys, *xd, *yd;

		p = PRIMES[u].p;
		p0i = modp_ninv31(p);
		R2 = modp_R2(p, p0i);
		Rx = modp_Rx((unsigned)dlen, p, p0i, R2);
		for (v = 0, xs = Fd, ys = Gd, xd = Ft + u, yd = Gt + u;
			v < hn;
			v ++, xs += dlen, ys += dlen, xd += llen, yd += llen)
		{
			*xd = zint_mod_small_signed(xs, dlen, p, p0i, R2, Rx);
			*yd = zint_mod_small_signed(ys, dlen, p, p0i, R2, Rx);
		}
	}

	/*
	 * Now Fd and Gd are not needed anymore; we can squeeze them out.
	 */
	memmove(tmp, Ft, llen * n * sizeof(uint32_t));
	Ft = tmp;
	memmove(Ft + llen * n, Gt, llen * n * sizeof(uint32_t));
	Gt = Ft + llen * n;
	ft = Gt + llen * n;
	gt = ft + slen * n;

	t1 = gt + slen * n;

	/*
	 * Compute our F and G modulo sufficiently many small primes.
	 */
	for (u = 0; u < llen; u ++) {
		uint32_t p, p0i, R2;
		uint32_t *gm, *igm, *fx, *gx, *Fp, *Gp;
		unsigned e;
		size_t v;

		/*
		 * All computations are done modulo p.
		 */
		p = PRIMES[u].p;
		p0i = modp_ninv31(p);
		R2 = modp_R2(p, p0i);

		/*
		 * We recompute things from the source f and g, of full
		 * degree. However, we will need only the n first elements
		 * of the inverse NTT table (igm); the call to modp_mkgm()
		 * below will fill n_top elements in igm[] (thus overflowing
		 * into fx[]) but later code will overwrite these extra
		 * elements.
		 */
		gm = t1;
		igm = gm + n_top;
		fx = igm + n;
		gx = fx + n_top;
		modp_mkgm2(gm, igm, logn_top, PRIMES[u].g, p, p0i);

		/*
		 * Set ft and gt to f and g modulo p, respectively.
		 */
		for (v = 0; v < n_top; v ++) {
			fx[v] = modp_set(f[v], p);
			gx[v] = modp_set(g[v], p);
		}

		/*
		 * Convert to NTT and compute our f and g.
		 */
		modp_NTT2(fx, gm, logn_top, p, p0i);
		modp_NTT2(gx, gm, logn_top, p, p0i);
		for (e = logn_top; e > logn; e --) {
			modp_poly_rec_res(fx, e, p, p0i, R2);
			modp_poly_rec_res(gx, e, p, p0i, R2);
		}

		/*
		 * From that point onward, we only need tables for
		 * degree n, so we can save some space.
		 */
		if (depth > 0) { /* always true */
			memmove(gm + n, igm, n * sizeof *igm);
			igm = gm + n;
			memmove(igm + n, fx, n * sizeof *ft);
			fx = igm + n;
			memmove(fx + n, gx, n * sizeof *gt);
			gx = fx + n;
		}

		/*
		 * Get F' and G' modulo p and in NTT representation
		 * (they have degree n/2). These values were computed
		 * in a previous step, and stored in Ft and Gt.
		 */
		Fp = gx + n;
		Gp = Fp + hn;
		for (v = 0, x = Ft + u, y = Gt + u;
			v < hn; v ++, x += llen, y += llen)
		{
			Fp[v] = *x;
			Gp[v] = *y;
		}
		modp_NTT2(Fp, gm, logn - 1, p, p0i);
		modp_NTT2(Gp, gm, logn - 1, p, p0i);

		/*
		 * Compute our F and G modulo p.
		 *
		 * Equations are:
		 *
		 *   f'(x^2) = N(f)(x^2) = f * adj(f)
		 *   g'(x^2) = N(g)(x^2) = g * adj(g)
		 *
		 *   f'*G' - g'*F' = q
		 *
		 *   F = F'(x^2) * adj(g)
		 *   G = G'(x^2) * adj(f)
		 *
		 * The NTT representation of f is f(w) for all w which
		 * are roots of phi. In the binary case, as well as in
		 * the ternary case for all depth except the deepest,
		 * these roots can be grouped in pairs (w,-w), and we
		 * then have:
		 *
		 *   f(w) = adj(f)(-w)
		 *   f(-w) = adj(f)(w)
		 *
		 * and w^2 is then a root for phi at the half-degree.
		 *
		 * At the deepest level in the ternary case, this still
		 * holds, in the following sense: the roots of x^2-x+1
		 * are (w,-w^2) (for w^3 = -1, and w != -1), and we
		 * have:
		 *
		 *   f(w) = adj(f)(-w^2)
		 *   f(-w^2) = adj(f)(w)
		 *
		 * In all case, we can thus compute F and G in NTT
		 * representation by a few simple multiplications.
		 * Moreover, the two roots for each pair are consecutive
		 * in our bit-reversal encoding.
		 */
		for (v = 0, x = Ft + u, y = Gt + u;
			v < hn; v ++, x += (llen << 1), y += (llen << 1))
		{
			uint32_t ftA, ftB, gtA, gtB;
			uint32_t mFp, mGp;

			ftA = fx[(v << 1) + 0];
			ftB = fx[(v << 1) + 1];
			gtA = gx[(v << 1) + 0];
			gtB = gx[(v << 1) + 1];
			mFp = modp_montymul(Fp[v], R2, p, p0i);
			mGp = modp_montymul(Gp[v], R2, p, p0i);
			x[0] = modp_montymul(gtB, mFp, p, p0i);
			x[llen] = modp_montymul(gtA, mFp, p, p0i);
			y[0] = modp_montymul(ftB, mGp, p, p0i);
			y[llen] = modp_montymul(ftA, mGp, p, p0i);
		}
		modp_iNTT2_ext(Ft + u, llen, igm, logn, p, p0i);
		modp_iNTT2_ext(Gt + u, llen, igm, logn, p, p0i);

		/*
		 * Also save ft and gt (only up to size slen).
		 */
		if (u < slen) {
			modp_iNTT2(fx, igm, logn, p, p0i);
			modp_iNTT2(gx, igm, logn, p, p0i);
			for (v = 0, x = ft + u, y = gt + u;
				v < n; v ++, x += slen, y += slen)
			{
				*x = fx[v];
				*y = gx[v];
			}
		}
	}

	/*
	 * Rebuild f, g, F and G with the CRT. Note that the elements of F
	 * and G are consecutive, and thus can be rebuilt in a single
	 * loop; similarly, the elements of f and g are consecutive.
	 */
	zint_rebuild_CRT(Ft, llen, llen, n << 1, PRIMES, 1, t1);
	zint_rebuild_CRT(ft, slen, slen, n << 1, PRIMES, 1, t1);

	/*
	 * Here starts the Babai reduction, specialized for depth = 1.
	 *
	 * Candidates F and G (from Ft and Gt), and base f and g (ft and gt),
	 * are converted to floating point. There is no scaling, and a
	 * single pass is sufficient.
	 */

	/*
	 * Convert F and G into floating point (rt1 and rt2).
	 */
	rt1 = align_fpr(tmp, gt + slen * n);
	rt2 = rt1 + n;
	poly_big_to_fp(rt1, Ft, llen, llen, logn);
	poly_big_to_fp(rt2, Gt, llen, llen, logn);

	/*
	 * Integer representation of F and G is no longer needed, we
	 * can remove it.
	 */
	memmove(tmp, ft, 2 * slen * n * sizeof *ft);
	ft = tmp;
	gt = ft + slen * n;
	rt3 = align_fpr(tmp, gt + slen * n);
	memmove(rt3, rt1, 2 * n * sizeof *rt1);
	rt1 = rt3;
	rt2 = rt1 + n;
	rt3 = rt2 + n;
	rt4 = rt3 + n;

	/*
	 * Convert f and g into floating point (rt3 and rt4).
	 */
	poly_big_to_fp(rt3, ft, slen, slen, logn);
	poly_big_to_fp(rt4, gt, slen, slen, logn);

	/*
	 * Remove unneeded ft and gt.
	 */
	memmove(tmp, rt1, 4 * n * sizeof *rt1);
	rt1 = (fpr *)tmp;
	rt2 = rt1 + n;
	rt3 = rt2 + n;
	rt4 = rt3 + n;

	/*
	 * We now have:
	 *   rt1 = F
	 *   rt2 = G
	 *   rt3 = f
	 *   rt4 = g
	 * in that order in RAM. We convert all of them to FFT.
	 */
	Zf(FFT)(rt1, logn);
	Zf(FFT)(rt2, logn);
	Zf(FFT)(rt3, logn);
	Zf(FFT)(rt4, logn);

	/*
	 * Compute:
	 *   rt5 = F*adj(f) + G*adj(g)
	 *   rt6 = 1 / (f*adj(f) + g*adj(g))
	 * (Note that rt6 is half-length.)
	 */
	rt5 = rt4 + n;
	rt6 = rt5 + n;
	Zf(poly_add_muladj_fft)(rt5, rt1, rt2, rt3, rt4, logn);
	Zf(poly_invnorm2_fft)(rt6, rt3, rt4, logn);

	/*
	 * Compute:
	 *   rt5 = (F*adj(f)+G*adj(g)) / (f*adj(f)+g*adj(g))
	 */
	Zf(poly_mul_autoadj_fft)(rt5, rt6, logn);

	/*
	 * Compute k as the rounded version of rt5. Check that none of
	 * the values is larger than 2^63-1 (in absolute value)
	 * because that would make the fpr_rint() do something undefined;
	 * note that any out-of-bounds value here implies a failure and
	 * (f,g) will be discarded, so we can make a simple test.
	 */
	Zf(iFFT)(rt5, logn);
	for (u = 0; u < n; u ++) {
		fpr z;

		z = rt5[u];
		if (!fpr_lt(z, fpr_ptwo63m1) || !fpr_lt(fpr_mtwo63m1, z)) {
			return 0;
		}
		rt5[u] = fpr_of(fpr_rint(z));
	}
	Zf(FFT)(rt5, logn);

	/*
	 * Subtract k*f from F, and k*g from G.
	 */
	Zf(poly_mul_fft)(rt3, rt5, logn);
	Zf(poly_mul_fft)(rt4, rt5, logn);
	Zf(poly_sub)(rt1, rt3, logn);
	Zf(poly_sub)(rt2, rt4, logn);
	Zf(iFFT)(rt1, logn);
	Zf(iFFT)(rt2, logn);

	/*
	 * Convert back F and G to integers, and return.
	 */
	Ft = tmp;
	Gt = Ft + n;
	rt3 = align_fpr(tmp, Gt + n);
	memmove(rt3, rt1, 2 * n * sizeof *rt1);
	rt1 = rt3;
	rt2 = rt1 + n;
	for (u = 0; u < n; u ++) {
		Ft[u] = (uint32_t)fpr_rint(rt1[u]);
		Gt[u] = (uint32_t)fpr_rint(rt2[u]);
	}

	return 1;
}

/*
 * Solving the NTRU equation, top level. Upon entry, the F and G
 * from the previous level should be in the tmp[] array.
 *
 * Returned value: 1 on success, 0 on error.
 */
static int
solve_NTRU_binary_depth0(unsigned logn,
	const int8_t *f, const int8_t *g, uint32_t *tmp)
{
	size_t n, hn, u;
	uint32_t p, p0i, R2;
	uint32_t *Fp, *Gp, *t1, *t2, *t3, *t4, *t5;
	uint32_t *gm, *igm, *ft, *gt;
	fpr *rt2, *rt3;

	n = (size_t)1 << logn;
	hn = n >> 1;

	/*
	 * Equations are:
	 *
	 *   f' = f0^2 - X^2*f1^2
	 *   g' = g0^2 - X^2*g1^2
	 *   F' and G' are a solution to f'G' - g'F' = q (from deeper levels)
	 *   F = F'*(g0 - X*g1)
	 *   G = G'*(f0 - X*f1)
	 *
	 * f0, f1, g0, g1, f', g', F' and G' are all "compressed" to
	 * degree N/2 (their odd-indexed coefficients are all zero).
	 *
	 * Everything should fit in 31-bit integers, hence we can just use
	 * the first small prime p = 2147473409.
	 */
	p = PRIMES[0].p;
	p0i = modp_ninv31(p);
	R2 = modp_R2(p, p0i);

	Fp = tmp;
	Gp = Fp + hn;
	ft = Gp + hn;
	gt = ft + n;
	gm = gt + n;
	igm = gm + n;

	modp_mkgm2(gm, igm, logn, PRIMES[0].g, p, p0i);

	/*
	 * Convert F' anf G' in NTT representation.
	 */
	for (u = 0; u < hn; u ++) {
		Fp[u] = modp_set(zint_one_to_plain(Fp + u), p);
		Gp[u] = modp_set(zint_one_to_plain(Gp + u), p);
	}
	modp_NTT2(Fp, gm, logn - 1, p, p0i);
	modp_NTT2(Gp, gm, logn - 1, p, p0i);

	/*
	 * Load f and g and convert them to NTT representation.
	 */
	for (u = 0; u < n; u ++) {
		ft[u] = modp_set(f[u], p);
		gt[u] = modp_set(g[u], p);
	}
	modp_NTT2(ft, gm, logn, p, p0i);
	modp_NTT2(gt, gm, logn, p, p0i);

	/*
	 * Build the unreduced F,G in ft and gt.
	 */
	for (u = 0; u < n; u += 2) {
		uint32_t ftA, ftB, gtA, gtB;
		uint32_t mFp, mGp;

		ftA = ft[u + 0];
		ftB = ft[u + 1];
		gtA = gt[u + 0];
		gtB = gt[u + 1];
		mFp = modp_montymul(Fp[u >> 1], R2, p, p0i);
		mGp = modp_montymul(Gp[u >> 1], R2, p, p0i);
		ft[u + 0] = modp_montymul(gtB, mFp, p, p0i);
		ft[u + 1] = modp_montymul(gtA, mFp, p, p0i);
		gt[u + 0] = modp_montymul(ftB, mGp, p, p0i);
		gt[u + 1] = modp_montymul(ftA, mGp, p, p0i);
	}
	modp_iNTT2(ft, igm, logn, p, p0i);
	modp_iNTT2(gt, igm, logn, p, p0i);

	Gp = Fp + n;
	t1 = Gp + n;
	memmove(Fp, ft, 2 * n * sizeof *ft);

	/*
	 * We now need to apply the Babai reduction. At that point,
	 * we have F and G in two n-word arrays.
	 *
	 * We can compute F*adj(f)+G*adj(g) and f*adj(f)+g*adj(g)
	 * modulo p, using the NTT. We still move memory around in
	 * order to save RAM.
	 */
	t2 = t1 + n;
	t3 = t2 + n;
	t4 = t3 + n;
	t5 = t4 + n;

	/*
	 * Compute the NTT tables in t1 and t2. We do not keep t2
	 * (we'll recompute it later on).
	 */
	modp_mkgm2(t1, t2, logn, PRIMES[0].g, p, p0i);

	/*
	 * Convert F and G to NTT.
	 */
	modp_NTT2(Fp, t1, logn, p, p0i);
	modp_NTT2(Gp, t1, logn, p, p0i);

	/*
	 * Load f and adj(f) in t4 and t5, and convert them to NTT
	 * representation.
	 */
	t4[0] = t5[0] = modp_set(f[0], p);
	for (u = 1; u < n; u ++) {
		t4[u] = modp_set(f[u], p);
		t5[n - u] = modp_set(-f[u], p);
	}
	modp_NTT2(t4, t1, logn, p, p0i);
	modp_NTT2(t5, t1, logn, p, p0i);

	/*
	 * Compute F*adj(f) in t2, and f*adj(f) in t3.
	 */
	for (u = 0; u < n; u ++) {
		uint32_t w;

		w = modp_montymul(t5[u], R2, p, p0i);
		t2[u] = modp_montymul(w, Fp[u], p, p0i);
		t3[u] = modp_montymul(w, t4[u], p, p0i);
	}

	/*
	 * Load g and adj(g) in t4 and t5, and convert them to NTT
	 * representation.
	 */
	t4[0] = t5[0] = modp_set(g[0], p);
	for (u = 1; u < n; u ++) {
		t4[u] = modp_set(g[u], p);
		t5[n - u] = modp_set(-g[u], p);
	}
	modp_NTT2(t4, t1, logn, p, p0i);
	modp_NTT2(t5, t1, logn, p, p0i);

	/*
	 * Add G*adj(g) to t2, and g*adj(g) to t3.
	 */
	for (u = 0; u < n; u ++) {
		uint32_t w;

		w = modp_montymul(t5[u], R2, p, p0i);
		t2[u] = modp_add(t2[u],
			modp_montymul(w, Gp[u], p, p0i), p);
		t3[u] = modp_add(t3[u],
			modp_montymul(w, t4[u], p, p0i), p);
	}

	/*
	 * Convert back t2 and t3 to normal representation (normalized
	 * around 0), and then
	 * move them to t1 and t2. We first need to recompute the
	 * inverse table for NTT.
	 */
	modp_mkgm2(t1, t4, logn, PRIMES[0].g, p, p0i);
	modp_iNTT2(t2, t4, logn, p, p0i);
	modp_iNTT2(t3, t4, logn, p, p0i);
	for (u = 0; u < n; u ++) {
		t1[u] = (uint32_t)modp_norm(t2[u], p);
		t2[u] = (uint32_t)modp_norm(t3[u], p);
	}

	/*
	 * At that point, array contents are:
	 *
	 *   F (NTT representation) (Fp)
	 *   G (NTT representation) (Gp)
	 *   F*adj(f)+G*adj(g) (t1)
	 *   f*adj(f)+g*adj(g) (t2)
	 *
	 * We want to divide t1 by t2. The result is not integral; it
	 * must be rounded. We thus need to use the FFT.
	 */

	/*
	 * Get f*adj(f)+g*adj(g) in FFT representation. Since this
	 * polynomial is auto-adjoint, all its coordinates in FFT
	 * representation are actually real, so we can truncate off
	 * the imaginary parts.
	 */
	rt3 = align_fpr(tmp, t3);
	for (u = 0; u < n; u ++) {
		rt3[u] = fpr_of(((int32_t *)t2)[u]);
	}
	Zf(FFT)(rt3, logn);
	rt2 = align_fpr(tmp, t2);
	memmove(rt2, rt3, hn * sizeof *rt3);

	/*
	 * Convert F*adj(f)+G*adj(g) in FFT representation.
	 */
	rt3 = rt2 + hn;
	for (u = 0; u < n; u ++) {
		rt3[u] = fpr_of(((int32_t *)t1)[u]);
	}
	Zf(FFT)(rt3, logn);

	/*
	 * Compute (F*adj(f)+G*adj(g))/(f*adj(f)+g*adj(g)) and get
	 * its rounded normal representation in t1.
	 */
	Zf(poly_div_autoadj_fft)(rt3, rt2, logn);
	Zf(iFFT)(rt3, logn);
	for (u = 0; u < n; u ++) {
		t1[u] = modp_set((int32_t)fpr_rint(rt3[u]), p);
	}

	/*
	 * RAM contents are now:
	 *
	 *   F (NTT representation) (Fp)
	 *   G (NTT representation) (Gp)
	 *   k (t1)
	 *
	 * We want to compute F-k*f, and G-k*g.
	 */
	t2 = t1 + n;
	t3 = t2 + n;
	t4 = t3 + n;
	t5 = t4 + n;
	modp_mkgm2(t2, t3, logn, PRIMES[0].g, p, p0i);
	for (u = 0; u < n; u ++) {
		t4[u] = modp_set(f[u], p);
		t5[u] = modp_set(g[u], p);
	}
	modp_NTT2(t1, t2, logn, p, p0i);
	modp_NTT2(t4, t2, logn, p, p0i);
	modp_NTT2(t5, t2, logn, p, p0i);
	for (u = 0; u < n; u ++) {
		uint32_t kw;

		kw = modp_montymul(t1[u], R2, p, p0i);
		Fp[u] = modp_sub(Fp[u],
			modp_montymul(kw, t4[u], p, p0i), p);
		Gp[u] = modp_sub(Gp[u],
			modp_montymul(kw, t5[u], p, p0i), p);
	}
	modp_iNTT2(Fp, t3, logn, p, p0i);
	modp_iNTT2(Gp, t3, logn, p, p0i);
	for (u = 0; u < n; u ++) {
		Fp[u] = (uint32_t)modp_norm(Fp[u], p);
		Gp[u] = (uint32_t)modp_norm(Gp[u], p);
	}

	return 1;
}

/*
 * Solve the NTRU equation. Returned value is 1 on success, 0 on error.
 * G can be NULL, in which case that value is computed but not returned.
 * If any of the coefficients of F and G exceeds lim (in absolute value),
 * then 0 is returned.
 */
static int
solve_NTRU(unsigned logn, int8_t *F, int8_t *G,
	const int8_t *f, const int8_t *g, int lim, uint32_t *tmp)
{
	size_t n, u;
	uint32_t *ft, *gt, *Ft, *Gt, *gm;
	uint32_t p, p0i, r;
	const small_prime *primes;

	n = MKN(logn);

	if (!solve_NTRU_deepest(logn, f, g, tmp)) {
		return 0;
	}

	/*
	 * For logn <= 2, we need to use solve_NTRU_intermediate()
	 * directly, because coefficients are a bit too large and
	 * do not fit the hypotheses in solve_NTRU_binary_depth0().
	 */
	if (logn <= 2) {
		unsigned depth;

		depth = logn;
		while (depth -- > 0) {
			if (!solve_NTRU_intermediate(logn, f, g, depth, tmp)) {
				return 0;
			}
		}
	} else {
		unsigned depth;

		depth = logn;
		while (depth -- > 2) {
            //printf("\nslove_NTRU depth[%d]",depth);
			if (!solve_NTRU_intermediate(logn, f, g, depth, tmp)) {
				return 0;
			}
		}
		if (!solve_NTRU_binary_depth1(logn, f, g, tmp)) {
			return 0;
		}
		if (!solve_NTRU_binary_depth0(logn, f, g, tmp)) {
			return 0;
		}
	}
    
	/*
	 * If no buffer has been provided for G, use a temporary one.
	 */
	if (G == NULL) {
		G = (int8_t *)(tmp + 2 * n);
	}

	/*
	 * Final F and G are in fk->tmp, one word per coefficient
	 * (signed value over 31 bits).
	 */
	if (!poly_big_to_small(F, tmp, lim, logn)
		|| !poly_big_to_small(G, tmp + n, lim, logn))
	{
		return 0;
	}
    
    
	/*
	 * Verify that the NTRU equation is fulfilled. Since all elements
	 * have short lengths, verifying modulo a small prime p works, and
	 * allows using the NTT.
	 *
	 * We put Gt[] first in tmp[], and process it first, so that it does
	 * not overlap with G[] in case we allocated it ourselves.
	 */
	Gt = tmp;
	ft = Gt + n;
	gt = ft + n;
	Ft = gt + n;
	gm = Ft + n;

	primes = PRIMES;
	p = primes[0].p;
	p0i = modp_ninv31(p);
	modp_mkgm2(gm, tmp, logn, primes[0].g, p, p0i);
	for (u = 0; u < n; u ++) {
		Gt[u] = modp_set(G[u], p);
	}
	for (u = 0; u < n; u ++) {
		ft[u] = modp_set(f[u], p);
		gt[u] = modp_set(g[u], p);
		Ft[u] = modp_set(F[u], p);
	}
	modp_NTT2(ft, gm, logn, p, p0i);
	modp_NTT2(gt, gm, logn, p, p0i);
	modp_NTT2(Ft, gm, logn, p, p0i);
	modp_NTT2(Gt, gm, logn, p, p0i);
	r = modp_montymul(12289, 1, p, p0i);
	for (u = 0; u < n; u ++) {
		uint32_t z;

		z = modp_sub(modp_montymul(ft[u], Gt[u], p, p0i),
			modp_montymul(gt[u], Ft[u], p, p0i), p);
		if (z != r) {
			return 0;
		}
	}

	return 1;
}


/* see falcon.h */
void
Zf(keygen)(inner_shake256_context *rng,
	int8_t *f, int8_t *g, int8_t *F, int8_t *G, uint16_t *h,
	unsigned logn, uint8_t *tmp)
{
	/*
	 * Algorithm is the following:
	 *
	 *  - Generate f and g with the Gaussian distribution.
	 *
	 *  - If either Res(f,phi) or Res(g,phi) is even, try again.
	 *
	 *  - If ||(f,g)|| is too large, try again.
	 *
	 *  - If ||B~_{f,g}|| is too large, try again.
	 *
	 *  - If f is not invertible mod phi mod q, try again.
	 *
	 *  - Compute h = g/f mod phi mod q.
	 *
	 *  - Solve the NTRU equation fG - gF = q; if the solving fails,
	 *    try again. Usual failure condition is when Res(f,phi)
	 *    and Res(g,phi) are not prime to each other.
	 */
	size_t n, u;
	uint16_t *h2, *tmp2;
	RNG_CONTEXT *rc;
	n = MKN(logn);
	rc = rng;
    int iteration = 1;
    double projection = 0;
	/*
	 * We need to generate f and g randomly, until we find values
	 * such that the norm of (g,-f), and of the orthogonalized
	 * vector, are satisfying. The orthogonalized vector is:
	 *   (q*adj(f)/(f*adj(f)+g*adj(g)), q*adj(g)/(f*adj(f)+g*adj(g)))
	 * (it is actually the (N+1)-th row of the Gram-Schmidt basis).
	 *
	 * In the binary case, coefficients of f and g are generated
	 * independently of each other, with a discrete Gaussian
	 * distribution of standard deviation 1.17*sqrt(q/(2*N)). Then,
	 * the two vectors have expected norm 1.17*sqrt(q), which is
	 * also our acceptance bound: we require both vectors to be no
	 * larger than that (this will be satisfied about 1/4th of the
	 * time, thus we expect sampling new (f,g) about 4 times for that
	 * step).
	 *
	 * We require that Res(f,phi) and Res(g,phi) are both odd (the
	 * NTRU equation solver requires it).
	 */
    for(int i = 0; i < iteration; i++){
	for (;;) {
		fpr *rt1, *rt2, *rt3;
		fpr bnorm;
        fpr bnorm_F;
        fpr bnorm_G;
        
		int32_t normf, normg, norm;
		int lim;

		/*
		 * The poly_small_mkgauss() function makes sure
		 * that the sum of coefficients is 1 modulo 2
		 * (i.e. the resultant of the polynomial with phi
		 * will be odd).
		 */
		//poly_small_mkgauss(rc, f, logn);
		//poly_small_mkgauss(rc, g, logn);
        Zf(CBD)(rc, f, logn);
        Zf(CBD)(rc, g, logn);
        /*printf("\n");
        for(u = 0; u < n; u++)
            printf("f[%d]",f[u]);
        for(u = 0; u < n; u++)
            printf("g[%d]",g[u]);*/

		/*
		 * Verify that all coefficients are within the bounds
		 * defined in max_fg_bits. This is the case with
		 * overwhelming probability; this guarantees that the
		 * key will be encodable with FALCON_COMP_TRIM.
		 */
		lim = 1 << (Zf(max_fg_bits)[logn] - 1);
		for (u = 0; u < n; u ++) {
			/*
			 * We can use non-CT tests since on any failure
			 * we will discard f and g.
			 */
			if (f[u] >= lim || f[u] <= -lim
				|| g[u] >= lim || g[u] <= -lim)
			{
				lim = -1;
				break;
			}
		}
		if (lim < 0) {
			continue;
		}

		/*
		 * Bound is 1.17*sqrt(q). We compute the squared
		 * norms. With q = 12289, the squared bound is:
		 *   (1.17^2)* 12289 = 16822.4121
		 * Since f and g are integral, the squared norm
		 * of (g,-f) is an integer.
		 */
		normf = poly_small_sqnorm(f, logn);
		normg = poly_small_sqnorm(g, logn);
		norm = (normf + normg) | -((normf | normg) >> 31);
        //printf("\n[original norm]normf : %d, normg : %d, norm : %d", normf, normg, norm);
		//if (norm >= 16823) {
		//	continue;
		//}

		/*
		 * We compute the orthogonalized vector norm.
		 */
		rt1 = (fpr *)tmp;
		rt2 = rt1 + n;
		rt3 = rt2 + n;
		poly_small_to_fp(rt1, f, logn);
		poly_small_to_fp(rt2, g, logn);
        
        //rt1 print EYSEO
        /*printf("original f(x) in fp form : ");
        for (int i = 0; i < 1024; i++)
        {
            printf("[%d]",f[i]);
            //printf("%f,", rt1[i]);
        }
        printf("\n");
        printf("original g(x) in fp form : ");
        for (int i = 0; i < 1024; i++)
        {
            printf("[%d]",g[i]);
            //printf("%f,", rt1[i]);
        }
        printf("\n");*/
        
		Zf(FFT)(rt1, logn);
		Zf(FFT)(rt2, logn);
		Zf(poly_invnorm2_fft)(rt3, rt1, rt2, logn);
		Zf(poly_adj_fft)(rt1, logn);
		Zf(poly_adj_fft)(rt2, logn);
		Zf(poly_mulconst)(rt1, fpr_q, logn);
		Zf(poly_mulconst)(rt2, fpr_q, logn);
		Zf(poly_mul_autoadj_fft)(rt1, rt3, logn);
		Zf(poly_mul_autoadj_fft)(rt2, rt3, logn);
		Zf(iFFT)(rt1, logn);
		Zf(iFFT)(rt2, logn);
		bnorm = fpr_zero;
        bnorm_F = fpr_zero;
        bnorm_G = fpr_zero;
		for (u = 0; u < n; u ++) {
            bnorm_F = fpr_add(bnorm_F, fpr_sqr(rt1[u]));
            bnorm_G = fpr_add(bnorm_G, fpr_sqr(rt2[u]));
			//bnorm = fpr_add(bnorm, fpr_sqr(rt1[u]));
			//bnorm = fpr_add(bnorm, fpr_sqr(rt2[u]));
		}
        bnorm = fpr_add(bnorm_F, bnorm_G);
        //printf("\n[expected norm]bnorm_F:%f, bnorm_G:%f, bnorm:%f", bnorm_F, bnorm_G, bnorm);
        //printf("\nbnorm:%f\n",bnorm);
		//if (!fpr_lt(bnorm, fpr_bnorm_max)) {
		//	continue;
		//}
        //여기서 r1, r2의 값을 찍어보면 orthogonal polynomial의 값을 알아볼 수 있음.
        //꼭 이 자리에서만 찍어야지 더 아래에서 찍으면 tmp의 값이 바뀌기 때문에 rt1에 있는 값이 바뀐다는 것에 유의
        //rt1 print EYSEO
        /*printf("\n G of orthogonal : ");
        for (int i = 0; i < 10; i++)
        {
            printf("[%f]", rt1[i]);
        }
        printf("\n F of orthogonal : ");
        for (int i = 0; i < 10; i++)
        {
            printf("[%f]", rt2[i]);
        }*/
		/*
		 * Compute public key h = g/f mod X^N+1 mod q. If this
		 * fails, we must restart.
		 */
		if (h == NULL) {
			h2 = (uint16_t *)tmp;
			tmp2 = h2 + n;
		} else {
			h2 = h;
			tmp2 = (uint16_t *)tmp;
		}
		if (!Zf(compute_public)(h2, f, g, logn, (uint8_t *)tmp2)) {
            //printf("\ncompute_public fail");
			continue;
		}

		/*
		 * Solve the NTRU equation to get F and G.
		 */
		lim = (1 << (Zf(max_FG_bits)[logn] - 1)) - 1;
		if (!solve_NTRU(logn, F, G, f, g, lim, (uint32_t *)tmp)) {
            //printf("\nsolve_NTRU fail");
			continue;
		}
        
        
        /*printf("\nFinal G : ");
        for (int i = 0; i < 10; i++)
        {
            printf("[%d]", G[i]);
        }
        printf("\nFinal F : ");
        for (int i = 0; i < 10; i++)
        {
            printf("[%d]", F[i]);
        }
        printf("\n");*/
        
        normf = poly_small_sqnorm(F, logn);
        normg = poly_small_sqnorm(G, logn);
        norm = (normf + normg) | -((normf | normg) >> 31);
        //printf("\n[Final norm] norm_F : %d, norm_G : %d, norm : %d", normf, normg, norm);
        //v = (g,-f), w = (G, -F) <v,w>/||w|| calculation
        normf = 0;
        normg = 0;
        for (u = 0; u < n; u ++)
        {
            normf = normf + (f[u]*F[u]);
            normg = normg + (g[u]*G[u]);
        }
        projection = projection + fabs((double)(normf+normg))/sqrt((double)norm);
        //printf("\ntest[%d]", i);
        //printf("\n projection:%f, normf:%d, normg:%d, norm: %d\n", projection, normf, normg, norm);
		/*
		 * Key pair is generated.
		 */
		break;
	}
    }
    projection = projection/iteration;
    //printf("\naverage:%f", projection);
}

/*void Zf(CBD)(inner_shake256_context *rng, int8_t *f, unsigned logn)//EYSEO CBD -3<x<3
{
    size_t n;
    RNG_CONTEXT *rc;
    n = MKN(logn - 2);
    rc = rng;
    uint64_t r;
    int i, j;
    uint32_t d, a[4], b[4];
    unsigned mod2 = 0;
    
    restart:
        for(i = 0; i < n; i++)
        {
            r = get_rng_u64(rc);
            d = 0;
            for (j = 0; j < 3; j++)
                d += (r >> j) & 0x249249;

            a[0] = d & 0x3;
            b[0] = (d >> 3) & 0x3;
            a[1] = (d >> 6) & 0x3;
            b[1] = (d >> 9) & 0x3;
            a[2] = (d >> 12) & 0x3;
            b[2] = (d >> 15) & 0x3;
            a[3] = (d >> 18) & 0x3;
            b[3] = (d >> 21) & 0x3;

            f[4 * i + 0] = (int8_t)(a[0] - b[0]);
            f[4 * i + 1] = (int8_t)(a[1] - b[1]);
            f[4 * i + 2] = (int8_t)(a[2] - b[2]);
            f[4 * i + 3] = (int8_t)(a[3] - b[3]);
        
            mod2 ^= (unsigned)((f[4*i]+f[4*i+1]+f[4*i+2]+f[4*i+3]) & 1);
        }
        if ((mod2 & 1) == 0) {
            printf("\n even number restart");
            goto restart;
        }
}*/

/*void Zf(CBD)(inner_shake256_context *rng, int8_t *f, unsigned logn)//EYSEO -4<x<4
{
    size_t n;
    RNG_CONTEXT *rc;
    n = MKN(logn - 2);
    rc = rng;
    uint64_t r;
    int i, j;
    uint32_t d, a[4], b[4];
    unsigned mod2 = 0;
    
    restart:
        for(i = 0; i < n; i++)
        {
            r = get_rng_u64(rc);
            d = 0;
            for (j = 0; j < 4; j++)
                d += (r >> j) & 0x11111111;

            a[0] = d & 0x7;
            b[0] = (d >> 4) & 0x7;
            a[1] = (d >> 8) & 0x7;
            b[1] = (d >> 12) & 0x7;
            a[2] = (d >> 16) & 0x7;
            b[2] = (d >> 20) & 0x7;
            a[3] = (d >> 24) & 0x7;
            b[3] = (d >> 28) & 0x7;

            f[4 * i + 0] = (int8_t)(a[0] - b[0]);
            f[4 * i + 1] = (int8_t)(a[1] - b[1]);
            f[4 * i + 2] = (int8_t)(a[2] - b[2]);
            f[4 * i + 3] = (int8_t)(a[3] - b[3]);
        
            mod2 ^= (unsigned)((f[4*i]+f[4*i+1]+f[4*i+2]+f[4*i+3]) & 1);
        }
        if ((mod2 & 1) == 0) {
            printf("\n even number restart");
            goto restart;
        }
}*/
/*void Zf(CBD)(inner_shake256_context *rng, int8_t *f, unsigned logn)//EYSEO -5<x<5
{
    size_t n;
    RNG_CONTEXT *rc;
    n = MKN(logn - 2);
    rc = rng;
    uint64_t r, d;
    int i, j;
    uint32_t a[4], b[4];
    unsigned mod2 = 0;
    
    restart:
        for(i = 0; i < n; i++)
        {
            r = get_rng_u64(rc);
            d = 0;
            for (j = 0; j < 5; j++)
                d += (r >> j) & 0x2108421;

            a[0] = d & 0x7;
            b[0] = (d >> 5) & 0x7;
            a[1] = (d >> 10) & 0x7;
            b[1] = (d >> 15) & 0x7;
            a[2] = (d >> 20) & 0x7;
            b[2] = (d >> 25) & 0x7;
            a[3] = (d >> 30) & 0x7;
            b[3] = (d >> 35) & 0x7;
            //printf("\na0[%d],b0[%d],a1[%d],b1[%d],a2[%d],b2[%d],a3[%d],b3[%d]",a[0],b[0],a[1],b[1],a[2],b[2],a[3],b[3]);

            f[4 * i + 0] = (int8_t)(a[0] - b[0]);
            f[4 * i + 1] = (int8_t)(a[1] - b[1]);
            f[4 * i + 2] = (int8_t)(a[2] - b[2]);
            f[4 * i + 3] = (int8_t)(a[3] - b[3]);
        
            mod2 ^= (unsigned)((f[4*i]+f[4*i+1]+f[4*i+2]+f[4*i+3]) & 1);
        }
        if ((mod2 & 1) == 0) {
            printf("\n even number restart");
            goto restart;
        }
}*/
/*void Zf(CBD)(inner_shake256_context *rng, int8_t *f, unsigned logn)//EYSEO -6<x<6
{
    size_t n;
    RNG_CONTEXT *rc;
    n = MKN(logn - 2);
    rc = rng;
    uint64_t r, d;
    int i, j;
    uint32_t a[4], b[4];
    unsigned mod2 = 0;
    
    restart:
        for(i = 0; i < n; i++)
        {
            r = get_rng_u64(rc);
            d = 0;
            for (j = 0; j < 6; j++)
                d += (r >> j) & 0x41041041041;

            a[0] = d & 0x7;
            b[0] = (d >> 6) & 0x7;
            a[1] = (d >> 12) & 0x7;
            b[1] = (d >> 18) & 0x7;
            a[2] = (d >> 24) & 0x7;
            b[2] = (d >> 30) & 0x7;
            a[3] = (d >> 36) & 0x7;
            b[3] = (d >> 42) & 0x7;
            //printf("\na0[%d],b0[%d],a1[%d],b1[%d],a2[%d],b2[%d],a3[%d],b3[%d]",a[0],b[0],a[1],b[1],a[2],b[2],a[3],b[3]);

            f[4 * i + 0] = (int8_t)(a[0] - b[0]);
            f[4 * i + 1] = (int8_t)(a[1] - b[1]);
            f[4 * i + 2] = (int8_t)(a[2] - b[2]);
            f[4 * i + 3] = (int8_t)(a[3] - b[3]);
        
            mod2 ^= (unsigned)((f[4*i]+f[4*i+1]+f[4*i+2]+f[4*i+3]) & 1);
        }
        if ((mod2 & 1) == 0) {
            printf("\n even number restart");
            goto restart;
        }
}*/

/*void Zf(CBD)(inner_shake256_context *rng, int8_t *f, unsigned logn)//EYSEO -7<x<7
{
    size_t n;
    RNG_CONTEXT *rc;
    n = MKN(logn - 2);
    rc = rng;
    uint64_t r, d;
    int i, j;
    uint32_t a[4], b[4];
    unsigned mod2;
    
    restart:
        mod2 = 0;
        for(i = 0; i < n; i++)
        {
            r = get_rng_u64(rc);
            d = 0;
            for (j = 0; j < 7; j++)
                d += (r >> j) & 0x2040810204081;

            a[0] = d & 0x7;
            b[0] = (d >> 7) & 0x7;
            a[1] = (d >> 14) & 0x7;
            b[1] = (d >> 21) & 0x7;
            a[2] = (d >> 28) & 0x7;
            b[2] = (d >> 35) & 0x7;
            a[3] = (d >> 42) & 0x7;
            b[3] = (d >> 49) & 0x7;
            //printf("\na0[%d],b0[%d],a1[%d],b1[%d],a2[%d],b2[%d],a3[%d],b3[%d]",a[0],b[0],a[1],b[1],a[2],b[2],a[3],b[3]);

            f[4 * i + 0] = (int8_t)(a[0] - b[0]);
            f[4 * i + 1] = (int8_t)(a[1] - b[1]);
            f[4 * i + 2] = (int8_t)(a[2] - b[2]);
            f[4 * i + 3] = (int8_t)(a[3] - b[3]);
        
            mod2 ^= (unsigned)((f[4*i]+f[4*i+1]+f[4*i+2]+f[4*i+3]) & 1);
        }
        if ((mod2 & 1) == 0) {
            printf("\n even number restart");
            goto restart;
        }
}*/
/*void Zf(CBD)(inner_shake256_context *rng, int8_t *f, unsigned logn)//EYSEO -8<x<8
{
    size_t n;
    RNG_CONTEXT *rc;
    n = MKN(logn - 2);
    rc = rng;
    uint64_t r, d;
    int i, j;
    uint32_t a[4], b[4];
    unsigned mod2;
    
    restart:
        mod2 = 0;
        for(i = 0; i < n; i++)
        {
            r = get_rng_u64(rc);
            d = 0;
            for (j = 0; j < 8; j++)
                d += (r >> j) & 0x101010101010101;

            a[0] = d & 0x5;
            b[0] = (d >> 8) & 0x5;
            a[1] = (d >> 16) & 0x5;
            b[1] = (d >> 24) & 0x5;
            a[2] = (d >> 32) & 0x5;
            b[2] = (d >> 40) & 0x5;
            a[3] = (d >> 48) & 0x5;
            b[3] = (d >> 56) & 0x5;
            //printf("\na0[%d],b0[%d],a1[%d],b1[%d],a2[%d],b2[%d],a3[%d],b3[%d]",a[0],b[0],a[1],b[1],a[2],b[2],a[3],b[3]);

            f[4 * i + 0] = (int8_t)(a[0] - b[0]);
            f[4 * i + 1] = (int8_t)(a[1] - b[1]);
            f[4 * i + 2] = (int8_t)(a[2] - b[2]);
            f[4 * i + 3] = (int8_t)(a[3] - b[3]);
            //printf("\nf[%d],f[%d],f[%d],f[%d]",f[4*i],f[4*i+1],f[4*i+2],f[4*i+3]);
        
            mod2 ^= (unsigned)((f[4*i]+f[4*i+1]+f[4*i+2]+f[4*i+3]) & 1);
        }
        if ((mod2 & 1) == 0) {
            printf("\neven number restart");
            goto restart;
        }
}*/

/*void Zf(CBD)(inner_shake256_context *rng, int8_t *f, unsigned logn)//EYSEO -9<x<9
{
    size_t n;
    RNG_CONTEXT *rc;
    n = MKN(logn - 1);
    rc = rng;
    uint64_t r, d;
    int i, j;
    uint32_t a[2], b[2];
    unsigned mod2;
    int mask;
    
    restart:
        mod2 = 0;
        for(i = 0; i < n; i++)
        {
            r = get_rng_u64(rc);
            //printf("\n");
            //for (int k = 61; k >= 0; k--)
            //{
                //mask = 1 << k;
                //printf("%d", r & mask ? 1 : 0);
            //}
            
            d = 0;
            for (j = 0; j < 9; j++)
                d += (r >> j) & 0x8040201;

            a[0] = d & 0xf;
            b[0] = (d >> 9) & 0xf;
            a[1] = (d >> 18) & 0xf;
            b[1] = (d >> 27) & 0xf;
            //printf("\na0[%d],b0[%d],a1[%d],b1[%d],a2[%d],b2[%d],a3[%d],b3[%d]",a[0],b[0],a[1],b[1],a[2],b[2],a[3],b[3]);
            //printf("\na0[%d],b0[%d],a1[%d],b1[%d]",a[0],b[0],a[1],b[1]);

            f[2 * i] = (int8_t)(a[0] - b[0]);
            f[2 * i + 1] = (int8_t)(a[1] - b[1]);
            //printf("\nf[%d],f[%d]",f[2*i],f[2*i+1]);
        
            mod2 ^= (unsigned)((f[2*i]+f[2*i+1]) & 1);
        }
        if ((mod2 & 1) == 0) {
            printf("\neven number restart");
            goto restart;
        }
}*/
/*void Zf(CBD)(inner_shake256_context *rng, int8_t *f, unsigned logn)//EYSEO -3<=x<=4 uniform
{
    size_t n;
    RNG_CONTEXT *rc;
    n = MKN(logn - 2);
    rc = rng;
    uint64_t r, d;
    int i, j;
    uint32_t a[4], b[4];
    unsigned mod2;
    int mask;
    
    restart:
        mod2 = 0;
        for(i = 0; i < n; i++)
        {
            r = get_rng_u64(rc);
           

            f[4 * i] = (int8_t)(r & 0xf)-7;
            f[4 * i + 1] = (int8_t)((r>>4) & 0xf)-7;
            f[4 * i + 2] = (int8_t)((r>>8) & 0xf)-7;
            f[4 * i + 3] = (int8_t)((r>>12) & 0xf)-7;
            //printf("\nf[%d],f[%d],f[%d],f[%d]",f[4*i],f[4*i+1],f[4*i+2],f[4*i+3]);
        
            mod2 ^= (unsigned)((f[4*i]+f[4*i+1]+f[4*i+2]+f[4*i+3]) & 1);
        }
        if ((mod2 & 1) == 0) {
            //printf("\neven number restart");
            goto restart;
        }
    printf("finish");
}*/
/*void Zf(CBD)(inner_shake256_context *rng, int8_t *f, unsigned logn)//EYSEO -10<x<10
{
    size_t n;
    RNG_CONTEXT *rc;
    n = MKN(logn - 1);
    rc = rng;
    uint64_t r, d;
    int i, j;
    uint32_t a[2], b[2];
    unsigned mod2;
    //int mask;
    
    restart:
        mod2 = 0;
        for(i = 0; i < n; i++)
        {
            r = get_rng_u64(rc);
            
            d = 0;
            for (j = 0; j < 10; j++)
                d += (r >> j) & 0x40100401;

            a[0] = d & 0xf;
            b[0] = (d >> 10) & 0xf;
            a[1] = (d >> 20) & 0xf;
            b[1] = (d >> 30) & 0xf;
            //printf("\na0[%d],b0[%d],a1[%d],b1[%d],a2[%d],b2[%d],a3[%d],b3[%d]",a[0],b[0],a[1],b[1],a[2],b[2],a[3],b[3]);
            //printf("\na0[%d],b0[%d],a1[%d],b1[%d]",a[0],b[0],a[1],b[1]);

            f[2 * i] = (int8_t)(a[0] - b[0]);
            f[2 * i + 1] = (int8_t)(a[1] - b[1]);
            printf("f[%d],f[%d]",f[2*i],f[2*i+1]);
        
            mod2 ^= (unsigned)((f[2*i]+f[2*i+1]) & 1);
        }
        if ((mod2 & 1) == 0) {
            printf("\neven number restart");
            goto restart;
        }
}*/

/*void Zf(CBD)(inner_shake256_context *rng, int8_t *f, unsigned logn)//EYSEO -11<x<11
{
    size_t n;
    RNG_CONTEXT *rc;
    n = MKN(logn - 1);
    rc = rng;
    uint64_t r, d;
    int i, j;
    uint32_t a[2], b[2];
    unsigned mod2;
    //int mask;
    
    restart:
        mod2 = 0;
        for(i = 0; i < n; i++)
        {
            r = get_rng_u64(rc);
            
            d = 0;
            for (j = 0; j < 11; j++)
                d += (r >> j) & 0x200400801;

            a[0] = d & 0xf;
            b[0] = (d >> 11) & 0xf;
            a[1] = (d >> 22) & 0xf;
            b[1] = (d >> 33) & 0xf;
            //printf("\na0[%d],b0[%d],a1[%d],b1[%d],a2[%d],b2[%d],a3[%d],b3[%d]",a[0],b[0],a[1],b[1],a[2],b[2],a[3],b[3]);
            //printf("\na0[%d],b0[%d],a1[%d],b1[%d]",a[0],b[0],a[1],b[1]);

            f[2 * i] = abs((int8_t)(a[0] - b[0]));
            f[2 * i + 1] = (int8_t)(a[1] - b[1]);
            //printf("f[%d],f[%d]",f[2*i],f[2*i+1]);
        
            mod2 ^= (unsigned)((f[2*i]+f[2*i+1]) & 1);
        }
        if ((mod2 & 1) == 0) {
            //printf("\neven number restart");
            goto restart;
        }
}*/

/*void Zf(CBD)(inner_shake256_context *rng, int8_t *f, unsigned logn)//EYSEO -12<x<12
{
    size_t n;
    RNG_CONTEXT *rc;
    n = MKN(logn - 1);
    rc = rng;
    uint64_t r, d;
    int i, j;
    uint32_t a[2], b[2];
    unsigned mod2;
    //int mask;
    
    restart:
        mod2 = 0;
        for(i = 0; i < n; i++)
        {
            r = get_rng_u64(rc);
            
            d = 0;
            for (j = 0; j < 12; j++)
                d += (r >> j) & 0x1001001001;

            a[0] = d & 0xf;
            b[0] = (d >> 12) & 0xf;
            a[1] = (d >> 24) & 0xf;
            b[1] = (d >> 36) & 0xf;
            //printf("\na0[%d],b0[%d],a1[%d],b1[%d],a2[%d],b2[%d],a3[%d],b3[%d]",a[0],b[0],a[1],b[1],a[2],b[2],a[3],b[3]);
            //printf("\na0[%d],b0[%d],a1[%d],b1[%d]",a[0],b[0],a[1],b[1]);

            f[2 * i] = (int8_t)(a[0] - b[0]);
            f[2 * i + 1] = (int8_t)(a[1] - b[1]);
            //printf("f[%d],f[%d]",f[2*i],f[2*i+1]);
        
            mod2 ^= (unsigned)((f[2*i]+f[2*i+1]) & 1);
        }
        if ((mod2 & 1) == 0) {
            //printf("\neven number restart");
            goto restart;
        }
}*/
void Zf(CBD)(inner_shake256_context *rng, int8_t *f, unsigned logn)//EYSEO -13<=x<=13
{
    size_t n;
    RNG_CONTEXT *rc;
    n = MKN(logn - 1);
    rc = rng;
    uint64_t r, d;
    int i, j;
    uint32_t a[2], b[2];
    unsigned mod2;
    
    restart:
        mod2 = 0;
        for(i = 0; i < n; i++)
        {
            r = get_rng_u64(rc);
            
            d = 0;
            for (j = 0; j < 13; j++)
                d += (r >> j) & 0x8004002001;

            a[0] = d & 0xf;
            b[0] = (d >> 13) & 0xf;
            a[1] = (d >> 26) & 0xf;
            b[1] = (d >> 39) & 0xf;
            //printf("\na0[%d],b0[%d],a1[%d],b1[%d],a2[%d],b2[%d],a3[%d],b3[%d]",a[0],b[0],a[1],b[1],a[2],b[2],a[3],b[3]);
            //printf("\na0[%d],b0[%d],a1[%d],b1[%d]",a[0],b[0],a[1],b[1]);

            f[2 * i] = (int8_t)(a[0] - b[0]);
            f[2 * i + 1] = (int8_t)(a[1] - b[1]);
            //printf("f[%d],f[%d]",f[2*i],f[2*i+1]);
        
            mod2 ^= (unsigned)((f[2*i]+f[2*i+1]) & 1);
        }
        if ((mod2 & 1) == 0) {
            //printf("\neven number restart");
            goto restart;
        }
}
/*void Zf(CBD)(inner_shake256_context *rng, int8_t *f, unsigned logn)//EYSEO -14<=x<=14
{
    size_t n;
    RNG_CONTEXT *rc;
    n = MKN(logn - 1);
    rc = rng;
    uint64_t r, d;
    int i, j;
    uint32_t a[2], b[2];
    unsigned mod2;
    
    restart:
        mod2 = 0;
        for(i = 0; i < n; i++)
        {
            r = get_rng_u64(rc);
            
            d = 0;
            for (j = 0; j < 14; j++)
                d += (r >> j) & 0x40010004001;

            a[0] = d & 0xf;
            b[0] = (d >> 14) & 0xf;
            a[1] = (d >> 28) & 0xf;
            b[1] = (d >> 42) & 0xf;

            f[2 * i] = (int8_t)(a[0] - b[0]);
            f[2 * i + 1] = (int8_t)(a[1] - b[1]);
        
            mod2 ^= (unsigned)((f[2*i]+f[2*i+1]) & 1);
        }
        if ((mod2 & 1) == 0) {
            //printf("\neven number restart");
            goto restart;
        }
}*/
/*void Zf(CBD)(inner_shake256_context *rng, int8_t *f, unsigned logn)//EYSEO -15<=x<=15
{
    size_t n;
    RNG_CONTEXT *rc;
    n = MKN(logn - 1);
    rc = rng;
    uint64_t r, d;
    int i, j;
    uint32_t a[2], b[2];
    unsigned mod2;
    
    restart:
        mod2 = 0;
        for(i = 0; i < n; i++)
        {
            r = get_rng_u64(rc);
            
            d = 0;
            for (j = 0; j < 15; j++)
                d += (r >> j) & 0x200040008001;

            a[0] = d & 0xf;
            b[0] = (d >> 15) & 0xf;
            a[1] = (d >> 30) & 0xf;
            b[1] = (d >> 45) & 0xf;

            f[2 * i] = (int8_t)(a[0] - b[0]);
            f[2 * i + 1] = (int8_t)(a[1] - b[1]);
        
            mod2 ^= (unsigned)((f[2*i]+f[2*i+1]) & 1);
        }
        if ((mod2 & 1) == 0) {
            //printf("\neven number restart");
            goto restart;
        }
}*/






