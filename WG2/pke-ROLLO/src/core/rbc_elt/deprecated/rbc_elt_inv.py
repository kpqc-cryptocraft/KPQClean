from math import ceil

def preprocessing_rbc_elt_inv(arch, m):
    result = ""

    result += "/**\n"
    result += " * \\fn rbc_elt_inv(rbc_elt o, const rbc_elt e)\n"
    result += " * \\brief This function computes the multiplicative inverse of a finite field element.\n"
    result += " *\n"
    result += " * \param[out] o rbc_elt\n"
    result += " * \param[in] e rbc_elt\n"
    result += " */\n"
    result += "void rbc_elt_inv(rbc_elt o, const rbc_elt e) {\n"

    if(arch == "c32"): result += preprocessing_c32_rbc_elt_inv(m)
    if(arch == "c64" or arch == "avx"): result += preprocessing_c64_rbc_elt_inv(m)

    result += "}\n"
    return result



# C32 implementation
def preprocessing_c32_rbc_elt_inv(m):
    result = ""

    result +="  const uint32_t shift_inv = "+ str(m) + " - 32 * (RBC_ELT_UINT32 - 1);\n"
    result +="\n"
    result +="  uint32_t delta = 0;\n"
    result +="\n"
    result +="  rbc_elt u, v, s, t,tmp;\n"
    result +="  rbc_elt_ur u1, u2;\n"
    result +="\n" 
    result +="  rbc_elt_set_one(u);\n"
    result +="  rbc_elt_set_zero(v);\n"
    result +="  rbc_elt_set(s, RBC_ELT_MODULUS);\n"
    result +="  rbc_elt_set_zero(t);\n"
    result +="  rbc_elt_set(tmp, e);\n"
    result +="  rbc_elt_ur_set_zero(u1);\n"
    result +="  rbc_elt_ur_set_zero(u2);\n"
    result +="\n"
    result +="  for(int i = 0 ; i < 2 * " + str(m) +" ; ++i) {\n"
    result +="    if((tmp[RBC_ELT_UINT32-1] >> shift_inv) == 0) {\n"
    result +="      mult_x(tmp);\n"
    result +="      mult_x(u);\n"
    result +="  \n"
    result +="      if((u[RBC_ELT_UINT32-1] >> shift_inv) == 1) {\n"
    result +="        mod(u, shift_inv);\n"
    result +="      }\n"
    result +="  \n"
    result +="      delta++;\n"
    result +="    }\n"
    result +="    else {\n"
    result +="      if(s[RBC_ELT_UINT32-1] >> shift_inv) {\n"
    result +="        rbc_elt_add(s, s, tmp);\n"
    result +="        rbc_elt_add(v, v, u);\n"
    result +="      }\n"
    result +="  \n"
    result +="      mult_x(s);\n"
    result +="  \n"
    result +="      if(delta == 0) {\n"
    result +="        rbc_elt_set(t, tmp);\n"
    result +="        rbc_elt_set(tmp, s);\n"
    result +="        rbc_elt_set(s, t);\n"
    result +="        rbc_elt_set(t, u);\n"
    result +="        rbc_elt_set(u, v);\n"
    result +="        rbc_elt_set(v, t);\n"
    result +="  \n"
    result +="        mult_x(u);\n"
    result +="  \n"
    result +="        if((u[RBC_ELT_UINT32-1] >> shift_inv) == 1) {\n"
    result +="          mod(u, shift_inv);\n"
    result +="        }\n"
    result +="  \n"
    result +="        delta = 1;\n"
    result +="  \n"
    result +="      } else {\n"
    result +="        if(u[0] & 0x1) {\n"
    result +="          rbc_elt_add(u, u, RBC_ELT_MODULUS);\n"
    result +="        }\n"
    result +="  \n"
    result +="        div_x(u);\n"
    result +="        delta--;\n"
    result +="      }\n"
    result +="    }\n"
    result +="  }\n"
    result +="  \n"
    result +="  rbc_elt_set(o, u);\n"

    return result.rstrip()



# C64 implementation

def preprocessing_c64_rbc_elt_inv(m):
    # Implementation working for 0 <= m < 192 only
    # Euclidean algorithm to find o such that e * o + g * RBC_ELT_MODULUS = 1

    result = ""
    if(m >= 0):    result += "  rbc_elt u, v, g, tmp;\n"
    if(m >= 0):    result += "  uint32_t u_degree, v_degree;\n"
    if(m >= 0):    result += "  uint64_t carry;\n"
    if(m >= 0):    result += "  int32_t j;\n"

    result += "\n"
    if(m >= 0):    result += "  rbc_elt_set(u, e);\n"
    if(m >= 0):    result += "  rbc_elt_set(v, RBC_ELT_MODULUS);\n"

    result += "\n"
    if(m >= 0):    result += "  rbc_elt_set_one(o);\n"
    if(m >= 0):    result += "  rbc_elt_set_zero(g);\n"

    result += "\n"
    if(m >= 0):    result += "  while(u[0] != 1) {\n"
    if(m >= 0):    result += "    u_degree = rbc_elt_get_degree(u);\n"
    if(m >= 0):    result += "    v_degree = rbc_elt_get_degree(v);\n"
    if(m >= 0):    result += "    j = u_degree - v_degree;\n"

    result += "\n"
    if(m >= 0):    result += "    if(j < 0) {\n"
    if(m >= 0):    result += "      rbc_elt_set(tmp, u);\n"
    if(m >= 0):    result += "      rbc_elt_set(u, v);\n"
    if(m >= 0):    result += "      rbc_elt_set(v, tmp);\n"

    result += "\n"
    if(m >= 0):    result += "      rbc_elt_set(tmp, o);\n"
    if(m >= 0):    result += "      rbc_elt_set(o, g);\n"
    if(m >= 0):    result += "      rbc_elt_set(g, tmp);\n"

    result += "\n"
    if(m >= 0):    result += "      j -= j;\n";
    if(m >= 0):    result += "    }\n"

    result += "\n"
    if(m >= 0):    result += "    if(j == 0) {\n"
    if(m >= 0):    result += "      rbc_elt_add(u, u, v);\n"
    if(m >= 0):    result += "      rbc_elt_add(o, o, g);\n"
    if(m >= 0):    result += "    }\n"

    result += "\n"
    if(m >= 0):    result += "    // 0 < j <= 64\n"
    if(m >= 0):    result += "    else if (j <= 64) {\n"
    if(m >= 0):    result += "      carry = v[0] >> (64 - j);\n"
    if(m >= 0):    result += "      u[0] ^= (v[0] << j);\n"
    if(m >= 0):    result += "      u[1] ^= (v[1] << j) ^ carry;\n"
    if(m >= 128):  result += "      carry = v[1] >> (64 - j);\n"
    if(m >= 128):  result += "      u[2] ^= (v[2] << j) ^ carry;\n"

    result += "\n"
    if(m >= 0):    result += "      carry = g[0] >> (64 - j);\n"
    if(m >= 0):    result += "      o[0] ^= g[0] << j;\n"
    if(m >= 0):    result += "      o[1] ^= (g[1] << j) ^ carry;\n"
    if(m >= 128):  result += "      carry = g[1] >> (64 - j);\n"
    if(m >= 128):  result += "      o[2] ^= (g[2] << j) ^ carry;\n"
    if(m >= 0):    result += "    }\n"

    result += "\n"
    if(m < 128):
                          result += "    // 64 < j < M\n"
                          result += "    else {\n"
                          result += "      u[1] ^= v[0] << (j - 64);\n"
                          result += "      o[1] ^= g[0] << (j - 64);\n"
                          result += "    }\n"
    else:
                          result += "    // 64 < j <= 128\n"
                          result += "    else if(j > 64 && j <= 128) {\n"
                          result += "      u[1] ^= v[0] << (j - 64);\n"
                          result += "      u[2] ^= v[0] >> (j - 128);\n"

                          result += "\n"
                          result += "      o[1] ^= g[0] << (j - 64);\n"
                          result += "      o[2] ^= g[0] >> (j - 128);\n"
                          result += "    }\n"

                          result += "\n"
                          result += "    // 128 < j <= M\n"
                          result += "    else {\n"
                          result += "      u[2] ^= v[0] << (j - 128);\n"
                          result += "      o[2] ^= g[0] << (j - 128);\n"
                          result += "    }\n"

    if(m >= 0):     result += "  }"

    return result.rstrip()
