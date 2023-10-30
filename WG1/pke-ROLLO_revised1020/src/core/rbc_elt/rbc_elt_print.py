
import os
import sys
from math import ceil

root_path = os.path.dirname(os.path.realpath(__file__)) + "/../../.."
core_path = root_path + "/src/core"
sys.path.insert(1, core_path)
from rbc_core_utils import *

def preprocessing_rbc_elt_print(arch, m):
    result = ""

    result += "/**\n"
    result += " * \\fn void rbc_elt_print(const rbc_elt e)\n"
    result += " * \\brief This function displays a finite field element.\n"
    result += " *\n"
    result += " * \param[in] e rbc_elt\n"
    result += " */\n"
    result += "void rbc_elt_print(const rbc_elt e) {\n"

    if(arch == "c32"): 
        result += preprocessing_c32_rbc_elt_print(arch, m)
    if(arch == "c64" or arch == "avx"): 
        result += preprocessing_c64_rbc_elt_print(arch, m)

    result += "}\n"
    return result



def preprocessing_c32_rbc_elt_print(arch, m):

    result = "  printf(\"[\");\n  printf("
    for i in range(0, rbc_core_utils_get_data_size(arch, m)):
        result += "\" %16\" PRIx32 "

    for i in range(0, rbc_core_utils_get_data_size(arch, m)):
        result += ", e[" + str(i) +"]"

    result += ");\n  printf(\" ]\");\n"
    return result



def preprocessing_c64_rbc_elt_print(arch, m):

    result = "  printf(\"[\");\n  printf("
    for i in range(0, rbc_core_utils_get_data_size(arch, m)):
        result += "\" %16\" PRIx64 "

    for i in range(0, rbc_core_utils_get_data_size(arch, m)):
        result += ", e[" + str(i) +"]"

    result += ");\n  printf(\" ]\");\n"
    return result 

