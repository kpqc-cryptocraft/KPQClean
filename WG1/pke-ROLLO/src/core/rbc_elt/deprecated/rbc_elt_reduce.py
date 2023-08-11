
def preprocessing_rbc_elt_reduce_generic_SLOW(m, word_length):
    monomials = getPolynomial(m)
    max_monomial = monomials[0]
    poly_words = ceil(max_monomial / word_length)
    rbc_length = ceil(m / word_length)

    result = ""

    #Precomputation
    polys = "{"
    for i in range(word_length):
        polys += getPartialHexPolynomial(word_length, m, poly_words+1, i) + ", "
    polys += "}"

    result += "  uint{}_t polynomials[{}][{}] = {};\n".format(word_length, word_length, poly_words+1, polys) 

    #Reduction
    result += "  rbc_elt_ur e2;\n"
    result += "  for(int i=0 ; i<RBC_ELT_UR_UINT{} ; i++) e2[i] = e[i];\n".format(word_length)
    result += "  for(int i={} ; i>={} ; i--)".format(2*m-1, m) + " {\n"
    result += "    size_t position = i / {};\n".format(word_length)
    result += "    size_t shift = i % {};\n".format(word_length)
    result += "    if(((e2[position] >> shift) & 0x01) == 1) {\n"
    result += "      position = (i - {}) / {};\n".format(m, word_length)
    result += "      shift = (i - {}) % {};\n".format(m, word_length)
    for i in range(poly_words+1):
        result += "      e2[position+{}] ^= polynomials[shift][{}];\n".format(i, i)
    result += "    }\n"
    result += "  }\n"

    result += "  for(int i=0 ; i<RBC_ELT_UINT{} ; i++) o[i] = e2[i];".format(word_length)
    result += "\n  o[" + str(rbc_length-1) + "] &= 0x{0:X};\n".format(2**(m % word_length) - 1);
    
    return result

