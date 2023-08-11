/** 
 * \file parameters.h
 * \brief Parameters of the BIIX(b=2) scheme
 */

#ifndef BIIX_128_PARAMETER_H
#define BIIX_128_PARAMETER_H

#define BIIX_128_PARAM_M 67 /**< Parameter m of the scheme (finite field GF(2^m)) */
#define BIIX_128_PARAM_N 37 /**< Parameter n of the scheme (code length) */
#define BIIX_128_PARAM_D 2 /**< Parameter d of the scheme (weight of vectors) */
#define BIIX_128_PARAM_R 3 /**< Parameter r of the scheme (weight of vectors) */
#define BIIX_128_PARAM_DFR 31 /**< Decryption Failure Rate (2^-30) */
#define BIIX_128_PARAM_SECURITY 128 /**< Expected security level */

#define BIIX_128_PUBLIC_KEY_BYTES 1280
#define BIIX_128_SECRET_KEY_BYTES 120
#define BIIX_128_CIPHERTEXT_BYTES 640
#define BIIX_128_SHARED_SECRET_BYTES 64

#define BIIX_128_RBC_VEC_R_BYTES 67 //Number of bytes to store R elements of GF2^m
#define BIIX_128_RBC_VEC_N_BYTES 1280 //Number of bytes to store N elements of GF2^m

#define SHA512_BYTES 64 /**< Size of SHA512 output */

#define SEEDEXPANDER_SEED_BYTES 40 /**< Seed size of the NIST seed expander */
#define SEEDEXPANDER_MAX_LENGTH 4294967295 /**< Max length of the NIST seed expander */

#endif

