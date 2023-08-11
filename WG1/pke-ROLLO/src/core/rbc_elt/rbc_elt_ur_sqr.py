
import os
import sys
from math import ceil

root_path = os.path.dirname(os.path.realpath(__file__)) + "/../../.."
core_path = root_path + "/src/core"
sys.path.insert(1, core_path)
from rbc_core_utils import *

def preprocessing_rbc_elt_ur_sqr(arch, m):
    result = ""

    result += "/**\n"
    result += " * \\fn void rbc_elt_ur_sqr(rbc_elt o, const rbc_elt e)\n"
    result += " * \\brief This function computes the unreduced square of a finite field element.\n"
    result += " *\n"
    result += " * \param[out] o rbc_elt_ur equal to \f$ e^{2} \f$\n"
    result += " * \param[in]  e rbc_elt\n"
    result += "*/\n"
    result += "void rbc_elt_ur_sqr(rbc_elt_ur o, const rbc_elt e) {\n"

    if(arch == "c32"): 
        result += preprocessing_c32_rbc_elt_ur_sqr(arch, m)
    if(arch == "c64"): 
        result += preprocessing_c64_rbc_elt_ur_sqr(arch, m)
    if(arch == "avx" and m <= 64): 
        result += preprocessing_avx_rbc_elt_ur_sqr_m64()
    if(arch == "avx" and m > 64  and m <= 128): 
        result += preprocessing_avx_rbc_elt_ur_sqr_m128()
    if(arch == "avx" and m > 128 and m <= 192): 
        result += preprocessing_avx_rbc_elt_ur_sqr_m192()
    if(arch == "avx" and m > 192): # Fallback in plain C
        result += preprocessing_c64_rbc_elt_ur_sqr(arch, m)

    result += "}\n"
    return result



# C32 implementations

def preprocessing_c32_rbc_elt_ur_sqr(arch, m):
    is_rbc_elt_ur_uint32_odd = ceil((2 * m - 1) / 32) % 2

    result = ""
    result += "  uint32_t u0, u1, u2, u3;\n"

    result += "  \n"
    if(is_rbc_elt_ur_uint32_odd == 1):
        result += "  for(size_t i = 0 ; i < RBC_ELT_SIZE - 1 ; ++i) {\n"
    else:
        result += "  for(size_t i = 0 ; i < RBC_ELT_SIZE ; ++i) {\n"
    result += "    u0 = e[i]  & 0xff;\n"
    result += "    u1 = (e[i] & 0xff00) >> 8;\n"
    result += "    u2 = (e[i] & 0xff0000) >> 16;\n"
    result += "    u3 = (e[i] & 0xff000000) >> 24;\n"

    result += "  \n"
    result += "    o[2*i] = (RBC_SQR_LOOKUP_TABLE[u1] << 16) ^ RBC_SQR_LOOKUP_TABLE[u0];\n"
    result += "    o[2*i+1] = (RBC_SQR_LOOKUP_TABLE[u3] << 16) ^ RBC_SQR_LOOKUP_TABLE[u2];\n"
    result += "  }\n"

    result += "\n"
    if(is_rbc_elt_ur_uint32_odd == 1):
        result += "  u0 = e[RBC_ELT_SIZE - 1] & 0xff;\n"
        result += "  u1 = (e[RBC_ELT_SIZE - 1] & 0xff00) >> 8;\n"
        result += "  o[2 * (RBC_ELT_SIZE - 1)] = (RBC_SQR_LOOKUP_TABLE[u1] << 16) ^ RBC_SQR_LOOKUP_TABLE[u0];\n"

    return result.rstrip()



# C64 implementations

def preprocessing_c64_rbc_elt_ur_sqr(arch, m):
    is_rbc_elt_ur_uint64_odd = ceil((2 * m - 1) / 64) % 2

    result = ""
    result += "uint64_t u0, u1, u2, u3, u4, u5, u6, u7;\n"

    result += "\n"
    if(is_rbc_elt_ur_uint64_odd == 1 or rbc_core_utils_get_size(arch, m) != rbc_core_utils_get_data_size(arch, m)):
        result += "  for(size_t i = 0 ; i < RBC_ELT_SIZE - 1 ; ++i) {\n"
    else:
        result += "  for(size_t i = 0 ; i < RBC_ELT_SIZE ; ++i) {\n"

    result += "    u0 = e[i] & 0xff;\n"
    result += "    u1 = (e[i] & 0xff00) >> 8;\n"
    result += "    u2 = (e[i] & 0xff0000) >> 16;\n"
    result += "    u3 = (e[i] & 0xff000000) >> 24;\n"
    result += "    u4 = (e[i] & 0xff00000000) >> 32;\n"
    result += "    u5 = (e[i] & 0xff0000000000) >> 40;\n"
    result += "    u6 = (e[i] & 0xff000000000000) >> 48;\n"
    result += "    u7 = (e[i] & 0xff00000000000000) >> 56;\n"

    result += "\n"
    result += "    o[2 * i] = (RBC_SQR_LOOKUP_TABLE[u3] << 48) ^ (RBC_SQR_LOOKUP_TABLE[u2] << 32) ^ (RBC_SQR_LOOKUP_TABLE[u1] << 16) ^ RBC_SQR_LOOKUP_TABLE[u0];\n"
    result += "    o[2 * i + 1] = (RBC_SQR_LOOKUP_TABLE[u7] << 48) ^ (RBC_SQR_LOOKUP_TABLE[u6] << 32) ^ (RBC_SQR_LOOKUP_TABLE[u5] << 16) ^ RBC_SQR_LOOKUP_TABLE[u4];\n"
    result += "  }\n"

    result += "\n"
    if(is_rbc_elt_ur_uint64_odd == 1):
        result += "  u0 = e[RBC_ELT_SIZE - 1] & 0xff;\n"
        result += "  u1 = (e[RBC_ELT_SIZE - 1] & 0xff00) >> 8;\n"
        result += "  u2 = (e[RBC_ELT_SIZE - 1] & 0xff0000) >> 16;\n"
        result += "  u3 = (e[RBC_ELT_SIZE - 1] & 0xff000000) >> 24;\n"
        result += "  o[2 * (RBC_ELT_SIZE - 1)] = (RBC_SQR_LOOKUP_TABLE[u3] << 48) ^ (RBC_SQR_LOOKUP_TABLE[u2] << 32) ^ (RBC_SQR_LOOKUP_TABLE[u1] << 16) ^ RBC_SQR_LOOKUP_TABLE[u0];\n"

    return result.rstrip()



# AVX implementations

def preprocessing_avx_rbc_elt_ur_sqr_m64():
    result = ""
    result += "  __m128i tmp, e_l, e_h;\n"

    result += "\n"
    result += "  tmp = _mm_loadl_epi64((__m128i*) e);\n"
    result += "  e_l = _mm_and_si128(tmp, RBC_ELT_SQR_MASK_128);\n"
    result += "  e_h = _mm_and_si128(_mm_srli_epi64(tmp, 4), RBC_ELT_SQR_MASK_128);\n"

    result += "\n"
    result += "  e_l = _mm_shuffle_epi8(RBC_ELT_SQR_LOOKUP_TABLE_128, e_l);\n"
    result += "  e_h = _mm_shuffle_epi8(RBC_ELT_SQR_LOOKUP_TABLE_128, e_h);\n"

    result += "\n"
    result += "  tmp = _mm_unpacklo_epi8(e_l, e_h);\n"
    result += "  _mm_store_si128((__m128i*) o, tmp);\n"

    return result.rstrip()



def preprocessing_avx_rbc_elt_ur_sqr_m128():
    result = ""
    result += "  __m128i tmp1, e_l, e_h;\n"
    result += "  __m128i tmp2[2];\n"

    result += "\n"
    result += "  tmp1 = _mm_load_si128((__m128i*) e);\n"
    result += "  e_l = _mm_and_si128(tmp1, RBC_ELT_SQR_MASK_128);\n"
    result += "  e_h = _mm_and_si128(_mm_srli_epi64(tmp1, 4), RBC_ELT_SQR_MASK_128);\n"

    result += "\n"
    result += "  e_l = _mm_shuffle_epi8(RBC_ELT_SQR_LOOKUP_TABLE_128, e_l);\n"
    result += "  e_h = _mm_shuffle_epi8(RBC_ELT_SQR_LOOKUP_TABLE_128, e_h);\n"

    result += "\n"
    result += "  tmp2[0] = _mm_unpacklo_epi8(e_l, e_h);\n"
    result += "  tmp2[1] = _mm_unpackhi_epi8(e_l, e_h);\n"

    result += "\n"
    result += "  _mm_store_si128((__m128i*) o, tmp2[0]);\n"
    result += "  _mm_store_si128((__m128i*) (o + 2), tmp2[1]);\n"

    return result.rstrip()



def preprocessing_avx_rbc_elt_ur_sqr_m192():
    result = ""
    result += "  __m128i e_0, e_l, e_h;\n"
    result += "  __m128i* tmp = (__m128i*) malloc(2 * sizeof(__m128i));\n"

    result += "\n"
    result += "  e_0 = _mm_load_si128((__m128i*) e);\n"
    result += "  e_l = _mm_and_si128(e_0, RBC_ELT_SQR_MASK_128);\n"
    result += "  e_h = _mm_and_si128(_mm_srli_epi64(e_0, 4), RBC_ELT_SQR_MASK_128);\n"
    result += "\n"
    result += "  e_l = _mm_shuffle_epi8(RBC_ELT_SQR_LOOKUP_TABLE_128, e_l);\n"
    result += "  e_h = _mm_shuffle_epi8(RBC_ELT_SQR_LOOKUP_TABLE_128, e_h);\n"

    result += "\n"
    result += "  tmp[0] = _mm_unpacklo_epi8(e_l, e_h);\n"
    result += "  tmp[1] = _mm_unpackhi_epi8(e_l, e_h);\n"

    result += "\n"
    result += "  _mm_store_si128((__m128i*) o, tmp[0]);\n"
    result += "  _mm_store_si128((__m128i*) (o + 2), tmp[1]);\n"

    result += "\n"
    result += "  e_0 = _mm_load_si128((__m128i *) (e + 2));\n"
    result += "  e_l = _mm_and_si128(e_0, RBC_ELT_SQR_MASK_128);\n"
    result += "  e_h = _mm_and_si128(_mm_srli_epi64(e_0, 4), RBC_ELT_SQR_MASK_128);\n"

    result += "\n"
    result += "  e_l = _mm_shuffle_epi8(RBC_ELT_SQR_LOOKUP_TABLE_128, e_l);\n"
    result += "  e_h = _mm_shuffle_epi8(RBC_ELT_SQR_LOOKUP_TABLE_128, e_h);\n"

    result += "\n"
    result += "  tmp[0] = _mm_unpacklo_epi8(e_l, e_h);\n"
    result += "  _mm_store_si128((__m128i*) (o + 4), tmp[0]);\n"

    result += "\n"
    result += "  free(tmp);\n"

    return result.rstrip()



def preprocessing_avx2_rbc_elt_ur_sqr_m192():
    result = ""
    result += "  __m256i e_l, e_h, tmp1, tmp2;\n"

    result += "\n"
    result += "  tmp1 = _mm256_loadu_si256((__m256i*) e);\n"
    result += "  e_l = _mm256_and_si256(tmp1, RBC_ELT_SQR_MASK_256);\n"
    result += "  e_h = _mm256_and_si256(_mm256_srli_epi64(tmp1, 4), RBC_ELT_SQR_MASK_256);\n"

    result += "\n"
    result += "  e_l = _mm256_shuffle_epi8(RBC_ELT_SQR_LOOKUP_TABLE_256, e_l);\n"
    result += "  e_h = _mm256_shuffle_epi8(RBC_ELT_SQR_LOOKUP_TABLE_256, e_h);\n"

    result += "\n"
    result += "  tmp1 = _mm256_unpacklo_epi8(e_l, e_h);\n"
    result += "  tmp2 = _mm256_unpackhi_epi8(e_l, e_h);\n"

    result += "\n"
    result += "  o[0] = _mm256_extract_epi64(tmp1, 0);\n"
    result += "  o[1] = _mm256_extract_epi64(tmp1, 1);\n"
    result += "  o[2] = _mm256_extract_epi64(tmp2, 0);\n"
    result += "  o[3] = _mm256_extract_epi64(tmp2, 1);\n"
    result += "  o[4] = _mm256_extract_epi64(tmp1, 2);\n"
    result += "  o[5] = _mm256_extract_epi64(tmp1, 3);\n"

    return result.rstrip()

