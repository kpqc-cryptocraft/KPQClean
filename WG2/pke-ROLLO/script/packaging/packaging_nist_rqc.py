
import os
import subprocess
import shutil
import sys

root_path = os.path.dirname(os.path.realpath(__file__)) + "/../.."
script_path = root_path + "/script"
packaging_path = root_path + "/script/packaging"
build_path = root_path + "/build/packaging/nist/rqc"

sys.path.insert(1, script_path)
from rbc_utils import *


rqc_version = "2020_04_15"

implem_list = [
    {"name":"Reference_Implementation", "core":"c64"},
    {"name":"Optimized_Implementation", "core":"avx"},
]

rqc_list = [
    {"name":"rqc128", "name2":"rqc_128", "name3":"rqc-128", "core":"rbc-127"},
    {"name":"rqc192", "name2":"rqc_192", "name3":"rqc-192", "core":"rbc-151"},
    {"name":"rqc256", "name2":"rqc_256", "name3":"rqc-256", "core":"rbc-181"},
]


### Creating output folder
subprocess.run("rm -rf " + build_path, shell=True)
subprocess.run("mkdir -p " + build_path, shell=True)

for implem in implem_list:
    for rqc in rqc_list:
        subprocess.run("mkdir -p " + build_path + "/" + str(implem['name']) + "/" + str(rqc['name']), shell=True)


### Packaging third party lib
for implem in implem_list:
    for rqc in rqc_list:
        lib_folder = build_path + "/" + implem['name'] + "/" + rqc['name'] + "/lib"
        subprocess.run("mkdir -p " + lib_folder, shell=True)
        subprocess.run("cp -r " + root_path + "/lib/randombytes-nist " + lib_folder, shell=True)
        subprocess.run("cp -r " + root_path + "/lib/seedexpander-nist " + lib_folder, shell=True)
        subprocess.run("cp -r " + root_path + "/lib/sha2-openssl " + lib_folder, shell=True)
        subprocess.run("cp -r " + root_path + "/lib/fips202-portable " + lib_folder, shell=True)

        subprocess.run("mv " + lib_folder + "/sha2-openssl " + lib_folder + "/sha2", shell=True)
        subprocess.run("mv " + lib_folder + "/fips202-portable " + lib_folder + "/fips202", shell=True)
        subprocess.run("mv " + lib_folder + "/randombytes-nist " + lib_folder + "/randombytes", shell=True)
        subprocess.run("mv " + lib_folder + "/seedexpander-nist " + lib_folder + "/seedexpander", shell=True)


### Packaging sources
for implem in implem_list:
    for rqc in rqc_list:
        src_folder = build_path + "/" + implem['name'] + "/" + rqc['name'] + "/src"
        subprocess.run("mkdir -p " + src_folder + "/" + rqc['core'] + "/", shell=True)
        subprocess.run("cp -r " + root_path + "/src/core/" + implem['core'] + "/" + rqc['core'] + "/* " + src_folder + "/" + rqc['core'], shell=True)
        subprocess.run("cp -r " + root_path + "/src/codes/gabidulin/* " + src_folder, shell=True)
        subprocess.run("cp -r " + root_path + "/src/params/" + rqc['name'] + "/* " + src_folder, shell=True)
        subprocess.run("cp -r " + root_path + "/src/schemes/rqc/* "+ src_folder, shell=True)
        subprocess.run("cp " + root_path + "/script/packaging/rqc/" + rqc['name'] + "/api.h " + src_folder, shell=True)

        subprocess.run("mv " + src_folder + "/rbc_qpoly.h " + src_folder + "/qpoly.h", shell=True)
        subprocess.run("mv " + src_folder + "/rbc_qpoly.c " + src_folder + "/qpoly.c", shell=True)
        subprocess.run("mv " + src_folder + "/rbc_gabidulin.h " + src_folder + "/gabidulin.h", shell=True)
        subprocess.run("mv " + src_folder + "/rbc_gabidulin.c " + src_folder + "/gabidulin.c", shell=True)
        subprocess.run("mv " + src_folder + "/" + rqc['name2'] + "_parameters.h " + src_folder + "/parameters.h", shell=True)
        subprocess.run("mv " + src_folder + "/rqc_parsing.h " + src_folder + "/parsing.h", shell=True)
        subprocess.run("mv " + src_folder + "/rqc_parsing.c " + src_folder + "/parsing.c", shell=True)
        subprocess.run("mv " + src_folder + "/rqc_pke.h " + src_folder + "/rqc.h", shell=True)
        subprocess.run("mv " + src_folder + "/rqc_pke.c " + src_folder + "/rqc.c", shell=True)
        subprocess.run("mv " + src_folder + "/rqc_kem.c " + src_folder + "/kem.c", shell=True)
        subprocess.run("rm " + src_folder + "/rqc_kem.h", shell=True)

        replace_in_file(src_folder + "/parameters.h", rqc['name2'].upper(), "RQC");
        replace_in_file(src_folder + "/parsing.c", "rqc_parameters.h", "parameters.h");
        replace_in_file(src_folder + "/parsing.c", "rqc_parsing.h", "parsing.h");
        replace_in_file(src_folder + "/qpoly.c", "rbc_qpoly.h", "qpoly.h");
        replace_in_file(src_folder + "/gabidulin.c", "rbc_qpoly.h", "qpoly.h");
        replace_in_file(src_folder + "/gabidulin.c", "rbc_gabidulin.h", "gabidulin.h");
        replace_in_file(src_folder + "/rqc.c", "rbc_gabidulin.h", "gabidulin.h");
        replace_in_file(src_folder + "/rqc.c", "rqc_parameters.h", "parameters.h");
        replace_in_file(src_folder + "/rqc.c", "rqc_parsing.h", "parsing.h");
        replace_in_file(src_folder + "/rqc.c", "rqc_pke.h", "rqc.h");
        replace_in_file(src_folder + "/kem.c", "rqc_parameters.h", "parameters.h");
        replace_in_file(src_folder + "/kem.c", "rqc_parsing.h", "parsing.h");
        replace_in_file(src_folder + "/kem.c", "rqc_pke.h", "rqc.h");
        replace_in_file(src_folder + "/kem.c", "rqc_kem.h", "api.h");
        replace_in_file(src_folder + "/kem.c", "rqc_kem_keygen", "crypto_kem_keypair");
        replace_in_file(src_folder + "/kem.c", "rqc_kem_encaps", "crypto_kem_enc");
        replace_in_file(src_folder + "/kem.c", "rqc_kem_decaps", "crypto_kem_dec");


### Packaging documentation
for implem in implem_list:
    for rqc in rqc_list:
        doc_folder = build_path + "/" + implem['name'] + "/" + rqc['name'] + "/doc"
        subprocess.run("mkdir -p " + doc_folder, shell=True)
        subprocess.run("cp -r " + root_path + "/script/packaging/rqc/doc/* " + doc_folder, shell=True)
        subprocess.run("cp " + root_path + "/script/packaging/rqc/doxygen.conf " + doc_folder + "/..", shell=True)
    subprocess.run("cp " + root_path + "/script/packaging/rqc/README " + build_path + "/" + implem['name'] + "/", shell=True)


### Packaging build system
for implem in implem_list:
    for rqc in rqc_list:
        build_folder = build_path + "/" + implem['name'] + "/" + rqc['name']
        subprocess.run("cp " + packaging_path + "/rqc/main_rqc.c " + build_folder + "/src", shell=True)
        subprocess.run("cp " + packaging_path + "/rqc/main_kat.c " + build_folder + "/src", shell=True)
        subprocess.run("cp " + packaging_path + "/rqc/" + rqc['name'] + "/Makefile " + build_folder, shell=True)
        if implem == "Reference_Implementation":
            replace_in_file(build_folder + "/Makefile", "-mavx2 -mpclmul -msse4.1 ", "")


### Packaging KATs
for implem in implem_list:
    for rqc in rqc_list:
        kat_folder = build_path + "/KATs/" + implem['name'] + "/" + rqc['name']
        subprocess.run("mkdir -p " + kat_folder, shell=True)
        subprocess.run("cp " + packaging_path + "/rqc/" + rqc['name'] + "/rqc* " + kat_folder, shell=True)
        subprocess.run("cp " + root_path + "/test/kat_values/" + rqc['name3'] + "/* " + kat_folder, shell=True)


### Creating zip archive
shutil.make_archive(root_path + "/bin/rqc-submission_" + rqc_version, 'zip', build_path)

