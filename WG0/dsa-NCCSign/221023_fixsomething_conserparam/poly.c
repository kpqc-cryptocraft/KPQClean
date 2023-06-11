#include <stdio.h>

#include <stdint.h>
#include "params.h"
#include "poly.h"
#include "reduce.h"
#include "rounding.h"
#include "symmetric.h"
#include <string.h>
#include <time.h>
#include "packing.h"
#ifdef DBENCH
#include "test/cpucycles.h"
extern const uint64_t timing_overhead;
extern uint64_t *tred, *tadd, *tmul, *tround, *tsample, *tpack;
#define DBENCH_START() uint64_t time = cpucycles()
#define DBENCH_STOP(t) t += cpucycles() - time - timing_overhead
#else
#define DBENCH_START()
#define DBENCH_STOP(t)
#endif

#define N_RES (NR << 1)
#define N_SB (NR >> 2)
#define N_SB_RES (2*N_SB-1)
#define NN (NR >>2)

#if NIMS_MODE == 2
#define inv2   4851564
#define inv3   3234376
#define inv4   2425782
#define inv9   6837889
#define inv8   1212891
#define inv15  14470308

#define inv24  404297
#define inv18  12058590
#define inv60  3617577

#define mont2  2126965
#define mont4  4253930
#define mont8  8507860
#define mont64 16225007
#define mont45 4658485
#define mont30 14625184
#define Q64    74213989760346427
#define hQ     74213989743067136
#define QINV   2471409677

#elif NIMS_MODE == 3
#define inv2   1571764
#define inv3   6816423
#define inv4   785882
#define inv9   2272141
#define inv8   392941
#define inv15  8285581

#define inv24  11668141
#define inv18  9788941
#define inv60  15050701

#define mont2  6287056
#define mont4  12574112
#define mont8  7842483
#define mont64 10822641
#define mont45 3012832
#define mont30 7777135
#define Q64    74327591645352077
#define hQ     74327591628046336
#define QINV   772263355

#elif NIMS_MODE == 5
#define inv2   3843196
#define inv3   8324605
#define inv4   1921598
#define inv9   14299817
#define inv8   960799
#define inv15  1664921

#define inv24  11845215
#define inv18  15793620
#define inv60  4738086

#define mont2  15372784
#define mont4  13458145
#define mont8  9628867
#define mont64 7881244
#define mont45 139180
#define mont30 5855261
#define Q64    74248916434405631
#define hQ     74248916417118208
#define QINV   3050883329
#endif


void poly_tomont(poly *a_mont, poly *a) {
	unsigned int i;

	for (i = 0; i < N; ++i)
		a_mont->coeffs[i] = to_mont(a->coeffs[i]);

}

void poly_frommont(poly *a, poly *a_mont) {
	unsigned int i;
	for (i = 0; i < N; ++i)
		a->coeffs[i] = from_mont(a_mont->coeffs[i]);
}

void poly_abs(poly *aout, poly *a) {
	unsigned int i;
	for (i = 0; i < N; ++i)
	{
		if (a->coeffs[i] < 0)
		{
			aout->coeffs[i] = a->coeffs[i] + Q;
		}
		else
		{
			aout->coeffs[i] = a->coeffs[i];
		}
	}
}

void poly_mont_tbl(poly *aout, poly *a)
{
	int i;
	for (i = 0; i < N; i++)
	{
		aout->coeffs[i] = 0;
		if (a->coeffs[i] == 1)
			aout->coeffs[i] = 83081;
		else if (a->coeffs[i] == -1)
			aout->coeffs[i] = 8256500;
	}
}

int poly_check(poly *a, poly *b) {
	unsigned int i;
	for (i = 0; i < N; ++i)
	{
		if (a->coeffs[i] != b->coeffs[i])
		{
			printf("a->coeffs[%d]: %d", i, a->coeffs[i]);
			printf("b->coeffs[%d]: %d", i, b->coeffs[i]);
			return -1;
		}
	}
	return 0;
}

void karatsuba_simple(const uint32_t* a_1, const uint32_t* b_1, uint32_t* result_final) {
	uint32_t d01[NN / 2 - 1];
	uint32_t d0123[NN / 2 - 1];
	uint32_t d23[NN / 2 - 1];
	uint32_t result_d01[NN - 1];
	uint32_t t0;
	uint32_t i, j;

	memset(result_d01, 0, (NN - 1) * sizeof(uint32_t));
	memset(d01, 0, (NN / 2 - 1) * sizeof(uint32_t));
	memset(d0123, 0, (NN / 2 - 1) * sizeof(uint32_t));
	memset(d23, 0, (NN / 2 - 1) * sizeof(uint32_t));
	memset(result_final, 0, (2 * NN - 1) * sizeof(uint32_t));

	uint32_t acc1, acc2, acc3, acc4, acc5, acc6, acc7, acc8, acc9, acc10;
	uint64_t imn = 0;
	__uint128_t t, b, mt, ttmp;
	int64_t t1, t2;

	uint64_t mask_coef = 0x00000000FFFFFFFF;

	for (i = 0; i < NN / 4; i++) {
		acc1 = a_1[i];
		acc2 = a_1[i + NN / 4];
		acc3 = a_1[i + 2 * NN / 4];
		acc4 = a_1[i + 3 * NN / 4];
		for (j = 0; j < NN / 4; j++) {

			acc5 = b_1[j];
			acc6 = b_1[j + NN / 4];

			t = (__uint128_t)acc1*(__uint128_t)acc5;
			ttmp = (t & 0xffffffff) << 64;
			t = t << 64;
			imn = (uint64_t)acc2*(uint64_t)acc6;
			t = t ^ imn;
			ttmp = ttmp ^ (imn & 0xffffffff);

			mt = ttmp * QINV;
			mt = mt & ((__uint128_t)(mask_coef) << 64 | (__uint128_t)(mask_coef));

			mt *= Q;
			mt = t + mt;
			b = mt >> 32;

			t1 = (uint64_t)(b & 0xFFFFFFFF);
			t1 -= ((Q - t1) >> 63) & Q;

			t2 = (b) >> 64;
			t2 -= ((Q - t2) >> 63) & Q;

			result_final[i + j + 0 * NN / 4] = mod_add(result_final[i + j + 0 * NN / 4], t2);
			result_final[i + j + 2 * NN / 4] = mod_add(result_final[i + j + 2 * NN / 4], t1);

			acc7 = acc5 + acc6;
			acc8 = acc1 + acc2;

			imn = (uint64_t)acc7*(uint64_t)acc8;
			t0 = montgomery_reduce(imn);
			d01[i + j] = mod_add(d01[i + j], t0);

			acc7 = b_1[j + 2 * NN / 4];
			acc8 = b_1[j + 3 * NN / 4];

			t = (__uint128_t)acc7*(__uint128_t)acc3;
			ttmp = (t & 0xffffffff) << 64;
			t = t << 64;
			imn = (uint64_t)acc8*(uint64_t)acc4;
			t = t ^ imn;
			ttmp = ttmp ^ (imn & 0xffffffff);

			mt = ttmp * QINV;
			mt = mt & ((__uint128_t)(mask_coef) << 64 | (__uint128_t)(mask_coef));

			mt *= Q;
			mt = t + mt;
			b = mt >> 32;

			t1 = (uint64_t)(b & 0xFFFFFFFF);
			t1 -= ((Q - t1) >> 63) & Q;

			t2 = (b) >> 64;
			t2 -= ((Q - t2) >> 63) & Q;

			result_final[i + j + 4 * NN / 4] = mod_add(result_final[i + j + 4 * NN / 4], t2);
			result_final[i + j + 6 * NN / 4] = mod_add(result_final[i + j + 6 * NN / 4], t1);

			acc9 = acc3 + acc4;
			acc10 = acc7 + acc8;

			acc5 = acc5 + acc7;
			acc7 = acc1 + acc3;

			t = (__uint128_t)acc9*(__uint128_t)acc10;
			ttmp = (t & 0xffffffff) << 64;
			t = t << 64;
			imn = (uint64_t)acc5*(uint64_t)acc7;
			t = t ^ imn;
			ttmp = ttmp ^ (imn & 0xffffffff);

			mt = ttmp * QINV;
			mt = mt & ((__uint128_t)(mask_coef) << 64 | (__uint128_t)(mask_coef));

			mt *= Q;
			mt = t + mt;
			b = mt >> 32;

			t1 = (uint64_t)(b & 0xFFFFFFFF);
			t1 -= ((Q - t1) >> 63) & Q;

			t2 = (b) >> 64;
			t2 -= ((Q - t2) >> 63) & Q;

			d23[i + j] = mod_add(d23[i + j], t2);
			result_d01[i + j + 0 * NN / 4] = mod_add(result_d01[i + j + 0 * NN / 4], t1);

			acc6 = acc6 + acc8;
			acc8 = acc2 + acc4;

			acc5 = acc5 + acc6;
			acc7 = acc7 + acc8;

			t = (__uint128_t)acc6*(__uint128_t)acc8;
			ttmp = (t & 0xffffffff) << 64;
			t = t << 64;
			imn = (uint64_t)acc5*(uint64_t)acc7;
			t = t ^ imn;
			ttmp = ttmp ^ (imn & 0xffffffff);

			mt = ttmp * QINV;
			mt = mt & ((__uint128_t)(mask_coef) << 64 | (__uint128_t)(mask_coef));

			mt *= Q;
			mt = t + mt;
			b = mt >> 32;

			t1 = (uint64_t)(b & 0xFFFFFFFF);
			t1 -= ((Q - t1) >> 63) & Q;

			t2 = (b) >> 64;
			t2 -= ((Q - t2) >> 63) & Q;

			result_d01[i + j + 2 * NN / 4] = mod_add(result_d01[i + j + 2 * NN / 4], t2);
			d0123[i + j] = mod_add(d0123[i + j], t1);
		}
	}

	//------------------2nd last stage-------------------------
	for (i = 0; i < NN / 2 - 1; i++) {
		d0123[i] = mod_sub(d0123[i], result_d01[i + 0 * NN / 4]);
		d0123[i] = mod_sub(d0123[i], result_d01[i + 2 * NN / 4]);

		d01[i] = mod_sub(d01[i], result_final[i + 0 * NN / 4]);
		d01[i] = mod_sub(d01[i], result_final[i + 2 * NN / 4]);

		d23[i] = mod_sub(d23[i], result_final[i + 4 * NN / 4]);
		d23[i] = mod_sub(d23[i], result_final[i + 6 * NN / 4]);
	}

	for (i = 0; i < NN / 2 - 1; i++) {
		result_d01[i + 1 * NN / 4] = mod_add(result_d01[i + 1 * NN / 4], d0123[i]);
		result_final[i + 1 * NN / 4] = mod_add(result_final[i + 1 * NN / 4], d01[i]);
		result_final[i + 5 * NN / 4] = mod_add(result_final[i + 5 * NN / 4], d23[i]);
	}

	//------------Last stage---------------------------
	for (i = 0; i < NN - 1; i++) {
		result_d01[i] = mod_sub(result_d01[i], result_final[i]);
		result_d01[i] = mod_sub(result_d01[i], result_final[i + NN]);
	}

	for (i = 0; i < NN - 1; i++) {
		result_final[i + 1 * NN / 2] = mod_add(result_final[i + 1 * NN / 2], result_d01[i]);
	}

}

void toom_cook_4way(const uint32_t* a1, const uint32_t* b1, uint32_t* result)
{
	uint32_t aw1[N_SB], aw2[N_SB], aw3[N_SB], aw4[N_SB], aw5[N_SB], aw6[N_SB], aw7[N_SB];
	uint32_t bw1[N_SB], bw2[N_SB], bw3[N_SB], bw4[N_SB], bw5[N_SB], bw6[N_SB], bw7[N_SB];
	uint32_t w1[N_SB_RES] = { 0 }, w2[N_SB_RES] = { 0 }, w3[N_SB_RES] = { 0 }, w4[N_SB_RES] = { 0 },
		w5[N_SB_RES] = { 0 }, w6[N_SB_RES] = { 0 }, w7[N_SB_RES] = { 0 };

	uint32_t r0, r1, r2, r3, r4, r5, r6, r7;
	uint32_t *A0, *A1, *A2, *A3, *B0, *B1, *B2, *B3;
	A0 = (uint32_t*)a1;
	A1 = (uint32_t*)&a1[N_SB];
	A2 = (uint32_t*)&a1[2 * N_SB];
	A3 = (uint32_t*)&a1[3 * N_SB];
	B0 = (uint32_t*)b1;
	B1 = (uint32_t*)&b1[N_SB];
	B2 = (uint32_t*)&b1[2 * N_SB];
	B3 = (uint32_t*)&b1[3 * N_SB];

	uint32_t * C;
	C = result;
	uint32_t Ctmp[N_RES] = { 0, };

	int i, j;
	uint32_t t0, t1, t2;
	uint64_t inm = 0;
	uint64_t tt0, tt1, tt2, tt3, tt4, tt5, tt6, tt7, ss0, ss1, ss2;

	int64_t k, chkkay;

	for (j = 0; j < (N_SB) >> 1; ++j) {
		tt0 = ((uint64_t)A0[(j) << 1] << 32) ^ A0[((j) << 1) + 1];
		tt1 = ((uint64_t)A1[(j) << 1] << 32) ^ A1[((j) << 1) + 1];
		tt2 = ((uint64_t)A2[(j) << 1] << 32) ^ A2[((j) << 1) + 1];
		tt3 = ((uint64_t)A3[(j) << 1] << 32) ^ A3[((j) << 1) + 1];

		k = tt0 + tt2;
		k = k - Q64;
		k += (int32_t)(-((k >> 31) & 1))&Q;
		k += (-((k >> 63) & 1))&hQ;
		tt4 = k;

		k = tt1 + tt3;
		k = k - Q64;
		k += (int32_t)(-((k >> 31) & 1))&Q;
		k += (-((k >> 63) & 1))&hQ;
		tt5 = k;

		k = tt4 + tt5;
		k = k - Q64;
		k += (int32_t)(-((k >> 31) & 1))&Q;
		k += (-((k >> 63) & 1))&hQ;
		tt6 = k;

		k = tt4 - tt5;
		k += (int32_t)(-((k >> 31) & 1))&Q;
		k += (-((k >> 63) & 1))&hQ;
		tt7 = k;

		aw3[(j) << 1] = tt6 >> 32;
		aw3[((j) << 1) + 1] = tt6 & 0xffffffff;
		aw4[(j) << 1] = tt7 >> 32;
		aw4[((j) << 1) + 1] = tt7 & 0xffffffff;

		k = tt0 + tt0;
		k = k - Q64;
		k += (int32_t)(-((k >> 31) & 1))&Q;
		k += (-((k >> 63) & 1))&hQ;
		ss0 = k;

		k = ss0 + ss0;
		k = k - Q64;
		k += (int32_t)(-((k >> 31) & 1))&Q;
		k += (-((k >> 63) & 1))&hQ;
		ss0 = k;

		k = ss0 + tt2;
		k = k - Q64;
		k += (int32_t)(-((k >> 31) & 1))&Q;
		k += (-((k >> 63) & 1))&hQ;
		ss0 = k;

		k = ss0 + ss0;
		k = k - Q64;
		k += (int32_t)(-((k >> 31) & 1))&Q;
		k += (-((k >> 63) & 1))&hQ;
		tt4 = k;

		k = tt1 + tt1;
		k = k - Q64;
		k += (int32_t)(-((k >> 31) & 1))&Q;
		k += (-((k >> 63) & 1))&hQ;
		ss0 = k;

		k = ss0 + ss0;
		k = k - Q64;
		k += (int32_t)(-((k >> 31) & 1))&Q;
		k += (-((k >> 63) & 1))&hQ;
		ss0 = k;

		k = ss0 + tt3;
		k = k - Q64;
		k += (int32_t)(-((k >> 31) & 1))&Q;
		k += (-((k >> 63) & 1))&hQ;
		tt5 = k;

		k = tt4 + tt5;
		k = k - Q64;
		k += (int32_t)(-((k >> 31) & 1))&Q;
		k += (-((k >> 63) & 1))&hQ;
		tt6 = k;

		k = tt4 - tt5;
		k += (int32_t)(-((k >> 31) & 1))&Q;
		k += (-((k >> 63) & 1))&hQ;
		tt7 = k;

		aw5[(j) << 1] = tt6 >> 32;
		aw5[((j) << 1) + 1] = tt6 & 0xffffffff;
		aw6[(j) << 1] = tt7 >> 32;
		aw6[((j) << 1) + 1] = tt7 & 0xffffffff;

		k = tt3 + tt3;
		k = k - Q64;
		k += (int32_t)(-((k >> 31) & 1))&Q;
		k += (-((k >> 63) & 1))&hQ;
		ss0 = k;

		k = ss0 + ss0;
		k = k - Q64;
		k += (int32_t)(-((k >> 31) & 1))&Q;
		k += (-((k >> 63) & 1))&hQ;
		ss0 = k;

		k = ss0 + ss0;
		k = k - Q64;
		k += (int32_t)(-((k >> 31) & 1))&Q;
		k += (-((k >> 63) & 1))&hQ;
		ss0 = k;

		k = tt2 + tt2;
		k = k - Q64;
		k += (int32_t)(-((k >> 31) & 1))&Q;
		k += (-((k >> 63) & 1))&hQ;
		ss1 = k;

		k = ss1 + ss1;
		k = k - Q64;
		k += (int32_t)(-((k >> 31) & 1))&Q;
		k += (-((k >> 63) & 1))&hQ;
		ss1 = k;

		k = tt1 + tt1;
		k = k - Q64;
		k += (int32_t)(-((k >> 31) & 1))&Q;
		k += (-((k >> 63) & 1))&hQ;
		ss2 = k;

		k = ss0 + ss1;
		k = k - Q64;
		k += (int32_t)(-((k >> 31) & 1))&Q;
		k += (-((k >> 63) & 1))&hQ;
		ss0 = k;

		k = ss0 + ss2;
		k = k - Q64;
		k += (int32_t)(-((k >> 31) & 1))&Q;
		k += (-((k >> 63) & 1))&hQ;
		ss0 = k;

		k = ss0 + tt0;
		k = k - Q64;
		k += (int32_t)(-((k >> 31) & 1))&Q;
		k += (-((k >> 63) & 1))&hQ;
		tt4 = k;

		aw2[(j) << 1] = tt4 >> 32;
		aw2[((j) << 1) + 1] = tt4 & 0xffffffff;
		aw7[(j) << 1] = tt0 >> 32;
		aw7[((j) << 1) + 1] = tt0 & 0xffffffff;
		aw1[(j) << 1] = tt3 >> 32;
		aw1[((j) << 1) + 1] = tt3 & 0xffffffff;
	}

	for (j = 0; j < (N_SB) >> 1; ++j) {
		tt0 = ((uint64_t)B0[(j) << 1] << 32) ^ B0[((j) << 1) + 1];
		tt1 = ((uint64_t)B1[(j) << 1] << 32) ^ B1[((j) << 1) + 1];
		tt2 = ((uint64_t)B2[(j) << 1] << 32) ^ B2[((j) << 1) + 1];
		tt3 = ((uint64_t)B3[(j) << 1] << 32) ^ B3[((j) << 1) + 1];

		k = tt0 + tt2;
		k = k - Q64;
		k += (int32_t)(-((k >> 31) & 1))&Q;
		k += (-((k >> 63) & 1))&hQ;
		tt4 = k;

		k = tt1 + tt3;
		k = k - Q64;
		k += (int32_t)(-((k >> 31) & 1))&Q;
		k += (-((k >> 63) & 1))&hQ;
		tt5 = k;

		k = tt4 + tt5;
		k = k - Q64;
		k += (int32_t)(-((k >> 31) & 1))&Q;
		k += (-((k >> 63) & 1))&hQ;
		tt6 = k;

		k = tt4 - tt5;
		k += (int32_t)(-((k >> 31) & 1))&Q;
		k += (-((k >> 63) & 1))&hQ;
		tt7 = k;

		bw3[(j) << 1] = tt6 >> 32;
		bw3[((j) << 1) + 1] = tt6 & 0xffffffff;
		bw4[(j) << 1] = tt7 >> 32;
		bw4[((j) << 1) + 1] = tt7 & 0xffffffff;

		k = tt0 + tt0;
		k = k - Q64;
		k += (int32_t)(-((k >> 31) & 1))&Q;
		k += (-((k >> 63) & 1))&hQ;
		ss0 = k;

		k = ss0 + ss0;
		k = k - Q64;
		k += (int32_t)(-((k >> 31) & 1))&Q;
		k += (-((k >> 63) & 1))&hQ;
		ss0 = k;

		k = ss0 + tt2;
		k = k - Q64;
		k += (int32_t)(-((k >> 31) & 1))&Q;
		k += (-((k >> 63) & 1))&hQ;
		ss0 = k;

		k = ss0 + ss0;
		k = k - Q64;
		k += (int32_t)(-((k >> 31) & 1))&Q;
		k += (-((k >> 63) & 1))&hQ;
		tt4 = k;

		k = tt1 + tt1;
		k = k - Q64;
		k += (int32_t)(-((k >> 31) & 1))&Q;
		k += (-((k >> 63) & 1))&hQ;
		ss0 = k;

		k = ss0 + ss0;
		k = k - Q64;
		k += (int32_t)(-((k >> 31) & 1))&Q;
		k += (-((k >> 63) & 1))&hQ;
		ss0 = k;

		k = ss0 + tt3;
		k = k - Q64;
		k += (int32_t)(-((k >> 31) & 1))&Q;
		k += (-((k >> 63) & 1))&hQ;
		tt5 = k;

		k = tt4 + tt5;
		k = k - Q64;
		k += (int32_t)(-((k >> 31) & 1))&Q;
		k += (-((k >> 63) & 1))&hQ;
		tt6 = k;

		k = tt4 - tt5;
		k += (int32_t)(-((k >> 31) & 1))&Q;
		k += (-((k >> 63) & 1))&hQ;
		tt7 = k;

		bw5[(j) << 1] = tt6 >> 32;
		bw5[((j) << 1) + 1] = tt6 & 0xffffffff;
		bw6[(j) << 1] = tt7 >> 32;
		bw6[((j) << 1) + 1] = tt7 & 0xffffffff;

		k = tt3 + tt3;
		k = k - Q64;
		k += (int32_t)(-((k >> 31) & 1))&Q;
		k += (-((k >> 63) & 1))&hQ;
		ss0 = k;

		k = ss0 + ss0;
		k = k - Q64;
		k += (int32_t)(-((k >> 31) & 1))&Q;
		k += (-((k >> 63) & 1))&hQ;
		ss0 = k;

		k = ss0 + ss0;
		k = k - Q64;
		k += (int32_t)(-((k >> 31) & 1))&Q;
		k += (-((k >> 63) & 1))&hQ;
		ss0 = k;

		k = tt2 + tt2;
		k = k - Q64;
		k += (int32_t)(-((k >> 31) & 1))&Q;
		k += (-((k >> 63) & 1))&hQ;
		ss1 = k;

		k = ss1 + ss1;
		k = k - Q64;
		k += (int32_t)(-((k >> 31) & 1))&Q;
		k += (-((k >> 63) & 1))&hQ;
		ss1 = k;

		k = tt1 + tt1;
		k = k - Q64;
		k += (int32_t)(-((k >> 31) & 1))&Q;
		k += (-((k >> 63) & 1))&hQ;
		ss2 = k;

		k = ss0 + ss1;
		k = k - Q64;
		k += (int32_t)(-((k >> 31) & 1))&Q;
		k += (-((k >> 63) & 1))&hQ;
		ss0 = k;

		k = ss0 + ss2;
		k = k - Q64;
		k += (int32_t)(-((k >> 31) & 1))&Q;
		k += (-((k >> 63) & 1))&hQ;
		ss0 = k;

		k = ss0 + tt0;
		k = k - Q64;
		k += (int32_t)(-((k >> 31) & 1))&Q;
		k += (-((k >> 63) & 1))&hQ;
		tt4 = k;

		bw2[(j) << 1] = tt4 >> 32;
		bw2[((j) << 1) + 1] = tt4 & 0xffffffff;
		bw7[(j) << 1] = tt0 >> 32;
		bw7[((j) << 1) + 1] = tt0 & 0xffffffff;
		bw1[(j) << 1] = tt3 >> 32;
		bw1[((j) << 1) + 1] = tt3 & 0xffffffff;
	}

	karatsuba_simple(aw1, bw1, w1);
	karatsuba_simple(aw2, bw2, w2);
	karatsuba_simple(aw3, bw3, w3);
	karatsuba_simple(aw4, bw4, w4);
	karatsuba_simple(aw5, bw5, w5);
	karatsuba_simple(aw6, bw6, w6);
	karatsuba_simple(aw7, bw7, w7);

	uint64_t mask_coef = 0x00000000FFFFFFFF, si2 = 0;
	__uint128_t mt, t, b;


	__uint128_t Q128 = ((__uint128_t)Q << 64) ^ Q, ttmp;

	for (i = 0; i < N_SB_RES; ++i) {
		r0 = w1[i];
		r1 = w2[i];
		r2 = w3[i];
		r3 = w4[i];
		r4 = w5[i];
		r5 = w6[i];
		r6 = w7[i];

		r1 = mod_add(r1, r4);
		r5 = mod_sub(r5, r4);
		t0 = mod_sub(r3, r2);
		r4 = mod_sub(r4, r0);

		t = (uint64_t)t0*(uint64_t)inv2;
		t = t << 64;
		si2 = (uint64_t)r6*(uint64_t)mont64;
		t = t ^ si2;
		ttmp = t & ((__uint128_t)(mask_coef) << 64 | (__uint128_t)(mask_coef));

		mt = ttmp * QINV;
		mt = mt & ((__uint128_t)(mask_coef) << 64 | (__uint128_t)(mask_coef));

		mt *= Q;
		mt = t + mt;
		b = mt >> 32;

		t1 = (uint64_t)(b & 0xFFFFFFFF);
		t1 -= ((Q - t1) >> 63) & Q;

		t2 = (b) >> 64;
		t2 -= ((Q - t2) >> 63) & Q;

		r3 = t2;
		t0 = t1;

		r4 = mod_sub(r4, t0);

		t0 = r4 + r4;
		r4 = (t0 + r5) % Q;

		r2 = mod_add(r2, r3);

		inm = (uint64_t)r2*(uint64_t)mont64;
		t0 = montgomery_reduce((uint64_t)(inm));
		r1 = mod_sub(r1, t0);
		r1 = mod_sub(r1, r2);

		r2 = mod_sub(r2, r6);
		r2 = mod_sub(r2, r0);

		t = (uint64_t)r2*(uint64_t)mont45;
		t = t << 64;

		t0 = r2 + r2;
		t0 = t0 + t0;
		t0 = (t0 + t0) % Q;
		t0 = mod_sub(r4, t0);

		si2 = (uint64_t)t0*(uint64_t)inv24;
		t = t ^ si2;
		ttmp = t & ((__uint128_t)(mask_coef) << 64 | (__uint128_t)(mask_coef));

		mt = ttmp * QINV;
		mt = mt & ((__uint128_t)(mask_coef) << 64 | (__uint128_t)(mask_coef));

		mt *= Q;
		mt = t + mt;
		b = mt >> 32;

		t1 = (uint64_t)(b & 0xFFFFFFFF);
		t1 -= ((Q - t1) >> 63) & Q;

		t2 = (b) >> 64;
		t2 -= ((Q - t2) >> 63) & Q;

		t0 = t2;
		r1 = mod_add(r1, t0);
		r4 = t1;

		r5 = mod_add(r5, r1);

		t0 = r3 + r3;
		t0 = t0 + t0;
		t0 = t0 + t0;
		t0 = t0 + t0;

		t0 = (r1 + t0) % Q;

		inm = (uint64_t)t0*(uint64_t)inv18;
		r1 = montgomery_reduce((uint64_t)(inm));

		r3 = mod_add(r3, r1);
		r3 = mod_sub(Q, r3);

		inm = (uint64_t)r1*(uint64_t)mont30;
		t0 = montgomery_reduce(inm);
		t0 = mod_sub(t0, r5);

		inm = (uint64_t)t0*(uint64_t)inv60;
		r5 = montgomery_reduce(inm);

		r2 = mod_sub(r2, r4);
		r1 = mod_sub(r1, r5);

		C[i] = mod_add(C[i], r6);
		C[i + N_SB] = mod_add(C[i + N_SB], r5);
		C[i + (N_SB * 2)] = mod_add(C[i + (N_SB * 2)], r4);
		C[i + (N_SB * 3)] = mod_add(C[i + (N_SB * 3)], r3);
		C[i + (N_SB * 4)] = mod_add(C[i + (N_SB * 4)], r2);
		C[i + (N_SB * 5)] = mod_add(C[i + (N_SB * 5)], r1);
		C[i + (N_SB * 6)] = mod_add(C[i + (N_SB * 6)], r0);
	}
}


void poly_mul(poly* res, poly* a, poly* b)
{
	uint32_t i;
	uint32_t c[NR << 1];

	for (i = 0; i < (NR << 1); i++) c[i] = 0;

	for (i = N; i < NR; i++)
	{
		a->coeffs[i] = 0;
		b->coeffs[i] = 0;
	}

	toom_cook_4way(a->coeffs, b->coeffs, c);

	for (i = N; i < 2 * N - 1; i++) {
		c[i - N] = mod_add(c[i - N], c[i]);
		c[i - N + 1] = mod_add(c[i - N + 1], c[i]);
	}

	for (i = 0; i < N; i++)
		res->coeffs[i] = c[i];

}

void poly_mul_schoolbook(poly* a, poly* b, poly* res)
{
	// Polynomial multiplication using the schoolbook method, c[x] = a[x]*b[x] 
	// SECURITY NOTE: TO BE USED FOR TESTING ONLY.  
	uint32_t i, j;

	uint32_t c[NR << 1];
	uint32_t t0;
	for (i = 0; i < (NR << 1); i++) c[i] = 0;

	for (i = 0; i < N; i++)
	{
		for (j = 0; j < N; j++)
		{
			t0 = ((uint64_t)a->coeffs[i] * (uint64_t)b->coeffs[j]) % Q;
			c[i + j] = (c[i + j] + t0) % Q;
		}
	}

	for (i = N; i < 2 * N - 1; i++) {
		c[i - N] = mod_add(c[i - N], c[i]);
		c[i - N + 1] = mod_add(c[i - N + 1], c[i]);
	}
	c[N - 1] = mod_add(c[N - 1], c[2 * N - 1]);
	c[0] = mod_add(c[0], c[2 * N - 1]);
	c[1] = mod_add(c[1], c[2 * N - 1]);

	for (i = 0; i < N; i++)
		res->coeffs[i] = c[i];
}

void poly_reduce(poly *a) {
	unsigned int i;
	DBENCH_START();

	for (i = 0; i < N; ++i)
		a->coeffs[i] = reduce32(a->coeffs[i]);

	DBENCH_STOP(*tred);
}

void poly_caddq(poly *a) {
	unsigned int i;
	DBENCH_START();

	for (i = 0; i < N; ++i)
		a->coeffs[i] = caddq(a->coeffs[i]);

	DBENCH_STOP(*tred);
}

void poly_freeze(poly *a) {
	unsigned int i;
	DBENCH_START();

	for (i = 0; i < N; ++i)
		a->coeffs[i] = freeze(a->coeffs[i]);

	DBENCH_STOP(*tred);
}

void poly_add(poly *c, const poly *a, const poly *b) {
	unsigned int i;
	DBENCH_START();

	for (i = 0; i < N; ++i)
		c->coeffs[i] = a->coeffs[i] + b->coeffs[i];

	DBENCH_STOP(*tadd);
}

void poly_modadd(poly *c, const poly *a, const poly *b) {
	unsigned int i;

	for (i = 0; i < N; ++i)
		c->coeffs[i] = mod_add(a->coeffs[i], b->coeffs[i]);

}

void poly_sub(poly *c, const poly *a, const poly *b) {
	unsigned int i;
	DBENCH_START();

	for (i = 0; i < N; ++i)
		c->coeffs[i] = a->coeffs[i] - b->coeffs[i];

	DBENCH_STOP(*tadd);
}

void poly_modsub(poly *c, const poly *a, const poly *b) {
	unsigned int i;

	for (i = 0; i < N; ++i)
		c->coeffs[i] = mod_sub(a->coeffs[i], b->coeffs[i]);

}

void poly_shiftl(poly *a) {
	unsigned int i;
	DBENCH_START();

	for (i = 0; i < N; ++i)
		a->coeffs[i] <<= D;

	DBENCH_STOP(*tmul);
}

void poly_pointwise_montgomery(poly *c, const poly *a, const poly *b) {
	unsigned int i;
	DBENCH_START();

	for (i = 0; i < N; ++i)
		c->coeffs[i] = montgomery_reduce((int64_t)a->coeffs[i] * b->coeffs[i]);

	DBENCH_STOP(*tmul);
}

void poly_power2round(poly *a1, poly *a0, const poly *a) {
	unsigned int i;
	DBENCH_START();

	for (i = 0; i < N; ++i)
		a1->coeffs[i] = power2round(&a0->coeffs[i], a->coeffs[i]);

	DBENCH_STOP(*tround);
}

void poly_decompose(poly *a1, poly *a0, const poly *a) {
	unsigned int i;
	DBENCH_START();

	for (i = 0; i < N; ++i)
		a1->coeffs[i] = decompose(&a0->coeffs[i], a->coeffs[i]);

	DBENCH_STOP(*tround);
}

unsigned int poly_make_hint(poly *h, const poly *a0, const poly *a1) {
	unsigned int i, s = 0;
	DBENCH_START();

	for (i = 0; i < N; ++i) {
		h->coeffs[i] = make_hint(a0->coeffs[i], a1->coeffs[i]);
		s += h->coeffs[i];
	}

	DBENCH_STOP(*tround);
	return s;
}

void poly_use_hint(poly *b, const poly *a, const poly *h) {
	unsigned int i;
	DBENCH_START();

	for (i = 0; i < N; ++i)
		b->coeffs[i] = use_hint(a->coeffs[i], h->coeffs[i]);

	DBENCH_STOP(*tround);
}

int poly_chknorm(poly *a, int32_t B) {
	unsigned int i;
	int32_t t;
	DBENCH_START();

	if (B > (Q - 1) / 8)
		return 1;

	for (i = 0; i < N; ++i) {
		/* Absolute value */
		t = a->coeffs[i] >> 31;
		t = a->coeffs[i] - (t & 2 * a->coeffs[i]);

		if (t >= B) {
			DBENCH_STOP(*tsample);
			return 1;
		}
	}
	DBENCH_STOP(*tsample);
	return 0;
}

static unsigned int rej_uniform(int32_t *a,
	unsigned int len,
	const uint8_t *buf,
	unsigned int buflen)
{
	unsigned int ctr, pos;
	uint32_t t;
	DBENCH_START();

	ctr = pos = 0;
	while (ctr < len && pos + 3 <= buflen) {
		t = buf[pos++];
		t |= (uint32_t)buf[pos++] << 8;
		t |= (uint32_t)buf[pos++] << 16;
		t &= 0x7FFFFF;

		if (t < Q)
			a[ctr++] = t;
	}

	DBENCH_STOP(*tsample);
	return ctr;
}

#define POLY_UNIFORM_NBLOCKS ((768 + STREAM128_BLOCKBYTES - 1)/STREAM128_BLOCKBYTES)
void poly_uniform(poly *a,
	const uint8_t seed[SEEDBYTES],
	uint16_t nonce)
{
	unsigned int i, ctr, off;
	unsigned int buflen = POLY_UNIFORM_NBLOCKS * STREAM128_BLOCKBYTES;
	uint8_t buf[POLY_UNIFORM_NBLOCKS*STREAM128_BLOCKBYTES + 2];
	stream128_state state;

	stream128_init(&state, seed, nonce);
	stream128_squeezeblocks(buf, POLY_UNIFORM_NBLOCKS, &state);

	ctr = rej_uniform(a->coeffs, N, buf, buflen);

	while (ctr < N) {
		off = buflen % 3;
		for (i = 0; i < off; ++i)
			buf[i] = buf[buflen - off + i];

		stream128_squeezeblocks(buf + off, 1, &state);
		buflen = STREAM128_BLOCKBYTES + off;
		ctr += rej_uniform(a->coeffs + ctr, N - ctr, buf, buflen);
	}
}

static unsigned int rej_eta(int32_t *a,
	unsigned int len,
	const uint8_t *buf,
	unsigned int buflen)
{
	unsigned int ctr, pos;
	uint32_t t0, t1;
	DBENCH_START();

	ctr = pos = 0;
	while (ctr < len && pos < buflen) {
		t0 = buf[pos] & 0x0F;
		t1 = buf[pos++] >> 4;

		if (t0 < 15) {
			t0 = t0 - (205 * t0 >> 10) * 5;
			a[ctr++] = 2 - t0;
		}
		if (t1 < 15 && ctr < len) {
			t1 = t1 - (205 * t1 >> 10) * 5;
			a[ctr++] = 2 - t1;
		}
	}

	DBENCH_STOP(*tsample);
	return ctr;
}

#define POLY_UNIFORM_ETA_NBLOCKS ((136 + STREAM256_BLOCKBYTES - 1)/STREAM256_BLOCKBYTES)
void poly_uniform_eta(poly *a,
	const uint8_t seed[CRHBYTES],
	uint16_t nonce)
{
	unsigned int ctr;
	unsigned int buflen = POLY_UNIFORM_ETA_NBLOCKS * STREAM256_BLOCKBYTES;
	uint8_t buf[POLY_UNIFORM_ETA_NBLOCKS*STREAM256_BLOCKBYTES];
	stream256_state state;

	stream256_init(&state, seed, nonce);
	stream256_squeezeblocks(buf, POLY_UNIFORM_ETA_NBLOCKS, &state);

	ctr = rej_eta(a->coeffs, N, buf, buflen);

	while (ctr < N) {
		stream256_squeezeblocks(buf, 1, &state);
		ctr += rej_eta(a->coeffs + ctr, N - ctr, buf, STREAM256_BLOCKBYTES);
	}
}

#if N==1201
#define POLY_UNIFORM_GAMMA1_NBLOCKS ((3003 + STREAM256_BLOCKBYTES - 1)/STREAM256_BLOCKBYTES)
#elif N==1607
#define POLY_UNIFORM_GAMMA1_NBLOCKS ((4018 + STREAM256_BLOCKBYTES - 1)/STREAM256_BLOCKBYTES)
#elif N==2039
#define POLY_UNIFORM_GAMMA1_NBLOCKS ((5098 + STREAM256_BLOCKBYTES - 1)/STREAM256_BLOCKBYTES)
#endif
void poly_uniform_gamma1(poly *a,
	const uint8_t seed[CRHBYTES],
	uint16_t nonce)
{
	uint8_t buf[POLY_UNIFORM_GAMMA1_NBLOCKS*STREAM256_BLOCKBYTES];
	stream256_state state;

	stream256_init(&state, seed, nonce);
	stream256_squeezeblocks(buf, POLY_UNIFORM_GAMMA1_NBLOCKS, &state);
	polyz_unpack(a, buf);
}

void poly_challenge(poly *c, const uint8_t seed[SEEDBYTES]) {
	unsigned int i, b, pos;
	uint32_t signs;
	uint8_t buf[SHAKE256_RATE];
	keccak_state state;

	shake256_init(&state);
	shake256_absorb(&state, seed, SEEDBYTES);
	shake256_finalize(&state);
	shake256_squeezeblocks(buf, 1, &state);

	signs = 0;
	for (i = 0; i < 4; ++i)
		signs |= (uint32_t)buf[i] << 8 * i;
	pos = 4;

	for (i = 0; i < N; ++i)
		c->coeffs[i] = 0;
#if (N==1021)
	for (i = N - TAU; i < N; ++i) {
		do {
			if (pos >= SHAKE256_RATE) {
				shake256_squeezeblocks(buf, 1, &state);
				pos = 0;
			}

			b = (uint32_t)buf[pos++] << 2;
			b |= (buf[pos++] & 0x3);
		} while (b > i);

		c->coeffs[i] = c->coeffs[b];
		c->coeffs[b] = 1 - 2 * (signs & 1);
		signs >>= 1;
	}
#else
	for (i = N - TAU; i < N; ++i) {
		do {
			if (pos >= SHAKE256_RATE) {
				shake256_squeezeblocks(buf, 1, &state);
				pos = 0;
			}

			b = (uint32_t)buf[pos++] << 3;
			b |= (buf[pos++] & 0x7);
		} while (b > i);

		c->coeffs[i] = c->coeffs[b];
		c->coeffs[b] = 1 - 2 * (signs & 1);
		signs >>= 1;
	}
#endif
}
