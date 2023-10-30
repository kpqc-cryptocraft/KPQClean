
import os
import subprocess
import sys
import yaml

root_path = os.path.dirname(os.path.realpath(__file__)) + "/../.."
script_path = root_path + "/script"
unit_test_path = root_path + "/test/unit_test"

sys.path.insert(1, script_path)
from rbc_utils import replace_in_file

target_add_subdirectories = ""




### Unit test elt preprocessing function
def preprocessing_unit_test_elt(param_m):
    param_m = str(param_m)

    src_path = root_path + "/test/unit_test/"
    build_path = root_path + "/build/test/unit_test/rbc_" + param_m + "_elt"
    test_file_path = build_path + "/test_rbc_" + param_m + "_elt.cpp"

    subprocess.run("mkdir -p " + build_path, shell=True)

    subprocess.run("cp -r " + src_path + "rbc_elt/test_rbc_elt.cpp" + " " + build_path, shell=True)

    subprocess.run("cp -r " + src_path + "rbc_elt/CMakeLists.txt" + " " + build_path, shell=True)
    subprocess.run("cp -r " + src_path + "utils.h" + " " + build_path, shell=True)

    subprocess.run("mv " + build_path + "/test_rbc_elt.cpp" + " " + test_file_path, shell=True)

    global target_add_subdirectories
    target_add_subdirectories = target_add_subdirectories + "add_subdirectory(rbc_" + param_m + "_elt)" + "\n"

    replace_in_file(build_path + "/CMakeLists.txt",     "rbc_",  "rbc_" + param_m + "_");

    replace_in_file(build_path + "/utils.h",            "rbc_",  "rbc_" + param_m + "_");
    replace_in_file(build_path + "/utils.h",            "RBC_",  "RBC_" + param_m + "_");

    replace_in_file(test_file_path,                     "rbc_",  "rbc_" + param_m + "_");
    replace_in_file(test_file_path,                     "RBC_",  "RBC_" + param_m + "_");


### Unit test poly preprocessing function
def preprocessing_unit_test_poly(param_m, param_n):
    param_m = str(param_m)
    param_n = str(param_n)

    src_path = root_path + "/test/unit_test/"
    build_path = root_path + "/build/test/unit_test/rbc_" + param_m + "_poly_" + param_n
    test_file_path = build_path + "/test_rbc_" + param_m + "_poly_" + param_n + ".cpp"

    subprocess.run("mkdir -p " + build_path, shell=True)

    subprocess.run("cp -r " + src_path + "rbc_poly/test_rbc_poly.cpp" + " " + build_path, shell=True)

    subprocess.run("cp -r " + src_path + "rbc_poly/CMakeLists.txt" + " " + build_path, shell=True)
    subprocess.run("cp -r " + src_path + "utils.h" + " " + build_path, shell=True)

    subprocess.run("mv " + build_path + "/test_rbc_poly.cpp" + " " + test_file_path, shell=True)

    global target_add_subdirectories
    target_add_subdirectories = target_add_subdirectories + "add_subdirectory(rbc_" + param_m + "_poly_" + param_n + ")" + "\n"

    replace_in_file(build_path + "/CMakeLists.txt",     "rbc_poly",  "rbc_" + param_m + "_poly_" + param_n);

    replace_in_file(build_path + "/utils.h",            "rbc_",  "rbc_" + param_m + "_");
    replace_in_file(build_path + "/utils.h",            "RBC_",  "RBC_" + param_m + "_");

    replace_in_file(test_file_path,                     "rbc_",  "rbc_" + param_m + "_");
    replace_in_file(test_file_path,                     "RBC_",  "RBC_" + param_m + "_");
    replace_in_file(test_file_path,                     "DEGREE;",  "DEGREE = " + param_n + ";");


### Unit test vspace preprocessing function
def preprocessing_unit_test_vspace(param_m):
    param_m = str(param_m)

    src_path = root_path + "/test/unit_test/"
    build_path = root_path + "/build/test/unit_test/rbc_" + param_m + "_vspace"
    test_file_path = build_path + "/test_rbc_" + param_m + "_vspace.cpp"

    subprocess.run("mkdir -p " + build_path, shell=True)
    subprocess.run("cp -r " + src_path + "rbc_vspace/*" + " " + build_path, shell=True)
    subprocess.run("cp -r " + src_path + "utils.h" + " " + build_path, shell=True)
    subprocess.run("mv " + build_path + "/test_rbc_vspace.cpp" + " " + test_file_path, shell=True)

    global target_add_subdirectories
    target_add_subdirectories = target_add_subdirectories + "add_subdirectory(rbc_" + param_m + "_vspace)" + "\n"

    replace_in_file(build_path + "/CMakeLists.txt",     "rbc_",  "rbc_" + param_m + "_");

    replace_in_file(build_path + "/utils.h",            "rbc_",  "rbc_" + param_m + "_");
    replace_in_file(build_path + "/utils.h",            "RBC_",  "RBC_" + param_m + "_");

    replace_in_file(test_file_path,                     "rbc_",  "rbc_" + param_m + "_");
    replace_in_file(test_file_path,                     "RBC_",  "RBC_" + param_m + "_");





### Parsing configuration file
config_file = sys.argv[1] if len(sys.argv) > 1 else root_path + "/config.yml"
with open(config_file, 'r') as yaml_file: 
    config = yaml.safe_load(yaml_file)

subprocess.run("mkdir -p " + root_path + "/build/test/unit_test", shell=True)
subprocess.run("cp " + unit_test_path + "/CMakeLists.txt" + " " + root_path + "/build/test/unit_test/", shell=True)
replace_in_file(root_path + "/build/test/unit_test/CMakeLists.txt",   "CONFIG_LIB_DIR", root_path + "/bin");

m_done = []
mn_done = []

for scheme in config['rbc_build_parameters']:
    if scheme['m'] not in m_done:
        preprocessing_unit_test_elt(scheme['m'])
        preprocessing_unit_test_vspace(scheme['m'])
        m_done.append(scheme['m'])

    if (scheme['m'], scheme['n']) not in mn_done:
        preprocessing_unit_test_poly(scheme['m'], scheme['n'])
        mn_done.append((scheme['m'], scheme['n']))

replace_in_file(root_path + "/build/test/unit_test/CMakeLists.txt",   "{TEMPLATING_CMAKE_ADD_SUBDIRECTORY}", target_add_subdirectories)