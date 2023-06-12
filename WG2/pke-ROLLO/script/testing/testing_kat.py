import hashlib
import os
import subprocess
import sys
import yaml

root_path = os.path.dirname(os.path.realpath(__file__)) + "/../.."

### SHA256 computation function
def sha256sum(file_path):
    BUFFER_SIZE = 65536
    
    sha256 = hashlib.sha256()

    with open(file_path, 'rb') as file:
        while True:
            data = file.read(BUFFER_SIZE)
            if not data:
                break

            sha256.update(data)

    return sha256.hexdigest()


### KAT compare function
def compare_kat(scheme, sec_level, path1, path2):
    sec_level = str(sec_level)

    hash1 = sha256sum(path1)
    hash2 = sha256sum(path2)

    result = hash1 == hash2

    if(result):
        print("\n" + scheme + "_" + sec_level + ": SUCCESS")
    else:
        print("\n" + scheme + "_" + sec_level + ": FAILURE")

    print("    " + hash1)
    print("    " + hash2 + "\n")

    return result


### KAT test function
def test_kat(scheme, sec_level):
    KAT_NAME = {
        "rqc128": 1874,
        "rqc192": 2893,
        "rqc256": 4130,
    	"rolloI128": 40,
    	"rolloI192": 40,
    	"rolloI256": 40,
    	"rolloII128": 2001,
    	"rolloII192": 2381,
    	"rolloII256": 2599,
    }

    sec_level = str(sec_level)

    subprocess.run(root_path + "/bin/kat_" + scheme + "_" + sec_level, shell=True)

    kat1_path = root_path + "/PQCkemKAT_" + str(KAT_NAME[scheme + sec_level])
    kat2_path = root_path + "/test/kat_values/" + scheme + "-" + sec_level + "/" + scheme + "-" + sec_level + "_kat.rsp"

    result = compare_kat(scheme, sec_level, kat1_path + ".rsp", kat2_path)

    subprocess.run("rm " + kat1_path + ".req", shell=True)
    subprocess.run("rm " + kat1_path + ".rsp", shell=True)

    return result





### Parsing configuration file
config_file = sys.argv[1] if len(sys.argv) > 1 else root_path + "/config.yml"
with open(config_file, 'r') as yaml_file: 
    config = yaml.safe_load(yaml_file)


### KAT test
print("\n\n### Testing KAT\n\n")

result = True
for scheme in config['rbc_build_parameters']:
    if(scheme['name'] != "dummy"):
        result &= test_kat(scheme['name'], scheme['sec_lvl'])


if(result == True):
    os._exit(0)
else:
    os._exit(1)

