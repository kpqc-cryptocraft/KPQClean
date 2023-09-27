#ifndef _MQS_KEYPAIR_H_
#define _MQS_KEYPAIR_H_

#include "mqs_config.h"

#define N_TRIANGLE_TERMS(n_var) (n_var*(n_var+1)/2)

#ifdef  __cplusplus
extern  "C" {
#endif

#pragma pack(push,1)

typedef
struct mqs_publickey {
	// add linear and constant terms 
	unsigned char pk[(_PUB_M)* N_TRIANGLE_TERMS(_PUB_N) + _PUB_M * _PUB_N + _PUB_M];
} pk_mqs;

typedef
struct mqss_secretkey {
	unsigned char sk_seed[LEN_SKSEED];

	unsigned char mat_t[_V * _O]; // part of Tl 
	unsigned char vec_t[_PUB_N];

	unsigned char Fq1[_O * _V]; // sparse v-v
	unsigned char Fq2[_O * _V]; // sparse o-v

	unsigned char Fl[_O * _PUB_N];

	unsigned char Fc[_O];
} sk_mqss;


/// length for secret key ( extra 1 for length of salt)

#define _SEC_KEY_LEN (sizeof(sk_mqss) + 1)

#pragma pack(pop)

int generate_keypair_mqss(pk_mqs* rpk, sk_mqss* sk, const unsigned char* sk_seed);


#ifdef  __cplusplus
}
#endif

#endif
