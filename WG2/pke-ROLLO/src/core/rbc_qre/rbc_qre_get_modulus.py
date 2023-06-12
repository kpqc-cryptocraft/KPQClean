import os
import sys

root_path = os.path.dirname(os.path.realpath(__file__)) + "/../../.."
core_path = root_path + "/src/core"
sys.path.insert(1, core_path)
from rbc_core_utils import *

def preprocessing_rbc_qre_get_modulus(arch, m, n_list):
    result = ""

    result += "/**\n"
    result += " * \\fn rbc_poly_sparse rbc_qre_get_modulus(uint32_t degree) {\n"
    result += " * \\brief This function return the sparse polynomial used as the quotient ring modulus for PARAM_N.\n"
    result += " *\n"
    result += " * \param[in] degree Degree of the modulus\n"
    result += " */\n"
    result += "rbc_poly_sparse rbc_qre_get_modulus(uint32_t degree) {\n"
    result += "  rbc_poly_sparse modulus = NULL;\n"
    result += "  uint32_t* values;\n"

    first_n = 1

    for n in n_list:
        if first_n == 1:
            result += "  if(degree == {}) ".format(n) + "{\n"
            first_n = 0
        else:
            result += "  else if(degree == {}) ".format(n) + "{\n"
        
        monomials = rbc_core_utils_get_polynomial(n)
        result += "    values = (uint32_t*) malloc({} * sizeof(uint32_t));\n".format(len(monomials) + 1)

        for i in range(len(monomials)):
            result += "    values[{}] = {};\n".format(i, monomials[-1-i])
        
        result += "    values[{}] = {};\n".format(len(monomials), n)
        result += "    rbc_poly_sparse_init(&modulus, {}, values);\n".format(len(monomials) + 1)
        result += "    free(values);\n"
        result += "  }\n"

    result += "\n return modulus;\n}\n"
    return result

