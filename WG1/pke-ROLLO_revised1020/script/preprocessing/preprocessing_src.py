
import os
import subprocess
import sys
import yaml
from preprocessing_src_cmake import *
from preprocessing_src_core import *
from preprocessing_src_codes import *
from preprocessing_src_schemes import *

root_path = os.path.dirname(os.path.realpath(__file__)) + "/../.."


### Parsing configuration file
config_file = sys.argv[1] if len(sys.argv) > 1 else root_path + "/config.yml"
with open(config_file, 'r') as yaml_file: 
    config = yaml.safe_load(yaml_file)


### CMakeList preprocessing
subprocess.run("mkdir -p " + root_path + "/build/templating", shell=True)
preprocessing_src_cmake(config)


### rbc.h include preprocessing
subprocess.run("mkdir -p " + root_path + "/build/src/include", shell=True)
preprocessing_src_rbc_include(config)


### External lib preprocessing
subprocess.run("cp -r " + root_path + "/lib " + root_path + "/build", shell=True)


### Core preprocessing
n_list = []
for scheme in config['rbc_build_parameters']:
    if scheme['n'] not in n_list:
        n_list.append(scheme['n'])

for scheme in config['rbc_build_parameters']:
    if scheme['bn'] not in n_list:
        n_list.append(scheme['bn'])        

for scheme in config['rbc_build_parameters']:
    print("    Preprocessing core" + str(scheme['m']))
    preprocessing_src_core(config['rbc_core_layer_implem'], scheme['m'], n_list)



# Codes preprocessing
for scheme in config['rbc_build_parameters']:
    try:
        scheme_config_file = root_path + "/src/schemes/" + scheme['name'] + "/config.yml"
        
        with open(scheme_config_file, 'r') as yaml_file: 
            scheme_config = yaml.safe_load(yaml_file)

        if(scheme_config['gabidulin'] == True):
            print("    Preprocessing gabidulin" + str(scheme['m']))
            preprocessing_src_gabidulin(scheme['m'])
    except (KeyError, FileNotFoundError):
        continue

for scheme in config['rbc_build_parameters']:
    try:
        scheme_config_file = root_path + "/src/schemes/" + scheme['name'] + "/config.yml"
        
        with open(scheme_config_file, 'r') as yaml_file: 
            scheme_config = yaml.safe_load(yaml_file)

        if scheme_config['lrpc'] == True:
            print("    Preprocessing lrpc" + str(scheme['m']))
            preprocessing_src_lrpc(scheme['m'])
    except (KeyError, FileNotFoundError):
        continue

for scheme in config['rbc_build_parameters']:
    if scheme['name'] == "dummy":
        preprocessing_src_gabidulin(scheme['m'])
        preprocessing_src_lrpc(scheme['m'])



# Schemes preprocessing
for scheme in config['rbc_build_parameters']:
    if scheme['name'] != "dummy":
        print("    Preprocessing " + str(scheme['name']) + "_" + str(scheme['sec_lvl']))
        preprocessing_src_scheme(scheme['name'], scheme['sec_lvl'], scheme['m'])
