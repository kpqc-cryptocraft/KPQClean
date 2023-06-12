
import os
from math import ceil

polynomials = {}

def rbc_core_utils_get_size(arch, m):
    if(arch == "c32"): 
        return ceil(m / 32);

    if(arch == "c64"): 
        return ceil(m / 64);

    if(arch == "avx"):
        return 2 * ceil(m / 128);



def rbc_core_utils_get_data_size(arch, m):
    if(arch == "c32"): 
        return rbc_core_utils_get_size(arch, m);

    if(arch == "c64"): 
        return rbc_core_utils_get_size(arch, m);

    if(arch == "avx"):
        if(rbc_core_utils_get_size(arch, m) == ceil(m / 64)):
            return rbc_core_utils_get_size(arch, m);
        else:
            return rbc_core_utils_get_size(arch, m) - 1;



def rbc_core_utils_get_uint8_size(arch, m):
    return ceil(m / 8);



def rbc_core_utils_get_ur_size(arch, m):
    if(arch == "c32"): 
        return ceil((2 * m - 1) / 32)

    if(arch == "c64"): 
        return ceil((2 * m - 1) / 64)

    if(arch == "avx"):
        return 2 * ceil((2 * m - 1) / 128)



def rbc_core_utils_get_ur_data_size(arch, m):
    if(arch == "c32"): 
        return rbc_core_utils_get_ur_size(arch, m);

    if(arch == "c64"): 
        return rbc_core_utils_get_ur_size(arch, m);

    if(arch == "avx"):
        if(rbc_core_utils_get_ur_size(arch, m) == ceil((2 * m - 1) / 64)):
            return rbc_core_utils_get_ur_size(arch, m);
        else:
            return rbc_core_utils_get_ur_size(arch, m) - 1;



def rbc_core_utils_get_ur_uint8_size(arch, m):
    return ceil((2 * m - 1) / 8);



def rbc_core_utils_get_register_size(arch, m):
    if(arch == "c32"):
        return 32;
    else:
        return 64;



def rbc_core_utils_load_polynomials():
	root_path = os.path.dirname(os.path.realpath(__file__)) + "/../.."
	poly_database = root_path + "/src/core/polynomial_db"

	if polynomials == {}:
		file = open(poly_database, "r")

		for line in file:
			coeffs = line.rstrip().split(" ")
			polynomials[int(coeffs[0])] = []

			for coeff in coeffs[1:]:
				polynomials[int(coeffs[0])].append(int(coeff))

		file.close()



def rbc_core_utils_get_hex_polynomial(word_length, m):
    rbc_core_utils_load_polynomials()

    word_number = (m // word_length) + 1
    integers = []
    for i in range(word_number):
        integers.append(0)

    for coeff in polynomials[m]:
        index = coeff // word_length
        pos = coeff % word_length

        integers[index] = integers[index] + 2**pos

    index = m // word_length
    pos = m % word_length

    integers[index] = integers[index] + 2**pos

    ret_value = "{0x"
    if word_length == 32:
        ret_value = ret_value + "{0:08x}".format(integers[0])
    else:
        ret_value = ret_value + "{0:016x}".format(integers[0])

    for i in range(1, word_number):
        ret_value = ret_value + ", 0x"
        if word_length == 32:
            ret_value = ret_value + "{0:08x}".format(integers[i])
        else:
            ret_value = ret_value + "{0:016x}".format(integers[i])

    ret_value = ret_value + "}"

    return ret_value



def rbc_core_utils_get_partial_hex_polynomial(word_length, m, word_number, shift):
    rbc_core_utils_load_polynomials()

    integers = []
    for i in range(word_number):
        integers.append(0)

    for coeff in polynomials[m]:
        coeff += shift
        index = coeff // word_length
        pos = coeff % word_length

        integers[index] = integers[index] + 2**pos

    ret_value = "{0x"
    if word_length == 32:
        ret_value = ret_value + "{0:08x}".format(integers[0])
    else:
        ret_value = ret_value + "{0:016x}".format(integers[0])

    for i in range(1, word_number):
        ret_value = ret_value + ", 0x"
        if word_length == 32:
            ret_value = ret_value + "{0:08x}".format(integers[i])
        else:
            ret_value = ret_value + "{0:016x}".format(integers[i])

    ret_value = ret_value + "}"

    return ret_value



def rbc_core_utils_get_polynomial(m):
    rbc_core_utils_load_polynomials()
    return polynomials[m]

