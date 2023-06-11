#include <stdint.h>
#include <stdio.h>
#include "../params.h"
#include "../randombytes.h"
#include "../poly.h"
#include "cpucycles.h"

#define NTESTS 10000

static void poly_naivemul(poly *c, const poly *a, const poly *b) {
	unsigned int i, j;
	int32_t r[2 * N] = { 0 };

	for (i = 0; i < N; i++)
		for (j = 0; j < N; j++)
			r[i + j] = (r[i + j] + (int64_t)a->coeffs[i] * b->coeffs[j]) % Q;

	for (i = N; i < 2 * N - 1; i++) {
		r[i - N] = (r[i - N] + r[i]) % Q;
		r[i - N + 1] = (r[i - N + 1] + r[i]) % Q;
	}
	r[N - 1] = (r[N - 1] + r[2 * N - 1]) % Q;
	r[0] = (r[0] + r[2 * N - 1]) % Q;
	r[1] = (r[1] + r[2 * N - 1]) % Q;


	for (i = 0; i < N; i++)
		c->coeffs[i] = r[i];
}

void test_orimul()
{
	uint8_t seed[SEEDBYTES];
	uint16_t nonce = 0;
	poly a, acpy, b, bcpy, c, d;
	uint64_t cycles1, cycles2, cycles_orimul = 0, cycles_school = 0, cycles_toom = 0;;
	poly_uniform(&a, seed, nonce++);
	poly_uniform(&b, seed, nonce++);

	int i;

	for (i = 0; i < NTESTS; ++i) {
		poly_uniform(&a, seed, nonce++);
		poly_uniform(&b, seed, nonce++);

		cycles1 = cpucycles();
		poly_naivemul(&c, &a, &b);
		cycles2 = cpucycles();
		cycles_orimul = cycles_orimul + (cycles2 - cycles1);
	}
	printf(" ORIMUL in ....................................... %10lld ", cycles_orimul / NTESTS);
	printf("\n");

	for (i = 0; i < NTESTS; ++i) {
		poly_uniform(&a, seed, nonce++);
		poly_uniform(&b, seed, nonce++);

		cycles1 = cpucycles();
		poly_mul_schoolbook(&a, &b, &c);
		cycles2 = cpucycles();
		cycles_school = cycles_school + (cycles2 - cycles1);
	}
	printf(" SCHOOLBOOK runs in ....................................... %10lld ", cycles_school / NTESTS);
	printf("\n");

	for (i = 0; i < NTESTS; ++i) {
		poly_uniform(&a, seed, nonce++);
		poly_uniform(&b, seed, nonce++);

		cycles1 = cpucycles();
		poly_mul(&a, &b, &c);
		cycles2 = cpucycles();
		cycles_toom = cycles_toom + (cycles2 - cycles1);
	}
	printf("  TOOMCOOK runs in ....................................... %10lld ", cycles_toom / NTESTS);
	printf("\n");
}


void test_toomcook()
{
	unsigned int i, j;
	uint8_t seed[SEEDBYTES];
	uint16_t nonce = 0;
	poly a, acpy, b, bcpy, c, d;

	poly_uniform(&a, seed, nonce++);
	poly_uniform(&b, seed, nonce++);

	printf("Degree: %d \n", N);

	poly_mul_schoolbook(&a, &b, &d);

	for (i = 0; i < N; i++)
	{
		acpy.coeffs[i] = a.coeffs[i];
		bcpy.coeffs[i] = b.coeffs[i];
	}

	for (i = 0; i < N; i++)
	{
		acpy.coeffs[i] = to_mont(acpy.coeffs[i]);
		bcpy.coeffs[i] = to_mont(bcpy.coeffs[i]);
	}

	poly_mul(&c, &acpy, &bcpy);

	for (i = 0; i < N; i++)
	{
		c.coeffs[i] = from_mont(c.coeffs[i]);
		acpy.coeffs[i] = from_mont(acpy.coeffs[i]);
		bcpy.coeffs[i] = from_mont(bcpy.coeffs[i]);
	}
	int ret0, ret1;
	int32_t tmp = 5829270;
	uint32_t t0, t1;
	t0 = to_mont(tmp);
	t1 = from_mont(t0);

	for (i = 0; i < N; i++)
	{
		if (a.coeffs[i] != acpy.coeffs[i])
		{
			printf("error in index %d, %d, %d\n", i, a.coeffs[i], acpy.coeffs[i]);
		}

		if (b.coeffs[i] != bcpy.coeffs[i])
		{
			printf("error in index %d, %d, %d\n", i, b.coeffs[i], bcpy.coeffs[i]);
		}
	}
	ret0 = memcmp(a.coeffs, acpy.coeffs, sizeof(int32_t)*N);
	ret1 = memcmp(b.coeffs, bcpy.coeffs, sizeof(int32_t)*N);



	for (j = 0; j < N; ++j) {
		if ((d.coeffs[j] - c.coeffs[j]) % Q)
			fprintf(stderr, "ERROR in multiplication: d[%d] = %d != %d\n",
				j, d.coeffs[j], c.coeffs[j]);
	}
	printf("CLEAR\n");
}

int main(void) {
	unsigned int i, j;
	uint8_t seed[SEEDBYTES];
	uint16_t nonce = 0;
	poly a, b, c, d;

	randombytes(seed, sizeof(seed));
	test_toomcook();

	printf("  ........TESTING SPEED....... ");
	printf("\n");
	test_orimul();

	return 0;
}
