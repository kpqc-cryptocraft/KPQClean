#include <stdint.h>
#include "params.h"
#include "sign.h"
#include "packing.h"
#include "poly.h"
#include "randombytes.h"
#include "symmetric.h"
#include "fips202.h"
#include "stdio.h"

int crypto_sign_keypair(uint8_t *pk, uint8_t *sk) {
	uint8_t zeta[SEEDBYTES];
	uint8_t seedbuf[3 * SEEDBYTES];
	uint8_t tr[SEEDBYTES];
	const uint8_t *xi_1, *xi_2, *key;

	poly mat;
	poly s1, s1hat, s2, t1, t0;
	poly ms1, mt;

	randombytes(zeta, SEEDBYTES);
	randombytes(seedbuf, SEEDBYTES);
	shake256(seedbuf, 3 * SEEDBYTES, seedbuf, SEEDBYTES);
	xi_1 = seedbuf;
	xi_2 = seedbuf + SEEDBYTES;
	key = seedbuf + 2 * SEEDBYTES;

	poly_uniform(&mat, zeta, 0);
	poly_uniform_eta(&s1, xi_1, 0);
	poly_uniform_eta(&s2, xi_2, 0);

	s1hat = s1;

	poly_tomont(&ms1, &s1hat);
	poly_mul(&mt, &ms1, &mat);
	poly_frommont(&t1, &mt);

	poly_add(&t1, &t1, &s2);
	poly_caddq(&t1);

	poly_power2round(&t1, &t0, &t1);

	pack_pk(pk, zeta, &t1);

	shake256(tr, SEEDBYTES, pk, CRYPTO_PUBLICKEYBYTES);

	pack_sk(sk, zeta, tr, key, &t0, &s1, &s2);

	return 0;
}

int crypto_sign_signature(uint8_t *sig,
                        	size_t *siglen,
                        	const uint8_t *m,
                        	size_t mlen,
                        	const uint8_t *sk)
{
	unsigned int n;
	uint8_t seedbuf[3 * SEEDBYTES + 2 * CRHBYTES];
	uint8_t *zeta, *tr, *key, *mu, *rho;
	uint16_t nonce = 0;

	poly mat, s1, y, z, t0, s2, w1, w0, h;
	poly cp;
	keccak_state state;
	poly ms1, ms2, mt0, mz, mw1, mcp, mh;

	zeta = seedbuf;
	tr = zeta + SEEDBYTES;
	key = tr + SEEDBYTES;
	mu = key + SEEDBYTES;
	rho = mu + CRHBYTES;
	unpack_sk(zeta, tr, key, &t0, &s1, &s2, sk);

	shake256_init(&state);
	shake256_absorb(&state, tr, SEEDBYTES);
	shake256_absorb(&state, m, mlen);
	shake256_finalize(&state);
	shake256_squeeze(mu, CRHBYTES, &state);

#ifdef NIMS_RANDOMIZED_SIGNING
	randombytes(rho, CRHBYTES);
#else
	shake256(rho, CRHBYTES, key, SEEDBYTES + CRHBYTES);
#endif

	poly_uniform(&mat, zeta, 0);

	poly_tomont(&ms1, &s1);
	poly_tomont(&ms2, &s2);
	poly_tomont(&mt0, &t0);

rej:
	poly_uniform_gamma1(&y, rho, nonce++);

	z = y;

	poly_tomont(&mz, &z);
	poly_mul(&mw1, &mz, &mat);
	poly_frommont(&w1, &mw1);

	poly_decompose(&w1, &w0, &w1);
	polyw1_pack(sig, &w1);

	shake256_init(&state);
	shake256_absorb(&state, mu, CRHBYTES);
	shake256_absorb(&state, sig, POLYW1_PACKEDBYTES);
	shake256_finalize(&state);
	shake256_squeeze(sig, SEEDBYTES, &state);
	poly_challenge(&cp, sig);

	poly_tomont(&mcp, &cp);
	poly_mul(&mz, &mcp, &ms1);
	poly_frommont(&z, &mz);

	poly_add(&z, &z, &y);

	if (poly_chknorm(&z, GAMMA1 - BETA))
		goto rej;

	poly_mul(&mh, &mcp, &ms2);
	poly_frommont(&h, &mh);

	poly_sub(&w0, &w0, &h);

	if (poly_chknorm(&w0, GAMMA2 - BETA))
		goto rej;

	poly_mul(&mh, &mcp, &mt0);
	poly_frommont(&h, &mh);

	if (poly_chknorm(&h, GAMMA2))
		goto rej;

	poly_add(&w0, &w0, &h);
	poly_caddq(&w0);
	n = poly_make_hint(&h, &w0, &w1);

	if (n > OMEGA)
		goto rej;

	pack_sig(sig, sig, &z, &h);
	*siglen = CRYPTO_BYTES;
	return 0;
}

int crypto_sign(uint8_t *sm,
              	size_t *smlen,
              	const uint8_t *m,
              	size_t mlen,
              	const uint8_t *sk)
{
	size_t i;

	for (i = 0; i < mlen; ++i)
		sm[CRYPTO_BYTES + mlen - 1 - i] = m[mlen - 1 - i];
	crypto_sign_signature(sm, smlen, sm + CRYPTO_BYTES, mlen, sk);
	*smlen += mlen;
	return 0;
}

int crypto_sign_verify(const uint8_t *sig,
                       size_t siglen,
                       const uint8_t *m,
                       size_t mlen,
                       const uint8_t *pk)
{
	unsigned int i;
	uint8_t buf[POLYW1_PACKEDBYTES];
	uint8_t zeta[SEEDBYTES];
	uint8_t mu[CRHBYTES];
	uint8_t c[SEEDBYTES];
	uint8_t c2[SEEDBYTES];

	poly cp, mat, z, t1, w1, h;
	keccak_state state;
	poly mz, mw1, mcp, mt1, mt1_temp;

	if (siglen != CRYPTO_BYTES)
		return -1;
	unpack_pk(zeta, &t1, pk);
	if (unpack_sig(c, &z, &h, sig))
		return -1;

	if (poly_chknorm(&z, GAMMA1 - BETA))
		return -1;

	shake256(mu, SEEDBYTES, pk, CRYPTO_PUBLICKEYBYTES);
	shake256_init(&state);
	shake256_absorb(&state, mu, SEEDBYTES);
	shake256_absorb(&state, m, mlen);
	shake256_finalize(&state);
	shake256_squeeze(mu, CRHBYTES, &state);

	poly_challenge(&cp, c);

	poly_uniform(&mat, zeta, 0);

	poly_tomont(&mz, &z);
	poly_mul(&mw1, &mz, &mat);
	poly_frommont(&w1, &mw1);

	poly_shiftl(&t1);

	poly_tomont(&mcp, &cp);
	poly_tomont(&mt1_temp, &t1);
	poly_mul(&mt1, &mcp, &mt1_temp);
	poly_frommont(&t1, &mt1);

	poly_modsub(&w1, &w1, &t1);

	poly_use_hint(&w1, &w1, &h);
	polyw1_pack(buf, &w1);

	shake256_init(&state);
	shake256_absorb(&state, mu, CRHBYTES);
	shake256_absorb(&state, buf, POLYW1_PACKEDBYTES);
	shake256_finalize(&state);
	shake256_squeeze(c2, SEEDBYTES, &state);
	for (i = 0; i < SEEDBYTES; ++i)
		if (c[i] != c2[i])
			return -1;

	return 0;
}

int crypto_sign_open(uint8_t *m,
                     size_t *mlen,
                     const uint8_t *sm,
                     size_t smlen,
                     const uint8_t *pk)
{
	size_t i;

	if (smlen < CRYPTO_BYTES)
		goto badsig;

	*mlen = smlen - CRYPTO_BYTES;
	if (crypto_sign_verify(sm, CRYPTO_BYTES, sm + CRYPTO_BYTES, *mlen, pk))
		goto badsig;
	else {
		for (i = 0; i < *mlen; ++i)
			m[i] = sm[CRYPTO_BYTES + i];
		return 0;
	}

badsig:
	/* Signature verification failed */
	*mlen = -1;
	for (i = 0; i < smlen; ++i)
		m[i] = 0;

	return -1;
}
