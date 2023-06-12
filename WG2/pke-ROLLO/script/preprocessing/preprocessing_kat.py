
import os
import subprocess
import sys
import yaml

root_path = os.path.dirname(os.path.realpath(__file__)) + "/../.."
script_path = root_path + "/script"
kat_path = root_path + "/test/kat"

sys.path.insert(1, script_path)
from rbc_utils import replace_in_file

target_add_subdirectories = ""

### KAT preprocessing function
def preprocessing_kat(scheme, sec_level):
    scheme = str(scheme)
    sec_level = str(sec_level)

    src_path = root_path + "/test/kat/kat"
    build_path = root_path + "/build/test/kat/" + scheme + sec_level

    subprocess.run("mkdir -p " + build_path, shell=True)
    subprocess.run("cp -r " + src_path + "/*" + " " + build_path, shell=True)

    global target_add_subdirectories
    target_add_subdirectories = target_add_subdirectories + "add_subdirectory(" + scheme + sec_level + ")" + "\n"
    
    replace_in_file(build_path + "/CMakeLists.txt",   "kat", "kat_" + scheme + "_" + sec_level);

    replace_in_file(build_path + "/main.c",           "scheme_", scheme + "_" + sec_level + "_");
    replace_in_file(build_path + "/main.c",           "SCHEME_", scheme.upper() + "_" + sec_level + "_");





### Parsing configuration file
config_file = sys.argv[1] if len(sys.argv) > 1 else root_path + "/config.yml"
with open(config_file, 'r') as yaml_file: 
    config = yaml.safe_load(yaml_file)

subprocess.run("mkdir -p " + root_path + "/build/test/kat", shell=True)
subprocess.run("cp " + kat_path + "/CMakeLists.txt" + " " + root_path + "/build/test/kat/", shell=True)
replace_in_file(root_path + "/build/test/kat/CMakeLists.txt",   "CONFIG_LIB_DIR", root_path + "/bin");


### KAT preprocessing
for scheme in config['rbc_build_parameters']:
    if(scheme['name'] != "dummy"):
        preprocessing_kat(scheme['name'], scheme['sec_lvl'])

replace_in_file(root_path + "/build/test/kat/CMakeLists.txt",   "{TEMPLATING_CMAKE_ADD_SUBDIRECTORY}", target_add_subdirectories) 
