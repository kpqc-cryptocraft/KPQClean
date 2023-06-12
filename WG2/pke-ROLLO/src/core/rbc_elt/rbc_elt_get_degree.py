
def preprocessing_rbc_elt_get_degree(arch, m):
    result = ""

    result += "/**\n"
    result += " * \\fn int32_t rbc_elt_get_degree(const rbc_elt e)\n"
    result += " * \\brief This function returns the degree of a finite field element.\n"
    result += " *\n"
    result += " * \param[out] degree of <b>e</b>\n"
    result += " * \param[in] e rbc_elt\n"
    result += " */\n"
    result += "int32_t rbc_elt_get_degree(const rbc_elt e) {\n"

    if(arch == "c32"): 
        result += preprocessing_c32_rbc_elt_get_degree()
    if(arch == "c64" or arch == "avx"): 
        result += preprocessing_c64_rbc_elt_get_degree()

    result += "}\n"
    return result



def preprocessing_c32_rbc_elt_get_degree():
    result = "  int32_t index = RBC_ELT_SIZE - 1;\n"
    result += "\n"

    result += "  while(index >= 0 && e[index] == 0) --index;\n"
    result += "  if(index == -1) return -1;\n"

    result += "\n"
    result += "  uint32_t msb = 0;\n"
    result += "  uint32_t tmp = e[index];\n"
    result += "  while(tmp >>= 1) ++msb;\n"

    result += "\n"
    result += "  return msb + 32 * index;\n"

    return result



def preprocessing_c64_rbc_elt_get_degree():
    result = "  int64_t index = 0, result = -1;\n"
    result += "  int8_t mask = 0;\n"
    result += "\n"

    result += "  for(uint8_t i = 0 ; i < RBC_ELT_DATA_SIZE ; i++) {\n"
    result += "    __asm__ volatile(\"bsr %1,%0;\" : \"=r\"(index) : \"r\"(e[i]));\n"
    result += "    mask = (e[i] != 0);\n";
    result += "    result = mask * (index + 64 * i) + (1 - mask) * result;\n"
    result += "  }\n\n"

    result += "  return result;\n"
    return result

