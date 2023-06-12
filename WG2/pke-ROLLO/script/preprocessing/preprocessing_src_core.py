
import os
import subprocess
import sys
import yaml
from math import ceil

root_path = os.path.dirname(os.path.realpath(__file__)) + "/../.."
core_path = root_path + "/src/core"
script_path = root_path + "/script"
preprocessing_rbc_elt_path = root_path + "/src/core/rbc_elt"
preprocessing_rbc_qre_path = root_path + "/src/core/rbc_qre"
templating_path = root_path + "/build/templating"

sys.path.insert(1, script_path)
sys.path.insert(1, core_path)
sys.path.insert(1, preprocessing_rbc_elt_path)
sys.path.insert(1, preprocessing_rbc_qre_path)

from rbc_h                          import *
from rbc_field_init                 import *
from rbc_elt_set_mask1              import *
from rbc_elt_set_mask2              import *
from rbc_elt_set_from_uint64        import *
from rbc_elt_set                    import *
from rbc_elt_get_degree             import *
from rbc_elt_get_coefficient        import *
from rbc_elt_set_coefficient        import *
from rbc_elt_add                    import *
from rbc_elt_reduce                 import *
from rbc_elt_print                  import *
from rbc_elt_ur_set                 import *
from rbc_elt_ur_set_from_uint64     import *
from rbc_elt_ur_mul                 import *
from rbc_elt_ur_sqr                 import *
from rbc_elt_ur_print               import *
from rbc_qre_get_modulus            import *
from rbc_utils                      import *



### Core preprocessing function
def preprocessing_src_core(arch, m, n_list):
    subprocess.run("mkdir -p " + root_path + "/build/src/core/" + arch + "/rbc-" + str(m), shell=True)

    copy_file(root_path + "/src/core/CMakeLists.txt", root_path + "/build/src/core/" + arch + "/rbc-" + str(m))
    replace_in_file(root_path + "/build/src/core/" + arch + "/rbc-" + str(m) + "/CMakeLists.txt", "core", "core_" + str(m));
    replace_in_file(root_path + "/build/src/core/" + arch + "/rbc-" + str(m) + "/CMakeLists.txt", "rbc", "rbc_" + str(m));

    preprocessing_src_core_header(arch, m)
    preprocessing_src_core_elt(arch, m)
    preprocessing_src_core_vec(arch, m)
    preprocessing_src_core_poly(arch, m)
    preprocessing_src_core_qre(arch, m, n_list)
    preprocessing_src_core_vspace(arch, m)



### Core header preprocessing function
def preprocessing_src_core_header(arch, m):
    # Generate rbc.h file to build folder 
    output_path = root_path + "/build/src/core/" + arch + "/rbc-" + str(m)
    write_in_file(output_path + "/rbc_" + str(m) + ".h", preprocessing_rbc_h(arch, m))

    # Preprocessing adding parameter m to files
    replace_in_file(output_path + "/rbc_" + str(m) + ".h", "rbc", "rbc_" + str(m));
    replace_in_file(output_path + "/rbc_" + str(m) + ".h", "RBC", "RBC_" + str(m));



### Core elt preprocessing function
def preprocessing_src_core_elt(arch, m):
    # Copy files from templating folder to build folder 
    output_path = root_path + "/build/src/core/" + arch + "/rbc-" + str(m)
    copy_file(templating_path + "/core/rbc_elt.c", output_path + "/rbc_elt.c")
    copy_file(templating_path + "/core/rbc_elt.h", output_path + "/rbc_elt.h")

    # Preprocessing replacing templates by specific implementation
    output_file1 = output_path + "/rbc_elt.c"
    output_file2 = output_path + "/rbc_elt.h"

    replace_in_file(output_file1, "{TEMPLATING_RBC_ELT_FIELD_INIT}",                        preprocessing_rbc_elt_field_init(arch, m))
    replace_in_file(output_file1, "{TEMPLATING_RBC_ELT_SET}",                               preprocessing_rbc_elt_set(arch, m))
    replace_in_file(output_file1, "{TEMPLATING_RBC_ELT_SET_MASK1}",                         preprocessing_rbc_elt_set_mask1(arch, m))
    replace_in_file(output_file1, "{TEMPLATING_RBC_ELT_SET_MASK2}",                         preprocessing_rbc_elt_set_mask2(arch, m))
    replace_in_file(output_file1, "{TEMPLATING_RBC_ELT_GET_DEGREE}",                        preprocessing_rbc_elt_get_degree(arch, m))
    replace_in_file(output_file1, "{TEMPLATING_RBC_ELT_PRINT}",                             preprocessing_rbc_elt_print(arch, m))
    replace_in_file(output_file1, "{TEMPLATING_RBC_ELT_UR_PRINT}",                          preprocessing_rbc_elt_ur_print(arch, m))
    replace_in_file(output_file1, "{TEMPLATING_RBC_ELT_GET_COEFFICIENT}",                   preprocessing_rbc_elt_get_coefficient(arch, m))
    replace_in_file(output_file1, "{TEMPLATING_RBC_ELT_SET_COEFFICIENT}",                   preprocessing_rbc_elt_set_coefficient(arch, m))
    replace_in_file(output_file1, "{TEMPLATING_RBC_ELT_ADD}",                               preprocessing_rbc_elt_add(arch, m))
    replace_in_file(output_file1, "{TEMPLATING_RBC_ELT_REDUCE}",                            preprocessing_rbc_elt_reduce(arch, m))
    replace_in_file(output_file1, "{TEMPLATING_RBC_ELT_UR_SET}",                            preprocessing_rbc_elt_ur_set(arch, m))
    replace_in_file(output_file1, "{TEMPLATING_RBC_ELT_UR_MUL}",                            preprocessing_rbc_elt_ur_mul(arch, m))
    replace_in_file(output_file1, "{TEMPLATING_RBC_ELT_UR_SQR}",                            preprocessing_rbc_elt_ur_sqr(arch, m))
    replace_in_file(output_file1, "{TEMPLATING_RBC_ELT_SET_FROM_UINT64}",                   preprocessing_rbc_elt_set_from_uint64(arch, m))
    replace_in_file(output_file1, "{TEMPLATING_RBC_ELT_UR_SET_FROM_UINT64}",                preprocessing_rbc_elt_ur_set_from_uint64(arch, m))

    # Preprocessing replacing templates by generic implementation
    replace_in_file(output_file1, "{TEMPLATING_RBC_ELT_SET_ONE}",                           read_file(core_path + "/rbc_elt/rbc_elt_set_one.c"))
    replace_in_file(output_file1, "{TEMPLATING_RBC_ELT_SET_RANDOM}",                        read_file(core_path + "/rbc_elt/rbc_elt_set_random.c"))
    replace_in_file(output_file1, "{TEMPLATING_RBC_ELT_SET_ZERO}",                          read_file(core_path + "/rbc_elt/rbc_elt_set_zero.c"))
    replace_in_file(output_file1, "{TEMPLATING_RBC_ELT_IS_EQUAL_TO}",                       read_file(core_path + "/rbc_elt/rbc_elt_is_equal_to.c"))
    replace_in_file(output_file1, "{TEMPLATING_RBC_ELT_IS_ZERO}",                           read_file(core_path + "/rbc_elt/rbc_elt_is_zero.c"))
    replace_in_file(output_file1, "{TEMPLATING_RBC_ELT_IS_GREATER_THAN}",                   read_file(core_path + "/rbc_elt/rbc_elt_is_greater_than.c"))
    replace_in_file(output_file1, "{TEMPLATING_RBC_ELT_MUL}",                               read_file(core_path + "/rbc_elt/rbc_elt_mul.c"))
    replace_in_file(output_file1, "{TEMPLATING_RBC_ELT_SQR}",                               read_file(core_path + "/rbc_elt/rbc_elt_sqr.c"))
    replace_in_file(output_file1, "{TEMPLATING_RBC_ELT_NTH_ROOT}",                          read_file(core_path + "/rbc_elt/rbc_elt_nth_root.c"))
    replace_in_file(output_file1, "{TEMPLATING_RBC_ELT_INV}",                               read_file(core_path + "/rbc_elt/rbc_elt_inv.c"))
    replace_in_file(output_file1, "{TEMPLATING_RBC_ELT_UR_SET_ZERO}",                       read_file(core_path + "/rbc_elt/rbc_elt_ur_set_zero.c"))

    replace_in_file(output_file2, "{TEMPLATING_RBC_ELT_SET_ZERO}",                          read_file(core_path + "/rbc_elt/rbc_elt_set_zero.h"))
    replace_in_file(output_file2, "{TEMPLATING_RBC_ELT_SET_ONE}",                           read_file(core_path + "/rbc_elt/rbc_elt_set_one.h"))
    replace_in_file(output_file2, "{TEMPLATING_RBC_ELT_SET}",                               read_file(core_path + "/rbc_elt/rbc_elt_set.h"))
    replace_in_file(output_file2, "{TEMPLATING_RBC_ELT_SET_MASK1}",                         read_file(core_path + "/rbc_elt/rbc_elt_set_mask1.h"))
    replace_in_file(output_file2, "{TEMPLATING_RBC_ELT_SET_MASK2}",                         read_file(core_path + "/rbc_elt/rbc_elt_set_mask2.h"))
    replace_in_file(output_file2, "{TEMPLATING_RBC_ELT_SET_FROM_UINT64}",                   read_file(core_path + "/rbc_elt/rbc_elt_set_from_uint64.h"))
    replace_in_file(output_file2, "{TEMPLATING_RBC_ELT_SET_RANDOM}",                        read_file(core_path + "/rbc_elt/rbc_elt_set_random.h"))
    replace_in_file(output_file2, "{TEMPLATING_RBC_ELT_GET_DEGREE}",                        read_file(core_path + "/rbc_elt/rbc_elt_get_degree.h"))
    replace_in_file(output_file2, "{TEMPLATING_RBC_ELT_GET_COEFFICIENT}",                   read_file(core_path + "/rbc_elt/rbc_elt_get_coefficient.h"))
    replace_in_file(output_file2, "{TEMPLATING_RBC_ELT_SET_COEFFICIENT}",                   read_file(core_path + "/rbc_elt/rbc_elt_set_coefficient.h"))
    replace_in_file(output_file2, "{TEMPLATING_RBC_ELT_IS_ZERO}",                           read_file(core_path + "/rbc_elt/rbc_elt_is_zero.h"))
    replace_in_file(output_file2, "{TEMPLATING_RBC_ELT_IS_EQUAL_TO}",                       read_file(core_path + "/rbc_elt/rbc_elt_is_equal_to.h"))
    replace_in_file(output_file2, "{TEMPLATING_RBC_ELT_IS_GREATER_THAN}",                   read_file(core_path + "/rbc_elt/rbc_elt_is_greater_than.h"))
    replace_in_file(output_file2, "{TEMPLATING_RBC_ELT_ADD}",                               read_file(core_path + "/rbc_elt/rbc_elt_add.h"))
    replace_in_file(output_file2, "{TEMPLATING_RBC_ELT_MUL}",                               read_file(core_path + "/rbc_elt/rbc_elt_mul.h"))
    replace_in_file(output_file2, "{TEMPLATING_RBC_ELT_INV}",                               read_file(core_path + "/rbc_elt/rbc_elt_inv.h"))
    replace_in_file(output_file2, "{TEMPLATING_RBC_ELT_SQR}",                               read_file(core_path + "/rbc_elt/rbc_elt_sqr.h"))
    replace_in_file(output_file2, "{TEMPLATING_RBC_ELT_NTH_ROOT}",                          read_file(core_path + "/rbc_elt/rbc_elt_nth_root.h"))
    replace_in_file(output_file2, "{TEMPLATING_RBC_ELT_REDUCE}",                            read_file(core_path + "/rbc_elt/rbc_elt_reduce.h"))
    replace_in_file(output_file2, "{TEMPLATING_RBC_ELT_PRINT}",                             read_file(core_path + "/rbc_elt/rbc_elt_print.h"))
    replace_in_file(output_file2, "{TEMPLATING_RBC_ELT_UR_SET}",                            read_file(core_path + "/rbc_elt/rbc_elt_ur_set.h"))
    replace_in_file(output_file2, "{TEMPLATING_RBC_ELT_UR_SET_ZERO}",                       read_file(core_path + "/rbc_elt/rbc_elt_ur_set_zero.h"))
    replace_in_file(output_file2, "{TEMPLATING_RBC_ELT_UR_SET_FROM_UINT64}",                read_file(core_path + "/rbc_elt/rbc_elt_ur_set_from_uint64.h"))
    replace_in_file(output_file2, "{TEMPLATING_RBC_ELT_UR_MUL}",                            read_file(core_path + "/rbc_elt/rbc_elt_ur_mul.h"))
    replace_in_file(output_file2, "{TEMPLATING_RBC_ELT_UR_SQR}",                            read_file(core_path + "/rbc_elt/rbc_elt_ur_sqr.h"))
    replace_in_file(output_file2, "{TEMPLATING_RBC_ELT_UR_PRINT}",                          read_file(core_path + "/rbc_elt/rbc_elt_ur_print.h"))

    # Preprocessing adding parameter m to files
    rename_file(output_path + "/rbc_elt.h", output_path + "/rbc_" + str(m) + "_elt.h")
    replace_in_file(output_path + "/rbc_" + str(m) + "_elt.h", "rbc", "rbc_" + str(m));
    replace_in_file(output_path + "/rbc_" + str(m) + "_elt.h", "RBC", "RBC_" + str(m));

    rename_file(output_path + "/rbc_elt.c", output_path + "/rbc_" + str(m) + "_elt.c")
    replace_in_file(output_path + "/rbc_" + str(m) + "_elt.c", "rbc", "rbc_" + str(m));
    replace_in_file(output_path + "/rbc_" + str(m) + "_elt.c", "RBC", "RBC_" + str(m));



### Core vec preprocessing function
def preprocessing_src_core_vec(arch, m):
    # Copy files from templating folder to build folder 
    output_path = root_path + "/build/src/core/" + arch + "/rbc-" + str(m)
    copy_file(templating_path + "/core/rbc_vec.c", output_path + "/rbc_vec.c")
    copy_file(templating_path + "/core/rbc_vec.h", output_path + "/rbc_vec.h")

    # Preprocessing replacing templates by generic implementation
    output_file1 = output_path + "/rbc_vec.c"
    replace_in_file(output_file1, "{TEMPLATING_RBC_VEC_INIT}",                              read_file(core_path + "/rbc_vec/rbc_vec_init.c"))
    replace_in_file(output_file1, "{TEMPLATING_RBC_VEC_CLEAR}",                             read_file(core_path + "/rbc_vec/rbc_vec_clear.c"))
    replace_in_file(output_file1, "{TEMPLATING_RBC_VEC_SET_ZERO}",                          read_file(core_path + "/rbc_vec/rbc_vec_set_zero.c"))
    replace_in_file(output_file1, "{TEMPLATING_RBC_VEC_SET}",                               read_file(core_path + "/rbc_vec/rbc_vec_set.c"))
    replace_in_file(output_file1, "{TEMPLATING_RBC_VEC_SET_RANDOM}",                        read_file(core_path + "/rbc_vec/rbc_vec_set_random.c"))
    replace_in_file(output_file1, "{TEMPLATING_RBC_VEC_SET_RANDOM_FULL_RANK}",              read_file(core_path + "/rbc_vec/rbc_vec_set_random_full_rank.c"))
    replace_in_file(output_file1, "{TEMPLATING_RBC_VEC_SET_RANDOM_FULL_RANK_WITH_ONE}",     read_file(core_path + "/rbc_vec/rbc_vec_set_random_full_rank_with_one.c"))
    replace_in_file(output_file1, "{TEMPLATING_RBC_VEC_SET_RANDOM_FROM_SUPPORT}",           read_file(core_path + "/rbc_vec/rbc_vec_set_random_from_support.c"))
    replace_in_file(output_file1, "{TEMPLATING_RBC_VEC_SET_RANDOM_PAIR_FROM_SUPPORT}",      read_file(core_path + "/rbc_vec/rbc_vec_set_random_pair_from_support.c"))
    replace_in_file(output_file1, "{TEMPLATING_RBC_VEC_GAUSS}",                             read_file(core_path + "/rbc_vec/rbc_vec_gauss.c"))
    replace_in_file(output_file1, "{TEMPLATING_RBC_VEC_GET_RANK}",                          read_file(core_path + "/rbc_vec/rbc_vec_get_rank.c"))
    replace_in_file(output_file1, "{TEMPLATING_RBC_VEC_ECHELONIZE}",                        read_file(core_path + "/rbc_vec/rbc_vec_echelonize.c"))
    replace_in_file(output_file1, "{TEMPLATING_RBC_VEC_ADD}",                               read_file(core_path + "/rbc_vec/rbc_vec_add.c"))
    replace_in_file(output_file1, "{TEMPLATING_RBC_VEC_SCALAR_MUL}",                        read_file(core_path + "/rbc_vec/rbc_vec_scalar_mul.c"))
    replace_in_file(output_file1, "{TEMPLATING_RBC_VEC_TO_STRING}",                         read_file(core_path + "/rbc_vec/rbc_vec_to_string.c"))
    replace_in_file(output_file1, "{TEMPLATING_RBC_VEC_FROM_STRING}",                       read_file(core_path + "/rbc_vec/rbc_vec_from_string.c"))
    replace_in_file(output_file1, "{TEMPLATING_RBC_VEC_PRINT}",                             read_file(core_path + "/rbc_vec/rbc_vec_print.c"))

    output_file2 = output_path + "/rbc_vec.h"
    replace_in_file(output_file2, "{TEMPLATING_RBC_VEC_INIT}",                              read_file(core_path + "/rbc_vec/rbc_vec_init.h"))
    replace_in_file(output_file2, "{TEMPLATING_RBC_VEC_CLEAR}",                             read_file(core_path + "/rbc_vec/rbc_vec_clear.h"))
    replace_in_file(output_file2, "{TEMPLATING_RBC_VEC_SET_ZERO}",                          read_file(core_path + "/rbc_vec/rbc_vec_set_zero.h"))
    replace_in_file(output_file2, "{TEMPLATING_RBC_VEC_SET}",                               read_file(core_path + "/rbc_vec/rbc_vec_set.h"))
    replace_in_file(output_file2, "{TEMPLATING_RBC_VEC_SET_RANDOM}",                        read_file(core_path + "/rbc_vec/rbc_vec_set_random.h"))
    replace_in_file(output_file2, "{TEMPLATING_RBC_VEC_SET_RANDOM_FULL_RANK}",              read_file(core_path + "/rbc_vec/rbc_vec_set_random_full_rank.h"))
    replace_in_file(output_file2, "{TEMPLATING_RBC_VEC_SET_RANDOM_FULL_RANK_WITH_ONE}",     read_file(core_path + "/rbc_vec/rbc_vec_set_random_full_rank_with_one.h"))
    replace_in_file(output_file2, "{TEMPLATING_RBC_VEC_SET_RANDOM_FROM_SUPPORT}",           read_file(core_path + "/rbc_vec/rbc_vec_set_random_from_support.h"))
    replace_in_file(output_file2, "{TEMPLATING_RBC_VEC_SET_RANDOM_PAIR_FROM_SUPPORT}",      read_file(core_path + "/rbc_vec/rbc_vec_set_random_pair_from_support.h"))
    replace_in_file(output_file2, "{TEMPLATING_RBC_VEC_GAUSS}",                             read_file(core_path + "/rbc_vec/rbc_vec_gauss.h"))
    replace_in_file(output_file2, "{TEMPLATING_RBC_VEC_GET_RANK}",                          read_file(core_path + "/rbc_vec/rbc_vec_get_rank.h"))
    replace_in_file(output_file2, "{TEMPLATING_RBC_VEC_ECHELONIZE}",                        read_file(core_path + "/rbc_vec/rbc_vec_echelonize.h"))
    replace_in_file(output_file2, "{TEMPLATING_RBC_VEC_ADD}",                               read_file(core_path + "/rbc_vec/rbc_vec_add.h"))
    replace_in_file(output_file2, "{TEMPLATING_RBC_VEC_SCALAR_MUL}",                        read_file(core_path + "/rbc_vec/rbc_vec_scalar_mul.h"))
    replace_in_file(output_file2, "{TEMPLATING_RBC_VEC_TO_STRING}",                         read_file(core_path + "/rbc_vec/rbc_vec_to_string.h"))
    replace_in_file(output_file2, "{TEMPLATING_RBC_VEC_FROM_STRING}",                       read_file(core_path + "/rbc_vec/rbc_vec_from_string.h"))
    replace_in_file(output_file2, "{TEMPLATING_RBC_VEC_PRINT}",                             read_file(core_path + "/rbc_vec/rbc_vec_print.h"))

    # Preprocessing adding parameter m to files
    rename_file(output_path + "/rbc_vec.h", output_path + "/rbc_" + str(m) + "_vec.h")
    replace_in_file(output_path + "/rbc_" + str(m) + "_vec.h", "rbc", "rbc_" + str(m));
    replace_in_file(output_path + "/rbc_" + str(m) + "_vec.h", "RBC", "RBC_" + str(m));

    rename_file(output_path + "/rbc_vec.c", output_path + "/rbc_" + str(m) + "_vec.c")
    replace_in_file(output_path + "/rbc_" + str(m) + "_vec.c", "rbc", "rbc_" + str(m));
    replace_in_file(output_path + "/rbc_" + str(m) + "_vec.c", "RBC", "RBC_" + str(m));



### Core poly preprocessing function
def preprocessing_src_core_poly(arch, m):
    # Copy files from templating folder to build folder 
    output_path = root_path + "/build/src/core/" + arch + "/rbc-" + str(m)
    copy_file(templating_path + "/core/rbc_poly.c", output_path + "/rbc_poly.c")
    copy_file(templating_path + "/core/rbc_poly.h", output_path + "/rbc_poly.h")

    # Preprocessing replacing templates by generic implementation
    output_file1 = output_path + "/rbc_poly.c"
    replace_in_file(output_file1, "{TEMPLATING_RBC_POLY_INIT}",                              read_file(core_path + "/rbc_poly/rbc_poly_init.c"))
    replace_in_file(output_file1, "{TEMPLATING_RBC_POLY_CLEAR}",                             read_file(core_path + "/rbc_poly/rbc_poly_clear.c"))
    replace_in_file(output_file1, "{TEMPLATING_RBC_POLY_SPARSE_INIT}",                       read_file(core_path + "/rbc_poly/rbc_poly_sparse_init.c"))
    replace_in_file(output_file1, "{TEMPLATING_RBC_POLY_SPARSE_CLEAR}",                      read_file(core_path + "/rbc_poly/rbc_poly_sparse_clear.c"))
    replace_in_file(output_file1, "{TEMPLATING_RBC_POLY_SET_ZERO}",                          read_file(core_path + "/rbc_poly/rbc_poly_set_zero.c"))
    replace_in_file(output_file1, "{TEMPLATING_RBC_POLY_SET}",                               read_file(core_path + "/rbc_poly/rbc_poly_set.c"))
    replace_in_file(output_file1, "{TEMPLATING_RBC_POLY_SET_RANDOM}",                        read_file(core_path + "/rbc_poly/rbc_poly_set_random.c"))
    replace_in_file(output_file1, "{TEMPLATING_RBC_POLY_SET_RANDOM_FULL_RANK}",              read_file(core_path + "/rbc_poly/rbc_poly_set_random_full_rank.c"))
    replace_in_file(output_file1, "{TEMPLATING_RBC_POLY_SET_RANDOM_FULL_RANK_WITH_ONE}",     read_file(core_path + "/rbc_poly/rbc_poly_set_random_full_rank_with_one.c"))
    replace_in_file(output_file1, "{TEMPLATING_RBC_POLY_SET_RANDOM_FROM_SUPPORT}",           read_file(core_path + "/rbc_poly/rbc_poly_set_random_from_support.c"))
    replace_in_file(output_file1, "{TEMPLATING_RBC_POLY_SET_RANDOM_PAIR_FROM_SUPPORT}",      read_file(core_path + "/rbc_poly/rbc_poly_set_random_pair_from_support.c"))
    replace_in_file(output_file1, "{TEMPLATING_RBC_POLY_IS_EQUAL_TO}",                       read_file(core_path + "/rbc_poly/rbc_poly_is_equal_to.c"))
    replace_in_file(output_file1, "{TEMPLATING_RBC_POLY_ADD}",                               read_file(core_path + "/rbc_poly/rbc_poly_add.c"))
    replace_in_file(output_file1, "{TEMPLATING_RBC_POLY_MUL}",                               read_file(core_path + "/rbc_poly/rbc_poly_mul.c"))
    replace_in_file(output_file1, "{TEMPLATING_RBC_POLY_MULMOD_SPARSE}",                     read_file(core_path + "/rbc_poly/rbc_poly_mulmod_sparse.c"))
    replace_in_file(output_file1, "{TEMPLATING_RBC_POLY_INV}",                               read_file(core_path + "/rbc_poly/rbc_poly_inv.c"))
    replace_in_file(output_file1, "{TEMPLATING_RBC_POLY_TO_STRING}",                         read_file(core_path + "/rbc_poly/rbc_poly_to_string.c"))
    replace_in_file(output_file1, "{TEMPLATING_RBC_POLY_FROM_STRING}",                       read_file(core_path + "/rbc_poly/rbc_poly_from_string.c"))
    replace_in_file(output_file1, "{TEMPLATING_RBC_POLY_PRINT}",                             read_file(core_path + "/rbc_poly/rbc_poly_print.c"))
    replace_in_file(output_file1, "{TEMPLATING_RBC_POLY_SPARSE_PRINT}",                      read_file(core_path + "/rbc_poly/rbc_poly_sparse_print.c"))

    output_file2 = output_path + "/rbc_poly.h"
    replace_in_file(output_file2, "{TEMPLATING_RBC_POLY_INIT}",                              read_file(core_path + "/rbc_poly/rbc_poly_init.h"))
    replace_in_file(output_file2, "{TEMPLATING_RBC_POLY_CLEAR}",                             read_file(core_path + "/rbc_poly/rbc_poly_clear.h"))
    replace_in_file(output_file2, "{TEMPLATING_RBC_POLY_SPARSE_INIT}",                       read_file(core_path + "/rbc_poly/rbc_poly_sparse_init.h"))
    replace_in_file(output_file2, "{TEMPLATING_RBC_POLY_SPARSE_CLEAR}",                      read_file(core_path + "/rbc_poly/rbc_poly_sparse_clear.h"))
    replace_in_file(output_file2, "{TEMPLATING_RBC_POLY_SET_ZERO}",                          read_file(core_path + "/rbc_poly/rbc_poly_set_zero.h"))
    replace_in_file(output_file2, "{TEMPLATING_RBC_POLY_SET}",                               read_file(core_path + "/rbc_poly/rbc_poly_set.h"))
    replace_in_file(output_file2, "{TEMPLATING_RBC_POLY_SET_RANDOM}",                        read_file(core_path + "/rbc_poly/rbc_poly_set_random.h"))
    replace_in_file(output_file2, "{TEMPLATING_RBC_POLY_SET_RANDOM_FULL_RANK}",              read_file(core_path + "/rbc_poly/rbc_poly_set_random_full_rank.h"))
    replace_in_file(output_file2, "{TEMPLATING_RBC_POLY_SET_RANDOM_FULL_RANK_WITH_ONE}",     read_file(core_path + "/rbc_poly/rbc_poly_set_random_full_rank_with_one.h"))
    replace_in_file(output_file2, "{TEMPLATING_RBC_POLY_SET_RANDOM_FROM_SUPPORT}",           read_file(core_path + "/rbc_poly/rbc_poly_set_random_from_support.h"))
    replace_in_file(output_file2, "{TEMPLATING_RBC_POLY_SET_RANDOM_PAIR_FROM_SUPPORT}",      read_file(core_path + "/rbc_poly/rbc_poly_set_random_pair_from_support.h"))
    replace_in_file(output_file2, "{TEMPLATING_RBC_POLY_IS_EQUAL_TO}",                       read_file(core_path + "/rbc_poly/rbc_poly_is_equal_to.h"))
    replace_in_file(output_file2, "{TEMPLATING_RBC_POLY_ADD}",                               read_file(core_path + "/rbc_poly/rbc_poly_add.h"))
    replace_in_file(output_file2, "{TEMPLATING_RBC_POLY_MUL}",                               read_file(core_path + "/rbc_poly/rbc_poly_mul.h"))
    replace_in_file(output_file2, "{TEMPLATING_RBC_POLY_MULMOD_SPARSE}",                     read_file(core_path + "/rbc_poly/rbc_poly_mulmod_sparse.h"))
    replace_in_file(output_file2, "{TEMPLATING_RBC_POLY_INV}",                               read_file(core_path + "/rbc_poly/rbc_poly_inv.h"))
    replace_in_file(output_file2, "{TEMPLATING_RBC_POLY_TO_STRING}",                         read_file(core_path + "/rbc_poly/rbc_poly_to_string.h"))
    replace_in_file(output_file2, "{TEMPLATING_RBC_POLY_FROM_STRING}",                       read_file(core_path + "/rbc_poly/rbc_poly_from_string.h"))
    replace_in_file(output_file2, "{TEMPLATING_RBC_POLY_PRINT}",                             read_file(core_path + "/rbc_poly/rbc_poly_print.h"))
    replace_in_file(output_file2, "{TEMPLATING_RBC_POLY_SPARSE_PRINT}",                      read_file(core_path + "/rbc_poly/rbc_poly_sparse_print.h"))

    # Preprocessing adding parameter m to files
    rename_file(output_path + "/rbc_poly.h", output_path + "/rbc_" + str(m) + "_poly.h")
    replace_in_file(output_path + "/rbc_" + str(m) + "_poly.h", "rbc", "rbc_" + str(m));
    replace_in_file(output_path + "/rbc_" + str(m) + "_poly.h", "RBC", "RBC_" + str(m));

    rename_file(output_path + "/rbc_poly.c", output_path + "/rbc_" + str(m) + "_poly.c")
    replace_in_file(output_path + "/rbc_" + str(m) + "_poly.c", "rbc", "rbc_" + str(m));
    replace_in_file(output_path + "/rbc_" + str(m) + "_poly.c", "RBC", "RBC_" + str(m));



### Core qre preprocessing function
def preprocessing_src_core_qre(arch, m, n_list):
    # Copy files from templating folder to build folder 
    output_path = root_path + "/build/src/core/" + arch + "/rbc-" + str(m)
    copy_file(templating_path + "/core/rbc_qre.c", output_path + "/rbc_qre.c")
    copy_file(templating_path + "/core/rbc_qre.h", output_path + "/rbc_qre.h")

    # Preprocessing replacing templates by specific implementation
    output_file1 = output_path + "/rbc_qre.c"
    replace_in_file(output_file1, "{TEMPLATING_RBC_QRE_GET_MODULUS}",                       preprocessing_rbc_qre_get_modulus(arch, m, n_list))

    # Preprocessing replacing templates by generic implementation
    replace_in_file(output_file1, "{TEMPLATING_RBC_QRE_INIT_MODULUS}",                      read_file(core_path + "/rbc_qre/rbc_qre_init_modulus.c"))
    replace_in_file(output_file1, "{TEMPLATING_RBC_QRE_CLEAR_MODULUS}",                     read_file(core_path + "/rbc_qre/rbc_qre_clear_modulus.c"))
    replace_in_file(output_file1, "{TEMPLATING_RBC_QRE_INIT}",                              read_file(core_path + "/rbc_qre/rbc_qre_init.c"))
    replace_in_file(output_file1, "{TEMPLATING_RBC_QRE_CLEAR}",                             read_file(core_path + "/rbc_qre/rbc_qre_clear.c"))
    replace_in_file(output_file1, "{TEMPLATING_RBC_QRE_SET_ZERO}",                          read_file(core_path + "/rbc_qre/rbc_qre_set_zero.c"))
    replace_in_file(output_file1, "{TEMPLATING_RBC_QRE_SET_RANDOM}",                        read_file(core_path + "/rbc_qre/rbc_qre_set_random.c"))
    replace_in_file(output_file1, "{TEMPLATING_RBC_QRE_SET_RANDOM_FULL_RANK}",              read_file(core_path + "/rbc_qre/rbc_qre_set_random_full_rank.c"))
    replace_in_file(output_file1, "{TEMPLATING_RBC_QRE_SET_RANDOM_FULL_RANK_WITH_ONE}",     read_file(core_path + "/rbc_qre/rbc_qre_set_random_full_rank_with_one.c"))
    replace_in_file(output_file1, "{TEMPLATING_RBC_QRE_SET_RANDOM_FROM_SUPPORT}",           read_file(core_path + "/rbc_qre/rbc_qre_set_random_from_support.c"))
    replace_in_file(output_file1, "{TEMPLATING_RBC_QRE_SET_RANDOM_PAIR_FROM_SUPPORT}",      read_file(core_path + "/rbc_qre/rbc_qre_set_random_pair_from_support.c"))
    replace_in_file(output_file1, "{TEMPLATING_RBC_QRE_IS_EQUAL_TO}",                       read_file(core_path + "/rbc_qre/rbc_qre_is_equal_to.c"))
    replace_in_file(output_file1, "{TEMPLATING_RBC_QRE_ADD}",                               read_file(core_path + "/rbc_qre/rbc_qre_add.c"))
    replace_in_file(output_file1, "{TEMPLATING_RBC_QRE_MUL}",                               read_file(core_path + "/rbc_qre/rbc_qre_mul.c"))
    replace_in_file(output_file1, "{TEMPLATING_RBC_QRE_INV}",                               read_file(core_path + "/rbc_qre/rbc_qre_inv.c"))
    replace_in_file(output_file1, "{TEMPLATING_RBC_QRE_TO_STRING}",                         read_file(core_path + "/rbc_qre/rbc_qre_to_string.c"))
    replace_in_file(output_file1, "{TEMPLATING_RBC_QRE_FROM_STRING}",                       read_file(core_path + "/rbc_qre/rbc_qre_from_string.c"))
    replace_in_file(output_file1, "{TEMPLATING_RBC_QRE_PRINT}",                             read_file(core_path + "/rbc_qre/rbc_qre_print.c"))

    output_file2 = output_path + "/rbc_qre.h"
    replace_in_file(output_file2, "{TEMPLATING_RBC_QRE_INIT_MODULUS}",                      read_file(core_path + "/rbc_qre/rbc_qre_init_modulus.h"))
    replace_in_file(output_file2, "{TEMPLATING_RBC_QRE_CLEAR_MODULUS}",                     read_file(core_path + "/rbc_qre/rbc_qre_clear_modulus.h"))
    replace_in_file(output_file2, "{TEMPLATING_RBC_QRE_GET_MODULUS}",                       read_file(core_path + "/rbc_qre/rbc_qre_get_modulus.h"))
    replace_in_file(output_file2, "{TEMPLATING_RBC_QRE_INIT}",                              read_file(core_path + "/rbc_qre/rbc_qre_init.h"))
    replace_in_file(output_file2, "{TEMPLATING_RBC_QRE_CLEAR}",                             read_file(core_path + "/rbc_qre/rbc_qre_clear.h"))
    replace_in_file(output_file2, "{TEMPLATING_RBC_QRE_SET_ZERO}",                          read_file(core_path + "/rbc_qre/rbc_qre_set_zero.h"))
    replace_in_file(output_file2, "{TEMPLATING_RBC_QRE_SET_RANDOM}",                        read_file(core_path + "/rbc_qre/rbc_qre_set_random.h"))
    replace_in_file(output_file2, "{TEMPLATING_RBC_QRE_SET_RANDOM_FULL_RANK}",              read_file(core_path + "/rbc_qre/rbc_qre_set_random_full_rank.h"))
    replace_in_file(output_file2, "{TEMPLATING_RBC_QRE_SET_RANDOM_FULL_RANK_WITH_ONE}",     read_file(core_path + "/rbc_qre/rbc_qre_set_random_full_rank_with_one.h"))
    replace_in_file(output_file2, "{TEMPLATING_RBC_QRE_SET_RANDOM_FROM_SUPPORT}",           read_file(core_path + "/rbc_qre/rbc_qre_set_random_from_support.h"))
    replace_in_file(output_file2, "{TEMPLATING_RBC_QRE_SET_RANDOM_PAIR_FROM_SUPPORT}",      read_file(core_path + "/rbc_qre/rbc_qre_set_random_pair_from_support.h"))
    replace_in_file(output_file2, "{TEMPLATING_RBC_QRE_IS_EQUAL_TO}",                       read_file(core_path + "/rbc_qre/rbc_qre_is_equal_to.h"))
    replace_in_file(output_file2, "{TEMPLATING_RBC_QRE_ADD}",                               read_file(core_path + "/rbc_qre/rbc_qre_add.h"))
    replace_in_file(output_file2, "{TEMPLATING_RBC_QRE_MUL}",                               read_file(core_path + "/rbc_qre/rbc_qre_mul.h"))
    replace_in_file(output_file2, "{TEMPLATING_RBC_QRE_INV}",                               read_file(core_path + "/rbc_qre/rbc_qre_inv.h"))
    replace_in_file(output_file2, "{TEMPLATING_RBC_QRE_TO_STRING}",                         read_file(core_path + "/rbc_qre/rbc_qre_to_string.h"))
    replace_in_file(output_file2, "{TEMPLATING_RBC_QRE_FROM_STRING}",                       read_file(core_path + "/rbc_qre/rbc_qre_from_string.h"))
    replace_in_file(output_file2, "{TEMPLATING_RBC_QRE_PRINT}",                             read_file(core_path + "/rbc_qre/rbc_qre_print.h"))

    # Preprocessing adding parameter m to files
    rename_file(output_path + "/rbc_qre.h", output_path + "/rbc_" + str(m) + "_qre.h")
    replace_in_file(output_path + "/rbc_" + str(m) + "_qre.h", "rbc", "rbc_" + str(m));
    replace_in_file(output_path + "/rbc_" + str(m) + "_qre.h", "RBC", "RBC_" + str(m));

    rename_file(output_path + "/rbc_qre.c", output_path + "/rbc_" + str(m) + "_qre.c")
    replace_in_file(output_path + "/rbc_" + str(m) + "_qre.c", "rbc", "rbc_" + str(m));
    replace_in_file(output_path + "/rbc_" + str(m) + "_qre.c", "RBC", "RBC_" + str(m));



### Core vspace preprocessing function
def preprocessing_src_core_vspace(arch, m):
    # Copy files from templating folder to build folder 
    output_path = root_path + "/build/src/core/" + arch + "/rbc-" + str(m)
    copy_file(templating_path + "/core/rbc_vspace.c", output_path + "/rbc_vspace.c")
    copy_file(templating_path + "/core/rbc_vspace.h", output_path + "/rbc_vspace.h")

    # Preprocessing replacing templates by generic implementation
    output_file1 = output_path + "/rbc_vspace.c"
    replace_in_file(output_file1, "{TEMPLATING_RBC_VSPACE_INIT}",                              read_file(core_path + "/rbc_vspace/rbc_vspace_init.c"))
    replace_in_file(output_file1, "{TEMPLATING_RBC_VSPACE_CLEAR}",                             read_file(core_path + "/rbc_vspace/rbc_vspace_clear.c"))
    replace_in_file(output_file1, "{TEMPLATING_RBC_VSPACE_SET}",                               read_file(core_path + "/rbc_vspace/rbc_vspace_set.c"))
    replace_in_file(output_file1, "{TEMPLATING_RBC_VSPACE_SET_ZERO}",                          read_file(core_path + "/rbc_vspace/rbc_vspace_set_zero.c"))
    replace_in_file(output_file1, "{TEMPLATING_RBC_VSPACE_SET_RANDOM_FULL_RANK}",              read_file(core_path + "/rbc_vspace/rbc_vspace_set_random_full_rank.c"))
    replace_in_file(output_file1, "{TEMPLATING_RBC_VSPACE_SET_RANDOM_FULL_RANK_WITH_ONE}",     read_file(core_path + "/rbc_vspace/rbc_vspace_set_random_full_rank_with_one.c"))
    replace_in_file(output_file1, "{TEMPLATING_RBC_VSPACE_DIRECT_SUM}",                        read_file(core_path + "/rbc_vspace/rbc_vspace_direct_sum.c"))
    replace_in_file(output_file1, "{TEMPLATING_RBC_VSPACE_INTERSECTION}",                      read_file(core_path + "/rbc_vspace/rbc_vspace_intersection.c"))
    replace_in_file(output_file1, "{TEMPLATING_RBC_VSPACE_PRODUCT}",                           read_file(core_path + "/rbc_vspace/rbc_vspace_product.c"))
    replace_in_file(output_file1, "{TEMPLATING_RBC_VSPACE_PRINT}",                             read_file(core_path + "/rbc_vspace/rbc_vspace_print.c"))

    output_file2 = output_path + "/rbc_vspace.h"
    replace_in_file(output_file2, "{TEMPLATING_RBC_VSPACE_INIT}",                              read_file(core_path + "/rbc_vspace/rbc_vspace_init.h"))
    replace_in_file(output_file2, "{TEMPLATING_RBC_VSPACE_CLEAR}",                             read_file(core_path + "/rbc_vspace/rbc_vspace_clear.h"))
    replace_in_file(output_file2, "{TEMPLATING_RBC_VSPACE_SET}",                               read_file(core_path + "/rbc_vspace/rbc_vspace_set.h"))
    replace_in_file(output_file2, "{TEMPLATING_RBC_VSPACE_SET_ZERO}",                          read_file(core_path + "/rbc_vspace/rbc_vspace_set_zero.h"))
    replace_in_file(output_file2, "{TEMPLATING_RBC_VSPACE_SET_RANDOM_FULL_RANK}",              read_file(core_path + "/rbc_vspace/rbc_vspace_set_random_full_rank.h"))
    replace_in_file(output_file2, "{TEMPLATING_RBC_VSPACE_SET_RANDOM_FULL_RANK_WITH_ONE}",     read_file(core_path + "/rbc_vspace/rbc_vspace_set_random_full_rank_with_one.h"))
    replace_in_file(output_file2, "{TEMPLATING_RBC_VSPACE_DIRECT_SUM}",                        read_file(core_path + "/rbc_vspace/rbc_vspace_direct_sum.h"))
    replace_in_file(output_file2, "{TEMPLATING_RBC_VSPACE_INTERSECTION}",                      read_file(core_path + "/rbc_vspace/rbc_vspace_intersection.h"))
    replace_in_file(output_file2, "{TEMPLATING_RBC_VSPACE_PRODUCT}",                           read_file(core_path + "/rbc_vspace/rbc_vspace_product.h"))
    replace_in_file(output_file2, "{TEMPLATING_RBC_VSPACE_PRINT}",                             read_file(core_path + "/rbc_vspace/rbc_vspace_print.h"))

    # Preprocessing adding parameter m to files
    rename_file(output_path + "/rbc_vspace.h", output_path + "/rbc_" + str(m) + "_vspace.h")
    replace_in_file(output_path + "/rbc_" + str(m) + "_vspace.h", "rbc", "rbc_" + str(m));
    replace_in_file(output_path + "/rbc_" + str(m) + "_vspace.h", "RBC", "RBC_" + str(m));

    rename_file(output_path + "/rbc_vspace.c", output_path + "/rbc_" + str(m) + "_vspace.c")
    replace_in_file(output_path + "/rbc_" + str(m) + "_vspace.c", "rbc", "rbc_" + str(m));
    replace_in_file(output_path + "/rbc_" + str(m) + "_vspace.c", "RBC", "RBC_" + str(m));

