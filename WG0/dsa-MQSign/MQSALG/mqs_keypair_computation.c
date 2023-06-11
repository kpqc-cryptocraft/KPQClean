#include "mqs_keypair.h"
#include "mqs_keypair_computation.h"

#include "blas_comm.h"
#include "blas.h"
#include "mqs_blas.h"

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "utils_malloc.h"

void extpk_to_pk(pk_mqs* pk, const ext_pk* extpk)
{
	const unsigned char* idx = extpk->quad1;
	for (unsigned i = 0; i < _V; i++) {
		for (unsigned j = i; j < _V; j++) {
			unsigned pub_idx = idx_of_trimat(i, j, _PUB_N);
			memcpy(&pk->pk[_PUB_M_BYTE * pub_idx], idx, _O);
			idx += _O;
		}
	}
	idx = extpk->quad2;
	for (unsigned i = 0; i < _V; i++) {
		for (unsigned j = _V; j < _V + _O; j++) {
			unsigned pub_idx = idx_of_trimat(i, j, _PUB_N);
			memcpy(&pk->pk[_PUB_M_BYTE * pub_idx], idx, _O);
			idx += _O;
		}
	}
	idx = extpk->quad3;
	for (unsigned i = _V; i < _V + _O; i++) {
		for (unsigned j = i; j < _V + _O; j++) {
			unsigned pub_idx = idx_of_trimat(i, j, _PUB_N);
			memcpy(&pk->pk[_PUB_M_BYTE * pub_idx], idx, _O);
			idx += _O;
		}
	}

	memcpy(pk->pk + (_PUB_M)* N_TRIANGLE_TERMS(_PUB_N), extpk->linear, _O * _PUB_N);
	memcpy(pk->pk + (_PUB_M)* N_TRIANGLE_TERMS(_PUB_N) + _O * _PUB_N, extpk->constants, _O);
}

// Choosing implementations depends on the macros: _BLAS_SSE_ and _BLAS_AVX2_
#if defined(_BLAS_AVX2_)
#include "mqs_keypair_computation_simd.h"
#define cal_ext_pk_mqss_impl			cal_ext_pk_mqss_simd

#else

#define cal_ext_pk_mqss_impl			cal_ext_pk_mqss_ref

// implement ref version 


void cal_ext_pk_mqss_ref(ext_pk* Qs, const sk_mqss* Fs, const sk_mqss* Ts)
{
#if defined(_MALLOC_)
	unsigned char* tmp = (unsigned char*)malloc(_O);
	unsigned char* tmp2 = (unsigned char*)malloc(_O * _V * _O);
	unsigned char* tmp3 = (unsigned char*)malloc(_O * _V * _O);
	unsigned char* tempQ = (unsigned char*)malloc(_O * _O * _O);
	unsigned char* tempFq1 = (unsigned char*)malloc(_O * _V);
	unsigned char* tempFq2 = (unsigned char*)malloc(_O * _V);
	unsigned char* tempFqt = (unsigned char*)malloc(_O * _PUB_N);
	unsigned char* tempFqt_tr = (unsigned char*)malloc(_O * _PUB_N);
	unsigned char* Fq2 = (unsigned char*)malloc(_O * _V * _O);
#else
	unsigned char tmp[_O];
	unsigned char tmp2[_O * _V * _O];
	unsigned char tmp3[_O * _V * _O];
	unsigned char tempQ[_O * _O * _O];
	unsigned char tempFq1[_O * _V];
	unsigned char tempFq2[_O * _V];
	unsigned char tempFqt[_O * _PUB_N];
	unsigned char tempFqt_tr[_O * _PUB_N];
	unsigned char Fq2[_O * _V * _O];
#endif

	memset(Qs->quad1, 0, _O * N_TRIANGLE_TERMS(_V));
	expand_sparmat_vv(Qs->quad1, Fs->Fq1, _O); // F1 

	expand_sparmat_vo(Fq2, Fs->Fq2, _O);

	// Quadratic part
	memcpy(Qs->quad2, Fq2, _O * _V * _O);

	batch_trimat_madd(tmp2, Qs->quad1, Ts->mat_t, _V, _V, _O, _O); // F1*Ts->mat_t
	gf256v_add(Qs->quad2, tmp2, _O * _V * _O); // F1*Ts->mat_t + F2
	memcpy(tmp3, Qs->quad2, _O * _V * _O); // be used to compute Linear part from Ts->vec_t

	memset(Qs->quad3, 0, _O * N_TRIANGLE_TERMS(_O));

	batch_matTr_madd(tempQ, Ts->mat_t, _V, _V, _O, Qs->quad2, _O, _O); // Ts->mat_t_tr*(F1*Ts->mat_t + F2)
	UpperTrianglize(Qs->quad3, tempQ, _O, _O);

	batch_trimatTr_madd(tmp2, Qs->quad1, Ts->mat_t, _V, _V, _O, _O); // F1_tr * Ts->mat_t
	gf256v_add(Qs->quad2, tmp2, _O * _V * _O);

	// Linear part
	memcpy(Qs->linear, Fs->Fl, _O * _PUB_N);
	for (unsigned i = 0; i < _O; i++) {
		for (unsigned j = 0; j < _V; j++) {
			gf256v_madd(Qs->linear + _O * _V + i * _O, Fs->Fl + j * _O, Ts->mat_t[i * _V + j] , _O);
		}
		// OxV mat * VxO mat  .. V-len vec with O iter. 
	}

	// Constant part
	memcpy(Qs->constants, Fs->Fc, _O); // Fc

	////////

	for (unsigned i = 0; i < _PUB_N; i++) {
		gf256v_madd(Qs->constants, Fs->Fl + i * _O, Ts->vec_t[i] , _O); // Fc + Fl*t
	}

	// Ts->vec_t = (tv | to)
	// Compute (Ts->vec_t_tr)*Fq*Ts->vec_t
	memset(tempFqt, 0, _O * _PUB_N);
	memset(tempFqt_tr, 0, _O * _PUB_N);
	memset(tempFq1, 0, _O * _V);
	memset(tempFq2, 0, _O * _V);
	memset(tmp, 0, _O);

	batch_trimat_madd(tempFq1, Qs->quad1, Ts->vec_t, _V, 1, 1, _O); // F1*tv
	batch_mat_madd(tempFq2, Fq2, _V, Ts->vec_t + _V , _O, 1, 1, _O); // F2*to

	gf256v_add(tempFqt, tempFq1, _O * _V);
	gf256v_add(tempFqt, tempFq2, _O * _V); // F1*tv + F2*to

	batch_matTr_madd(tmp, Ts->vec_t, _V, 1, 1, tempFqt, 1, _O); // tr_tv*(F1*tv + F2*to) 
	gf256v_add(Qs->constants, tmp, _O);

	// Compute linear parts derived from Ts->vec_t 
	batch_matTr_madd(tempFqt_tr, Ts->mat_t, _V, _V, _O, tempFqt, 1, _O); // tr_Ts->mat_t*(F1*tv + F2*to) 
	memcpy(tempFqt + _O * _V, tempFqt_tr, _O * _O); // tr_Ts->mat_t*(Fq*Ts->vec_t)

	gf256v_add(Qs->linear, tempFqt, _O * _PUB_N);

	batch_trimatTr_madd(tempFqt, Qs->quad1, Ts->vec_t, _V, 1, 1, _O); // tr_F1*tv
	batch_matTr_madd(tempFqt_tr, Ts->vec_t, _V, 1, 1, tmp3, _O, _O); // tr_Ts->vec_t * (F1*Ts->mat_t + F2) 

	gf256v_add(Qs->linear, tempFqt, _O * _V);
	gf256v_add(Qs->linear + _O * _V, tempFqt_tr, _O * _O);

	memset(tempQ, 0, _O * _O * _O);
	memset(tempFqt, 0, _O * _PUB_N);
	memset(tempFqt_tr, 0, _O * _PUB_N);
	memset(tempFq1, 0, _O * _V);
	memset(tempFq2, 0, _O * _V);
	memset(tmp, 0, _O);


#if defined(_MALLOC_)
	free(tmp);
	free(tmp2);
	free(tmp3);
	free(tempQ);
	free(tempFq1);
	free(tempFq2);
	free(tempFqt);
	free(tempFqt_tr);
	free(Fq2);
#endif
}


#endif 


///////////////////////////////////////////////////////////////////////


void cal_ext_pk_mqss(ext_pk* Qs, const sk_mqss* Fs, const sk_mqss* Ts)
{
	cal_ext_pk_mqss_impl(Qs, Fs, Ts);
}