
import os
import subprocess
import sys
import yaml

root_path = os.path.dirname(os.path.realpath(__file__)) + "/../.."
preprocessing_path = root_path + "/script/preprocessing"
script_path = root_path + "/script"

sys.path.insert(1, script_path)
from rbc_utils import *



def preprocessing_src_cmake(config):
    copy_file(root_path + "/src/CMakeLists.txt", root_path + "/build/CMakeLists.txt")

    replace_in_file(root_path + "/build/CMakeLists.txt", "{TEMPLATING_CMAKE_SET_DIR}", preprocessing_cmake_set_dir(config))
    replace_in_file(root_path + "/build/CMakeLists.txt", "{TEMPLATING_CMAKE_ADD_SUBDIRECTORY}", preprocessing_cmake_add_subdirectory(config))
    replace_in_file(root_path + "/build/CMakeLists.txt", "{TEMPLATING_CMAKE_ADD_LIBRARY}", preprocessing_cmake_add_library(config))
    replace_in_file(root_path + "/build/CMakeLists.txt", "{TEMPLATING_CMAKE_INSTALL_FILES}", preprocessing_cmake_install_files(config))
    replace_in_file(root_path + "/build/CMakeLists.txt", "CONFIG_CORE_LAYER_IMPLEM", config['rbc_core_layer_implem']);
    replace_in_file(root_path + "/build/CMakeLists.txt", "CONFIG_LIB_DIR", root_path + "/bin");



### Cmake set dir preprocessing
def preprocessing_cmake_set_dir(config):
    result = ""

    # Add core for each scheme or dummy keyword used
    for scheme in config['rbc_build_parameters']:
        result += "set(CORE_" + str(scheme['m']) + "_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src/core/${CORE_LAYER_IMPLEM}/rbc-" + str(scheme['m']) + ")\n"

    # Add Gabidulin codes for each scheme or dummy keyword used
    result += "\n"
    for scheme in config['rbc_build_parameters']:
        if(scheme['name'] == "dummy"):
            result += "set(GABIDULIN_" + str(scheme['m']) + "_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src/codes/gabidulin-" + str(scheme['m']) + ")\n"

        if(scheme['name'] != "dummy"):
            scheme_config_file = root_path + "/src/schemes/" + scheme['name'] + "/config.yml"
            with open(scheme_config_file, 'r') as yaml_file: 
                scheme_config = yaml.safe_load(yaml_file)
        
            if('gabidulin' in scheme_config):
                result += "set(GABIDULIN_" + str(scheme['m']) + "_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src/codes/gabidulin-" + str(scheme['m']) + ")\n"

    # Add LRPC codes for each scheme or dummy keyword used
    result += "\n"
    for scheme in config['rbc_build_parameters']:
        if(scheme['name'] == "dummy"):
            result += "set(LRPC_" + str(scheme['m']) + "_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src/codes/lrpc-" + str(scheme['m']) + ")\n"

        if(scheme['name'] != "dummy"):
            scheme_config_file = root_path + "/src/schemes/" + scheme['name'] + "/config.yml"
            with open(scheme_config_file, 'r') as yaml_file: 
                scheme_config = yaml.safe_load(yaml_file)
        
            if('lrpc' in scheme_config):
                result += "set(LRPC_" + str(scheme['m']) + "_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src/codes/lrpc-" + str(scheme['m']) + ")\n"

    # Add schemes
    result += "\n"
    for scheme in config['rbc_build_parameters']:
        if(scheme['name'] != "dummy"):
            result += "set(" + str(scheme['name']).upper() + str(scheme['sec_lvl']) + "_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src/schemes/" + str(scheme['name']) + str(scheme['sec_lvl']) + ")\n"

    return result.rstrip()



### Cmake add subdirectory preprocessing
def preprocessing_cmake_add_subdirectory(config):
    result = ""

    # Add core for each scheme or dummy keyword used
    for scheme in config['rbc_build_parameters']:
        result += "add_subdirectory(src/core/${CORE_LAYER_IMPLEM}/rbc-" + str(scheme['m']) + ")\n"

    # Add Gabidulin codes for each scheme or dummy keyword used
    result += "\n"
    for scheme in config['rbc_build_parameters']:
        if(scheme['name'] == "dummy"):
            result += "add_subdirectory(src/codes/gabidulin-" + str(scheme['m']) + ")\n"

        if(scheme['name'] != "dummy"):
            scheme_config_file = root_path + "/src/schemes/" + scheme['name'] + "/config.yml"
            with open(scheme_config_file, 'r') as yaml_file: 
                scheme_config = yaml.safe_load(yaml_file)
        
            if('gabidulin' in scheme_config):
                result += "add_subdirectory(src/codes/gabidulin-" + str(scheme['m']) + ")\n"

    # Add LRPC codes for each scheme or dummy keyword used
    result += "\n"
    for scheme in config['rbc_build_parameters']:
        if(scheme['name'] == "dummy"):
            result += "add_subdirectory(src/codes/lrpc-" + str(scheme['m']) + ")\n"

        if(scheme['name'] != "dummy"):
            scheme_config_file = root_path + "/src/schemes/" + scheme['name'] + "/config.yml"
            with open(scheme_config_file, 'r') as yaml_file: 
                scheme_config = yaml.safe_load(yaml_file)
        
            if('lrpc' in scheme_config):
                result += "add_subdirectory(src/codes/lrpc-" + str(scheme['m']) + ")\n"

    # Add schemes
    result += "\n"
    for scheme in config['rbc_build_parameters']:
        if(scheme['name'] != "dummy"):
            result += "add_subdirectory(src/schemes/" + str(scheme['name']) + str(scheme['sec_lvl']) + ")\n"

    return remove_duplicates(result).rstrip()



### Cmake add library preprocessing
def preprocessing_cmake_add_library(config):
    result = ""

    # Add core for each scheme or dummy keyword used
    for scheme in config['rbc_build_parameters']:
        result += "  $<TARGET_OBJECTS:core_" + str(scheme['m']) + ">\n"

    # Add Gabidulin codes for each scheme or dummy keyword used
    result += "\n"
    for scheme in config['rbc_build_parameters']:
        if(scheme['name'] == "dummy"):
            result += "  $<TARGET_OBJECTS:gabidulin_" + str(scheme['m']) + ">\n"

        if(scheme['name'] != "dummy"):
            scheme_config_file = root_path + "/src/schemes/" + scheme['name'] + "/config.yml"
            with open(scheme_config_file, 'r') as yaml_file: 
                scheme_config = yaml.safe_load(yaml_file)
        
            if('gabidulin' in scheme_config):
                result += "  $<TARGET_OBJECTS:gabidulin_" + str(scheme['m']) + ">\n"

    # Add LRPC codes for each scheme or dummy keyword used
    result += "\n"
    for scheme in config['rbc_build_parameters']:
        if(scheme['name'] == "dummy"):
            result += "  $<TARGET_OBJECTS:lrpc_" + str(scheme['m']) + ">\n"

        if(scheme['name'] != "dummy"):
            scheme_config_file = root_path + "/src/schemes/" + scheme['name'] + "/config.yml"
            with open(scheme_config_file, 'r') as yaml_file: 
                scheme_config = yaml.safe_load(yaml_file)
        
            if('lrpc' in scheme_config):
                result += "  $<TARGET_OBJECTS:lrpc_" + str(scheme['m']) + ">\n"

    # Add schemes
    result += "\n"
    for scheme in config['rbc_build_parameters']:
        if(scheme['name'] != "dummy"):
            result += "  $<TARGET_OBJECTS:" + scheme['name'] + "_" + str(scheme['sec_lvl']) + ">\n"

    return result.rstrip()



### Cmake install file preprocessing
def preprocessing_cmake_install_files(config):
    result = ""

    # Add core for each scheme or dummy keyword used
    for scheme in config['rbc_build_parameters']:
        result += "install(FILES ${RBC_LIB_SRC}/core/${CORE_LAYER_IMPLEM}/rbc-" + str(scheme['m']) + "/rbc_" + str(scheme['m']) + ".h        DESTINATION ${RBC_LIB_INSTALL_PATH_INC})\n"
        result += "install(FILES ${RBC_LIB_SRC}/core/${CORE_LAYER_IMPLEM}/rbc-" + str(scheme['m']) + "/rbc_" + str(scheme['m']) + "_elt.h    DESTINATION ${RBC_LIB_INSTALL_PATH_INC})\n"
        result += "install(FILES ${RBC_LIB_SRC}/core/${CORE_LAYER_IMPLEM}/rbc-" + str(scheme['m']) + "/rbc_" + str(scheme['m']) + "_vec.h    DESTINATION ${RBC_LIB_INSTALL_PATH_INC})\n"
        result += "install(FILES ${RBC_LIB_SRC}/core/${CORE_LAYER_IMPLEM}/rbc-" + str(scheme['m']) + "/rbc_" + str(scheme['m']) + "_poly.h   DESTINATION ${RBC_LIB_INSTALL_PATH_INC})\n"
        result += "install(FILES ${RBC_LIB_SRC}/core/${CORE_LAYER_IMPLEM}/rbc-" + str(scheme['m']) + "/rbc_" + str(scheme['m']) + "_qre.h    DESTINATION ${RBC_LIB_INSTALL_PATH_INC})\n"
        result += "install(FILES ${RBC_LIB_SRC}/core/${CORE_LAYER_IMPLEM}/rbc-" + str(scheme['m']) + "/rbc_" + str(scheme['m']) + "_vspace.h DESTINATION ${RBC_LIB_INSTALL_PATH_INC})\n"

    # Add Gabidulin codes for each scheme or dummy keyword used
    result += "\n"
    for scheme in config['rbc_build_parameters']:
        if(scheme['name'] == "dummy"):
            result += "install(FILES ${RBC_LIB_SRC}/codes/gabidulin-" + str(scheme['m']) + "/rbc_" + str(scheme['m']) + "_qpoly.h     DESTINATION ${RBC_LIB_INSTALL_PATH_INC})\n"
            result += "install(FILES ${RBC_LIB_SRC}/codes/gabidulin-" + str(scheme['m']) + "/rbc_" + str(scheme['m']) + "_gabidulin.h DESTINATION ${RBC_LIB_INSTALL_PATH_INC})\n"

        if(scheme['name'] != "dummy"):
            scheme_config_file = root_path + "/src/schemes/" + scheme['name'] + "/config.yml"
            with open(scheme_config_file, 'r') as yaml_file: 
                scheme_config = yaml.safe_load(yaml_file)
        
            if('gabidulin' in scheme_config):
                result += "install(FILES ${RBC_LIB_SRC}/codes/gabidulin-" + str(scheme['m']) + "/rbc_" + str(scheme['m']) + "_qpoly.h     DESTINATION ${RBC_LIB_INSTALL_PATH_INC})\n"
                result += "install(FILES ${RBC_LIB_SRC}/codes/gabidulin-" + str(scheme['m']) + "/rbc_" + str(scheme['m']) + "_gabidulin.h DESTINATION ${RBC_LIB_INSTALL_PATH_INC})\n"

    # Add LRPC codes for each scheme or dummy keyword used
    result += "\n"
    for scheme in config['rbc_build_parameters']:
        if(scheme['name'] == "dummy"):
            result += "install(FILES ${RBC_LIB_SRC}/codes/lrpc-" + str(scheme['m']) + "/rbc_" + str(scheme['m']) + "_lrpc.h           DESTINATION ${RBC_LIB_INSTALL_PATH_INC})\n"

        if(scheme['name'] != "dummy"):
            scheme_config_file = root_path + "/src/schemes/" + scheme['name'] + "/config.yml"
            with open(scheme_config_file, 'r') as yaml_file: 
                scheme_config = yaml.safe_load(yaml_file)
        
            if('lrpc' in scheme_config):
                result += "install(FILES ${RBC_LIB_SRC}/codes/lrpc-" + str(scheme['m']) + "/rbc_" + str(scheme['m']) + "_lrpc.h             DESTINATION ${RBC_LIB_INSTALL_PATH_INC})\n"

    # Add schemes
    result += "\n"
    for scheme in config['rbc_build_parameters']:
        if(scheme['name'] != "dummy"):
            local_config_file = root_path + "/src/schemes/" + scheme['name'] + "/config.yml"
            with open(local_config_file, 'r') as yaml_file: 
                local_config = yaml.safe_load(yaml_file)

            lw_scheme_name = local_config['lowercase_scheme_name']
            for filename in local_config['source_files']:
                if filename[-2:] == ".h":
                    result += "install(FILES ${RBC_LIB_SRC}/schemes/" + lw_scheme_name + str(scheme['sec_lvl']) + "/" + lw_scheme_name + "_" + str(scheme['sec_lvl']) + "_" + filename + "           DESTINATION ${RBC_LIB_INSTALL_PATH_INC})\n"

            result += "install(FILES ${RBC_LIB_SRC}/schemes/" + scheme['name'] + str(scheme['sec_lvl']) + "/" + scheme['name'] + "_" + str(scheme['sec_lvl']) + "_parameters.h           DESTINATION ${RBC_LIB_INSTALL_PATH_INC})\n"
            result += "\n"

    return result.rstrip()



### Global rbc.h preprocessing
def preprocessing_src_rbc_include(config):
    # Preprocessing for src
    result =""

    result += "#ifndef RBC_LIB_H\n"
    result += "#define RBC_LIB_H\n"
    result += "\n"

    # Add core for each scheme or dummy keyword used
    for scheme in config['rbc_build_parameters']:
        result += "#include \"rbc_" + str(scheme['m']) + ".h\"\n"
        result += "#include \"rbc_" + str(scheme['m']) + "_elt.h\"\n"
        result += "#include \"rbc_" + str(scheme['m']) + "_vec.h\"\n"
        result += "#include \"rbc_" + str(scheme['m']) + "_poly.h\"\n"
        result += "#include \"rbc_" + str(scheme['m']) + "_qre.h\"\n"
        result += "#include \"rbc_" + str(scheme['m']) + "_vspace.h\"\n"
        result += "\n"

    # Add Gabidulin codes for each scheme or dummy keyword used
    result += "\n"
    for scheme in config['rbc_build_parameters']:
        if(scheme['name'] == "dummy"):
            result += "#include \"rbc_" + str(scheme['m']) + "_qpoly.h\"\n"
            result += "#include \"rbc_" + str(scheme['m']) + "_gabidulin.h\"\n"

        if(scheme['name'] != "dummy"):
            scheme_config_file = root_path + "/src/schemes/" + scheme['name'] + "/config.yml"
            with open(scheme_config_file, 'r') as yaml_file: 
                scheme_config = yaml.safe_load(yaml_file)
        
            if('gabidulin' in scheme_config):
                result += "#include \"rbc_" + str(scheme['m']) + "_qpoly.h\"\n"
                result += "#include \"rbc_" + str(scheme['m']) + "_gabidulin.h\"\n"

    # Add LRPC codes for each scheme or dummy keyword used
    result += "\n"
    for scheme in config['rbc_build_parameters']:
        if(scheme['name'] == "dummy"):
            result += "#include \"rbc_" + str(scheme['m']) + "_lrpc.h\"\n"

        if(scheme['name'] != "dummy"):
            scheme_config_file = root_path + "/src/schemes/" + scheme['name'] + "/config.yml"
            with open(scheme_config_file, 'r') as yaml_file: 
                scheme_config = yaml.safe_load(yaml_file)
        
            if('lrpc' in scheme_config):
                result += "#include \"rbc_" + str(scheme['m']) + "_lrpc.h\"\n"

    # Add schemes
    result += "\n"
    for scheme in config['rbc_build_parameters']:
        if(scheme['name'] != "dummy"):
            scheme_config_file = root_path + "/src/schemes/" + scheme['name'] + "/config.yml"
            with open(scheme_config_file, 'r') as yaml_file: 
                scheme_config = yaml.safe_load(yaml_file)

            lw_scheme_name = scheme_config['lowercase_scheme_name']
            for filename in scheme_config['source_files']:
                if filename[-2:] == ".h":
                    result += "#include \"" + lw_scheme_name + "_" + str(scheme['sec_lvl']) + "_" + filename + "\"\n"

            result += "#include \"" + scheme['name'] + "_" + str(scheme['sec_lvl']) + "_parameters.h\"\n"

    result += "\n#endif\n"

    subprocess.run("mkdir -p " + root_path + "/build/src/include", shell=True)
    with open(root_path + "/build/src/include/rbc.h", 'w') as file:
        file.write(result)
        file.close()

    # Preprocessing for build
    subprocess.run("mkdir -p " + root_path + "/build/src/include", shell=True)
