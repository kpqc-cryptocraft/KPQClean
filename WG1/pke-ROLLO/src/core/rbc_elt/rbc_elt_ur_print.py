
import os
import sys

root_path = os.path.dirname(os.path.realpath(__file__)) + "/../../.."
core_path = root_path + "/src/core"
sys.path.insert(1, core_path)
from rbc_core_utils import *
from rbc_elt_print import *

def preprocessing_rbc_elt_ur_print(arch, m):
    result = ""

    result += "/**\n"
    result += " * \\fn void rbc_elt_ur_print(const rbc_elt_ur e)\n"
    result += " * \\brief This function displays an unreduced finite field element.\n"
    result += " *\n"
    result += " * \param[in] e rbc_elt_ur\n"
    result += " */\n"
    result += "void rbc_elt_ur_print(const rbc_elt_ur e) {\n"

    if(arch == "c32"): 
        result += preprocessing_c32_rbc_elt_print(arch, m)
    if(arch == "c64" or arch == "avx"): 
        result += preprocessing_c64_rbc_elt_print(arch, m)

    result += "}\n"
    return result

