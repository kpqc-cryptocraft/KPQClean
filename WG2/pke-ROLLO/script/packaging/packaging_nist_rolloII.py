
import os
import subprocess
import shutil
import sys

root_path = os.path.dirname(os.path.realpath(__file__)) + "/../.."
script_path = root_path + "/script"
packaging_path = root_path + "/script/packaging"
build_path = root_path + "/build/packaging/nist/rolloII"

sys.path.insert(1, script_path)
from rbc_utils import *


rolloII_version = "2020_04_15"

implem_list = [
    {"name":"Reference_Implementation", "core":"c64"},
    {"name":"Optimized_Implementation", "core":"avx"},
]

rolloII_list = [
    {"name":"rolloII128", "name2":"rolloII_128", "name3":"rolloII-128", "core":"rbc-83"},
    {"name":"rolloII192", "name2":"rolloII_192", "name3":"rolloII-192", "core":"rbc-97"},
    {"name":"rolloII256", "name2":"rolloII_256", "name3":"rolloII-256", "core":"rbc-97"},
]


### Creating output folder
subprocess.run("rm -rf " + build_path, shell=True)
subprocess.run("mkdir -p " + build_path, shell=True)

for implem in implem_list:
    for rolloII in rolloII_list:
        subprocess.run("mkdir -p " + build_path + "/" + str(implem['name']) + "/" + str(rolloII['name']), shell=True)


### Packaging third party lib
for implem in implem_list:
    for rolloII in rolloII_list:
        lib_folder = build_path + "/" + implem['name'] + "/" + rolloII['name'] + "/lib"
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
    for rolloII in rolloII_list:
        src_folder = build_path + "/" + implem['name'] + "/" + rolloII['name'] + "/src"
        subprocess.run("mkdir -p " + src_folder + "/" + rolloII['core'] + "/", shell=True)
        subprocess.run("cp -r " + root_path + "/src/core/" + implem['core'] + "/" + rolloII['core'] + "/* " + src_folder + "/" + rolloII['core'], shell=True)
        subprocess.run("cp -r " + root_path + "/src/codes/lrpc/* " + src_folder, shell=True)
        subprocess.run("cp -r " + root_path + "/src/params/" + rolloII['name'] + "/* " + src_folder, shell=True)
        subprocess.run("cp -r " + root_path + "/src/schemes/rolloII/* "+ src_folder, shell=True)
        subprocess.run("cp " + root_path + "/script/packaging/rolloII/" + rolloII['name'] + "/api.h " + src_folder, shell=True)

        subprocess.run("mv " + src_folder + "/rbc_lrpc.h " + src_folder + "/lrpc.h", shell=True)
        subprocess.run("mv " + src_folder + "/rbc_lrpc.c " + src_folder + "/lrpc.c", shell=True)
        subprocess.run("mv " + src_folder + "/" + rolloII['name2'] + "_parameters.h " + src_folder + "/parameters.h", shell=True)
        subprocess.run("mv " + src_folder + "/rolloII_parsing.h " + src_folder + "/parsing.h", shell=True)
        subprocess.run("mv " + src_folder + "/rolloII_parsing.c " + src_folder + "/parsing.c", shell=True)
        subprocess.run("mv " + src_folder + "/rolloII_types.h " + src_folder + "/types.h", shell=True)
        subprocess.run("mv " + src_folder + "/rolloII_kem.c " + src_folder + "/rolloII.c", shell=True)

        replace_in_file(src_folder + "/parameters.h", rolloII['name2'].upper(), "ROLLOII");
        replace_in_file(src_folder + "/types.h", "rolloII_parameters.h", "parameters.h");
        replace_in_file(src_folder + "/parsing.c", "rolloII_parameters.h", "parameters.h");
        replace_in_file(src_folder + "/parsing.c", "rolloII_parsing.h", "parsing.h");
        replace_in_file(src_folder + "/parsing.c", "rolloII_types.h", "types.h");
        replace_in_file(src_folder + "/parsing.h", "rolloII_types.h", "types.h");
        replace_in_file(src_folder + "/lrpc.c", "rbc_lrpc.h", "lrpc.h");
        replace_in_file(src_folder + "/rolloII.c", "rbc_lrpc.h", "lrpc.h");
        replace_in_file(src_folder + "/rolloII.c", "rolloII_types.h", "types.h");
        replace_in_file(src_folder + "/rolloII.c", "rolloII_parameters.h", "parameters.h");
        replace_in_file(src_folder + "/rolloII.c", "rolloII_parsing.h", "parsing.h");
        replace_in_file(src_folder + "/rolloII.c", "rolloII_kem.h", "api.h");
        replace_in_file(src_folder + "/rolloII.c", "rolloII_kem_keygen", "crypto_kem_keypair");
        replace_in_file(src_folder + "/rolloII.c", "rolloII_kem_encaps", "crypto_kem_enc");
        replace_in_file(src_folder + "/rolloII.c", "rolloII_kem_decaps", "crypto_kem_dec");


### Packaging documentation
for implem in implem_list:
    for rolloII in rolloII_list:
        doc_folder = build_path + "/" + implem['name'] + "/" + rolloII['name'] + "/doc"
        subprocess.run("mkdir -p " + doc_folder, shell=True)
        subprocess.run("cp -r " + root_path + "/script/packaging/rolloII/doc/* " + doc_folder, shell=True)
        subprocess.run("cp " + root_path + "/script/packaging/rolloII/doxygen.conf " + doc_folder + "/..", shell=True)
    subprocess.run("cp " + root_path + "/script/packaging/rolloII/README " + build_path + "/" + implem['name'] + "/", shell=True)


### Packaging build system
for implem in implem_list:
    for rolloII in rolloII_list:
        build_folder = build_path + "/" + implem['name'] + "/" + rolloII['name']
        subprocess.run("cp " + packaging_path + "/rolloII/main_rolloII.c " + build_folder + "/src", shell=True)
        subprocess.run("cp " + packaging_path + "/rolloII/main_kat.c " + build_folder + "/src", shell=True)
        subprocess.run("cp " + packaging_path + "/rolloII/" + rolloII['name'] + "/Makefile " + build_folder, shell=True)
        if implem == "Reference_Implementation":
            replace_in_file(build_folder + "/Makefile", "-mavx2 -mpclmul -msse4.1 ", "")


### Packaging KATs
for implem in implem_list:
    for rolloII in rolloII_list:
        kat_folder = build_path + "/KATs/" + implem['name'] + "/" + rolloII['name']
        subprocess.run("mkdir -p " + kat_folder, shell=True)
        subprocess.run("cp " + root_path + "/test/kat_values/" + rolloII['name3'] + "/* " + kat_folder, shell=True)


### Creating zip archive
shutil.make_archive(root_path + "/bin/rolloII-submission_" + rolloII_version, 'zip', build_path)

