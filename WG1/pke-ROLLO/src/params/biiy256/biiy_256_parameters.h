/** 
 * \file parameters.h
 * \brief Parameters of the ROLLO scheme
 */

#ifndef BIIY_256_PARAMETER_H
#define BIIY_256_PARAMETER_H

#define BIIY_256_PARAM_M 97 /**< Parameter m of the scheme (finite field GF(2^m)) */
#define BIIY_256_PARAM_N 47 /**< Parameter n of the scheme (code length) */
#define BIIY_256_PARAM_D 2 /**< Parameter d of the scheme (weight of vectors) */
#define BIIY_256_PARAM_R 3 /**< Parameter r of the scheme (weight of vectors) */
#define BIIY_256_PARAM_DFR 41 /**< Decryption Failure Rate (2^-30) */
#define BIIY_256_PARAM_SECURITY 256 /**< Expected security level */

#define BIIY_256_PUBLIC_KEY_BYTES 5400
#define BIIY_256_SECRET_KEY_BYTES 3340
#define BIIY_256_CIPHERTEXT_BYTES 2700
#define BIIY_256_SHARED_SECRET_BYTES 64

#define BIIY_256_RBC_VEC_R_BYTES 97 //Number of bytes to store R elements of GF2^m
#define BIIY_256_RBC_VEC_N_BYTES 2700 //Number of bytes to store N elements of GF2^m

#define SHA512_BYTES 64 /**< Size of SHA512 output */

#define SEEDEXPANDER_SEED_BYTES 40 /**< Seed size of the NIST seed expander */
#define SEEDEXPANDER_MAX_LENGTH 4294967295 /**< Max length of the NIST seed expander */

#endif

