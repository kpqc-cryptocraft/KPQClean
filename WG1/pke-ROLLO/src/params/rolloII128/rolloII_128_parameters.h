/** 
 * \file parameters.h
 * \brief Parameters of the ROLLO scheme
 */

#ifndef ROLLOII_128_PARAMETER_H
#define ROLLOII_128_PARAMETER_H

#define ROLLOII_128_PARAM_M 83 /**< Parameter m of the scheme (finite field GF(2^m)) */
#define ROLLOII_128_PARAM_N 189 /**< Parameter n of the scheme (code length) */
#define ROLLOII_128_PARAM_D 8 /**< Parameter d of the scheme (weight of vectors) */
#define ROLLOII_128_PARAM_R 7 /**< Parameter r of the scheme (weight of vectors) */
#define ROLLOII_128_PARAM_DFR 128 /**< Decryption Failure Rate (2^-30) */
#define ROLLOII_128_PARAM_SECURITY 128 /**< Expected security level */

#define ROLLOII_128_PUBLIC_KEY_BYTES 1961
#define ROLLOII_128_SECRET_KEY_BYTES 2001
#define ROLLOII_128_CIPHERTEXT_BYTES 2089
#define ROLLOII_128_SHARED_SECRET_BYTES 64

#define ROLLOII_128_RBC_VEC_R_BYTES 73 //Number of bytes to store R elements of GF2^m
#define ROLLOII_128_RBC_VEC_N_BYTES 1961 //Number of bytes to store N elements of GF2^m

#define SHA512_BYTES 64 /**< Size of SHA512 output */

#define SEEDEXPANDER_SEED_BYTES 40 /**< Seed size of the NIST seed expander */
#define SEEDEXPANDER_MAX_LENGTH 4294967295 /**< Max length of the NIST seed expander */

#endif

