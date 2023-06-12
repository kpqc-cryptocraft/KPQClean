//
// Code for RM code Generation, Decoding.
//

#ifndef __RM_H
#define __RM_H

#include "common.h"

extern const uint16_t rm_dim[7][13]; 

matrix* rm_gen_mod(matrix* gen, uint16_t *part_perm1, uint16_t* part_perm2) ;

#endif
