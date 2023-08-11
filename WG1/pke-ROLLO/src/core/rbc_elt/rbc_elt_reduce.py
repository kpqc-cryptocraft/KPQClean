
import os
import sys

root_path = os.path.dirname(os.path.realpath(__file__)) + "/../../.."
core_path = root_path + "/src/core"
sys.path.insert(1, core_path)
from rbc_core_utils import *
from math import ceil, floor

def preprocessing_rbc_elt_reduce(arch, m):
    rbc_core_utils_load_polynomials()
    result = ""

    result += "/**\n"
    result += " * \\fn void rbc_elt_reduce(rbc_elt o, const rbc_elt_ur e)\n"
    result += " * \\brief This function reduces a finite field element.\n"
    result += " *\n"
    result += " * \param[out] o rbc_elt equal to \f$ e \pmod f \f$\n"
    result += " * \param[in] e rbc_elt\n"
    result += " */\n"
    result += "void rbc_elt_reduce(rbc_elt o, const rbc_elt_ur e) {\n"

    if(arch == "c32"):
        customs_m_values = [67, 79, 83, 97, 101, 113, 127, 151, 181]
        if m in customs_m_values:
            if(m == 67):   result += preprocessing_c32_rbc_elt_reduce_m67()
            if(m == 79):   result += preprocessing_c32_rbc_elt_reduce_m79()
            if(m == 83):   result += preprocessing_c32_rbc_elt_reduce_m83()
            if(m == 97):   result += preprocessing_c32_rbc_elt_reduce_m97()
            if(m == 101):  result += preprocessing_c32_rbc_elt_reduce_m101()
            if(m == 113):  result += preprocessing_c32_rbc_elt_reduce_m113()
            if(m == 127):  result += preprocessing_c32_rbc_elt_reduce_m127()
            if(m == 151):  result += preprocessing_c32_rbc_elt_reduce_m151()
            if(m == 181):  result += preprocessing_c32_rbc_elt_reduce_m181()
        else:
            result += preprocessing_rbc_elt_reduce_generic(m, 32)

    if(arch == "c64" or arch == "avx"):
        customs_m_values = [61, 67, 79, 83, 97, 101, 113, 127, 151, 181]
        if m in customs_m_values:
            if(m == 61):   result += preprocessing_c64_rbc_elt_reduce_m61()
            if(m == 67):   result += preprocessing_c64_rbc_elt_reduce_m67()
            if(m == 79):   result += preprocessing_c64_rbc_elt_reduce_m79()
            if(m == 83):   result += preprocessing_c64_rbc_elt_reduce_m83()
            if(m == 97):   result += preprocessing_c64_rbc_elt_reduce_m97()
            if(m == 101):  result += preprocessing_c64_rbc_elt_reduce_m101()
            if(m == 113):  result += preprocessing_c64_rbc_elt_reduce_m113()
            if(m == 127):  result += preprocessing_c64_rbc_elt_reduce_m127()
            if(m == 151):  result += preprocessing_c64_rbc_elt_reduce_m151()
            if(m == 181):  result += preprocessing_c64_rbc_elt_reduce_m181()
        else:
            result += preprocessing_rbc_elt_reduce_generic(m, 64)

    result += "}\n"
    return result



def preprocessing_c32_rbc_elt_reduce_m67():
    result = ""
    result += "  rbc_elt_set(o, e);\n"

    result += "\n"
    result += "  o[2] ^= (e[4] >> 3) ^ (e[4] >> 2) ^ (e[4] >> 1) ^ (e[3] >> 30);\n"
    result += "  o[2] ^= (e[4] << 2);\n"

    result += "\n"
    result += "  o[1] ^= (e[3] >> 3)  ^ (e[3] >> 2)  ^ (e[3] >> 1)  ^ (o[2] >> 30);\n"
    result += "  o[1] ^= (e[4] << 29) ^ (e[4] << 30) ^ (e[4] << 31) ^ (e[3] << 2);\n"

    result += "\n"
    result += "  uint32_t tmp = o[2] >> 3;\n"
    result += "  o[0] ^= tmp ^ (tmp << 1) ^ (tmp << 2) ^ (tmp << 5);\n"
    result += "  o[0] ^= (e[3] << 29) ^ (e[3] << 30) ^ (e[3] << 31);\n"

    result += "\n"
    result += "  o[2] &= 0x7;\n"

    return result



def preprocessing_c32_rbc_elt_reduce_m79():
    result = ""
    result += "  rbc_elt_set(o, e);\n"

    result += "\n" 
    result += "  o[2] ^= (e[4] >> 15) ^ (e[4] >> 6);\n"
    result += "  o[1] ^= (e[4] << 17) ^ (e[4] << 26) ^ (e[3] >> 15) ^ (e[3] >> 6);\n"
    result += "  o[0] ^= (e[3] << 17) ^ (e[3] << 26) ^ (o[2] >> 15) ^ (o[2] >> 15 << 9);\n"

    result += "\n" 
    result += "  o[2] &= 0x7FFF;\n"

    return result



def preprocessing_c32_rbc_elt_reduce_m83():
    result = ""
    result += "  rbc_elt_set(o, e);\n"

    result += "\n" 
    result += "  o[2] ^= (e[5] << 13) ^ (e[5] << 15) ^ (e[5] << 17) ^ (e[5] << 20);\n"
    result += "  o[2] ^= (e[4] >> 19) ^ (e[4] >> 17) ^ (e[4] >> 15) ^ (e[4] >> 12);\n"

    result += "\n" 
    result += "  o[1] ^= (e[4] << 13) ^ (e[4] << 15) ^ (e[4] << 17) ^ (e[4] << 20);\n"
    result += "  o[1] ^= (e[3] >> 19) ^ (e[3] >> 17) ^ (e[3] >> 15) ^ (e[3] >> 12);\n"

    result += "\n" 
    result += "  o[0] ^= (e[3] << 13) ^ (e[3] << 15) ^ (e[3] << 17) ^ (e[3] << 20);\n"

    result += "\n" 
    result += "  uint32_t tmp = o[2] >> 19;\n"
    result += "  o[0] ^= (tmp) ^ (tmp << 2) ^ (tmp << 4) ^ (tmp << 7);\n"

    result += "\n" 
    result += "  o[2] &= 0x7FFFF;\n"

    return result



def preprocessing_c32_rbc_elt_reduce_m97():
    result = ""
    result += "  rbc_elt_set(o, e);\n"

    result += "\n" 
    result += "  o[3] ^= (e[6] << 5)  ^ (e[5] >> 27);\n"
    result += "  o[2] ^= (e[6] << 31) ^ (e[5] >> 1) ^ (e[5] << 5) ^ (e[4] >> 27);\n"
    result += "  o[1] ^= (e[5] << 31) ^ (e[4] >> 1) ^ (e[4] << 5) ^ (o[3] >> 27);\n"
    result += "  o[0] ^= (e[4] << 31) ^ (o[3] >> 1) ^ (o[3] >> 1 << 6);\n"

    result += "\n" 
    result += "  o[3] &= 0x1;\n"

    return result



def preprocessing_c32_rbc_elt_reduce_m101():
    result = ""
    result += "  rbc_elt_set(o, e);\n"

    result += "\n"
    result += "  o[3] ^= (e[6] << 1) ^ (e[6] << 2) ^ (e[6] >> 5) ^ (e[6] >> 4);\n"
    result += "  o[2] ^= (e[6] << 27) ^ (e[6] << 28);\n"
    
    result += "\n"
    result += "  o[3] ^= (e[5] >> 31) ^ (e[5] >> 30);\n"
    result += "  o[2] ^= (e[5] << 1) ^ (e[5] << 2) ^ (e[5] >> 5) ^ (e[5] >> 4);\n"
    result += "  o[1] ^= (e[5] << 27) ^ (e[5] << 28);\n"

    result += "\n"
    result += "  o[2] ^= (e[4] >> 31) ^ (e[4] >> 30);\n"
    result += "  o[1] ^= (e[4] << 1) ^ (e[4] << 2) ^ (e[4] >> 5) ^ (e[4] >> 4);\n"
    result += "  o[0] ^= (e[4] << 27) ^ (e[4] << 28);\n"

    result += "\n" 
    result += "  uint32_t tmp = (o[3] >> 5);\n"
    result += "  o[0] ^= (tmp)  ^  (tmp << 1)  ^  (tmp << 6)  ^  (tmp << 7);\n"
    result += "  o[1] ^= (tmp >> 25)  ^  (tmp >> 26);\n"

    result += "\n"
    result += "  o[3] &= 0x0000001f;\n"

    return result



def preprocessing_c32_rbc_elt_reduce_m113():
    result = ""
    result += "  rbc_elt_set(o, e);\n"

    result += "\n"
    result += "  o[3] ^= (e[7] << 15) ^ (e[7] << 24);\n"
    
    result += "\n"
    result += "  o[3] ^= (e[6] >> 17) ^ (e[6] >> 8);\n"
    result += "  o[2] ^= (e[6] << 15) ^ (e[6] << 24);\n"
    
    result += "\n"
    result += "  o[2] ^= (e[5] >> 17) ^ (e[5] >> 8);\n"
    result += "  o[1] ^= (e[5] << 15) ^ (e[5] << 24);\n"
    
    result += "\n"
    result += "  o[1] ^= (e[4] >> 17) ^ (e[4] >> 8);\n"
    result += "  o[0] ^= (e[4] << 15) ^ (e[4] << 24);\n"

    result += "\n" 
    result += "  uint32_t tmp = (o[3] >> 17);\n"
    result += "  o[0] ^= (tmp)  ^  (tmp << 9);\n"

    result += "\n"
    result += "  o[3] &= 0x0001ffff;\n"

    return result



def preprocessing_c32_rbc_elt_reduce_m127():
    result = ""
    result += "  rbc_elt_set(o, e);\n"

    result += "\n" 
    result += "  o[3] ^= (e[7] << 1)  ^ (e[7] << 2);\n"
    result += "  o[3] ^= (e[6] >> 31) ^ (e[6] >> 30);\n"

    result += "\n" 
    result += "  o[2] ^= (e[6] << 1)  ^ (e[6] << 2);\n"
    result += "  o[2] ^= (e[5] >> 31) ^ (e[5] >> 30);\n"

    result += "\n" 
    result += "  o[1] ^= (e[5] << 1)  ^ (e[5] << 2);\n"
    result += "  o[1] ^= (e[4] >> 31) ^ (e[4] >> 30);\n"

    result += "\n" 
    result += "  o[0] ^= (e[4] << 1)  ^ (e[4] << 2);\n"
    result += "  o[0] ^= (o[3] >> 31) ^ (o[3] >> 31 << 1);\n"

    result += "\n" 
    result += "  o[3] &= 0x7fffffff;\n"

    return result



def preprocessing_c32_rbc_elt_reduce_m151():
    result = ""
    result += "  rbc_elt_set(o, e);\n"

    result += "\n" 
    result += "  o[4] ^= (e[9] << 9)  ^ (e[9] << 12);\n"
    result += "  o[4] ^= (e[8] >> 20) ^ (e[8] >> 23);\n"

    result += "\n" 
    result += "  o[3] ^= (e[8] << 9)  ^ (e[8] << 12);\n"
    result += "  o[3] ^= (e[7] >> 20) ^ (e[7] >> 23);\n"

    result += "\n" 
    result += "  o[2] ^= (e[7] << 9)  ^ (e[7] << 12);\n"
    result += "  o[2] ^= (e[6] >> 20) ^ (e[6] >> 23);\n"

    result += "\n" 
    result += "  o[1] ^= (e[6] << 9)  ^ (e[6] << 12);\n"
    result += "  o[1] ^= (e[5] >> 20) ^ (e[5] >> 23);\n"

    result += "\n" 
    result += "  o[0] ^= (e[5] << 9) ^ (e[5] << 12);\n"
    result += "  o[0] ^= (o[4] >> 23 << 3) ^ (o[4] >> 23);\n"

    result += "\n" 
    result += "  o[4] &= 0x7FFFFF;\n"

    return result



def preprocessing_c32_rbc_elt_reduce_m181():
    result = ""
    result += "  rbc_elt_set(o, e);\n"

    result += "\n" 
    result += "  o[5] ^= (e[11] << 11) ^ (e[11] << 12) ^ (e[11] << 17) ^ (e[11] << 18);\n"
    result += "  o[5] ^= (e[10] >> 21) ^ (e[10] >> 20) ^ (e[10] >> 15) ^ (e[10] >> 14);\n"

    result += "\n" 
    result += "  o[4] ^= (e[10] << 11) ^ (e[10] << 12) ^ (e[10] << 17) ^ (e[10] << 18);\n"
    result += "  o[4] ^= (e[9] >> 21)  ^ (e[9] >> 20)  ^ (e[9] >> 15)  ^ (e[9] >> 14);\n"

    result += "\n" 
    result += "  o[3] ^= (e[9] << 11) ^ (e[9] << 12) ^ (e[9] << 17) ^ (e[9] << 18);\n"
    result += "  o[3] ^= (e[8] >> 21) ^ (e[8] >> 20) ^ (e[8] >> 15) ^ (e[8] >> 14);\n"

    result += "\n" 
    result += "  o[2] ^= (e[8] << 11) ^ (e[8] << 12) ^ (e[8] << 17) ^ (e[8] << 18);\n"
    result += "  o[2] ^= (e[7] >> 21) ^ (e[7] >> 20) ^ (e[7] >> 15) ^ (e[7] >> 14);\n"

    result += "\n" 
    result += "  o[1] ^= (e[7] << 11) ^ (e[7] << 12) ^ (e[7] << 17) ^ (e[7] << 18);\n"
    result += "  o[1] ^= (e[6] >> 21) ^ (e[6] >> 20) ^ (e[6] >> 15) ^ (e[6] >> 14);\n"

    result += "\n" 
    result += "  o[0] ^= (e[6] << 11) ^ (e[6] << 12) ^ (e[6] << 17) ^ (e[6] << 18);\n"

    result += "\n" 
    result += "  uint32_t tmp = o[5] >> 21;\n"
    result += "  tmp ^= tmp << 1;\n"
    result += "  tmp ^= tmp << 6;\n"
    result += "  o[0] ^= tmp;\n"

    result += "\n" 
    result += "  o[5] &= 0x1FFFFF;\n"

    return result



def preprocessing_c64_rbc_elt_reduce_m61():
    result = ""

    result += "  uint64_t tmp = (e[1] << 3) ^ (e[0] >> 61) ^ (e[1] >> 53) ^ (e[1] >> 56);\n"
    result += "  o[0] = e[0] ^ tmp ^ (tmp << 1) ^ (tmp << 2) ^ (tmp << 5);\n"
    result += "  o[0] &= 0x1FFFFFFFFFFFFFFF;\n"

    return result



def preprocessing_c64_rbc_elt_reduce_m67():
    result = ""

    result += "  uint64_t tmp = (e[1] >> 62) ^ (e[2] << 2);\n"
    result += "  o[1] = e[1] ^ tmp ^ (tmp >> 3) ^ (tmp >> 4) ^ (tmp >> 5);\n"

    result += "\n"
    result += "  tmp = (o[1] >> 3) ^ (e[2] << 61);\n"
    result += "  o[0] = e[0] ^ tmp ^ (tmp << 1) ^ (tmp << 2) ^ (tmp << 5);\n"

    result += "\n"
    result += "  o[1] &= 0x0000000000000007;\n"

    return result



def preprocessing_c64_rbc_elt_reduce_m79():
    result = ""

    result += "  uint64_t tmp = e[2] >> 6;\n"
    result += "  o[1] = e[1] ^ tmp ^ (tmp >> 9);\n"

    result += "\n"
    result += "  tmp = (o[1] >> 15) ^ (e[2] << 49);\n"
    result += "  o[0] = e[0] ^ tmp ^ (tmp << 9);\n"

    result += "\n"
    result += "  o[1] &= 0x0000000000007FFF;\n"

    return result



def preprocessing_c64_rbc_elt_reduce_m83():
    result = ""

    result += "  uint64_t tmp = e[2] >> 12;\n"
    result += "  o[1] = e[1] ^ tmp ^ (tmp >> 3) ^ (tmp >> 5) ^ (tmp >> 7);\n"

    result += "\n"
    result += "  tmp = (o[1] >> 19) ^ (e[2] << 45);\n"
    result += "  o[0] = e[0] ^ tmp ^ (tmp << 2) ^ (tmp << 4) ^ (tmp << 7);\n"

    result += "\n"
    result += "  o[1] &= 0x000000000007FFFF;\n"

    return result



def preprocessing_c64_rbc_elt_reduce_m97():
    result = ""

    result += "\n"
    result += "  uint64_t tmp = (e[2] >> 27) ^ (e[3] << 37);\n"
    result += "  o[1] = e[1] ^ tmp ^ (tmp >> 6);\n"

    result += "\n"
    result += "  tmp = (o[1] >> 33) ^ (e[2] << 31);\n"
    result += "  o[0] = e[0] ^ tmp ^ (tmp << 6);\n"

    result += "\n"
    result += "  o[1] &= 0x00000001FFFFFFFF;\n"

    return result



def preprocessing_c64_rbc_elt_reduce_m101():
    result = ""

    result += "  uint64_t tmp = (e[2] >> 30) ^ (e[3] << 34);\n"
    result += "  o[1] = e[1] ^ tmp ^ (tmp >> 1) ^ (tmp >> 6) ^ (tmp >> 7);\n"

    result += "\n"
    result += "  tmp = (o[1] >> 37) ^ (e[2] << 27);\n"
    result += "  o[0] = e[0] ^ tmp ^ (tmp << 1) ^ (tmp << 6) ^ (tmp << 7);\n"

    result += "\n"
    result += "  o[1] &= 0x0000001FFFFFFFFF;\n"

    return result



def preprocessing_c64_rbc_elt_reduce_m113():
    result = ""

    result += "  uint64_t tmp = (e[2] >> 40) ^ (e[3] << 24);\n"
    result += "  o[1] = e[1] ^ tmp ^ (tmp >> 9);\n"

    result += "\n"
    result += "  tmp = (o[1] >> 49) ^ (e[2] << 15);\n"
    result += "  o[0] = e[0] ^ tmp ^ (tmp << 9);\n"

    result += "\n"
    result += "  o[1] &= 0x0001FFFFFFFFFFFF;\n"

    return result



def preprocessing_c64_rbc_elt_reduce_m127():
    result = ""

    result += "  uint64_t tmp = (e[2] >> 62) ^ (e[3] << 2);\n"
    result += "  o[1] = e[1] ^ tmp ^ (tmp >> 1);\n"

    result += "\n"
    result += "  tmp = (o[1] >> 63) ^ (e[2] << 1);\n"
    result += "  o[0] = e[0] ^ tmp ^ (tmp << 1);\n"

    result += "\n"
    result += "  o[1] &= 0x7FFFFFFFFFFFFFFF;\n"

    return result



def preprocessing_c64_rbc_elt_reduce_m151():
    result = ""

    result += "\n"
    result += "  uint64_t tmp = e[4] >> 20;\n"
    result += "  o[2] = e[2] ^ tmp ^ (tmp >> 3);\n"

    result += "\n"
    result += "  tmp = e[3] >> 20;\n"
    result += "  o[1] = e[1] ^ tmp ^ (tmp >> 3);\n"

    result += "  tmp = e[4] << 41;\n"
    result += "  o[1] ^= tmp ^ (tmp << 3);\n"

    result += "\n"
    result += "  tmp = (o[2] >> 23) ^ (e[3] << 41);\n"
    result += "  o[0] = e[0] ^ tmp ^ (tmp << 3);\n"

    result += "\n"
    result += "  o[2] &= 0x00000000007FFFFF;\n"

    return result



def preprocessing_c64_rbc_elt_reduce_m181():
    result = ""

    result += "  uint64_t tmp = e[5] << 11;\n"
    result += "  uint64_t tmp2 = e[4] >> 46;\n"

    result += "\n"
    result += "  o[2] = e[2]\n"
    result += "    ^ tmp ^ (tmp << 1) ^ (tmp << 6) ^ (tmp << 7)\n"
    result += "    ^ tmp2 ^ (tmp2 >> 1) ^ (tmp2 >> 6) ^ (tmp2 >> 7);\n"

    result += "\n"
    result += "  tmp = e[4] << 11;\n"
    result += "  tmp2 = e[3] >> 46;\n"

    result += "\n"
    result += "  o[1] = e[1]\n"
    result += "    ^ tmp ^ (tmp << 1) ^ (tmp << 6) ^ (tmp << 7)\n"
    result += "    ^ tmp2 ^ (tmp2 >> 1) ^ (tmp2 >> 6) ^ (tmp2 >> 7);\n"

    result += "\n"
    result += "  tmp = (e[3] << 11) ^ (o[2] >> 53);\n"
    result += "  o[0] = e[0] ^ tmp ^ (tmp << 1) ^ (tmp << 6) ^ (tmp << 7);\n"

    result += "\n"
    result += "  o[2] &= 0x001FFFFFFFFFFFFF;\n"

    return result



def preprocessing_rbc_elt_reduce_generic(m, word_length):
    monomials = rbc_core_utils_get_polynomial(m)

    # Number of bits in an unreduced element
    ur_len = 2 * (m - 1) + 1

    # Index of the rightmost block
    ind_max = floor(ur_len / word_length)

    # Index of the leftmost block
    ind_min = floor(m / word_length)

    reductions = {}
    for i in range(ind_max+1):
        reductions[i] = []

    for i in range(ind_max, ind_min, -1):
        base_coeff = i * word_length - m
        for monomial in monomials:
            coeff = base_coeff + monomial
            position = coeff // word_length
            left_shift = coeff % word_length

            reductions[position].append((i, "left", left_shift))

            if left_shift != 0:
                reductions[position + 1].append((i, "right", word_length - left_shift))

    result = "  rbc_elt_ur e2;\n"
    result += "  rbc_elt_ur_set(e2, e);\n"

    for i in range(ind_max, -1, -1):
        append = ""
        for (index, direction, shiftlen) in reductions[i]:
            if direction == "left":
                arrows = "<<"
            else:
                arrows = ">>"

            if append == "":
                append = "(e2[{}] {} {})".format(str(index), arrows, str(shiftlen))
            else:
                append += " ^ (e2[{}] {} {})".format(str(index), arrows, str(shiftlen))

        if append != "":
            result += "\n  e2[{}] ^= ".format(str(i)) + append + ";\n"

    # Last block reductions
    excess_bits = word_length - (m % word_length)

    result += "\n  uint{}_t tmp = (e2[{}] >> {});\n".format(word_length, ind_min, m % word_length)

    final_reductions = {}
    for i in range(ind_min+1):
        final_reductions[i] = []
    for monomial in monomials:
        position = monomial // word_length
        index = monomial % word_length
        
        final_reductions[position].append(("left", index))

        if index + excess_bits > word_length:
            final_reductions[position + 1].append(("right", word_length - index))

    for i in range(len(final_reductions)):
        append = ""
        if final_reductions[i] != []:
            for (direction, shift) in final_reductions[i]:
                if direction == "left":
                    arrows = "<<"
                else:
                    arrows = ">>"
                if append == "":
                    append = "(tmp {} {})".format(arrows, str(shift))
                else:
                    append += " ^ (tmp {} {})".format(arrows, str(shift))

            result += "  e2[{}] ^= {};\n".format(i, append)

    result += "\n  rbc_elt_set(o, e2);\n"
    result += "\n  o[" + str(ind_min) + "] &= 0x{0:X};\n".format(2**(m % word_length) - 1);

    return result
