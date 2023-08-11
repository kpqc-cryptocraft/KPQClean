
import os
import sys

root_path = os.path.dirname(os.path.realpath(__file__)) + "/../../.."
core_path = root_path + "/src/core"
sys.path.insert(1, core_path)
from rbc_core_utils import *

def preprocessing_rbc_elt_set_mask2(arch, m):
    result = ""

    result += "/**\n"
    result += " * \\fn void rbc_elt_set(rbc_elt o1, rbc_elt o2, const rbc_elt e, uint32_t mask)\n"
    result += " * \\brief This function copies e either into o1 or o2 depending on the mask value\n"
    result += " *\n"
    result += " * \param[out] o1 rbc_elt\n"
    result += " * \param[out] o2 rbc_elt\n"
    result += " * \param[in] e rbc_elt\n"
    result += " * \param[in] mask 1 to copy into o1 and 0 to copy into o2\n"
    result += " */\n"
    result += "void rbc_elt_set_mask2(rbc_elt o1, rbc_elt o2, const rbc_elt e, uint32_t mask) {\n"

    if(rbc_core_utils_get_size(arch, m) == rbc_core_utils_get_data_size(arch, m)):
        result += "  for(uint8_t i = 0 ; i < RBC_ELT_SIZE ; i++) {\n"
        result += "    o1[i] = mask * e[i] + (1 - mask) * o1[i];\n"
        result += "    o2[i] = mask * e[i] + (1 - mask) * o2[i];\n"
        result += "  }\n"
    else:
        result += "  for(uint8_t i = 0 ; i < RBC_ELT_SIZE - 1 ; i++) {\n"
        result += "    o1[i] = mask * e[i] + (1 - mask) * o1[i];\n"
        result += "    o2[i] = mask * e[i] + (1 - mask) * o2[i];\n"
        result += "  }\n"
        result += "  o1[RBC_ELT_SIZE - 1] = 0;\n"
        result += "  o2[RBC_ELT_SIZE - 1] = 0;\n"

    result += "}\n"
    return result

