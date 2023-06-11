#ifndef PARAMS_H
#define PARAMS_H

#include "config.h"

#define SEEDBYTES 32
#define CRHBYTES 64

#define ROOT_OF_UNITY 1753

#if NIMS_MODE == 2
#define N 1021
#define P 1021
#define NR 1024
#define Q 8339581
#define D 11
#define ETA 2
#define TAU 25
#define BETA 100
#define GAMMA1 (1 << 17)
#define GAMMA2 ((Q-1)/90)
#define OMEGA 80

#elif NIMS_MODE == 3
#define N 1429
#define P 1429
#define NR 1440
#define Q 8376649
#define D 12
#define ETA 2
#define TAU 29
#define BETA 116
#define GAMMA1 (1 << 18)
#define GAMMA2 ((Q-1)/56)
#define OMEGA 80

#elif NIMS_MODE == 5
#define N 1913
#define P 1913
#define NR 1920
#define Q 8343469
#define D 12
#define ETA 2
#define TAU 32
#define BETA 128
#define GAMMA1 (1 << 19)
#define GAMMA2 ((Q-1)/42)
#define OMEGA 80

#endif

//packing bytes define

#define POLYT1_PACKEDBYTES      (((N*(23-D)+7)/8))
#define POLYT0_PACKEDBYTES      (((N*D)+7)/8)
#define POLYH_PACKEDBYTES    ((N+7)/8)
#if (N==1021)
#define POLYZ_PACKEDBYTES       (((N*18)+7)/8)
#elif (N==1429)
#define POLYZ_PACKEDBYTES       (((N*19)+7)/8)
#elif (N==1913)
#define POLYZ_PACKEDBYTES       (((N*20)+7)/8)
#endif
#if (N==1021)
#define POLYW1_PACKEDBYTES      (((N*6)+7)/8)
#else
#define POLYW1_PACKEDBYTES      (((N*5)+7)/8)
#endif
#define POLYETA_PACKEDBYTES     (((N*3)+7)/8)
#define CRYPTO_PUBLICKEYBYTES   (SEEDBYTES + POLYT1_PACKEDBYTES)
#define CRYPTO_SECRETKEYBYTES   (3*SEEDBYTES\
                                + 2*POLYETA_PACKEDBYTES\
                                + POLYT0_PACKEDBYTES)
#define CRYPTO_BYTES            (SEEDBYTES + POLYZ_PACKEDBYTES + POLYH_PACKEDBYTES)

#endif
