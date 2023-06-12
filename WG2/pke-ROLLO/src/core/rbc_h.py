import os
import sys
from math import ceil
from rbc_core_utils import *

def preprocessing_rbc_h(arch, m):
    result = ""

    result += "#ifndef RBC_H\n"
    result += "#define RBC_H\n"
    result += "\n"
    result += "#include <inttypes.h>\n"
    result += "#include <stdint.h>\n"
    result += "#include <stdio.h>\n"
    result += "#include <stdlib.h>\n"
    result += "#include <string.h>\n"
    result += "#include <unistd.h>\n"
    if(arch == "c64" or arch == "avx"): 
        result += "#include <x86intrin.h>\n"

    result += "\n"
    result += "#define RBC_FIELD_Q 2\n"
    result += "#define RBC_FIELD_M " + str(m) + "\n"

    result += "\n"
    result += "#define RBC_ELT_SIZE "          + str(rbc_core_utils_get_size(arch, m)) + "\n"
    result += "#define RBC_ELT_DATA_SIZE "     + str(rbc_core_utils_get_data_size(arch, m)) + "\n"

    result += "\n"
    result += "#define RBC_ELT_UR_SIZE "       + str(rbc_core_utils_get_ur_size(arch, m)) + "\n"
    result += "#define RBC_ELT_UR_DATA_SIZE "  + str(rbc_core_utils_get_ur_data_size(arch, m)) + "\n"

    result += "\n"
    result += "#define RBC_ELT_UINT8_SIZE "    + str(rbc_core_utils_get_uint8_size(arch, m)) + "\n"
    result += "#define RBC_ELT_UR_UINT8_SIZE " + str(rbc_core_utils_get_ur_uint8_size(arch, m)) + "\n"
    result += "\n"

    if(arch == "c32"):
        result += "\n"
        result += "#define RBC_INTEGER_LENGTH 32\n"
        result += "\n"

    if(arch == "c64" or arch == "avx"):
        result += "\n"
        result += "#define RBC_INTEGER_LENGTH 64\n"
        result += "\n"

    if(arch == "c32"):
        result += "typedef int32_t rbc_elt_int;\n"
        result += "typedef uint32_t rbc_elt_uint;\n"
        result += "typedef uint32_t rbc_elt[RBC_ELT_SIZE];\n"
        result += "typedef uint32_t rbc_elt_ur[RBC_ELT_UR_SIZE];\n"
        result += "typedef uint32_t* rbc_elt_ptr;\n"

    if(arch == "c64" or arch == "avx"):
        result += "typedef int64_t rbc_elt_int;\n"
        result += "typedef uint64_t rbc_elt_uint;\n"
        result += "typedef uint64_t rbc_elt[RBC_ELT_SIZE];\n"
        result += "typedef uint64_t rbc_elt_ur[RBC_ELT_UR_SIZE];\n"
        result += "typedef uint64_t* rbc_elt_ptr;\n"

    result += "\n"
    result += "typedef rbc_elt* rbc_vec;\n"
    result += "typedef rbc_elt* rbc_vspace;\n"

    result += "\n"
    result += "typedef struct {\n"
    result += "  rbc_vec v;\n"
    result += "  int32_t max_degree;\n"
    result += "  // Do not use degree, it is deprecated and will be removed later\n"
    result += "  // Kept temporarily for compatibility with rbc_qpoly\n"
    result += "  int32_t degree;\n" 
    result += "} rbc_poly_struct;\n"

    result += "\n"
    result += "typedef struct {\n"
    result += "	 uint32_t coeffs_nb;\n"
    result += "	 uint32_t* coeffs;\n"
    result += "} rbc_poly_sparse_struct;\n"

    result += "\n"
    result += "typedef rbc_poly_struct* rbc_poly;\n"
    result += "typedef rbc_poly_sparse_struct* rbc_poly_sparse;\n"

    result += "\n"
    result += "typedef rbc_poly rbc_qre;\n"
    
    result += "\n"
    result += "void rbc_field_init();\n"

    if(arch == "c32"):
        result += "extern uint16_t RBC_SQR_LOOKUP_TABLE[256];\n"

    if(arch == "c64" or (arch == "avx" and m > 192)): 
        result += "extern uint64_t RBC_SQR_LOOKUP_TABLE[256];\n"

    if(arch == "avx" and m <= 192):
        result += "static const __m128i RBC_ELT_SQR_MASK_128 = {0x0F0F0F0F0F0F0F0F, 0x0F0F0F0F0F0F0F0F};\n"
        result += "static const __m128i RBC_ELT_SQR_LOOKUP_TABLE_128 = {0x1514111005040100, 0x5554515045444140};\n"
        
    result += "static const rbc_elt RBC_ELT_MODULUS = " + rbc_core_utils_get_hex_polynomial(rbc_core_utils_get_register_size(arch, m), m) + ";\n"

    result += "\n"
    result += "#ifndef min\n"
    result += "  #define min(a,b) (((a)<(b))?(a):(b))\n"
    result += "#endif\n"

    result += "\n"
    result += "#ifndef max\n"
    result += "  #define max(a,b) (((a)>(b))?(a):(b))\n"
    result += "#endif\n"

    result += "\n"
    result += "#endif\n"

    return result

