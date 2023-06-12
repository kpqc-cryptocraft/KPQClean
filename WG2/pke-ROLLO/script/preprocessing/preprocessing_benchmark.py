
import os
import subprocess
import sys
import yaml

root_path = os.path.dirname(os.path.realpath(__file__)) + "/../.."
script_path = root_path + "/script"
benchmark_path = root_path + "/test/benchmark"
build_path = root_path + "/build"

sys.path.insert(1, script_path)
from rbc_utils import replace_in_file

### Parsing configuration file
config_file = sys.argv[1] if len(sys.argv) > 1 else root_path + "/config.yml"
with open(config_file, 'r') as yaml_file: 
    config = yaml.safe_load(yaml_file)

subprocess.run("mkdir -p " + build_path + "/test/benchmark", shell=True)
subprocess.run("cp " + benchmark_path + "/CMakeLists.txt" + " " + build_path + "/test/benchmark/", shell=True)
replace_in_file(build_path + "/test/benchmark/CMakeLists.txt",   "CONFIG_LIB_DIR", root_path + "/bin");

target_add_subdirectories = ""

benchmarks_done = []

for scheme in config['rbc_build_parameters']:
	has_benchmark = os.path.exists(root_path + "/test/benchmark/" + scheme['name'])

	if scheme['name'] not in benchmarks_done and has_benchmark:
		subprocess.run("mkdir -p " + root_path + "/build/test/benchmark/" + scheme['name'], shell=True)
		subprocess.run("cp -r " + root_path + "/test/benchmark/" + scheme['name'] + "/* " + root_path + "/build/test/benchmark/"  + scheme['name'], shell=True)
		benchmarks_done.append(scheme['name'])
		target_add_subdirectories = target_add_subdirectories + "add_subdirectory(" + scheme['name'] + ")" + "\n"

replace_in_file(build_path + "/test/benchmark/CMakeLists.txt",   "{TEMPLATING_CMAKE_ADD_SUBDIRECTORY}", target_add_subdirectories)