/** 
 * \file parameters.h
 * \brief Parameters of the ROLLO scheme
 */

#ifndef BIIX_256_PARAMETER_H
#define BIIX_256_PARAMETER_H

#define BIIX_256_PARAM_M 97 /**< Parameter m of the scheme (finite field GF(2^m)) */
#define BIIX_256_PARAM_N 53 /**< Parameter n of the scheme (code length) */
#define BIIX_256_PARAM_ON 103 /**< Parameter outer n of the scheme (code length) */
#define BIIX_256_PARAM_PI_DEG 53 /**<Parameter of polynomial degree on PI */ 
#define BIIX_256_PARAM_PNA_DEG 4 /**<Parameter of polynomial degree on PNA */
#define BIIX_256_PARAM_PNC_DEG 46 /**<Parameter of polynomial degree on PNC */

#define BIIX_256_PARAM_D 2 /**< Parameter d of the scheme (weight of vectors) */
#define BIIX_256_PARAM_R 12 /**< Parameter r of the scheme (weight of vectors) */
#define BIIX_256_PARAM_DFR 32 /**< Decryption Failure Rate (2^-30) */
#define BIIX_256_PARAM_SECURITY 256 /**< Expected security level */

#define BIIX_256_PUBLIC_KEY_BYTES 3900
#define BIIX_256_SECRET_KEY_BYTES 120
#define BIIX_256_CIPHERTEXT_BYTES 1300
#define BIIX_256_SHARED_SECRET_BYTES 64

#define BIIX_256_RBC_VEC_R_BYTES 97 //Number of bytes to store R elements of GF2^m
#define BIIX_256_RBC_VEC_N_BYTES 3900 //Number of bytes to store N elements of GF2^m

#define SHA512_BYTES 64 /**< Size of SHA512 output */

#define SEEDEXPANDER_SEED_BYTES 40 /**< Seed size of the NIST seed expander */
#define SEEDEXPANDER_MAX_LENGTH 4294967295 /**< Max length of the NIST seed expander */

#endif

