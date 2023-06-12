
def preprocessing_rbc_elt_ur_set_from_uint64(arch, m):
    result = ""

    result += "/**\n"
    result += " * \\fn void rbc_elt_ur_set_from_uint64(rbc_elt_ur o, uint64_t* rbc_elt e)\n"
    result += " * \\brief This function set an unreduced finite field element from a pointer to uint64_t.\n"
    result += " *\n"
    result += " * \param[out] o rbc_elt_ur\n"
    result += " * \param[in] e Pointer to uint64_t\n"
    result += " */\n"
    result += "void rbc_elt_ur_set_from_uint64(rbc_elt_ur o, const uint64_t* e) {\n"

    if(arch == "c32"):
        result += preprocessing_c32_rbc_elt_ur_set_from_uint64()
    if(arch == "c64" or arch == "avx"): 
        result += preprocessing_c64_rbc_elt_ur_set_from_uint64()

    result += "}\n"
    return result



def preprocessing_c32_rbc_elt_ur_set_from_uint64():
    return "  memcpy((uint32_t*) o, e, RBC_ELT_SIZE * 4);\n"



def preprocessing_c64_rbc_elt_ur_set_from_uint64():
    return "  rbc_elt_ur_set(o, e);\n" 

