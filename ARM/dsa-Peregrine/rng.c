/*
 * PRNG and interface to the system RNG.
 * Peregrine Related Functions which are adopted from FALCON Implementation
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

#include <assert.h>
#include "inner.h"
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

/* see inner.h */
int
Zf(get_seed)(void *seed, size_t len)
{
	(void)seed;
	if (len == 0) {
		return 1;
	}

	int f;

	f = open("/dev/urandom", O_RDONLY);
	if (f >= 0) {
		while (len > 0) {
			ssize_t rlen;

			rlen = read(f, seed, len);
			if (rlen < 0) {
				if (errno == EINTR) {
					continue;
				}
				break;
			}
			seed = (uint8_t *)seed + rlen;
			len -= (size_t)rlen;
		}
		close(f);
		if (len == 0) {
			return 1;
		}
	}
	return 0;
}

/* see inner.h */
void
Zf(prng_init)(prng *p, inner_shake256_context *src)
{
	inner_shake256_extract(src, p->state.d, 56);
	Zf(prng_refill)(p);
}


void
Zf(prng_refill)(prng *p)
{
	static const uint32_t CW[] = {
		0x61707865, 0x3320646e, 0x79622d32, 0x6b206574
	};

	uint64_t cc;
	size_t u;

	/*
	 * State uses local endianness. Only the output bytes must be
	 * converted to little endian (if used on a big-endian machine).
	 */
	cc = *(uint64_t *)(p->state.d + 48);
	for (u = 0; u < 8; u ++) {
		uint32_t state[16];
		size_t v;
		int i;

		memcpy(&state[0], CW, sizeof CW);
		memcpy(&state[4], p->state.d, 48);
		state[14] ^= (uint32_t)cc;
		state[15] ^= (uint32_t)(cc >> 32);
		for (i = 0; i < 10; i ++) {

#define QROUND(a, b, c, d)   do { \
		state[a] += state[b]; \
		state[d] ^= state[a]; \
		state[d] = (state[d] << 16) | (state[d] >> 16); \
		state[c] += state[d]; \
		state[b] ^= state[c]; \
		state[b] = (state[b] << 12) | (state[b] >> 20); \
		state[a] += state[b]; \
		state[d] ^= state[a]; \
		state[d] = (state[d] <<  8) | (state[d] >> 24); \
		state[c] += state[d]; \
		state[b] ^= state[c]; \
		state[b] = (state[b] <<  7) | (state[b] >> 25); \
	} while (0)

			QROUND( 0,  4,  8, 12);
			QROUND( 1,  5,  9, 13);
			QROUND( 2,  6, 10, 14);
			QROUND( 3,  7, 11, 15);
			QROUND( 0,  5, 10, 15);
			QROUND( 1,  6, 11, 12);
			QROUND( 2,  7,  8, 13);
			QROUND( 3,  4,  9, 14);

#undef QROUND

		}

		for (v = 0; v < 4; v ++) {
			state[v] += CW[v];
		}
		for (v = 4; v < 14; v ++) {
			state[v] += ((uint32_t *)p->state.d)[v - 4];
		}
		state[14] += ((uint32_t *)p->state.d)[10]
			^ (uint32_t)cc;
		state[15] += ((uint32_t *)p->state.d)[11]
			^ (uint32_t)(cc >> 32);
		cc ++;

		/*
		 * We mimic the interleaving that is used in the AVX2
		 * implementation.
		 */
		for (v = 0; v < 16; v ++) {
			((uint32_t *)p->buf.d)[u + (v << 3)] = state[v];
		}
	}
	*(uint64_t *)(p->state.d + 48) = cc;
	p->ptr = 0;
}

/* see inner.h */
void
Zf(prng_get_bytes)(prng *p, void *dst, size_t len)
{
	uint8_t *buf;

	buf = dst;
	while (len > 0) {
		size_t clen;

		clen = (sizeof p->buf.d) - p->ptr;
		if (clen > len) {
			clen = len;
		}
		memcpy(buf, p->buf.d, clen);
		buf += clen;
		len -= clen;
		p->ptr += clen;
		if (p->ptr == sizeof p->buf.d) {
			Zf(prng_refill)(p);
		}
	}
}
