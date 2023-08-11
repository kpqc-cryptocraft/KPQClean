import os
import subprocess
import sys
import yaml

root_path = os.path.dirname(os.path.realpath(__file__)) + "/.."


### Parsing configuration file
config_file = sys.argv[1] if len(sys.argv) > 1 else root_path + "/config.yml"
with open(config_file, 'r') as yaml_file: 
    config = yaml.safe_load(yaml_file)


### Testing unit tests
print("\n\n### Testing constant time tests\n\n")

result = True
for scheme in config['rbc_build_parameters']:
    result &= subprocess.run("valgrind --track-origins=yes --error-exitcode=1 ./bin/test_cst_rbc_" + str(scheme['m']) + "_elt", shell=True).returncode == 0

for scheme in config['rbc_build_parameters']:
    result &= subprocess.run("valgrind --track-origins=yes --error-exitcode=1 ./bin/test_cst_rbc_" + str(scheme['m']) + "_vec", shell=True).returncode == 0

for scheme in config['rbc_build_parameters']:
    result &= subprocess.run("valgrind --track-origins=yes --error-exitcode=1 ./bin/test_cst_rbc_" + str(scheme['m']) + "_poly_" + str(scheme['n']) , shell=True).returncode == 0

for scheme in config['rbc_build_parameters']:
    result &= subprocess.run("valgrind --track-origins=yes --error-exitcode=1 ./bin/test_cst_rbc_" + str(scheme['m']) + "_vspace", shell=True).returncode == 0

if(result == True):
    os._exit(0)
else:
    os._exit(1)

