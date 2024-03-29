/** 
 * \file parameters.h
 * \brief Parameters of the BIIX(b=2) scheme
 */

#ifndef BIIX_192_PARAMETER_H
#define BIIX_192_PARAMETER_H

#define BIIX_192_PARAM_M 79 /**< Parameter m of the scheme (finite field GF(2^m)) */
#define BIIX_192_PARAM_N 43 /**< Parameter n of the scheme (code length) */
#define BIIX_192_PARAM_ON 71 /**< Parameter outer n of the scheme (code length) */
#define BIIX_192_PARAM_PI_DEG 43 /**<Parameter of polynomial degree on PI */ 
#define BIIX_192_PARAM_PNA_DEG 4 /**<Parameter of polynomial degree on PNA */
#define BIIX_192_PARAM_PNC_DEG 24 /**<Parameter of polynomial degree on PNC */

#define BIIX_192_PARAM_D 2 /**< Parameter d of the scheme (weight of vectors) */
#define BIIX_192_PARAM_R 9 /**< Parameter r of the scheme (weight of vectors) */
#define BIIX_192_PARAM_DFR 25 /**< Decryption Failure Rate (2^-30) */
#define BIIX_192_PARAM_SECURITY 192 /**< Expected security level */

#define BIIX_192_PUBLIC_KEY_BYTES 2190
#define BIIX_192_SECRET_KEY_BYTES 120
#define BIIX_192_CIPHERTEXT_BYTES 730
#define BIIX_192_SHARED_SECRET_BYTES 64

#define BIIX_192_RBC_VEC_R_BYTES 79 //Number of bytes to store R elements of GF2^m
#define BIIX_192_RBC_VEC_N_BYTES 2190 //Number of bytes to store N elements of GF2^m

#define SHA512_BYTES 64 /**< Size of SHA512 output */

#define SEEDEXPANDER_SEED_BYTES 40 /**< Seed size of the NIST seed expander */
#define SEEDEXPANDER_MAX_LENGTH 4294967295 /**< Max length of the NIST seed expander */

#endif

