/** 
 * \file parameters.h
 * \brief Parameters of the ROLLO scheme
 */

#ifndef BIIY_192_PARAMETER_H
#define BIIY_192_PARAMETER_H

#define BIIY_192_PARAM_M 79 /**< Parameter m of the scheme (finite field GF(2^m)) */
#define BIIY_192_PARAM_N 103 /**< Parameter n of the scheme (code length) */
#define BIIY_192_PARAM_D 2 /**< Parameter d of the scheme (weight of vectors) */
#define BIIY_192_PARAM_R 2 /**< Parameter r of the scheme (weight of vectors) */
#define BIIY_192_PARAM_DFR 33 /**< Decryption Failure Rate (2^-30) */
#define BIIY_192_PARAM_SECURITY 192 /**< Expected security level */

#define BIIY_192_PUBLIC_KEY_BYTES 5400
#define BIIY_192_SECRET_KEY_BYTES 3340
#define BIIY_192_CIPHERTEXT_BYTES 2700
#define BIIY_192_SHARED_SECRET_BYTES 64

#define BIIY_192_RBC_VEC_R_BYTES 79 //Number of bytes to store R elements of GF2^m
#define BIIY_192_RBC_VEC_N_BYTES 2700 //Number of bytes to store N elements of GF2^m

#define SHA512_BYTES 64 /**< Size of SHA512 output */

#define SEEDEXPANDER_SEED_BYTES 40 /**< Seed size of the NIST seed expander */
#define SEEDEXPANDER_MAX_LENGTH 4294967295 /**< Max length of the NIST seed expander */

#endif

