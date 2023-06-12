/*
 * Wrapper for implementing the NIST API for the PQC standardization
 * process.
 */

#include <stddef.h>
#include <string.h>
#include <stdio.h>

#include "api.h"
#include "inner.h"
#include "precomp.h"

/*
 * If stack usage is an issue, define TEMPALLOC to static in order to
 * allocate temporaries in the data section instead of the stack. This
 * would make the crypto_sign_keypair(), crypto_sign(), and
 * crypto_sign_open() functions not reentrant and not thread-safe, so
 * this should be done only for testing purposes.
 */
#define TEMPALLOC

void randombytes_init(unsigned char *entropy_input,
					  unsigned char *personalization_string,
					  int security_strength);
int randombytes(unsigned char *x, unsigned long long xlen);

// ============ Copied from main.c (kj) ====
// print functions for debugging

#define PRINT_START_END(str, x)                                                  \
	do                                                                           \
	{                                                                            \
		printf("* %s: ", str);                                                   \
		for (int i = 0; i < PRINT_SIZE; ++i)                                     \
			printf("%f%+fj  ", (x).coeffs[i].v, (x).coeffs[i + SOLMAE_D / 2].v); \
		printf("... ");                                                          \
		for (int i = SOLMAE_D / 2 - PRINT_SIZE; i < SOLMAE_D / 2; ++i)           \
			printf("%f%+fj  ", (x).coeffs[i].v, (x).coeffs[i + SOLMAE_D / 2].v); \
		printf("\n");                                                            \
	} while (0)
#if 0
static void print_sk(secret_key *sk)
{
	int PRINT_SIZE = 8; // SOLMAE_D;

	PRINT_START_END("b10", sk->b10);
	PRINT_START_END("b11", sk->b11);
	PRINT_START_END("b20", sk->b20);
	PRINT_START_END("b21", sk->b21);
	PRINT_START_END("b21", sk->b21);
	PRINT_START_END("GSO_b10", sk->GSO_b10);
	PRINT_START_END("GSO_b11", sk->GSO_b11);
	PRINT_START_END("GSO_b20", sk->GSO_b20);
	PRINT_START_END("GSO_b21", sk->GSO_b21);
	PRINT_START_END("sigma1", sk->sigma1);
	PRINT_START_END("sigma2", sk->sigma2);
	PRINT_START_END("beta10", sk->beta10);
	PRINT_START_END("beta11", sk->beta11);
	PRINT_START_END("beta20", sk->beta20);
	PRINT_START_END("beta21", sk->beta21);
}
#endif
// ==========

int crypto_sign_keypair(unsigned char *pk, unsigned char *sk)
{
	// ======= in FALCON ==========
	//	TEMPALLOC union {
	//		uint8_t b[FALCON_KEYGEN_TEMP_9];
	//		uint64_t dummy_u64;
	//		fpr dummy_fpr;
	//	} tmp;
	//	TEMPALLOC int8_t f[512], g[512], F[512];
	//	TEMPALLOC uint16_t h[512];
	//	TEMPALLOC unsigned char seed[48];
	//	TEMPALLOC inner_shake256_context rng;
	//	size_t u, v;
	// ==================================

	TEMPALLOC public_key pkstruct;
	TEMPALLOC secret_key skstruct;

//	printf("Hello world, signature is my Solmae- %u\n", SOLMAE_D);

	// Confirmed by kj
	// printf("Fixed Public Key before loading PK = \n");

	// for (int i = 0; i < SOLMAE_D; ++i)
	//	 printf("%f ", precomp_h[i]);

	// compute sk, pk from keygen.c
	keygen_full(&skstruct, &pkstruct);

	// printf("After Precomp. SK (print_sk) =\n ");
	// print_sk(&skstruct);
	// printf("\n");

	// add header byte
	pk[0] = 0x00 + 9;
	sk[0] = 0x50 + 9;

	memcpy(pk + 1, pkstruct.h.coeffs, sizeof(fpr) * SOLMAE_D);

	// concatenate f, g, F and G into 1-dimensional array
	memcpy(sk + 1, skstruct.b10.coeffs, sizeof(fpr) * SOLMAE_D);
	memcpy(sk + 1 + sizeof(fpr) * SOLMAE_D, skstruct.b11.coeffs, sizeof(fpr) * SOLMAE_D);
	memcpy(sk + 1 + 2 * sizeof(fpr) * SOLMAE_D, skstruct.b20.coeffs, sizeof(fpr) * SOLMAE_D);
	memcpy(sk + 1 + 3 * sizeof(fpr) * SOLMAE_D, skstruct.b21.coeffs, sizeof(fpr) * SOLMAE_D);

	return 0;
}

int crypto_sign(unsigned char *sm, unsigned long long *smlen,
				const unsigned char *m, unsigned long long mlen,
				const unsigned char *sk)
{
	TEMPALLOC signature s;
	TEMPALLOC secret_key skstruct;
	// TEMPALLOC unsigned char nonce[NONCELEN];

	// check if the header of sk is valid
	if (sk[0] != 0x50 + 9)
	{
		return -1;
	}
	// convert 1-dimensional secret key into struct secret_key
	// Beginning of load_B() and four FFT()
	memcpy(skstruct.b10.coeffs, sk + 1, sizeof(fpr) * SOLMAE_D);
	memcpy(skstruct.b11.coeffs, sk + 1 + sizeof(fpr) * SOLMAE_D, sizeof(fpr) * SOLMAE_D);
	memcpy(skstruct.b20.coeffs, sk + 1 + 2 * sizeof(fpr) * SOLMAE_D, sizeof(fpr) * SOLMAE_D);
	memcpy(skstruct.b21.coeffs, sk + 1 + 3 * sizeof(fpr) * SOLMAE_D, sizeof(fpr) * SOLMAE_D);
	// End of load_B() and four FFT()

	precomp_GSO(&skstruct);
	precomp_sigma(&skstruct);
	precomp_beta_hat(&skstruct);

	// sign m by skstruct, yield to signature s
	// randombytes(nonce, sizeof nonce);  //commented out -> Works good!!
	sign(m, &skstruct, &s);

	/*
	 * Encode the signature and bundle it with the message. Format is:
	 *   signature length not included - the length is fixed to 2 * 8 * SOLMAE_D
	 *   nonce                k / 8 (40) bytes
	 *   message              mlen bytes
	 *   signature header     1 byte
	 *   signature            s1 + s2 bytes
	 */
	memcpy(sm, s.r, SOLMAE_K / 8);
	memmove(sm + SOLMAE_K / 8, m, mlen);
	sm[SOLMAE_K / 8 + mlen] = 0x20 + 9;
	memcpy(sm + SOLMAE_K / 8 + mlen + 1, s.s1.coeffs, sizeof(fpr) * SOLMAE_D);
	memcpy(sm + SOLMAE_K / 8 + mlen + 1 + sizeof(fpr) * SOLMAE_D, s.s2.coeffs, sizeof(fpr) * SOLMAE_D);
	*smlen = SOLMAE_K / 8 + mlen + 1 + 2 * sizeof(fpr) * SOLMAE_D;

	return 0;
}

int crypto_sign_open(unsigned char *m, unsigned long long *mlen,
					 const unsigned char *sm, unsigned long long smlen,
					 const unsigned char *pk)
{
	TEMPALLOC public_key pkstruct;
	TEMPALLOC signature s;

	// check if the header of pk is valid
	if (pk[0] != 0x00 + 9)
	{
		return -1;
	}
	// check length of signature
	if (smlen < 1 + SOLMAE_K / 8)
	{
		return -1;
	}
	// copy pk, signature, r into pkstruct and s
	size_t msg_len = smlen - 1 - 2 * sizeof(fpr) * SOLMAE_D - SOLMAE_K / 8;
	memcpy(pkstruct.h.coeffs, pk + 1, sizeof(fpr) * SOLMAE_D);
	memcpy(s.r, sm, SOLMAE_K / 8);
	memcpy(s.s1.coeffs, sm + SOLMAE_K / 8 + msg_len + 1, sizeof(fpr) * SOLMAE_D);
	memcpy(s.s2.coeffs, sm + SOLMAE_K / 8 + msg_len + 1 + sizeof(fpr) * SOLMAE_D, sizeof(fpr) * SOLMAE_D);

	// If valid, return 0. Otherwise, return -1.
	if (!verify(m, &pkstruct, &s))
		return -1;
	// copy original message into m
	memcpy(m, sm + SOLMAE_K / 8, msg_len);
	*mlen = msg_len;
	return 0;
}
