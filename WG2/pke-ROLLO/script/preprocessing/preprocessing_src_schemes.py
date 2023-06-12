
import os
import subprocess
import sys
import yaml

root_path = os.path.dirname(os.path.realpath(__file__)) + "/../.."
script_path = root_path + "/script"

sys.path.insert(1, script_path)
from rbc_utils import *

def preprocessing_src_scheme(scheme_name, sec_level, param_m):
    sec_level = str(sec_level)
    param_m = str(param_m)

    src_path = root_path + "/src/schemes/" + scheme_name
    param_path = root_path + "/src/params/" + scheme_name
    build_path = root_path + "/build/src/schemes/" + scheme_name + sec_level
    config_file = root_path + "/src/schemes/" + scheme_name + "/config.yml"

    with open(config_file, 'r') as yaml_file: 
        config = yaml.safe_load(yaml_file)

    subprocess.run("mkdir -p " + build_path, shell=True)
    subprocess.run("cp -r " + src_path + "/*" + " " + build_path, shell=True)
    subprocess.run("cp -r " + param_path + sec_level + "/* " + root_path + "/build/src/schemes/" + scheme_name + sec_level, shell=True)

    lw_scheme_name = config['lowercase_scheme_name']
    up_scheme_name = config['uppercase_scheme_name']

    for filename in config['source_files']:
        rename_file(build_path + "/" + scheme_name + "_" + filename, build_path + "/" + scheme_name + "_" + sec_level + "_" + filename)
        replace_in_file(build_path + "/" + scheme_name + "_" + sec_level + "_" + filename, lw_scheme_name + "_", lw_scheme_name + "_" + sec_level + "_");
        replace_in_file(build_path + "/" + scheme_name + "_" + sec_level + "_" + filename, up_scheme_name + "_", up_scheme_name + "_" + sec_level + "_");
        replace_in_file(build_path + "/" + scheme_name + "_" + sec_level + "_" + filename, "rbc_",               "rbc_" + param_m + "_");

    replace_in_file(build_path + "/CMakeLists.txt", scheme_name,  scheme_name + "_" + sec_level);
    replace_in_file(build_path + "/CMakeLists.txt", "CORE_",      "CORE_" + param_m + "_");
    replace_in_file(build_path + "/CMakeLists.txt", "LRPC_",      "LRPC_" + param_m + "_");
    replace_in_file(build_path + "/CMakeLists.txt", "GABIDULIN_", "GABIDULIN_" + param_m + "_");