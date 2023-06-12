
def preprocessing_rbc_elt_field_init(arch, m):
    result = ""

    if(arch == "avx" and m <= 192):
        result += preprocessing_avx_global_var()
    if(arch == "c32"):
        result += preprocessing_c32_global_var()
    else:
        result += preprocessing_c64_global_var()

    result += "/**\n"
    result += " * \\fn void rbc_field_init()\n"
    result += " * \\brief This function initializes various constants used to perform finite field arithmetic.\n"
    result += " *\n"
    result += " */\n"
    result += "void rbc_field_init() {\n"

    if(arch == "avx" and m <= 192):
        result += preprocessing_avx_rbc_elt_field_init()
    if(arch == "c32"):
        result += preprocessing_c32_rbc_elt_field_init()
    else:
        result += preprocessing_c64_rbc_elt_field_init()

    result += "}\n"
    return result



def preprocessing_avx_global_var():
    return ""



def preprocessing_avx_rbc_elt_field_init():
    return ""



def preprocessing_c32_global_var():
    result = ""

    result += "static uint8_t rbc_init_field = 0;\n"
    result += "uint16_t RBC_SQR_LOOKUP_TABLE[256];\n"
    result += "\n"

    return result



def preprocessing_c32_rbc_elt_field_init():
    result = ""

    result += "  uint8_t bit = 0;\n"
    result += "  uint32_t mask = 0;\n"
    result += "\n"

    result += "  if(rbc_init_field == 0) {\n"
    result += "    memset(RBC_SQR_LOOKUP_TABLE, 0, 2 * 256);\n"
    result += "    for(size_t i = 0 ; i < 256 ; ++i) {\n"
    result += "      for(size_t j = 0 ; j < 8 ; ++j) {\n"
    result += "        mask = 1 << j;\n"
    result += "        bit = (mask & i) >> j;\n"
    result += "        RBC_SQR_LOOKUP_TABLE[i] ^= bit << (2 * j);\n"
    result += "      }\n"
    result += "    }\n"

    result += "\n"
    result += "    rbc_init_field = 1;\n"
    result += "  }\n"

    return result



def preprocessing_c64_global_var():
    result = ""

    result += "static uint8_t rbc_init_field = 0;\n"
    result += "uint64_t RBC_SQR_LOOKUP_TABLE[256];\n"
    result += "\n"

    return result



def preprocessing_c64_rbc_elt_field_init():
    result = ""

    result += "  uint8_t bit = 0;\n"
    result += "  uint64_t mask = 0;\n"
    result += "\n"

    result += "  if(rbc_init_field == 0) {\n"
    result += "    memset(RBC_SQR_LOOKUP_TABLE, 0, 8 * 256);\n"
    result += "    for(size_t i = 0 ; i < 256 ; ++i) {\n"
    result += "      for(size_t j = 0 ; j < 8 ; ++j) {\n"
    result += "        mask = 1 << j;\n"
    result += "        bit = (mask & i) >> j;\n"
    result += "        RBC_SQR_LOOKUP_TABLE[i] ^= bit << (2 * j);\n"
    result += "      }\n"
    result += "    }\n"

    result += "\n"
    result += "    rbc_init_field = 1;\n"
    result += "  }\n"

    return result

