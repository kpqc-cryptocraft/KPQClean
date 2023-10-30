
import os
import subprocess
import shutil
import sys

root_path = os.path.dirname(os.path.realpath(__file__)) + "/../.."
script_path = root_path + "/script"
packaging_path = root_path + "/script/packaging"
build_path = root_path + "/build/packaging/nist/rolloI"

sys.path.insert(1, script_path)
from rbc_utils import *


rolloI_version = "2020_04_15"

implem_list = [
    {"name":"Reference_Implementation", "core":"c64"},
    {"name":"Optimized_Implementation", "core":"avx"},
]

rolloI_list = [
    {"name":"rolloI128", "name2":"rolloI_128", "name3":"rolloI-128", "core":"rbc-67"},
    {"name":"rolloI192", "name2":"rolloI_192", "name3":"rolloI-192", "core":"rbc-79"},
    {"name":"rolloI256", "name2":"rolloI_256", "name3":"rolloI-256", "core":"rbc-97"},
]


### Creating output folder
subprocess.run("rm -rf " + build_path, shell=True)
subprocess.run("mkdir -p " + build_path, shell=True)

for implem in implem_list:
    for rolloI in rolloI_list:
        subprocess.run("mkdir -p " + build_path + "/" + str(implem['name']) + "/" + str(rolloI['name']), shell=True)


### Packaging third party lib
for implem in implem_list:
    for rolloI in rolloI_list:
        lib_folder = build_path + "/" + implem['name'] + "/" + rolloI['name'] + "/lib"
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
    for rolloI in rolloI_list:
        src_folder = build_path + "/" + implem['name'] + "/" + rolloI['name'] + "/src"
        subprocess.run("mkdir -p " + src_folder + "/" + rolloI['core'] + "/", shell=True)
        subprocess.run("cp -r " + root_path + "/src/core/" + implem['core'] + "/" + rolloI['core'] + "/* " + src_folder + "/" + rolloI['core'], shell=True)
        subprocess.run("cp -r " + root_path + "/src/codes/lrpc/* " + src_folder, shell=True)
        subprocess.run("cp -r " + root_path + "/src/params/" + rolloI['name'] + "/* " + src_folder, shell=True)
        subprocess.run("cp -r " + root_path + "/src/schemes/rolloI/* "+ src_folder, shell=True)
        subprocess.run("cp " + root_path + "/script/packaging/rolloI/" + rolloI['name'] + "/api.h " + src_folder, shell=True)

        subprocess.run("mv " + src_folder + "/rbc_lrpc.h " + src_folder + "/lrpc.h", shell=True)
        subprocess.run("mv " + src_folder + "/rbc_lrpc.c " + src_folder + "/lrpc.c", shell=True)
        subprocess.run("mv " + src_folder + "/" + rolloI['name2'] + "_parameters.h " + src_folder + "/parameters.h", shell=True)
        subprocess.run("mv " + src_folder + "/rolloI_parsing.h " + src_folder + "/parsing.h", shell=True)
        subprocess.run("mv " + src_folder + "/rolloI_parsing.c " + src_folder + "/parsing.c", shell=True)
        subprocess.run("mv " + src_folder + "/rolloI_types.h " + src_folder + "/types.h", shell=True)
        subprocess.run("mv " + src_folder + "/rolloI_kem.c " + src_folder + "/rolloI.c", shell=True)

        replace_in_file(src_folder + "/parameters.h", rolloI['name2'].upper(), "ROLLOI");
        replace_in_file(src_folder + "/parsing.c", "rolloI_parameters.h", "parameters.h");
        replace_in_file(src_folder + "/parsing.c", "rolloI_parsing.h", "parsing.h");
        replace_in_file(src_folder + "/parsing.c", "rolloI_types.h", "types.h");
        replace_in_file(src_folder + "/parsing.h", "rolloI_types.h", "types.h");
        replace_in_file(src_folder + "/lrpc.c", "rbc_lrpc.h", "lrpc.h");
        replace_in_file(src_folder + "/rolloI.c", "rbc_lrpc.h", "lrpc.h");
        replace_in_file(src_folder + "/rolloI.c", "rolloI_types.h", "types.h");
        replace_in_file(src_folder + "/rolloI.c", "rolloI_parameters.h", "parameters.h");
        replace_in_file(src_folder + "/rolloI.c", "rolloI_parsing.h", "parsing.h");
        replace_in_file(src_folder + "/rolloI.c", "rolloI_kem.h", "api.h");
        replace_in_file(src_folder + "/rolloI.c", "rolloI_kem_keygen", "crypto_kem_keypair");
        replace_in_file(src_folder + "/rolloI.c", "rolloI_kem_encaps", "crypto_kem_enc");
        replace_in_file(src_folder + "/rolloI.c", "rolloI_kem_decaps", "crypto_kem_dec");


### Packaging documentation
for implem in implem_list:
    for rolloI in rolloI_list:
        doc_folder = build_path + "/" + implem['name'] + "/" + rolloI['name'] + "/doc"
        subprocess.run("mkdir -p " + doc_folder, shell=True)
        subprocess.run("cp -r " + root_path + "/script/packaging/rolloI/doc/* " + doc_folder, shell=True)
        subprocess.run("cp " + root_path + "/script/packaging/rolloI/doxygen.conf " + doc_folder + "/..", shell=True)
    subprocess.run("cp " + root_path + "/script/packaging/rolloI/README " + build_path + "/" + implem['name'] + "/", shell=True)


### Packaging build system
for implem in implem_list:
    for rolloI in rolloI_list:
        build_folder = build_path + "/" + implem['name'] + "/" + rolloI['name']
        subprocess.run("cp " + packaging_path + "/rolloI/main_rolloI.c " + build_folder + "/src", shell=True)
        subprocess.run("cp " + packaging_path + "/rolloI/main_kat.c " + build_folder + "/src", shell=True)
        subprocess.run("cp " + packaging_path + "/rolloI/" + rolloI['name'] + "/Makefile " + build_folder, shell=True)
        if implem == "Reference_Implementation":
            replace_in_file(build_folder + "/Makefile", "-mavx2 -mpclmul -msse4.1 ", "")


### Packaging KATs
for implem in implem_list:
    for rolloI in rolloI_list:
        kat_folder = build_path + "/KATs/" + implem['name'] + "/" + rolloI['name']
        subprocess.run("mkdir -p " + kat_folder, shell=True)
        subprocess.run("cp " + root_path + "/test/kat_values/" + rolloI['name3'] + "/* " + kat_folder, shell=True)


### Creating zip archive
shutil.make_archive(root_path + "/bin/rolloI-submission_" + rolloI_version, 'zip', build_path)

