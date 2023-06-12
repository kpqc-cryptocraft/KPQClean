/********************************************************************************************
* CGLHash Library
*
* Abstract: supersingular isogeny parameters and generation of functions for P610
*********************************************************************************************/  

#include "api.h" 
#include "P610_internal.h"
#include "../internal.h"


// Encoding of field elements, elements over Z_order, elements over GF(p^2) and elliptic curve points:
// --------------------------------------------------------------------------------------------------
// Elements over GF(p) and Z_order are encoded with the least significant octet (and digit) located at the leftmost position (i.e., little endian format). 
// Elements (a+b*i) over GF(p^2), where a and b are defined over GF(p), are encoded as {a, b}, with a in the least significant position.
// Elliptic curve points P = (x,y) are encoded as {x, y}, with x in the least significant position. 
// Internally, the number of digits used to represent all these elements is obtained by approximating the number of bits to the immediately greater multiple of 32.
// For example, a 610-bit field element is represented with Ceil(610 / 64) = 10 64-bit digits or Ceil(610 / 32) = 20 32-bit digits.

//
//  Base curve: Montgomery curve By^2 = Cx^3 + Ax^2 + Cx defined over GF(p610^2), where A=0, B=1, C=1 and p610 = 2^607-1
//
         
const uint64_t p610[NWORDS64_FIELD]              = { 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 
                                                     0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0x7FFFFFFF };
const uint64_t p610x2[NWORDS64_FIELD]            = { 0xFFFFFFFFFFFFFFFE, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 
                                                     0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFF}; 
const uint64_t p610x4[NWORDS64_FIELD]            = { 0xFFFFFFFFFFFFFFFC, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 
                                                     0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0x1FFFFFFFF}; 
const uint64_t p610p1[NWORDS64_FIELD]            = { 0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000,
                                                     0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x80000000 };   
const uint64_t p610x16p[2*NWORDS64_FIELD]        = { 0x0000000000000010, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 
                                                     0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0xfffffff000000000, 
                                                     0xffffffffffffffff, 0xffffffffffffffff, 0xffffffffffffffff, 0xffffffffffffffff, 0xffffffffffffffff, 
                                                     0xffffffffffffffff, 0xffffffffffffffff, 0xffffffffffffffff, 0xffffffffffffffff, 0x3}; 
// Order of subgroup
const uint64_t Hash_order[NWORDS64_ORDER]       = { 0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0002000000000000 }; 


// Hash generator values {XPA0 + XPA1*i, XQA0 + xQA1*i, XRA0 + XRA1*i} in GF(p610^2), expressed in Montgomery representation
const uint64_t H_gen[6 * NWORDS64_FIELD]         = { 0xA00000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000,
                                                     0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000,   // XPA0
                                                     0x200000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000,
                                                     0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000,   // XPA1
                                                     0x1E00000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000,
                                                     0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000,   // XQA0
                                                     0x200000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000,
                                                     0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000,   // XQA1
                                                     0x39923A94847008D5, 0x7CBB3235B61AC001, 0x2D508C479BB44B39, 0x9009E573E1C0BBD9, 0xB04DC8E63B2F4C6D,
                                                     0x855A329C78B80273, 0xE0A080295B846569, 0xB3F6E7E861908089, 0x4139A0235EC5E0AD, 0x3AB43B59,   // XRA0
                                                     0x439C34383FA83840, 0xECF402DA848D1819, 0x7A2655BA61B5E398, 0xD2D1071485817404, 0xC74A444512A513A7,
                                                     0x017BEC8EE054F16A, 0x84EF0595D480BF56, 0xE54F14744813FA72, 0x629E3482D817F435, 0x3A40EDB }; // XRA1


// Montgomery constant Montgomery_R2 = (2^640)^2 mod p610
const uint64_t Montgomery_R2[NWORDS64_FIELD]     = { 0x0000000000000000, 0x4, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 
                                                     0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000};                                                    
// Value one in Montgomery representation 
const uint64_t Montgomery_one[NWORDS64_FIELD]    = { 0x200000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000,
                                                     0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000 };

const uint64_t Montgomery_inv2[NWORDS64_FIELD]    = { 0x100000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000,
                                                     0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000 };


// Fixed parameters for isogeny tree computation

const unsigned int strat_Hash[303] ={
0, 1, 1, 2, 2, 2, 3, 4, 4, 4, 4, 5, 5, 6, 7, 8, 8, 9, 9, 9, 9,
9, 9, 9, 12, 11, 12, 12, 13, 14, 15, 16, 16, 16, 16, 16, 16, 17, 17, 18, 18, 17,
21, 17, 18, 21, 20, 21, 21, 21, 21, 21, 22, 25, 25, 25, 26, 27, 28, 28, 29, 30,
31, 32, 32, 32, 32, 32, 32, 32, 33, 33, 33, 35, 36, 36, 33, 36, 35, 36, 36, 35,
36, 36, 37, 38, 38, 39, 40, 41, 42, 38, 39, 40, 41, 42, 40, 46, 42, 43, 46, 46,
46, 46, 48, 48, 48, 48, 49, 49, 48, 53, 54, 51, 52, 53, 54, 55, 56, 57, 58, 59,
59, 60, 62, 62, 63, 64, 64, 64, 64, 64, 64, 64, 64, 65, 65, 65, 65, 65, 66, 67,
65, 66, 67, 66, 69, 70, 66, 67, 66, 69, 70, 69, 70, 70, 71, 72, 71, 72, 72, 74,
74, 75, 72, 72, 74, 74, 75, 72, 72, 74, 75, 75, 72, 72, 74, 75, 75, 77, 77, 79,
80, 80, 82, 82, 84, 83, 83, 84, 85, 86, 87, 86, 87, 90, 87, 86, 87, 87, 88, 86,
88, 88, 87, 88, 86, 88, 88, 89, 90, 91, 93, 98, 94, 103, 96, 98, 98, 99, 100,
101, 103, 103, 104, 105, 106, 106, 106, 109, 108, 106, 106, 109, 109, 113, 111,
113, 113, 113, 118, 115, 120, 119, 119, 119, 120, 121, 121, 122, 123, 125, 125,
126, 127, 128, 128, 128, 128, 128, 128, 128, 128, 129, 129, 129, 129, 132, 129,
129, 133, 129, 129, 133, 129, 129, 129, 130, 133, 133, 129, 129, 130, 133, 133,
133, 133, 135, 133, 135, 134, 135, 136, 136, 137, 136, 139, 136, 138, 137, 139,
138, 140 };

/*
const unsigned int strat_Hash[153-1] = { 
67, 37, 21, 12, 7, 4, 2, 1, 1, 2, 1, 1, 3, 2, 1, 1, 1, 1, 5, 3, 2, 1, 1, 1, 1, 
2, 1, 1, 1, 9, 5, 3, 2, 1, 1, 1, 1, 2, 1, 1, 1, 4, 2, 1, 1, 1, 2, 1, 1, 16, 9, 
5, 3, 2, 1, 1, 1, 1, 2, 1, 1, 1, 4, 2, 1, 1, 1, 2, 1, 1, 8, 4, 2, 1, 1, 2, 1, 
1, 4, 2, 1, 1, 2, 1, 1, 33, 16, 8, 5, 2, 1, 1, 1, 2, 1, 1, 1, 4, 2, 1, 1, 2, 1, 
1, 8, 4, 2, 1, 1, 2, 1, 1, 4, 2, 1, 1, 2, 1, 1, 16, 8, 4, 2, 1, 1, 1, 2, 1, 1, 
4, 2, 1, 1, 2, 1, 1, 8, 4, 2, 1, 1, 2, 1, 1, 4, 2, 1, 1, 2, 1, 1 };
*/


// Setting up macro defines and including GF(p), GF(p^2), curve, isogeny and kex functions
#define fpcopy                        fpcopy610
#define fpzero                        fpzero610
#define fpadd                         fpadd610
#define fpsub                         fpsub610
#define fpneg                         fpneg610
#define fpdiv2                        fpdiv2_610
#define fpcorrection                  fpcorrection610
#define fpmul_mont                    fpmul610_mont
#define fpsqr_mont                    fpsqr610_mont
#define fpinv_mont                    fpinv610_mont
#define fpinv_chain_mont              fpinv610_chain_mont
#define fpinv_mont_bingcd             fpinv610_mont_bingcd
#define fp2copy                       fp2copy610
#define fp2zero                       fp2zero610
#define fp2add                        fp2add610
#define fp2sub                        fp2sub610
#define mp_sub_p2                     mp_sub610_p2
#define mp_sub_p4                     mp_sub610_p4
#define sub_p4                        mp_sub_p4
#define fp2neg                        fp2neg610
#define fp2div2                       fp2div2_610
#define fp2correction                 fp2correction610
#define fp2mul_mont                   fp2mul610_mont
#define fp2sqr_mont                   fp2sqr610_mont
#define fp2inv_mont                   fp2inv610_mont
#define fp2inv_mont_bingcd            fp2inv610_mont_bingcd
#define fpequal_non_constant_time     fpequal610_non_constant_time
#define mp_add_asm                    mp_add610_asm
#define mp_subaddx2_asm               mp_subadd610x2_asm
#define mp_dblsubx2_asm               mp_dblsub610x2_asm


#include "../fpx.c"
#include "../ec_isogeny.c"
#include "../cglhash.c"
