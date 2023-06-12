import os
import subprocess
import shutil
import sys
import yaml

root_path = os.path.dirname(os.path.realpath(__file__)) + "/../.."
script_path = root_path + "/script"
packaging_path = root_path + "/script/packaging"
build_path = root_path + "/build/packaging/mupq"
source_path = root_path + "/build/src"

sys.path.insert(1, script_path)
from rbc_utils import *

#Remove previous packaging
subprocess.run("rm -rf " + build_path, shell=True)

### Parsing configuration file
config_file = sys.argv[1] if len(sys.argv) > 1 else root_path + "/config.yml"
with open(config_file, 'r') as yaml_file: 
    config = yaml.safe_load(yaml_file)

for scheme in config['rbc_build_parameters']:
    if scheme['name'] != "dummy":
        package_name = scheme['name'] + str(scheme['sec_lvl']) + "/ref"

        #Create folder
        package_folder = build_path + "/" + package_name
        subprocess.run("mkdir -p " + package_folder, shell=True)

        #Copy core sources (hardcoded c32 for mupq)
        core_folder = source_path + "/core/c32/rbc-" + str(scheme['m'])
        subprocess.run("cp " + core_folder + "/*.c " + package_folder, shell=True)
        subprocess.run("cp " + core_folder + "/*.h " + package_folder, shell=True)

        #Copy code sources (if any)
        try:
            scheme_config_file = root_path + "/src/schemes/" + scheme['name'] + "/config.yml"
            
            with open(scheme_config_file, 'r') as yaml_file: 
                scheme_config = yaml.safe_load(yaml_file)

        except FileNotFoundError:
            print("Cannot load scheme config file")
            continue

        try:
            if scheme_config['lrpc']:
                lrpc_folder = source_path + "/codes/lrpc-" + str(scheme['m'])
                subprocess.run("cp " + lrpc_folder + "/*.c " + package_folder, shell=True)
                subprocess.run("cp " + lrpc_folder + "/*.h " + package_folder, shell=True)
        except KeyError:
            pass

        try:
            if scheme_config['gabidulin']:
                gabidulin_folder = source_path + "/codes/gabidulin-" + str(scheme['m'])
                subprocess.run("cp " + gabidulin_folder + "/*.c " + package_folder, shell=True)
                subprocess.run("cp " + gabidulin_folder + "/*.h " + package_folder, shell=True)
        except KeyError:
            pass

        #Copy scheme sources
        scheme_folder = source_path + "/schemes/" + scheme['name'] + str(scheme['sec_lvl'])
        subprocess.run("cp " + scheme_folder + "/*.c " + package_folder, shell=True)
        subprocess.run("cp " + scheme_folder + "/*.h " + package_folder, shell=True)

        #Copy RNG wrapper
        lib_folder = root_path + "/lib/random_source"
        subprocess.run("cp " + lib_folder + "/*.c " + package_folder, shell=True)
        subprocess.run("cp " + lib_folder + "/*.h " + package_folder, shell=True)

        #Copy and parse the api.h file
        api_file = packaging_path + "/mupq/api.h"
        subprocess.run("cp " + api_file + " " + package_folder, shell=True)

        parameters_file = package_folder + "/" + scheme['name'] + "_" + str(scheme['sec_lvl']) + "_parameters.h"
        with open(parameters_file, 'r') as file:
            for line in file.readlines():
                if "PUBLIC_KEY_BYTES" in line:
                    pk_size = int(line.split(' ')[2])
                if "SECRET_KEY_BYTES" in line:
                    sk_size = int(line.split(' ')[2])
                if "CIPHERTEXT_BYTES" in line:
                    ct_size = int(line.split(' ')[2])
                if "SHARED_SECRET_BYTES" in line:
                    ss_size = int(line.split(' ')[2])

        api_file = package_folder + "/api.h"
        replace_in_file(api_file, "{SCHEME_ALGNAME}", scheme['name'] + str(scheme['sec_lvl']))
        replace_in_file(api_file, "{SCHEME_SECRETKEYBYTES}", str(sk_size))
        replace_in_file(api_file, "{SCHEME_PUBLICKEYBYTES}", str(pk_size))
        replace_in_file(api_file, "{SCHEME_CIPHERTEXTBYTES}", str(ct_size))
        replace_in_file(api_file, "{SCHEME_CRYPTOBYTES}", str(ss_size))
        replace_in_file(api_file, "{SCHEME_INCLUDE}", "\"" + scheme['name'] + "_" + str(scheme['sec_lvl']) + "_kem.h" + "\"")
        replace_in_file(api_file, "{SCHEME_KEYGEN}", scheme['name'] + "_" + str(scheme['sec_lvl']) + "_keygen")
        replace_in_file(api_file, "{SCHEME_ENC}", scheme['name'] + "_" + str(scheme['sec_lvl']) + "_encaps")
        replace_in_file(api_file, "{SCHEME_DEC}", scheme['name'] + "_" + str(scheme['sec_lvl']) + "_decaps")

        #Comment exit calls to avoid linker problems
        for file in os.listdir(package_folder):
            lines_to_replace = []
            with open(package_folder + "/" + file, 'r') as opened_file:
                for line in opened_file.readlines():
                    if "exit(" in line:
                        lines_to_replace.append(line)

            for line in lines_to_replace:
                replace_in_file(package_folder + "/" + file, line, "//" + line)


        #Change some dependencies
        for file in os.listdir(package_folder):
            replace_in_file(package_folder + "/" + file, "\"seedexpander.h\"", "\"nistseedexpander.h\"")