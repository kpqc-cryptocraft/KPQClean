from math import ceil

def preprocessing_rbc_elt_ur_mul(arch, m):
    result = ""

    result += "/**\n"
    result += " * \\fn void rbc_elt_ur_mul(rbc_elt_ur o, const rbc_elt e1, const rbc_elt e2)\n"
    result += " * \\brief This function computes the unreduced multiplication of two finite field elements.\n"
    result += " *\n"
    result += " * \param[out] o rbc_elt equal to \\f$ e_1 \\times e_2 \f$\n"
    result += " * \param[in] e1 rbc_elt\n"
    result += " * \param[in] e2 rbc_elt\n"
    result += " */\n"
    result += "void rbc_elt_ur_mul(rbc_elt_ur o, const rbc_elt e1, const rbc_elt e2) {\n"

    if(arch == "c32"): 
        result += preprocessing_c32_rbc_elt_ur_mul()
    if(arch == "c64"): 
        result += preprocessing_c64_rbc_elt_ur_mul(arch, m)

    # rbc_elt_mul in avx:
    # 1. Load polynomial inputs as:
    #   - e1 = [ a || c ] = [ a0 || a1 || a2 || a3 ]
    #   - e2 = [ b || d ] = [ b0 || b1 || b2 || b3 ]
    # 2. Generate all a_i * b_j product such that i + j = k
    # 3. XOR all products to generate c_k = Sum a_i * b_j
    # 4. Extract c_k into outputs

    if(arch == "avx" and m <= 64): 
        result += preprocessing_avx_rbc_elt_ur_mul_m64()
    if(arch == "avx" and m > 64  and m <= 128): 
        result += preprocessing_avx_rbc_elt_ur_mul_m128()
    if(arch == "avx" and m > 128 and m <= 192): 
        result += preprocessing_avx_rbc_elt_ur_mul_m192(arch, m)
    if(arch == "avx" and m > 192 and m <= 256): 
        result += preprocessing_avx_rbc_elt_ur_mul_m256()
    if(arch == "avx" and m > 256): # Fallback in plain C
        result += preprocessing_c64_rbc_elt_ur_mul(m)

    result += "}\n"
    return result



def preprocessing_c32_rbc_elt_ur_mul():
    result = ""
    result += "  uint32_t word_size = 32;\n"
    result += "  uint32_t window_size = 4;\n"
    result += "  uint32_t bu_number = 2 << (window_size - 1);\n"
    result += "  uint32_t bu_size = (RBC_FIELD_M + window_size) % 32 == 0 ? (RBC_FIELD_M + window_size) / 32 : (RBC_FIELD_M + window_size) / 32 + 1;\n"
    result += "  uint32_t bu[bu_number][bu_size];\n"
    result += "  uint32_t u, tmp;\n"

    result += "\n"
    result += "  // Precomputation of u x b\n"
    result += "  memset(bu, 0, sizeof bu);\n"
    result += "  rbc_elt_set(bu[1], e2);\n"

    result += "\n"
    result += "  for(size_t i = 2; i < bu_number ; i += 2) {\n"
    result += "    bu[i][0] = bu[i >> 1][0] << 1;\n"
    result += "    bu[i + 1][0] = bu[i][0] ^ bu[1][0];\n"

    result += "\n"
    result += "    for(size_t j = 1 ; j < bu_size ; ++j) {\n"
    result += "      tmp = bu[i >> 1][j - 1] >> (word_size - 1);\n"
    result += "      bu[i][j] = (bu[i >> 1][j] << 1) ^ tmp;\n"
    result += "      bu[i + 1][j] = bu[i][j] ^ bu[1][j];\n"
    result += "    }\n"
    result += "  }\n"

    result += "\n"
    result += "  // Multiplication\n"
    result += "  for(int32_t k = word_size / window_size - 1 ; k > 0 ; --k) {\n"
    result += "    for(size_t j = 0 ; j < RBC_ELT_SIZE ; ++j) {\n"
    result += "      u = (e1[j] >> (window_size * k)) & 0xF;\n"
    result += "      for(size_t i = 0 ; i < bu_size ; ++i) {\n"
    result += "        o[j + i] ^= bu[u][i];\n"
    result += "      }\n"
    result +=  "    }\n"

    result += "\n"
    result += "     for(size_t i = RBC_ELT_UR_SIZE - 1 ; i > 0 ; --i) {\n"
    result += "      o[i] = (o[i] << window_size) ^ (o[i - 1] >> (word_size - 4));\n"
    result += "     }\n"
    result += "     o[0] = o[0] << window_size;\n"
    result += "  }\n"

    result += "\n"
    result += " for(size_t j = 0 ; j < RBC_ELT_SIZE ; ++j){\n"
    result += "     u = (e1[j] & 0xF);\n"
    result += "     for(size_t i = 0 ; i < bu_size ; ++i){\n"
    result += "         o[j + i] ^= bu[u][i];\n"
    result += "     }\n"
    result += " }\n"

    return result



def preprocessing_c64_rbc_elt_ur_mul(arch, m):
    result = ""

    result += "  uint32_t word_size = 64;\n"
    result += "  uint32_t window_size = 4;\n"
    result += "  uint32_t bu_number = 2 << (window_size - 1);\n"
    result += "  uint32_t bu_size = (RBC_FIELD_M + window_size) % 64 == 0 ? (RBC_FIELD_M + window_size) / 64 : (RBC_FIELD_M + window_size) / 64 + 1;\n"
    result += "  uint64_t bu[bu_number][bu_size];\n"
    result += "  uint64_t u, tmp;\n"

    result += "\n"
    result += "  // Precomputation of u x b\n"
    result += "  memset(bu, 0, sizeof bu);\n"
    result += "  rbc_elt_set(bu[1], e2);\n"

    result += "\n"
    result += "  for(size_t i = 2; i < bu_number ; i += 2) {\n"
    result += "    bu[i][0] = bu[i >> 1][0] << 1;\n"
    result += "    bu[i + 1][0] = bu[i][0] ^ e2[0];\n"

    result += "\n"
    if(m >= 124 and m < 128):     
        result += "    tmp = bu[i >> 1][0] >> (word_size - 1);\n"
        result += "    bu[i][1] = (bu[i >> 1][1] << 1) ^ tmp;\n"
        result += "    bu[i + 1][1] = bu[i][1] ^ e2[1];\n"

        result += "    tmp = bu[i >> 1][1] >> (word_size - 1);\n"
        result += "    bu[i][2] = (bu[i >> 1][2] << 1) ^ tmp;\n"
        result += "    bu[i + 1][2] = bu[i][2];\n"
        result += "  }\n"
    else:
        result += "    for(size_t j = 1 ; j < bu_size ; ++j) {\n"
        result += "      tmp = bu[i >> 1][j - 1] >> (word_size - 1);\n"
        result += "      bu[i][j] = (bu[i >> 1][j] << 1) ^ tmp;\n"
        result += "      bu[i + 1][j] = bu[i][j] ^ e2[j];\n"
        result += "    }\n"
        result += "  }\n"

    result += "\n"
    result += "  // Multiplication\n"
    result += "  for(int32_t k = word_size / window_size - 1 ; k >= 0 ; --k) {\n"
    result += "    for(size_t j = 0 ; j < RBC_ELT_SIZE ; ++j) {\n"
    result += "      u = (e1[j] >> (window_size * k)) & 0xF;\n"
    result += "      for(size_t i = 0 ; i < bu_size ; ++i) {\n"
    result += "        o[j + i] ^= bu[u][i];\n"
    result += "      }\n"
    result +=  "    }\n"

    result += "\n"
    result += "    if(k != 0) {\n"
    result += "      for(size_t i = RBC_ELT_UR_SIZE - 1 ; i > 0 ; --i) {\n"
    result += "        o[i] = (o[i] << window_size) ^ (o[i - 1] >> (word_size - 4));\n"
    result += "      }\n"
    result += "      o[0] = o[0] << window_size;\n"
    result += "    }\n"
    result += "  }\n"

    return result



def preprocessing_avx_rbc_elt_ur_mul_m64():
    result = ""

    result += "  __m128i a = _mm_loadl_epi64((__m128i*) e1);\n"
    result += "  __m128i b = _mm_loadl_epi64((__m128i*) e2);\n\n"
    result += "  __m128i a0_b0 = _mm_clmulepi64_si128(a, b, 0x00);\n\n"
    result += "  _mm_store_si128((__m128i*) o, a0_b0);\n"

    return result



def preprocessing_avx_rbc_elt_ur_mul_m128():
    result = ""

    result += "  __m128i a = _mm_load_si128((__m128i*) e1);\n"
    result += "  __m128i b = _mm_load_si128((__m128i*) e2);\n"

    result += "\n"
    result += "  __m128i a0_b0 = _mm_clmulepi64_si128(a, b, 0x00);\n"

    result += "\n"
    result += "  __m128i a0_b1 = _mm_clmulepi64_si128(a, b, 0x10);\n"
    result += "  __m128i a1_b0 = _mm_clmulepi64_si128(a, b, 0x01);\n"

    result += "\n"
    result += "  __m128i a1_b1 = _mm_clmulepi64_si128(a, b, 0x11);\n"

    result += "\n"
    result += "  __m128i c1 = _mm_xor_si128(a0_b1, a1_b0);\n"

    result += "\n"
    result += "  o[0] = _mm_extract_epi64(a0_b0, 0);\n"
    result += "  o[1] = _mm_extract_epi64(a0_b0, 1) ^ _mm_extract_epi64(c1, 0);\n"
    result += "  o[2] = _mm_extract_epi64(a1_b1, 0) ^ _mm_extract_epi64(c1, 1);\n"
    result += "  o[3] = _mm_extract_epi64(a1_b1, 1);\n"

    return result



def preprocessing_avx_rbc_elt_ur_mul_m192(arch, m):
    result = ""

    result += "  __m128i a = _mm_load_si128((__m128i*) e1);\n"
    result += "  __m128i b = _mm_load_si128((__m128i*) e2);\n"
    result += "  __m128i c = _mm_load_si128((__m128i*) (e1 + 2));\n"
    result += "  __m128i d = _mm_load_si128((__m128i*) (e2 + 2));\n"

    result += "\n"
    result += "  __m128i a0_b0 = _mm_clmulepi64_si128(a, b, 0x00);\n"

    result += "\n"
    result += "  __m128i a0_b1 = _mm_clmulepi64_si128(a, b, 0x10);\n"
    result += "  __m128i a1_b0 = _mm_clmulepi64_si128(a, b, 0x01);\n"

    result += "\n"
    result += "  __m128i a0_b2 = _mm_clmulepi64_si128(a, d, 0x00);\n"
    result += "  __m128i a1_b1 = _mm_clmulepi64_si128(a, b, 0x11);\n"
    result += "  __m128i a2_b0 = _mm_clmulepi64_si128(c, b, 0x00);\n"

    result += "\n"
    result += "  __m128i a1_b2 = _mm_clmulepi64_si128(a, d, 0x01);\n"
    result += "  __m128i a2_b1 = _mm_clmulepi64_si128(c, b, 0x10);\n"

    result += "\n"
    result += "  __m128i a2_b2 = _mm_clmulepi64_si128(c, d, 0x00);\n"

    result += "\n"
    result += "  __m128i c1 = _mm_xor_si128(a0_b1, a1_b0);\n"
    result += "  __m128i c2 = _mm_xor_si128(a0_b2, _mm_xor_si128(a1_b1, a2_b0));\n"
    result += "  __m128i c3 = _mm_xor_si128(a1_b2, a2_b1);\n"

    result += "\n"
    result += "  o[0] = _mm_extract_epi64(a0_b0, 0);\n"
    result += "  o[1] = _mm_extract_epi64(a0_b0, 1) ^ _mm_extract_epi64(c1, 0);\n"
    result += "  o[2] = _mm_extract_epi64(c1, 1) ^ _mm_extract_epi64(c2, 0);\n"
    result += "  o[3] = _mm_extract_epi64(c2, 1) ^ _mm_extract_epi64(c3, 0);\n"
    result += "  o[4] = _mm_extract_epi64(c3, 1) ^ _mm_extract_epi64(a2_b2, 0);\n"

    if(m < 161):
        result += "  o[5] = 0;\n"
    else:
        result += "  o[5] = _mm_extract_epi64(a2_b2, 1);\n"

    return result



def preprocessing_avx_rbc_elt_ur_mul_m256():
    result = ""

    result += "  __m128i a = _mm_load_si128((__m128i*) e1);\n"
    result += "  __m128i b = _mm_load_si128((__m128i*) e2);\n"
    result += "  __m128i c = _mm_load_si128((__m128i*) (e1 + 2));\n"
    result += "  __m128i d = _mm_load_si128((__m128i*) (e2 + 2));\n"

    result += "\n"
    result += "  __m128i a0_b0 = _mm_clmulepi64_si128(a, b, 0x00);\n"

    result += "\n"
    result += "  __m128i a0_b1 = _mm_clmulepi64_si128(a, b, 0x10);\n"
    result += "  __m128i a1_b0 = _mm_clmulepi64_si128(a, b, 0x01);\n"

    result += "\n"
    result += "  __m128i a0_b2 = _mm_clmulepi64_si128(a, d, 0x00);\n"
    result += "  __m128i a1_b1 = _mm_clmulepi64_si128(a, b, 0x11);\n"
    result += "  __m128i a2_b0 = _mm_clmulepi64_si128(c, b, 0x00);\n"

    result += "\n"
    result += "  __m128i a0_b3 = _mm_clmulepi64_si128(a, d, 0x10);\n"
    result += "  __m128i a1_b2 = _mm_clmulepi64_si128(a, d, 0x01);\n"
    result += "  __m128i a2_b1 = _mm_clmulepi64_si128(c, b, 0x10);\n"
    result += "  __m128i a3_b0 = _mm_clmulepi64_si128(c, b, 0x01);\n"

    result += "\n"
    result += "  __m128i a1_b3 = _mm_clmulepi64_si128(a, d, 0x11);\n"
    result += "  __m128i a2_b2 = _mm_clmulepi64_si128(c, d, 0x00);\n"
    result += "  __m128i a3_b1 = _mm_clmulepi64_si128(c, b, 0x11);\n"

    result += "\n"
    result += "  __m128i a2_b3 = _mm_clmulepi64_si128(c, d, 0x10);\n"
    result += "  __m128i a3_b2 = _mm_clmulepi64_si128(c, d, 0x01);\n"

    result += "\n"
    result += "  __m128i a3_b3 = _mm_clmulepi64_si128(c, d, 0x11);\n"

    result += "\n"
    result += "  __m128i c1 = _mm_xor_si128(a0_b1, a1_b0);\n"
    result += "  __m128i c2 = _mm_xor_si128(a0_b2, _mm_xor_si128(a1_b1, a2_b0));\n"
    result += "  __m128i c3 = _mm_xor_si128(a0_b3, _mm_xor_si128(a1_b2, _mm_xor_si128(a2_b1, a3_b0)));\n"
    result += "  __m128i c4 = _mm_xor_si128(a1_b3, _mm_xor_si128(a2_b2, a3_b1));\n"
    result += "  __m128i c5 = _mm_xor_si128(a2_b3, a3_b2);\n"

    result += "\n"
    result += "  o[0] = _mm_extract_epi64(a0_b0, 0);\n"
    result += "  o[1] = _mm_extract_epi64(a0_b0, 1) ^ _mm_extract_epi64(c1, 0);\n"
    result += "  o[2] = _mm_extract_epi64(c1, 1) ^ _mm_extract_epi64(c2, 0);\n"
    result += "  o[3] = _mm_extract_epi64(c2, 1) ^ _mm_extract_epi64(c3, 0);\n"
    result += "  o[4] = _mm_extract_epi64(c3, 1) ^ _mm_extract_epi64(c4, 0);\n"
    result += "  o[5] = _mm_extract_epi64(c4, 1) ^ _mm_extract_epi64(c5, 0);\n"
    result += "  o[6] = _mm_extract_epi64(c5, 1) ^ _mm_extract_epi64(a3_b3, 0);\n"
    result += "  o[7] = _mm_extract_epi64(a3_b3, 1);\n"

    return result

