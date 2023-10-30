#!/usr/bin/env python

import os
import subprocess
import sys
import yaml

root_path = os.path.dirname(os.path.realpath(__file__)) + "/../../"
script_path = root_path + "/script"
bin_path = root_path + "/bin"

sys.path.append(script_path)
from rbc_utils import *

tmp_file           = root_path + "benchmark_tmp"
result_all         = root_path + "benchmark_all"

targets = ['c32', 'c64', 'avx']

subprocess.run("rm -f " + result_all, shell=True)

for target in targets:
	config_file = root_path + ".config_git_" + target + ".yml"
	with open(config_file, 'r') as yaml_file: 
		config = yaml.safe_load(yaml_file)

	append_in_file(result_all, "### " + target)

	#Compile implementation
	subprocess.run("python -B " + root_path + "/rbc-lib.py .config_git_" + target + ".yml", shell=True)

	#Perform benchmakrs
	benchmarks_done = []

	for scheme in config['rbc_build_parameters']:
		has_benchmark = os.path.exists(root_path + "/test/benchmark/" + scheme['name'])
		
		if scheme['name'] not in benchmarks_done and has_benchmark:
			subprocess.run(bin_path + "/benchmark-" + scheme['name'] + " > "       + tmp_file, shell=True)
			append_file_to_file(tmp_file,     result_all)
			benchmarks_done.append(scheme['name'])

subprocess.run("rm " + tmp_file, shell=True)