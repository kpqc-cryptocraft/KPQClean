
import os
import subprocess
import sys
import yaml

root_path = os.path.dirname(os.path.realpath(__file__)) + "/../.."
script_path = root_path + "/script"
example_path = root_path + "/test/example"

sys.path.insert(1, script_path)
from rbc_utils import replace_in_file

target_add_subdirectories = ""

def preprocessing_example(scheme_name, sec_level):
    sec_level = str(sec_level)

    local_config_file = root_path + "/src/schemes/" + scheme_name + "/config.yml"

    with open(local_config_file, 'r') as yaml_file: 
        local_config = yaml.safe_load(yaml_file)

    lw_scheme_name = local_config['lowercase_scheme_name']
    up_scheme_name = local_config['uppercase_scheme_name']

    src_path = example_path + "/" + lw_scheme_name
    build_path = root_path + "/build/test/example/" + lw_scheme_name + sec_level

    subprocess.run("mkdir -p " + build_path, shell=True)
    subprocess.run("cp -r " + src_path + "/*" + " " + build_path, shell=True)

    global target_add_subdirectories
    target_add_subdirectories = target_add_subdirectories + "add_subdirectory(" + lw_scheme_name + sec_level + ")" + "\n"

    replace_in_file(build_path + "/CMakeLists.txt",   lw_scheme_name, lw_scheme_name + "_" + sec_level);
    replace_in_file(build_path + "/main.c",           lw_scheme_name + "_", lw_scheme_name + "_" + sec_level + "_");
    replace_in_file(build_path + "/main.c",           up_scheme_name + "_", up_scheme_name + "_" + sec_level + "_");


### Parsing configuration file
config_file = sys.argv[1] if len(sys.argv) > 1 else root_path + "/config.yml"
with open(config_file, 'r') as yaml_file: 
    config = yaml.safe_load(yaml_file)

subprocess.run("mkdir -p " + root_path + "/build/test/example", shell=True)
subprocess.run("cp " + example_path + "/CMakeLists.txt" + " " + root_path + "/build/test/example/", shell=True)
replace_in_file(root_path + "/build/test/example/CMakeLists.txt",   "CONFIG_LIB_DIR", root_path + "/bin");


### scheme example preprocessing
for scheme in config['rbc_build_parameters']:
    has_example = os.path.exists(root_path + "/test/example/" + scheme['name'])

    if has_example:
        preprocessing_example(scheme['name'], scheme['sec_lvl'])

replace_in_file(root_path + "/build/test/example/CMakeLists.txt",   "{TEMPLATING_CMAKE_ADD_SUBDIRECTORY}", target_add_subdirectories)