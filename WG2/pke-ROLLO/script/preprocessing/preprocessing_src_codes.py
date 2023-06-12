
import os
import subprocess
import sys
import yaml

root_path = os.path.dirname(os.path.realpath(__file__)) + "/../.."
script_path = root_path + "/script"

sys.path.insert(1, script_path)
from rbc_utils import *


### Gabidulin preprocessing
def preprocessing_src_gabidulin(param_m):
    param_m = str(param_m)

    src_path = root_path + "/src/codes/gabidulin"
    build_path = root_path + "/build/src/codes/gabidulin-" + param_m

    subprocess.run("mkdir -p " + build_path, shell=True)
    subprocess.run("cp -r " + src_path + "/*" + " " + build_path, shell=True)

    rename_file(build_path + "/rbc_qpoly.h",     build_path + "/rbc_" + param_m + "_qpoly.h")
    rename_file(build_path + "/rbc_qpoly.c",     build_path + "/rbc_" + param_m + "_qpoly.c")
    rename_file(build_path + "/rbc_gabidulin.h", build_path + "/rbc_" + param_m + "_gabidulin.h")
    rename_file(build_path + "/rbc_gabidulin.c", build_path + "/rbc_" + param_m + "_gabidulin.c")

    replace_in_file(build_path + "/CMakeLists.txt",                    "rbc_", "rbc_" + param_m + "_");
    replace_in_file(build_path + "/CMakeLists.txt",                    "gabidulin_", "gabidulin_" + param_m);
    replace_in_file(build_path + "/CMakeLists.txt",                    "CORE_", "CORE_" + param_m + "_");

    replace_in_file(build_path + "/rbc_" + param_m + "_qpoly.h",       "rbc_", "rbc_" + param_m + "_");
    replace_in_file(build_path + "/rbc_" + param_m + "_qpoly.h",       "RBC_", "RBC_" + param_m + "_");

    replace_in_file(build_path + "/rbc_" + param_m + "_qpoly.c",       "rbc_", "rbc_" + param_m + "_");
    replace_in_file(build_path + "/rbc_" + param_m + "_qpoly.c",       "RBC_", "RBC_" + param_m + "_");

    replace_in_file(build_path + "/rbc_" + param_m + "_gabidulin.h",   "rbc_", "rbc_" + param_m + "_");
    replace_in_file(build_path + "/rbc_" + param_m + "_gabidulin.h",   "RBC_", "RBC_" + param_m + "_");

    replace_in_file(build_path + "/rbc_" + param_m + "_gabidulin.c",   "rbc_", "rbc_" + param_m + "_");



### LRPC preprocessing
def preprocessing_src_lrpc(param_m):
    param_m = str(param_m)

    src_path = root_path + "/src/codes/lrpc"
    build_path = root_path + "/build/src/codes/lrpc-" + param_m

    subprocess.run("mkdir -p " + build_path, shell=True)
    subprocess.run("cp -r " + src_path + "/*" + " " + build_path, shell=True)

    rename_file(build_path + "/rbc_lrpc.h", build_path + "/rbc_" + param_m + "_lrpc.h")
    rename_file(build_path + "/rbc_lrpc.c", build_path + "/rbc_" + param_m + "_lrpc.c")

    replace_in_file(build_path + "/CMakeLists.txt",                    "rbc_", "rbc_" + param_m + "_");
    replace_in_file(build_path + "/CMakeLists.txt",                    "lrpc_", "lrpc_" + param_m);
    replace_in_file(build_path + "/CMakeLists.txt",                    "CORE_", "CORE_" + param_m + "_");

    replace_in_file(build_path + "/rbc_" + param_m + "_lrpc.h",       "rbc_", "rbc_" + param_m + "_");
    replace_in_file(build_path + "/rbc_" + param_m + "_lrpc.h",       "RBC_", "RBC_" + param_m + "_");

    replace_in_file(build_path + "/rbc_" + param_m + "_lrpc.c",       "rbc_", "rbc_" + param_m + "_");
    replace_in_file(build_path + "/rbc_" + param_m + "_lrpc.c",       "RBC_", "RBC_" + param_m + "_");

