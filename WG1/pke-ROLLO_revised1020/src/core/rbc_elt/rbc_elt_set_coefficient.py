
import os
import sys

root_path = os.path.dirname(os.path.realpath(__file__)) + "/../../.."
core_path = root_path + "/src/core"
sys.path.insert(1, core_path)
from rbc_core_utils import *

def preprocessing_rbc_elt_set_coefficient(arch, m):
    result = ""

    result += "/**\n"
    result += " * \\fn void rbc_elt_set_coefficient_vartime(rbc_elt o, uint32_t index, uint64_t bit)\n"
    result += " * \\brief This function set a coefficient of the polynomial <b>e</b>.\n"
    result += " *\n"
    result += " * \param[in] e rbc_elt\n"
    result += " * \param[in] index Index of the coefficient\n"
    result += " * \param[in] bit Value of the coefficient\n"
    result += " */\n"
    result += "void rbc_elt_set_coefficient_vartime(rbc_elt o, uint32_t index, uint8_t bit) {\n"

    result +="  size_t position = index / " + str(rbc_core_utils_get_register_size(arch, m)) + ";\n"
    result +="  o[position] |= (uint" + str(rbc_core_utils_get_register_size(arch, m)) + "_t) bit << (index % " + str(rbc_core_utils_get_register_size(arch, m)) + ");\n"
    
    result += "}\n"
    return result

