/** 
 * \file parameters.h
 * \brief Parameters of the ROLLO scheme
 */

#ifndef ROLLOII_192_PARAMETER_H
#define ROLLOII_192_PARAMETER_H

#define ROLLOII_192_PARAM_M 97 /**< Parameter m of the scheme (finite field GF(2^m)) */
#define ROLLOII_192_PARAM_N 193 /**< Parameter n of the scheme (code length) */
#define ROLLOII_192_PARAM_D 8 /**< Parameter d of the scheme (weight of vectors) */
#define ROLLOII_192_PARAM_R 8 /**< Parameter r of the scheme (weight of vectors) */
#define ROLLOII_192_PARAM_DFR 128 /**< Decryption Failure Rate (2^-30) */
#define ROLLOII_192_PARAM_SECURITY 192 /**< Expected security level */

#define ROLLOII_192_PUBLIC_KEY_BYTES 2341
#define ROLLOII_192_SECRET_KEY_BYTES 2381
#define ROLLOII_192_CIPHERTEXT_BYTES 2469
#define ROLLOII_192_SHARED_SECRET_BYTES 64

#define ROLLOII_192_RBC_VEC_R_BYTES 97 //Number of bytes to store R elements of GF2^m
#define ROLLOII_192_RBC_VEC_N_BYTES 2341 //Number of bytes to store N elements of GF2^m

#define SHA512_BYTES 64 /**< Size of SHA512 output */

#define SEEDEXPANDER_SEED_BYTES 40 /**< Seed size of the NIST seed expander */
#define SEEDEXPANDER_MAX_LENGTH 4294967295 /**< Max length of the NIST seed expander */

#endif

