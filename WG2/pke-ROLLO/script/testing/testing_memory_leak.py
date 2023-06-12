import os
import subprocess
import sys
import yaml


### Parsing configuration file
config_file = sys.argv[1] if len(sys.argv) > 1 else root_path + "/config.yml"
with open(config_file, 'r') as yaml_file: 
    config = yaml.safe_load(yaml_file)


### Memory test
print("\n\n### Testing memory leaks\n\n")

result = True

for scheme in config['rbc_build_parameters']:
    if(scheme['name'] != "dummy"):
    	result &= subprocess.run("valgrind --leak-check=full --error-exitcode=1 ./bin/" + str(scheme['name']) + "_" + str(scheme['sec_lvl']), shell=True).returncode == 0

if(result == True):
    os._exit(0)
else:
    os._exit(1)

