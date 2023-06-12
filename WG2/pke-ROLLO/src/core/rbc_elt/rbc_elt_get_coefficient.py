
import os
import sys

root_path = os.path.dirname(os.path.realpath(__file__)) + "/../../.."
core_path = root_path + "/src/core"
sys.path.insert(1, core_path)
from rbc_core_utils import *

def preprocessing_rbc_elt_get_coefficient(arch, m):
    result = ""

    result += "/**\n"
    result += " * \\fn uint8_t rbc_elt_get_coefficient_vartime(const rbc_elt e, uint32_t index)\n"
    result += " * \\brief This function returns the coefficient of the polynomial <b>e</b> at a given index.\n"
    result += " *\n"
    result += " * \param[in] e rbc_elt\n"
    result += " * \param[in] index Index of the coefficient\n"
    result += " * \return Coefficient of <b>e</b> at the given index\n"
    result += " */\n"
    result += "uint8_t rbc_elt_get_coefficient_vartime(const rbc_elt e, uint32_t index) {\n"

    result += "  size_t position = index / " + str(rbc_core_utils_get_register_size(arch, m)) + ";\n"
    result += "  return (e[position] >> (index % " + str(rbc_core_utils_get_register_size(arch, m)) + ")) & 0x01;\n"

    result += "}\n"
    return result

