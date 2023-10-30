/**
 * \file rbc_qre.c
 * \brief Implementation of rbc_qre.h
 */

#include "rbc_elt.h"
#include "rbc_qre.h"
#include "rbc_poly.h"

static uint32_t RBC_QRE_DEG;
static int rbc_init_qre_modulus = 0;
static rbc_poly_sparse rbc_qre_modulus;
static rbc_poly rbc_qre_modulus_inv;

