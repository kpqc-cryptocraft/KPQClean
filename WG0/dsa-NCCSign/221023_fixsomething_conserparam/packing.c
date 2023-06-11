#include "params.h"
#include "packing.h"
#include "poly.h"
#include <string.h>

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

void polyeta_pack(uint8_t *r, const poly *a) {
	unsigned int i;
	uint8_t t[8];
	DBENCH_START();

#if N==1201
	for (i = 0; i < 150; ++i) {
		t[0] = ETA - a->coeffs[8 * i + 0];
		t[1] = ETA - a->coeffs[8 * i + 1];
		t[2] = ETA - a->coeffs[8 * i + 2];
		t[3] = ETA - a->coeffs[8 * i + 3];
		t[4] = ETA - a->coeffs[8 * i + 4];
		t[5] = ETA - a->coeffs[8 * i + 5];
		t[6] = ETA - a->coeffs[8 * i + 6];
		t[7] = ETA - a->coeffs[8 * i + 7];

		r[3 * i + 0] = (t[0] >> 0) | (t[1] << 3) | (t[2] << 6);
		r[3 * i + 1] = (t[2] >> 2) | (t[3] << 1) | (t[4] << 4) | (t[5] << 7);
		r[3 * i + 2] = (t[5] >> 1) | (t[6] << 2) | (t[7] << 5);
	}
	r[450] = ETA - a->coeffs[1200];
#elif N==1607
	for (i = 0; i < 200; ++i) {
		t[0] = ETA - a->coeffs[8 * i + 0];
		t[1] = ETA - a->coeffs[8 * i + 1];
		t[2] = ETA - a->coeffs[8 * i + 2];
		t[3] = ETA - a->coeffs[8 * i + 3];
		t[4] = ETA - a->coeffs[8 * i + 4];
		t[5] = ETA - a->coeffs[8 * i + 5];
		t[6] = ETA - a->coeffs[8 * i + 6];
		t[7] = ETA - a->coeffs[8 * i + 7];

		r[3 * i + 0] = (t[0] >> 0) | (t[1] << 3) | (t[2] << 6);
		r[3 * i + 1] = (t[2] >> 2) | (t[3] << 1) | (t[4] << 4) | (t[5] << 7);
		r[3 * i + 2] = (t[5] >> 1) | (t[6] << 2) | (t[7] << 5);
	}

	t[0] = ETA - a->coeffs[1600];
	t[1] = ETA - a->coeffs[1601];
	t[2] = ETA - a->coeffs[1602];
	t[3] = ETA - a->coeffs[1603];
	t[4] = ETA - a->coeffs[1604];
	t[5] = ETA - a->coeffs[1605];
	t[6] = ETA - a->coeffs[1606];

	r[600] = (t[0] >> 0) | (t[1] << 3) | (t[2] << 6);
	r[601] = (t[2] >> 2) | (t[3] << 1) | (t[4] << 4) | (t[5] << 7);
	r[602] = (t[5] >> 1) | (t[6] << 2);
#elif N==2039
	for (i = 0; i < 254; ++i) {
		t[0] = ETA - a->coeffs[8 * i + 0];
		t[1] = ETA - a->coeffs[8 * i + 1];
		t[2] = ETA - a->coeffs[8 * i + 2];
		t[3] = ETA - a->coeffs[8 * i + 3];
		t[4] = ETA - a->coeffs[8 * i + 4];
		t[5] = ETA - a->coeffs[8 * i + 5];
		t[6] = ETA - a->coeffs[8 * i + 6];
		t[7] = ETA - a->coeffs[8 * i + 7];

		r[3 * i + 0] = (t[0] >> 0) | (t[1] << 3) | (t[2] << 6);
		r[3 * i + 1] = (t[2] >> 2) | (t[3] << 1) | (t[4] << 4) | (t[5] << 7);
		r[3 * i + 2] = (t[5] >> 1) | (t[6] << 2) | (t[7] << 5);
	}

	t[0] = ETA - a->coeffs[2032];
	t[1] = ETA - a->coeffs[2033];
	t[2] = ETA - a->coeffs[2034];
	t[3] = ETA - a->coeffs[2035];
	t[4] = ETA - a->coeffs[2036];
	t[5] = ETA - a->coeffs[2037];
	t[6] = ETA - a->coeffs[2038];

	r[762] = (t[0] >> 0) | (t[1] << 3) | (t[2] << 6);
	r[763] = (t[2] >> 2) | (t[3] << 1) | (t[4] << 4) | (t[5] << 7);
	r[764] = (t[5] >> 1) | (t[6] << 2);
#endif
	DBENCH_STOP(*tpack);
}

void polyeta_unpack(poly *r, const uint8_t *a) {
	unsigned int i;
	DBENCH_START();

#if N==1201 
	for (i = 0; i < 150; ++i) {
		r->coeffs[8 * i + 0] = (a[3 * i + 0] >> 0) & 7;
		r->coeffs[8 * i + 1] = (a[3 * i + 0] >> 3) & 7;
		r->coeffs[8 * i + 2] = ((a[3 * i + 0] >> 6) | (a[3 * i + 1] << 2)) & 7;
		r->coeffs[8 * i + 3] = (a[3 * i + 1] >> 1) & 7;
		r->coeffs[8 * i + 4] = (a[3 * i + 1] >> 4) & 7;
		r->coeffs[8 * i + 5] = ((a[3 * i + 1] >> 7) | (a[3 * i + 2] << 1)) & 7;
		r->coeffs[8 * i + 6] = (a[3 * i + 2] >> 2) & 7;
		r->coeffs[8 * i + 7] = (a[3 * i + 2] >> 5) & 7;

		r->coeffs[8 * i + 0] = ETA - r->coeffs[8 * i + 0];
		r->coeffs[8 * i + 1] = ETA - r->coeffs[8 * i + 1];
		r->coeffs[8 * i + 2] = ETA - r->coeffs[8 * i + 2];
		r->coeffs[8 * i + 3] = ETA - r->coeffs[8 * i + 3];
		r->coeffs[8 * i + 4] = ETA - r->coeffs[8 * i + 4];
		r->coeffs[8 * i + 5] = ETA - r->coeffs[8 * i + 5];
		r->coeffs[8 * i + 6] = ETA - r->coeffs[8 * i + 6];
		r->coeffs[8 * i + 7] = ETA - r->coeffs[8 * i + 7];
	}

	r->coeffs[1200] = ETA - a[450];
#elif N==1607 
	for (i = 0; i < 200; ++i) {
		r->coeffs[8 * i + 0] = (a[3 * i + 0] >> 0) & 7;
		r->coeffs[8 * i + 1] = (a[3 * i + 0] >> 3) & 7;
		r->coeffs[8 * i + 2] = ((a[3 * i + 0] >> 6) | (a[3 * i + 1] << 2)) & 7;
		r->coeffs[8 * i + 3] = (a[3 * i + 1] >> 1) & 7;
		r->coeffs[8 * i + 4] = (a[3 * i + 1] >> 4) & 7;
		r->coeffs[8 * i + 5] = ((a[3 * i + 1] >> 7) | (a[3 * i + 2] << 1)) & 7;
		r->coeffs[8 * i + 6] = (a[3 * i + 2] >> 2) & 7;
		r->coeffs[8 * i + 7] = (a[3 * i + 2] >> 5) & 7;

		r->coeffs[8 * i + 0] = ETA - r->coeffs[8 * i + 0];
		r->coeffs[8 * i + 1] = ETA - r->coeffs[8 * i + 1];
		r->coeffs[8 * i + 2] = ETA - r->coeffs[8 * i + 2];
		r->coeffs[8 * i + 3] = ETA - r->coeffs[8 * i + 3];
		r->coeffs[8 * i + 4] = ETA - r->coeffs[8 * i + 4];
		r->coeffs[8 * i + 5] = ETA - r->coeffs[8 * i + 5];
		r->coeffs[8 * i + 6] = ETA - r->coeffs[8 * i + 6];
		r->coeffs[8 * i + 7] = ETA - r->coeffs[8 * i + 7];
	}

	r->coeffs[1600] = (a[600] >> 0) & 7;
	r->coeffs[1601] = (a[600] >> 3) & 7;
	r->coeffs[1602] = ((a[600] >> 6) | (a[601] << 2)) & 7;
	r->coeffs[1603] = (a[601] >> 1) & 7;
	r->coeffs[1604] = (a[601] >> 4) & 7;
	r->coeffs[1605] = ((a[601] >> 7) | (a[602] << 1)) & 7;
	r->coeffs[1606] = (a[602] >> 2) & 7;

	r->coeffs[1600] = ETA - r->coeffs[1600];
	r->coeffs[1601] = ETA - r->coeffs[1601];
	r->coeffs[1602] = ETA - r->coeffs[1602];
	r->coeffs[1603] = ETA - r->coeffs[1603];
	r->coeffs[1604] = ETA - r->coeffs[1604];
	r->coeffs[1605] = ETA - r->coeffs[1605];
	r->coeffs[1606] = ETA - r->coeffs[1606];
#elif N==2039
	for (i = 0; i < 254; ++i) {
		r->coeffs[8 * i + 0] = (a[3 * i + 0] >> 0) & 7;
		r->coeffs[8 * i + 1] = (a[3 * i + 0] >> 3) & 7;
		r->coeffs[8 * i + 2] = ((a[3 * i + 0] >> 6) | (a[3 * i + 1] << 2)) & 7;
		r->coeffs[8 * i + 3] = (a[3 * i + 1] >> 1) & 7;
		r->coeffs[8 * i + 4] = (a[3 * i + 1] >> 4) & 7;
		r->coeffs[8 * i + 5] = ((a[3 * i + 1] >> 7) | (a[3 * i + 2] << 1)) & 7;
		r->coeffs[8 * i + 6] = (a[3 * i + 2] >> 2) & 7;
		r->coeffs[8 * i + 7] = (a[3 * i + 2] >> 5) & 7;

		r->coeffs[8 * i + 0] = ETA - r->coeffs[8 * i + 0];
		r->coeffs[8 * i + 1] = ETA - r->coeffs[8 * i + 1];
		r->coeffs[8 * i + 2] = ETA - r->coeffs[8 * i + 2];
		r->coeffs[8 * i + 3] = ETA - r->coeffs[8 * i + 3];
		r->coeffs[8 * i + 4] = ETA - r->coeffs[8 * i + 4];
		r->coeffs[8 * i + 5] = ETA - r->coeffs[8 * i + 5];
		r->coeffs[8 * i + 6] = ETA - r->coeffs[8 * i + 6];
		r->coeffs[8 * i + 7] = ETA - r->coeffs[8 * i + 7];
	}

	r->coeffs[2032] = (a[762] >> 0) & 7;
	r->coeffs[2033] = (a[762] >> 3) & 7;
	r->coeffs[2034] = ((a[762] >> 6) | (a[763] << 2)) & 7;
	r->coeffs[2035] = (a[763] >> 1) & 7;
	r->coeffs[2036] = (a[763] >> 4) & 7;
	r->coeffs[2037] = ((a[763] >> 7) | (a[764] << 1)) & 7;
	r->coeffs[2038] = (a[764] >> 2) & 7;

	r->coeffs[2032] = ETA - r->coeffs[2032];
	r->coeffs[2033] = ETA - r->coeffs[2033];
	r->coeffs[2034] = ETA - r->coeffs[2034];
	r->coeffs[2035] = ETA - r->coeffs[2035];
	r->coeffs[2036] = ETA - r->coeffs[2036];
	r->coeffs[2037] = ETA - r->coeffs[2037];
	r->coeffs[2038] = ETA - r->coeffs[2038];
#endif
	DBENCH_STOP(*tpack);
}

void polyt1_pack(uint8_t *r, const poly *a) {
	unsigned int i;
	DBENCH_START();

#if N==1201 
	for (i = 0; i < 150; ++i) {
		r[13 * i + 0] = (a->coeffs[8 * i + 0] >> 0);
		r[13 * i + 1] = (a->coeffs[8 * i + 0] >> 8) | (a->coeffs[8 * i + 1] << 5);
		r[13 * i + 2] = (a->coeffs[8 * i + 1] >> 3);
		r[13 * i + 3] = (a->coeffs[8 * i + 1] >> 11) | (a->coeffs[8 * i + 2] << 2);
		r[13 * i + 4] = (a->coeffs[8 * i + 2] >> 6) | (a->coeffs[8 * i + 3] << 7);
		r[13 * i + 5] = (a->coeffs[8 * i + 3] >> 1);
		r[13 * i + 6] = (a->coeffs[8 * i + 3] >> 9) | (a->coeffs[8 * i + 4] << 4);
		r[13 * i + 7] = (a->coeffs[8 * i + 4] >> 4);
		r[13 * i + 8] = (a->coeffs[8 * i + 4] >> 12) | (a->coeffs[8 * i + 5] << 1);
		r[13 * i + 9] = (a->coeffs[8 * i + 5] >> 7) | (a->coeffs[8 * i + 6] << 6);
		r[13 * i + 10] = (a->coeffs[8 * i + 6] >> 2);
		r[13 * i + 11] = (a->coeffs[8 * i + 6] >> 10) | (a->coeffs[8 * i + 7] << 3);
		r[13 * i + 12] = (a->coeffs[8 * i + 7] >> 5);
	}

	r[1950] = a->coeffs[1200] >> 0;
	r[1951] = a->coeffs[1200] >> 8;
#elif N==1607
	for (i = 0; i < 803; ++i) {
		r[3 * i + 0] = (a->coeffs[2 * i + 0] >> 0);
		r[3 * i + 1] = (a->coeffs[2 * i + 0] >> 8) | (a->coeffs[2 * i + 1] << 4);
		r[3 * i + 2] = (a->coeffs[2 * i + 1] >> 4);
	}

	r[2409] = a->coeffs[1606] >> 0;
	r[2410] = a->coeffs[1606] >> 8;
#elif N==2039
	for (i = 0; i < 1019; ++i) {
		r[3 * i + 0] = (a->coeffs[2 * i + 0] >> 0);
		r[3 * i + 1] = (a->coeffs[2 * i + 0] >> 8) | (a->coeffs[2 * i + 1] << 4);
		r[3 * i + 2] = (a->coeffs[2 * i + 1] >> 4);
	}

	r[3057] = (a->coeffs[2038] >> 0);
	r[3058] = (a->coeffs[2038] >> 8);
#endif
	DBENCH_STOP(*tpack);
}

void polyt1_unpack(poly *r, const uint8_t *a) {
	unsigned int i;
	DBENCH_START();

#if N==1201
	for (i = 0; i < 150; ++i) {
		r->coeffs[8 * i + 0] = ((a[13 * i + 0] >> 0) | ((uint32_t)a[13 * i + 1] << 8)) & 0x1FFF;
		r->coeffs[8 * i + 1] = ((a[13 * i + 1] >> 5) | ((uint32_t)a[13 * i + 2] << 3) | ((uint32_t)a[13 * i + 3] << 11)) & 0x1FFF;
		r->coeffs[8 * i + 2] = ((a[13 * i + 3] >> 2) | ((uint32_t)a[13 * i + 4] << 6)) & 0x1FFF;
		r->coeffs[8 * i + 3] = ((a[13 * i + 4] >> 7) | ((uint32_t)a[13 * i + 5] << 1) | ((uint32_t)a[13 * i + 6] << 9)) & 0x1FFF;
		r->coeffs[8 * i + 4] = ((a[13 * i + 6] >> 4) | ((uint32_t)a[13 * i + 7] << 4) | ((uint32_t)a[13 * i + 8] << 12)) & 0x1FFF;
		r->coeffs[8 * i + 5] = ((a[13 * i + 8] >> 1) | ((uint32_t)a[13 * i + 9] << 7)) & 0x1FFF;
		r->coeffs[8 * i + 6] = ((a[13 * i + 9] >> 6) | ((uint32_t)a[13 * i + 10] << 2) | ((uint32_t)a[13 * i + 11] << 10)) & 0x1FFF;
		r->coeffs[8 * i + 7] = ((a[13 * i + 11] >> 3) | ((uint32_t)a[13 * i + 12] << 5)) & 0x1FFF;
	}

	r->coeffs[1200] = ((a[1950] >> 0) | ((uint32_t)a[1951] << 8)) & 0x1FFF;
#elif N==1607
	for (i = 0; i < 803; ++i) {
		r->coeffs[2 * i + 0] = ((a[3 * i + 0] >> 0) | ((uint32_t)a[3 * i + 1] << 8)) & 0xFFF;
		r->coeffs[2 * i + 1] = ((a[3 * i + 1] >> 4) | ((uint32_t)a[3 * i + 2] << 4)) & 0xFFF;
	}

	r->coeffs[1606] = ((a[2409] >> 0) | ((uint32_t)a[2410] << 8)) & 0xFFF;
#elif N==2039
	for (i = 0; i < 1019; ++i) {
		r->coeffs[2 * i + 0] = ((a[3 * i + 0] >> 0) | ((uint32_t)a[3 * i + 1] << 8)) & 0xFFF;
		r->coeffs[2 * i + 1] = ((a[3 * i + 1] >> 4) | ((uint32_t)a[3 * i + 2] << 4)) & 0xFFF;
	}

	r->coeffs[2038] = ((a[3057] >> 0) | ((uint32_t)a[3058] << 8)) & 0xFFF;
#endif
	DBENCH_STOP(*tpack);
}

void polyt0_pack(uint8_t *r, const poly *a) {
	unsigned int i;
#if N==1201
	uint32_t t[2];
#else
	uint32_t t[8];
#endif
	DBENCH_START();
#if N==1201
	for (i = 0; i < 600; ++i) {
		t[0] = (1 << 11) - a->coeffs[2 * i + 0];
		t[1] = (1 << 11) - a->coeffs[2 * i + 1];

		r[3 * i + 0] = (t[0] >> 0);
		r[3 * i + 1] = (t[0] >> 8) | (t[1] << 4);
		r[3 * i + 2] = (t[1] >> 4);
	}

	t[0] = (1 << 11) - a->coeffs[1200];

	r[1800] = (t[0] >> 0);
	r[1801] = (t[0] >> 8);
#elif N==1607
	for (i = 0; i < 200; ++i) {
		t[0] = (1 << 12) - a->coeffs[8 * i + 0];
		t[1] = (1 << 12) - a->coeffs[8 * i + 1];
		t[2] = (1 << 12) - a->coeffs[8 * i + 2];
		t[3] = (1 << 12) - a->coeffs[8 * i + 3];
		t[4] = (1 << 12) - a->coeffs[8 * i + 4];
		t[5] = (1 << 12) - a->coeffs[8 * i + 5];
		t[6] = (1 << 12) - a->coeffs[8 * i + 6];
		t[7] = (1 << 12) - a->coeffs[8 * i + 7];

		r[13 * i + 0] = (t[0] >> 0);
		r[13 * i + 1] = (t[0] >> 8) | (t[1] << 5);
		r[13 * i + 2] = (t[1] >> 3);
		r[13 * i + 3] = (t[1] >> 11) | (t[2] << 2);
		r[13 * i + 4] = (t[2] >> 6) | (t[3] << 7);
		r[13 * i + 5] = (t[3] >> 1);
		r[13 * i + 6] = (t[3] >> 9) | (t[4] << 4);
		r[13 * i + 7] = (t[4] >> 4);
		r[13 * i + 8] = (t[4] >> 12) | (t[5] << 1);
		r[13 * i + 9] = (t[5] >> 7) | (t[6] << 6);
		r[13 * i + 10] = (t[6] >> 2);
		r[13 * i + 11] = (t[6] >> 10) | (t[7] << 3);
		r[13 * i + 12] = (t[7] >> 5);
	}

	t[0] = (1 << 12) - a->coeffs[1600];
	t[1] = (1 << 12) - a->coeffs[1601];
	t[2] = (1 << 12) - a->coeffs[1602];
	t[3] = (1 << 12) - a->coeffs[1603];
	t[4] = (1 << 12) - a->coeffs[1604];
	t[5] = (1 << 12) - a->coeffs[1605];
	t[6] = (1 << 12) - a->coeffs[1606];

	r[2600] = (t[0] >> 0);
	r[2601] = (t[0] >> 8) | (t[1] << 5);
	r[2602] = (t[1] >> 3);
	r[2603] = (t[1] >> 11) | (t[2] << 2);
	r[2604] = (t[2] >> 6) | (t[3] << 7);
	r[2605] = (t[3] >> 1);
	r[2606] = (t[3] >> 9) | (t[4] << 4);
	r[2607] = (t[4] >> 4);
	r[2608] = (t[4] >> 12) | (t[5] << 1);
	r[2609] = (t[5] >> 7) | (t[6] << 6);
	r[2610] = (t[6] >> 2);
	r[2611] = (t[6] >> 10);
#elif N==2039 
	for (i = 0; i < 254; ++i) {
		t[0] = (1 << 12) - a->coeffs[8 * i + 0];
		t[1] = (1 << 12) - a->coeffs[8 * i + 1];
		t[2] = (1 << 12) - a->coeffs[8 * i + 2];
		t[3] = (1 << 12) - a->coeffs[8 * i + 3];
		t[4] = (1 << 12) - a->coeffs[8 * i + 4];
		t[5] = (1 << 12) - a->coeffs[8 * i + 5];
		t[6] = (1 << 12) - a->coeffs[8 * i + 6];
		t[7] = (1 << 12) - a->coeffs[8 * i + 7];

		r[13 * i + 0] = (t[0] >> 0);
		r[13 * i + 1] = (t[0] >> 8) | (t[1] << 5);
		r[13 * i + 2] = (t[1] >> 3);
		r[13 * i + 3] = (t[1] >> 11) | (t[2] << 2);
		r[13 * i + 4] = (t[2] >> 6) | (t[3] << 7);
		r[13 * i + 5] = (t[3] >> 1);
		r[13 * i + 6] = (t[3] >> 9) | (t[4] << 4);
		r[13 * i + 7] = (t[4] >> 4);
		r[13 * i + 8] = (t[4] >> 12) | (t[5] << 1);
		r[13 * i + 9] = (t[5] >> 7) | (t[6] << 6);
		r[13 * i + 10] = (t[6] >> 2);
		r[13 * i + 11] = (t[6] >> 10) | (t[7] << 3);
		r[13 * i + 12] = (t[7] >> 5);
	}

	t[0] = (1 << 12) - a->coeffs[2032];
	t[1] = (1 << 12) - a->coeffs[2033];
	t[2] = (1 << 12) - a->coeffs[2034];
	t[3] = (1 << 12) - a->coeffs[2035];
	t[4] = (1 << 12) - a->coeffs[2036];
	t[5] = (1 << 12) - a->coeffs[2037];
	t[6] = (1 << 12) - a->coeffs[2038];

	r[3302] = (t[0] >> 0);
	r[3303] = (t[0] >> 8) | (t[1] << 5);
	r[3304] = (t[1] >> 3);
	r[3305] = (t[1] >> 11) | (t[2] << 2);
	r[3306] = (t[2] >> 6) | (t[3] << 7);
	r[3307] = (t[3] >> 1);
	r[3308] = (t[3] >> 9) | (t[4] << 4);
	r[3309] = (t[4] >> 4);
	r[3310] = (t[4] >> 12) | (t[5] << 1);
	r[3311] = (t[5] >> 7) | (t[6] << 6);
	r[3312] = (t[6] >> 2);
	r[3313] = (t[6] >> 10);
#endif
	DBENCH_STOP(*tpack);
}

void polyt0_unpack(poly *r, const uint8_t *a) {
	unsigned int i;
	DBENCH_START();
#if N==1201
	for (i = 0; i < 600; ++i) {
		r->coeffs[2 * i + 0] = ((a[3 * i + 0] >> 0) | ((uint32_t)a[3 * i + 1] << 8)) & 0xFFF;
		r->coeffs[2 * i + 1] = ((a[3 * i + 1] >> 4) | ((uint32_t)a[3 * i + 2] << 4)) & 0xFFF;

		r->coeffs[2 * i + 0] = (1 << 11) - r->coeffs[2 * i + 0];
		r->coeffs[2 * i + 1] = (1 << 11) - r->coeffs[2 * i + 1];
	}

	r->coeffs[1200] = ((a[1800] >> 0) | ((uint32_t)a[1801] << 8)) & 0xFFF;

	r->coeffs[1200] = (1 << 11) - r->coeffs[1200];
#elif N==1607
	for (i = 0; i < 200; ++i) {
		r->coeffs[8 * i + 0] = ((a[13 * i + 0] >> 0) | ((uint32_t)a[13 * i + 1] << 8)) & 0x1FFF;
		r->coeffs[8 * i + 1] = ((a[13 * i + 1] >> 5) | ((uint32_t)a[13 * i + 2] << 3) | ((uint32_t)a[13 * i + 3] << 11)) & 0x1FFF;
		r->coeffs[8 * i + 2] = ((a[13 * i + 3] >> 2) | ((uint32_t)a[13 * i + 4] << 6)) & 0x1FFF;
		r->coeffs[8 * i + 3] = ((a[13 * i + 4] >> 7) | ((uint32_t)a[13 * i + 5] << 1) | ((uint32_t)a[13 * i + 6] << 9)) & 0x1FFF;
		r->coeffs[8 * i + 4] = ((a[13 * i + 6] >> 4) | ((uint32_t)a[13 * i + 7] << 4) | ((uint32_t)a[13 * i + 8] << 12)) & 0x1FFF;
		r->coeffs[8 * i + 5] = ((a[13 * i + 8] >> 1) | ((uint32_t)a[13 * i + 9] << 7)) & 0x1FFF;
		r->coeffs[8 * i + 6] = ((a[13 * i + 9] >> 6) | ((uint32_t)a[13 * i + 10] << 2) | ((uint32_t)a[13 * i + 11] << 10)) & 0x1FFF;
		r->coeffs[8 * i + 7] = ((a[13 * i + 11] >> 3) | ((uint32_t)a[13 * i + 12] << 5)) & 0x1FFF;

		r->coeffs[8 * i + 0] = (1 << 12) - r->coeffs[8 * i + 0];
		r->coeffs[8 * i + 1] = (1 << 12) - r->coeffs[8 * i + 1];
		r->coeffs[8 * i + 2] = (1 << 12) - r->coeffs[8 * i + 2];
		r->coeffs[8 * i + 3] = (1 << 12) - r->coeffs[8 * i + 3];
		r->coeffs[8 * i + 4] = (1 << 12) - r->coeffs[8 * i + 4];
		r->coeffs[8 * i + 5] = (1 << 12) - r->coeffs[8 * i + 5];
		r->coeffs[8 * i + 6] = (1 << 12) - r->coeffs[8 * i + 6];
		r->coeffs[8 * i + 7] = (1 << 12) - r->coeffs[8 * i + 7];
	}

	r->coeffs[1600] = ((a[2600] >> 0) | ((uint32_t)a[2601] << 8)) & 0x1FFF;
	r->coeffs[1601] = ((a[2601] >> 5) | ((uint32_t)a[2602] << 3) | ((uint32_t)a[2603] << 11)) & 0x1FFF;
	r->coeffs[1602] = ((a[2603] >> 2) | ((uint32_t)a[2604] << 6)) & 0x1FFF;
	r->coeffs[1603] = ((a[2604] >> 7) | ((uint32_t)a[2605] << 1) | ((uint32_t)a[2606] << 9)) & 0x1FFF;
	r->coeffs[1604] = ((a[2606] >> 4) | ((uint32_t)a[2607] << 4) | ((uint32_t)a[2608] << 12)) & 0x1FFF;
	r->coeffs[1605] = ((a[2608] >> 1) | ((uint32_t)a[2609] << 7)) & 0x1FFF;
	r->coeffs[1606] = ((a[2609] >> 6) | ((uint32_t)a[2610] << 2) | ((uint32_t)a[2611] << 10)) & 0x1FFF;

	r->coeffs[1600] = (1 << 12) - r->coeffs[1600];
	r->coeffs[1601] = (1 << 12) - r->coeffs[1601];
	r->coeffs[1602] = (1 << 12) - r->coeffs[1602];
	r->coeffs[1603] = (1 << 12) - r->coeffs[1603];
	r->coeffs[1604] = (1 << 12) - r->coeffs[1604];
	r->coeffs[1605] = (1 << 12) - r->coeffs[1605];
	r->coeffs[1606] = (1 << 12) - r->coeffs[1606];
#elif N==2039
	for (i = 0; i < 254; ++i) {
		r->coeffs[8 * i + 0] = ((a[13 * i + 0] >> 0) | ((uint32_t)a[13 * i + 1] << 8)) & 0x1FFF;
		r->coeffs[8 * i + 1] = ((a[13 * i + 1] >> 5) | ((uint32_t)a[13 * i + 2] << 3) | ((uint32_t)a[13 * i + 3] << 11)) & 0x1FFF;
		r->coeffs[8 * i + 2] = ((a[13 * i + 3] >> 2) | ((uint32_t)a[13 * i + 4] << 6)) & 0x1FFF;
		r->coeffs[8 * i + 3] = ((a[13 * i + 4] >> 7) | ((uint32_t)a[13 * i + 5] << 1) | ((uint32_t)a[13 * i + 6] << 9)) & 0x1FFF;
		r->coeffs[8 * i + 4] = ((a[13 * i + 6] >> 4) | ((uint32_t)a[13 * i + 7] << 4) | ((uint32_t)a[13 * i + 8] << 12)) & 0x1FFF;
		r->coeffs[8 * i + 5] = ((a[13 * i + 8] >> 1) | ((uint32_t)a[13 * i + 9] << 7)) & 0x1FFF;
		r->coeffs[8 * i + 6] = ((a[13 * i + 9] >> 6) | ((uint32_t)a[13 * i + 10] << 2) | ((uint32_t)a[13 * i + 11] << 10)) & 0x1FFF;
		r->coeffs[8 * i + 7] = ((a[13 * i + 11] >> 3) | ((uint32_t)a[13 * i + 12] << 5)) & 0x1FFF;

		r->coeffs[8 * i + 0] = (1 << 12) - r->coeffs[8 * i + 0];
		r->coeffs[8 * i + 1] = (1 << 12) - r->coeffs[8 * i + 1];
		r->coeffs[8 * i + 2] = (1 << 12) - r->coeffs[8 * i + 2];
		r->coeffs[8 * i + 3] = (1 << 12) - r->coeffs[8 * i + 3];
		r->coeffs[8 * i + 4] = (1 << 12) - r->coeffs[8 * i + 4];
		r->coeffs[8 * i + 5] = (1 << 12) - r->coeffs[8 * i + 5];
		r->coeffs[8 * i + 6] = (1 << 12) - r->coeffs[8 * i + 6];
		r->coeffs[8 * i + 7] = (1 << 12) - r->coeffs[8 * i + 7];
	}

	r->coeffs[2032] = ((a[3302] >> 0) | ((uint32_t)a[3303] << 8)) & 0x1FFF;
	r->coeffs[2033] = ((a[3303] >> 5) | ((uint32_t)a[3304] << 3) | ((uint32_t)a[3305] << 11)) & 0x1FFF;
	r->coeffs[2034] = ((a[3305] >> 2) | ((uint32_t)a[3306] << 6)) & 0x1FFF;
	r->coeffs[2035] = ((a[3306] >> 7) | ((uint32_t)a[3307] << 1) | ((uint32_t)a[3308] << 9)) & 0x1FFF;
	r->coeffs[2036] = ((a[3308] >> 4) | ((uint32_t)a[3309] << 4) | ((uint32_t)a[3310] << 12)) & 0x1FFF;
	r->coeffs[2037] = ((a[3310] >> 1) | ((uint32_t)a[3311] << 7)) & 0x1FFF;
	r->coeffs[2038] = ((a[3311] >> 6) | ((uint32_t)a[3312] << 2) | ((uint32_t)a[3313] << 10)) & 0x1FFF;

	r->coeffs[2032] = (1 << 12) - r->coeffs[2032];
	r->coeffs[2033] = (1 << 12) - r->coeffs[2033];
	r->coeffs[2034] = (1 << 12) - r->coeffs[2034];
	r->coeffs[2035] = (1 << 12) - r->coeffs[2035];
	r->coeffs[2036] = (1 << 12) - r->coeffs[2036];
	r->coeffs[2037] = (1 << 12) - r->coeffs[2037];
	r->coeffs[2038] = (1 << 12) - r->coeffs[2038];
#endif
	DBENCH_STOP(*tpack);
}

void polyz_pack(uint8_t *r, const poly *a) {
	DBENCH_START();

	unsigned int i;
	uint32_t t[2];
	for (i = 0; i < (N >> 1); ++i)
	{
		t[0] = GAMMA1 - a->coeffs[2 * i + 0];
		t[1] = GAMMA1 - a->coeffs[2 * i + 1];

		r[5 * i + 0] = (t[0] >> 0);
		r[5 * i + 1] = (t[0] >> 8);
		r[5 * i + 2] = (t[0] >> 16) | (t[1] << 4);
		r[5 * i + 3] = (t[1] >> 4);
		r[5 * i + 4] = (t[1] >> 12);
	}
	t[0] = GAMMA1 - a->coeffs[N - 1];

	r[POLYZ_PACKEDBYTES - 3] = t[0] >> 0;
	r[POLYZ_PACKEDBYTES - 2] = t[0] >> 8;
	r[POLYZ_PACKEDBYTES - 1] = t[0] >> 16;

	DBENCH_STOP(*tpack);
}

void polyz_unpack(poly *r, const uint8_t *a) {
	unsigned int i;
	DBENCH_START();

	for (i = 0; i < (N >> 1); ++i)
	{
		r->coeffs[2 * i + 0] = ((a[5 * i + 0] >> 0) | ((uint32_t)a[5 * i + 1] << 8) | ((uint32_t)a[5 * i + 2] << 16)) & 0xFFFFF;
		r->coeffs[2 * i + 1] = ((a[5 * i + 2] >> 4) | ((uint32_t)a[5 * i + 3] << 4) | ((uint32_t)a[5 * i + 4] << 12)) & 0xFFFFF;

		r->coeffs[2 * i + 0] = GAMMA1 - r->coeffs[2 * i + 0];
		r->coeffs[2 * i + 1] = GAMMA1 - r->coeffs[2 * i + 1];
	}
	r->coeffs[N - 1] = ((a[POLYZ_PACKEDBYTES - 3] >> 0) | ((uint32_t)a[POLYZ_PACKEDBYTES - 2] << 8) | ((uint32_t)a[POLYZ_PACKEDBYTES - 1] << 16)) & 0xFFFFF;

	r->coeffs[N - 1] = GAMMA1 - r->coeffs[N - 1];

	DBENCH_STOP(*tpack);
}

void polyw1_pack(uint8_t *r, const poly *a) {
	unsigned int i;
	DBENCH_START();

#if N==1201
	for (i = 0; i < 300; ++i) {
		r[3 * i + 0] = (a->coeffs[4 * i + 0] >> 0) | (a->coeffs[4 * i + 1] << 6);
		r[3 * i + 1] = (a->coeffs[4 * i + 1] >> 2) | (a->coeffs[4 * i + 2] << 4);
		r[3 * i + 2] = (a->coeffs[4 * i + 2] >> 4) | (a->coeffs[4 * i + 3] << 2);
	}

	r[900] = (a->coeffs[1200]);
#elif N==1607
	for (i = 0; i < 200; ++i) {
		r[5 * i + 0] = (a->coeffs[8 * i + 0] >> 0) | (a->coeffs[8 * i + 1] << 5);
		r[5 * i + 1] = (a->coeffs[8 * i + 1] >> 3) | (a->coeffs[8 * i + 2] << 2) | (a->coeffs[8 * i + 3] << 7);
		r[5 * i + 2] = (a->coeffs[8 * i + 3] >> 1) | (a->coeffs[8 * i + 4] << 4);
		r[5 * i + 3] = (a->coeffs[8 * i + 4] >> 4) | (a->coeffs[8 * i + 5] << 1) | (a->coeffs[8 * i + 6] << 6);
		r[5 * i + 4] = (a->coeffs[8 * i + 6] >> 2) | (a->coeffs[8 * i + 7] << 3);
	}

	r[1000] = (a->coeffs[1600] >> 0) | (a->coeffs[1601] << 5);
	r[1001] = (a->coeffs[1601] >> 3) | (a->coeffs[1602] << 2) | (a->coeffs[1603] << 7);
	r[1002] = (a->coeffs[1603] >> 1) | (a->coeffs[1604] << 4);
	r[1003] = (a->coeffs[1604] >> 4) | (a->coeffs[1605] << 1) | (a->coeffs[1606] << 6);
	r[1004] = (a->coeffs[1606] >> 2);
#elif N==2039
	for (i = 0; i < 254; ++i) {
		r[5 * i + 0] = (a->coeffs[8 * i + 0] >> 0) | (a->coeffs[8 * i + 1] << 5);
		r[5 * i + 1] = (a->coeffs[8 * i + 1] >> 3) | (a->coeffs[8 * i + 2] << 2) | (a->coeffs[8 * i + 3] << 7);
		r[5 * i + 2] = (a->coeffs[8 * i + 3] >> 1) | (a->coeffs[8 * i + 4] << 4);
		r[5 * i + 3] = (a->coeffs[8 * i + 4] >> 4) | (a->coeffs[8 * i + 5] << 1) | (a->coeffs[8 * i + 6] << 6);
		r[5 * i + 4] = (a->coeffs[8 * i + 6] >> 2) | (a->coeffs[8 * i + 7] << 3);
	}

	r[1270] = (a->coeffs[2032] >> 0) | (a->coeffs[2033] << 5);
	r[1271] = (a->coeffs[2033] >> 3) | (a->coeffs[2034] << 2) | (a->coeffs[2035] << 7);
	r[1272] = (a->coeffs[2035] >> 1) | (a->coeffs[2036] << 4);
	r[1273] = (a->coeffs[2036] >> 4) | (a->coeffs[2037] << 1) | (a->coeffs[2038] << 6);
	r[1274] = (a->coeffs[2038] >> 2);
#endif
	DBENCH_STOP(*tpack);
}

void pack_pk(uint8_t pk[CRYPTO_PUBLICKEYBYTES],
	const uint8_t zeta[SEEDBYTES],
	const poly *t1)
{
	unsigned int i;

	for (i = 0; i < SEEDBYTES; ++i)
		pk[i] = zeta[i];
	pk += SEEDBYTES;

	polyt1_pack(pk, t1);
}

void unpack_pk(uint8_t zeta[SEEDBYTES],
	poly *t1,
	const uint8_t pk[CRYPTO_PUBLICKEYBYTES])
{
	unsigned int i;

	for (i = 0; i < SEEDBYTES; ++i)
		zeta[i] = pk[i];
	pk += SEEDBYTES;

	polyt1_unpack(t1, pk);
}

void pack_sk(uint8_t sk[CRYPTO_SECRETKEYBYTES],
	const uint8_t zeta[SEEDBYTES],
	const uint8_t tr[SEEDBYTES],
	const uint8_t key[SEEDBYTES],
	const poly *t0,
	const poly *s1,
	const poly *s2)
{
	unsigned int i;

	for (i = 0; i < SEEDBYTES; ++i)
		sk[i] = zeta[i];
	sk += SEEDBYTES;

	for (i = 0; i < SEEDBYTES; ++i)
		sk[i] = key[i];
	sk += SEEDBYTES;

	for (i = 0; i < SEEDBYTES; ++i)
		sk[i] = tr[i];
	sk += SEEDBYTES;

	polyeta_pack(sk, s1);
	sk += POLYETA_PACKEDBYTES;

	polyeta_pack(sk, s2);
	sk += POLYETA_PACKEDBYTES;

	polyt0_pack(sk, t0);
}

void unpack_sk(uint8_t zeta[SEEDBYTES],
	uint8_t tr[SEEDBYTES],
	uint8_t key[SEEDBYTES],
	poly *t0,
	poly *s1,
	poly *s2,
	const uint8_t sk[CRYPTO_SECRETKEYBYTES])
{
	unsigned int i;

	for (i = 0; i < SEEDBYTES; ++i)
		zeta[i] = sk[i];
	sk += SEEDBYTES;

	for (i = 0; i < SEEDBYTES; ++i)
		key[i] = sk[i];
	sk += SEEDBYTES;

	for (i = 0; i < SEEDBYTES; ++i)
		tr[i] = sk[i];
	sk += SEEDBYTES;

	polyeta_unpack(s1, sk);
	sk += POLYETA_PACKEDBYTES;

	polyeta_unpack(s2, sk);
	sk += POLYETA_PACKEDBYTES;

	polyt0_unpack(t0, sk);
}

void pack_sig(uint8_t sig[CRYPTO_BYTES],
	const uint8_t c[SEEDBYTES],
	const poly *z,
	const poly *h)
{
	unsigned int i;

	for (i = 0; i < SEEDBYTES; ++i)
		sig[i] = c[i];
	sig += SEEDBYTES;

	polyz_pack(sig, z);
	sig += POLYZ_PACKEDBYTES;

	/* Encode h */
	for (i = 0; i < POLYH_PACKEDBYTES - 1; i++)
	{
		sig[i] = (h->coeffs[8 * i + 0] << 0);
		sig[i] |= (h->coeffs[8 * i + 1] << 1);
		sig[i] |= (h->coeffs[8 * i + 2] << 2);
		sig[i] |= (h->coeffs[8 * i + 3] << 3);
		sig[i] |= (h->coeffs[8 * i + 4] << 4);
		sig[i] |= (h->coeffs[8 * i + 5] << 5);
		sig[i] |= (h->coeffs[8 * i + 6] << 6);
		sig[i] |= (h->coeffs[8 * i + 7] << 7);
	}
#if (N==1201)
	sig[POLYH_PACKEDBYTES - 1] = h->coeffs[8 * POLYH_PACKEDBYTES - 8];
#else
	sig[POLYH_PACKEDBYTES - 1] = (h->coeffs[8 * POLYH_PACKEDBYTES - 8] << 0);
	sig[POLYH_PACKEDBYTES - 1] |= (h->coeffs[8 * POLYH_PACKEDBYTES - 7] << 1);
	sig[POLYH_PACKEDBYTES - 1] |= (h->coeffs[8 * POLYH_PACKEDBYTES - 6] << 2);
	sig[POLYH_PACKEDBYTES - 1] |= (h->coeffs[8 * POLYH_PACKEDBYTES - 5] << 3);
	sig[POLYH_PACKEDBYTES - 1] |= (h->coeffs[8 * POLYH_PACKEDBYTES - 4] << 4);
	sig[POLYH_PACKEDBYTES - 1] |= (h->coeffs[8 * POLYH_PACKEDBYTES - 3] << 5);
	sig[POLYH_PACKEDBYTES - 1] |= (h->coeffs[8 * POLYH_PACKEDBYTES - 2] << 6);
#endif
}

int unpack_sig(uint8_t c[SEEDBYTES],
	poly *z,
	poly *h,
	const uint8_t sig[CRYPTO_BYTES])
{
	unsigned int i;

	for (i = 0; i < SEEDBYTES; ++i)
		c[i] = sig[i];
	sig += SEEDBYTES;

	polyz_unpack(z, sig);
	sig += POLYZ_PACKEDBYTES;

	/* Decode h */
	for (i = 0; i < POLYH_PACKEDBYTES - 1; i++)
	{
		h->coeffs[8 * i + 0] = (sig[i] >> 0) & 0x1;
		h->coeffs[8 * i + 1] = (sig[i] >> 1) & 0x1;
		h->coeffs[8 * i + 2] = (sig[i] >> 2) & 0x1;
		h->coeffs[8 * i + 3] = (sig[i] >> 3) & 0x1;
		h->coeffs[8 * i + 4] = (sig[i] >> 4) & 0x1;
		h->coeffs[8 * i + 5] = (sig[i] >> 5) & 0x1;
		h->coeffs[8 * i + 6] = (sig[i] >> 6) & 0x1;
		h->coeffs[8 * i + 7] = (sig[i] >> 7) & 0x1;
	}
#if (N==1201)
	h->coeffs[8 * POLYH_PACKEDBYTES - 8] = sig[POLYH_PACKEDBYTES - 1] & 0x1;
#else
	h->coeffs[8 * POLYH_PACKEDBYTES - 8] = (sig[POLYH_PACKEDBYTES - 1] >> 0) & 0x1;
	h->coeffs[8 * POLYH_PACKEDBYTES - 7] = (sig[POLYH_PACKEDBYTES - 1] >> 1) & 0x1;
	h->coeffs[8 * POLYH_PACKEDBYTES - 6] = (sig[POLYH_PACKEDBYTES - 1] >> 2) & 0x1;
	h->coeffs[8 * POLYH_PACKEDBYTES - 5] = (sig[POLYH_PACKEDBYTES - 1] >> 3) & 0x1;
	h->coeffs[8 * POLYH_PACKEDBYTES - 4] = (sig[POLYH_PACKEDBYTES - 1] >> 4) & 0x1;
	h->coeffs[8 * POLYH_PACKEDBYTES - 3] = (sig[POLYH_PACKEDBYTES - 1] >> 5) & 0x1;
	h->coeffs[8 * POLYH_PACKEDBYTES - 2] = (sig[POLYH_PACKEDBYTES - 1] >> 6) & 0x1;
#endif

	return 0;
}
