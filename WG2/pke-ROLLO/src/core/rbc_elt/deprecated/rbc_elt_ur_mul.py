from math import ceil

def preprocessing_c32_rbc_elt_mult_x():
    result = ""
    result +="/**\n"
    result +="* \fn static inline void mult_x(rbc_elt e)\n"
    result +="* \brief Multiply e by x\n"
    result +="*\n"
    result +="* \\param[out] e Finite field element\n"
    result +="*/\n"
    result +="static inline void mult_x(rbc_elt e) {\n"
    result +="  for(int i = RBC_ELT_UINT32 - 1 ; i > 0 ; --i) {\n"
    result +="    e[i] = (e[i] << 1) ^ (e[i - 1] >> 31);\n"
    result +="  }\n"
    result +="  e[0] = e[0] << 1;\n"
    result +="}\n"
 
    return result



def preprocessing_c32_rbc_elt_div_x():
    result = ""
    result += "/**\n"
    result += "* \fn static inline void div_x(rbc_elt e)\n"
    result += " * \brief Divides e by x\n"
    result += " *\n"
    result += " * \\param[out] e Finite field element\n"
    result += " */\n"
    result += "static inline void div_x(rbc_elt e) {\n"
    result += "  for(int i = 0 ; i < RBC_ELT_UINT32 - 1 ; ++i) {\n"
    result += "    e[i] = (e[i] >> 1) ^ (e[i + 1] << 31);\n"
    result += "  }\n"
    result += "  e[RBC_ELT_UINT32 - 1] = e[RBC_ELT_UINT32 - 1] >> 1;\n"
    result += "}\n"

    return result



def preprocessing_c32_rbc_elt_mod(m):
    result = ""
    result +="/**\n"
    result +="* \fn static inline void mod(rbc_elt e, uint32_t shift)\n"
    result +=" * \brief Reduce the power FIELD_M of e\n"
    result +=" *\n"
    result +=" * \\param[out] e Finite field element\n"
    result +=" * \\param[in]  shift Position of FIELD_M in its block\n"
    result +=" */\n"
    result +="static inline void mod(rbc_elt e, uint32_t shift) {\n"
    result +="  e[RBC_ELT_UINT32-1] = e[RBC_ELT_UINT32 - 1] & ((1 << shift) - 1);\n"
    result +="  for(int i = 0 ; i < RBC_ELT_UINT32 ; ++i) {\n"
    result +="    e[i] ^= RBC_" + str(m) + "_FIELD_R[i];\n"
    result +="  }\n"
    result +="}\n"

    return result
