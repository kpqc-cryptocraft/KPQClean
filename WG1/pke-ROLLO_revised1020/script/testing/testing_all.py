#!/usr/bin/env python

import os
import subprocess
import sys
import yaml

dir_path = os.path.dirname(os.path.realpath(__file__))
root_path = dir_path + "/../.."
testing_path = root_path + "/script/testing"


### Parsing configuration file
config_file = sys.argv[1] if len(sys.argv) > 1 else root_path + "/config.yml"


# Checking tests
kat = subprocess.run("python -B " + testing_path + "/testing_kat.py " + config_file, shell=True)
memory_leak  = subprocess.run("python -B " + testing_path + "/testing_memory_leak.py " + config_file, shell=True)
cst_time  = subprocess.run("python -B " + testing_path + "/testing_cst_time.py " + config_file, shell=True)
unit_test = subprocess.run("python -B " + testing_path + "/testing_unit_test.py " + config_file, shell=True)
result = (kat.returncode == 0) and (memory_leak.returncode == 0) and (cst_time.returncode == 0) and (unit_test.returncode == 0)


# Display messages
kat_str         = "success" if kat.returncode == 0          else "failure"
memory_leak_str = "success" if memory_leak.returncode == 0  else "failure"
cst_time_str    = "success" if cst_time.returncode == 0     else "failure"
unit_test_str   = "success" if unit_test.returncode == 0    else "failure"

subprocess.run("printf '\n------------------------------------------------------------------------\n\n'", shell=True)
subprocess.run("printf '  KAT:       " + kat_str + "\n'", shell=True)
subprocess.run("printf '  cst_time:  " + cst_time_str + "\n'", shell=True)
subprocess.run("printf '  mem_leak:  " + memory_leak_str + "\n'", shell=True)
subprocess.run("printf '  unit_test: " + unit_test_str + "\n'", shell=True)
subprocess.run("printf '\n------------------------------------------------------------------------\n\n'", shell=True)

if(result == True):
    os._exit(0)
else:
    os._exit(1)

