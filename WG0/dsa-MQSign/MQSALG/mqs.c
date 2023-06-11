#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "mqs_config.h"
#include "mqs_keypair.h"
#include "mqs.h"
#include "blas.h"
#include "mqs_blas.h"
#include "utils_prng.h"
#include "utils_hash.h"
#include "utils_malloc.h"

#define MAX_ATTEMPT_FRMAT  128
#define _MAX_O  _O
#define _MAX_O_BYTE  _O


static inline
void transpose(uint8_t * r, const uint8_t * a)
{
	for (unsigned i = 0; i < _O; i++) {
		for (unsigned j = 0; j < _O; j++) {
			r[i * _O + j] = a[j * _O + i];
		}
	}
}

int mqss_sign(uint8_t* signature, const sk_mqss* sk, const uint8_t* sk_seed, const uint8_t* _digest, uint8_t depth)
{
	uint8_t mat[_O * _O_BYTE];
	uint8_t mat_tmp[_O * _O_BYTE];

	prng_t prng_sign;
	uint8_t prng_preseed[LEN_SKSEED + _HASH_LEN];
	memcpy(prng_preseed, sk_seed, LEN_SKSEED);
	memcpy(prng_preseed + LEN_SKSEED, _digest, _HASH_LEN);

	uint8_t prng_seed[_HASH_LEN];
	hash_msg(prng_seed, _HASH_LEN, prng_preseed, _HASH_LEN + LEN_SKSEED);
	prng_set(&prng_sign, prng_seed, _HASH_LEN);

	for (unsigned i = 0; i < LEN_SKSEED + _HASH_LEN; i++) prng_preseed[i] ^= prng_preseed[i];
	for (unsigned i = 0; i < _HASH_LEN; i++) prng_seed[i] ^= prng_seed[i];

	uint8_t vinegar[_V_BYTE];
	unsigned n_attempt = 0;
	unsigned succ = 0;

	uint8_t y[_PUB_N_BYTE];
	uint8_t* x_v = vinegar;
	uint8_t x_o[_O_BYTE];

	uint8_t r_l1_F1[_O_BYTE] = { 0 };
	uint8_t digest_salt[_HASH_LEN + _SALT_BYTE];
	uint8_t* salt = digest_salt + _HASH_LEN;
	uint8_t temp_o[_MAX_O_BYTE + 32] = { 0 };

	memcpy(digest_salt, _digest, _HASH_LEN);
	prng_gen(&prng_sign, salt, _SALT_BYTE);
	hash_msg(y, _PUB_M_BYTE, digest_salt, _HASH_LEN + _SALT_BYTE);


	while (!succ) {
		if (MAX_ATTEMPT_FRMAT <= n_attempt) break;
		prng_gen(&prng_sign, vinegar, _V_BYTE);

		sparmat_gen_linear(mat_tmp, sk->Fq2, vinegar, _O);
		gf256v_add(mat_tmp, sk->Fl + _O * _V, _O * _O);

		sparmat_vv_eval(r_l1_F1, sk->Fq1, vinegar, _O);
		memcpy(temp_o, r_l1_F1, _O_BYTE);

		for (unsigned i = 0; i < _V; i++) {
			gf256v_madd(temp_o, sk->Fl + i * _O, vinegar[i], _O);
		}

		gf256v_add(temp_o, sk->Fc, _O);
		gf256v_add(temp_o, y, _O_BYTE);

		succ = gf256mat_solve_linear_eq(x_o, mat_tmp, _O_BYTE, temp_o, depth);
		n_attempt++;
	}

	//  w = T^-1 * y
	uint8_t w[_PUB_N_BYTE];
	// identity part of T.
	memcpy(w, x_v, _V_BYTE);
	memcpy(w + _V_BYTE, x_o, _O_BYTE);
	gf256v_add(w, sk->vec_t, _PUB_N_BYTE);
	// Computing the t1 part.
	gfmat_prod(y, sk->mat_t, _V_BYTE, _O, w + _V);
	gf256v_add(w, y, _V_BYTE);

	memset(signature, 0, _SIGNATURE_BYTE);  // set the output 0
	// clean
	memset(mat, 0, _O * _O_BYTE);
	memset(&prng_sign, 0, sizeof(prng_t));
	memset(vinegar, 0, _V_BYTE);
	memset(r_l1_F1, 0, _O_BYTE);
	memset(y, 0, _PUB_N_BYTE);
	memset(x_o, 0, _O_BYTE);
	memset(temp_o, 0, sizeof(temp_o));

	// return: copy w and salt to the signature.
	if (MAX_ATTEMPT_FRMAT <= n_attempt) return -1;
	gf256v_add(signature, w, _PUB_N_BYTE);
	gf256v_add(signature + _PUB_N_BYTE, salt, _SALT_BYTE);
	return 0;
}

int mqsc_verify(const uint8_t* digest, const uint8_t* signature, const uint8_t* pk)
{
	// assert(_O1>=_O2);
	unsigned char digest_ck[_PUB_M_BYTE];
	mqsc_pubmap(digest_ck, pk, signature);

	unsigned char correct[_PUB_M_BYTE];
	unsigned char digest_salt[_HASH_LEN + _SALT_BYTE];
	memcpy(digest_salt, digest, _HASH_LEN);
	memcpy(digest_salt + _HASH_LEN, signature + _PUB_N_BYTE, _SALT_BYTE);
	hash_msg(correct, _PUB_M_BYTE, digest_salt, _HASH_LEN + _SALT_BYTE);

	unsigned char cc = 0;
	for (unsigned i = 0; i < _PUB_M_BYTE; i++) {
		cc |= (digest_ck[i] ^ correct[i]);
	}

	return (0 == cc) ? 0 : -1;
}