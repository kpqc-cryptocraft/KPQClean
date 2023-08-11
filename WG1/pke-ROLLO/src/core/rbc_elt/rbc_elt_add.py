from math import ceil

def preprocessing_rbc_elt_add(arch, m):
    result = ""

    result += "/**\n"
    result += " * \\fn rbc_elt_add(rbc_elt o, const rbc_elt e1, const rbc_elt e2)\n"
    result += " * \\brief This function adds two finite field elements.\n"
    result += " *\n"
    result += " * \param[out] o Sum of <b>e1</b> and <b>e2</b>\n"
    result += " * \param[in] e1 rbc_elt\n"
    result += " * \param[in] e2 rbc_elt\n"
    result += " */\n"
    result += "void rbc_elt_add(rbc_elt o, const rbc_elt e1, const rbc_elt e2) {\n"

    if(arch == "avx" and m > 128):  
        result += preprocessing_avx_rbc_elt_add()
    else:
        result += preprocessing_c64_rbc_elt_add()

    result += "}\n"
    return result



def preprocessing_c64_rbc_elt_add():
    result = ""

    result += "  for(uint8_t i = 0 ; i < RBC_ELT_SIZE ; i++) {\n"
    result += "    o[i] = e1[i] ^ e2[i];\n"
    result += "  }\n"

    return result



def preprocessing_avx_rbc_elt_add():
    result = ""

    result += "  __m128i tmp1, tmp2;\n\n"
    result += "  for(uint8_t i = 0 ; i < RBC_ELT_SIZE ; i = i + 2) {\n"
    result += "    tmp1 = _mm_load_si128((__m128i*) (e1 + i));\n"
    result += "    tmp2 = _mm_load_si128((__m128i*) (e2 + i));\n"
    result += "    tmp1 = _mm_xor_si128(tmp1, tmp2);\n"
    result += "    _mm_store_si128((__m128i*) (o + i), tmp1);\n"
    result += "  }\n"

    return result



#def preprocessing_avx2_rbc_elt_add_m192():
#    result = ""
#
#    result += "  __m256i tmp1 = _mm256_loadu_si256((__m256i*) e1);\n"
#    result += "  __m256i tmp2 = _mm256_loadu_si256((__m256i*) e2);\n"
#    result += "  tmp1 = _mm256_xor_si256(tmp1, tmp2);\n"
#    result += "  _mm256_storeu_si256((__m256i*) o, tmp1);\n"
#
#    return result

