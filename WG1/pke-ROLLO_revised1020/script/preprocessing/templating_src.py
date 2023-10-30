
import os
import subprocess
import sys
import yaml

root_path = os.path.dirname(os.path.realpath(__file__)) + "/../.."
core_path = root_path + "/src/core"
script_path = root_path + "/script"
templating_input_path = root_path + "/script/preprocessing/templating"
templating_output_path = root_path + "/build/templating"

sys.path.insert(1, script_path)
from rbc_utils import *



### Parsing configuration file
config_file = sys.argv[1] if len(sys.argv) > 1 else root_path + "/config.yml"
with open(config_file, 'r') as yaml_file: 
    config = yaml.safe_load(yaml_file)



### Compute core functions template list 
print("    Templating core files")

subprocess.run("mkdir -p " + templating_output_path + "/core", shell=True)

templating = {}
templating_all = {}
templating_min = {}

# Check if a full build is required
is_full_build = False
if config['rbc_compile_unit_test'] == True:
    is_full_build = True;

for scheme in config['rbc_build_parameters']:
    if scheme['name'] == "dummy":
        is_full_build = True;

# Parse templating_all with all core functions 
with open(templating_input_path + "/all.yml", 'r') as yaml_file: 
    templating_all.update(yaml.safe_load(yaml_file))

# Parse templating_min with minimal core functions
if(is_full_build == False):
    for scheme in config['rbc_build_parameters']:
        if scheme['name'] != "dummy":
            with open(templating_input_path + "/" + scheme['name'] + ".yml", 'r') as yaml_file: 
                for key, value in yaml.safe_load(yaml_file).items():
                    if key in templating_min:
                        templating_min[key].extend(value)
                    else:
                        templating_min[key] = value

    for rbc_struct in templating_min:
        templating_min[rbc_struct] = remove_list_duplicates(templating_min[rbc_struct])

# Parse templating depending on build type (preserving order from all core functions template file)
if(is_full_build == True):
    templating = templating_all

if(is_full_build == False):
    for rbc_struct in templating_all:
        templating[rbc_struct] = intersect_list_preserving_order(templating_all[rbc_struct], templating_min[rbc_struct])



### Create template files
for rbc_struct in templating:
    output_path = templating_output_path + "/core/"
    copy_file(core_path + "/" + rbc_struct + "/" + rbc_struct + ".c", output_path + rbc_struct + ".c")
    copy_file(core_path + "/" + rbc_struct + "/" + rbc_struct + ".h", output_path + rbc_struct + ".h")

    for function in templating[rbc_struct]:
        append_in_file(output_path + "/" + rbc_struct + ".c", "{TEMPLATING_" + function.upper() + "}\n")
        append_in_file(output_path + "/" + rbc_struct + ".h", "{TEMPLATING_" + function.upper() + "}\n")
    append_in_file(output_path + "/" + rbc_struct + ".h", "#endif\n\n")

