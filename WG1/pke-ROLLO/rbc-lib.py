
import os
import subprocess
import sys
import yaml

dir_path = os.path.dirname(os.path.realpath(__file__))


### Parsing configuration file
print("\n\n### Parsing configuration file")
config_file = sys.argv[1] if len(sys.argv) > 1 else "config.yml"
with open(config_file, 'r') as yaml_file: 
    config = yaml.safe_load(yaml_file)


### Removing previous build and creating folders
print("\n\n### Removing previous build")
subprocess.run("rm -rf " + dir_path + "/build", shell=True)
subprocess.run("rm -rf " + dir_path + "/bin", shell=True)

subprocess.run("mkdir -p " + dir_path + "/build", shell=True)
subprocess.run("mkdir -p " + dir_path + "/bin/include", shell=True)


### Preprocessing rbc lib
print("\n\n### Preprocessing library")
subprocess.run("python -B script/preprocessing/templating_src.py" + " " + config_file, shell=True)
subprocess.run("python -B script/preprocessing/preprocessing_src.py" + " " + config_file, shell=True)


### Compiling rbc lib
print("\n\n### Compiling library\n")
subprocess.run("cd " + dir_path + "/build && cmake . && make install", shell=True)


### Preprocessing examples
if(config['rbc_compile_example'] == True):
    print("\n\n### Preprocessing examples")
    subprocess.run("python -B script/preprocessing/preprocessing_example.py " + config_file, shell=True)


### Compiling examples
if(config['rbc_compile_example'] == True):
    print("\n\n### Compiling examples\n")
    subprocess.run("cd " + dir_path + "/build/test/example && cmake . && make", shell=True)


### Preprocessing unit tests
if(config['rbc_compile_unit_test'] == True):
    print("\n\n### Preprocessing unit tests")
    subprocess.run("python -B script/preprocessing/preprocessing_unit_test.py " + config_file, shell=True)


### Compiling unit tests
if(config['rbc_compile_unit_test'] == True):
    print("\n\n### Compiling unit tests")
    subprocess.run("cd " + dir_path + "/build/test/unit_test && cmake . && make", shell=True)


### Preprocessing constant-time tests
if(config['rbc_compile_constant_time_test'] == True):
    print("\n\n### Preprocessing constant time tests")
    subprocess.run("python -B script/preprocessing/preprocessing_constant_time_test.py " + config_file, shell=True)


### Compiling constant-time tests
if(config['rbc_compile_constant_time_test'] == True):
    print("\n\n### Compiling constant time tests")
    subprocess.run("cd " + dir_path + "/build/test/constant_time_test && cmake . && make", shell=True)

### Preprocessing KAT
if(config['rbc_compile_kat'] == True):
    print("\n\n### Preprocessing KAT")
    subprocess.run("python -B script/preprocessing/preprocessing_kat.py " + config_file, shell=True)


### Compiling KAT
if(config['rbc_compile_kat'] == True):
    print("\n\n### Compiling KAT\n")
    subprocess.run("cd " + dir_path + "/build/test/kat && cmake . && make", shell=True)




### Preprocessing benchmark
if(config['rbc_compile_benchmark'] == True):
    print("\n\n### Preprocessing benchmark")
    subprocess.run("python -B script/preprocessing/preprocessing_benchmark.py " + config_file, shell=True)


### Compiling benchmark
if(config['rbc_compile_benchmark'] == True):
    print("\n\n### Compiling benchmark")
    subprocess.run("cd " + dir_path + "/build/test/benchmark && cmake . && make", shell=True)



### Packaging NIST
if(config['rbc_package_nist'] == True):
    print("\n\n### Packaging NIST RQC\n")
    subprocess.run("python -B script/packaging/packaging_nist_rqc.py", shell=True)

    print("\n\n### Packaging NIST ROLLOI\n")
    subprocess.run("python -B script/packaging/packaging_nist_rolloI.py", shell=True)

    print("\n\n### Packaging NIST ROLLOII\n")
    subprocess.run("python -B script/packaging/packaging_nist_rolloII.py", shell=True)

