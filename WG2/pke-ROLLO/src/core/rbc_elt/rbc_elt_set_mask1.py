
import os
import sys

root_path = os.path.dirname(os.path.realpath(__file__)) + "/../../.."
core_path = root_path + "/src/core"
sys.path.insert(1, core_path)
from rbc_core_utils import *

def preprocessing_rbc_elt_set_mask1(arch, m):
    result = ""

    result += "/**\n"
    result += "* \\fn void rbc_elt_set_mask1(rbc_elt o, const rbc_elt e1, const rbc_elt e2, uint32_t mask)\n"
    result += "* \\brief This function copies either e1 or e2 into o depending on the mask value\n"
    result += "*\n"
    result += "* \param[out] o rbc_elt\n"
    result += "* \param[in] e1 rbc_elt\n"
    result += "* \param[in] e2 rbc_elt_n"
    result += "* \param[in] mask 1 to copy e1 and 0 to copy e2\n"
    result += "*/\n"
    result += "void rbc_elt_set_mask1(rbc_elt o, const rbc_elt e1, const rbc_elt e2, uint32_t mask) {\n"

    if(rbc_core_utils_get_size(arch, m) == rbc_core_utils_get_data_size(arch, m)):
        result += "  for(uint8_t i = 0 ; i < RBC_ELT_SIZE ; i++) {\n"
        result += "    o[i] = mask * e1[i] + (1 - mask) * e2[i];\n"
        result += "  }\n"
    else:
        result += "  for(uint8_t i = 0 ; i < RBC_ELT_SIZE - 1 ; i++) {\n"
        result += "    o[i] = mask * e1[i] + (1 - mask) * e2[i];\n"
        result += "  }\n"
        result += "  o[RBC_ELT_SIZE - 1] = 0;\n"

    result += "}\n"
    return result

