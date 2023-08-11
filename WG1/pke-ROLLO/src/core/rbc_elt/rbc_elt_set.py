
import os
import sys

root_path = os.path.dirname(os.path.realpath(__file__)) + "/../../.."
core_path = root_path + "/src/core"
sys.path.insert(1, core_path)
from rbc_core_utils import *

def preprocessing_rbc_elt_set(arch, m):
    result = ""

    result += "/**\n"
    result += " * \\fn void rbc_elt_set(rbc_elt o, const rbc_elt e)\n"
    result += " * \\brief This function copies a finite field element into another one.\n"
    result += " *\n"
    result += " * \param[out] o rbc_elt\n"
    result += " * \param[in] e rbc_elt\n"
    result += " */\n"
    result += "void rbc_elt_set(rbc_elt o, const rbc_elt e) {\n"

    if(rbc_core_utils_get_size(arch, m) == rbc_core_utils_get_data_size(arch, m)):
        result += "  for(uint8_t i = 0 ; i < RBC_ELT_SIZE ; i++) {\n"
        result += "    o[i] = e[i];\n"
        result += "  }\n"
    else:
        result += "  for(uint8_t i = 0 ; i < RBC_ELT_SIZE - 1 ; i++) {\n"
        result += "    o[i] = e[i];\n"
        result += "  }\n"
        result += "  o[RBC_ELT_SIZE - 1] = 0;\n"

    result += "}\n"
    return result

