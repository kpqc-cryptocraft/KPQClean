/** 
 * \file parameters.h
 * \brief Parameters of the BIIX(b=2) scheme
 */

#ifndef BIIX_128_PARAMETER_H
#define BIIX_128_PARAMETER_H

#define BIIX_128_PARAM_M 67 /**< Parameter m of the scheme (finite field GF(2^m)) */
#define BIIX_128_PARAM_N 37 /**< Parameter n of the scheme (code length) */
#define BIIX_128_PARAM_ON 61 /**< Parameter outer n of the scheme (code length) */
#define BIIX_128_PARAM_PI_DEG 37 /**<Parameter of polynomial degree on PI */ 
#define BIIX_128_PARAM_PNA_DEG 4 /**<Parameter of polynomial degree on PNA */
#define BIIX_128_PARAM_PNC_DEG 19 /**<Parameter of polynomial degree on PNC */

#define BIIX_128_PARAM_D 2 /**< Parameter d of the scheme (weight of vectors) */
#define BIIX_128_PARAM_R 7 /**< Parameter r of the scheme (weight of vectors) */
#define BIIX_128_PARAM_DFR 23 /**< Decryption Failure Rate (2^-30) */
#define BIIX_128_PARAM_SECURITY 128 /**< Expected security level */

#define BIIX_128_PUBLIC_KEY_BYTES 1800
#define BIIX_128_SECRET_KEY_BYTES 120
#define BIIX_128_CIPHERTEXT_BYTES 600
#define BIIX_128_SHARED_SECRET_BYTES 64

#define BIIX_128_RBC_VEC_R_BYTES 67 //Number of bytes to store R elements of GF2^m
#define BIIX_128_RBC_VEC_N_BYTES 1800 //Number of bytes to store N elements of GF2^m

#define SHA512_BYTES 64 /**< Size of SHA512 output */

#define SEEDEXPANDER_SEED_BYTES 40 /**< Seed size of the NIST seed expander */
#define SEEDEXPANDER_MAX_LENGTH 4294967295 /**< Max length of the NIST seed expander */

#endif

