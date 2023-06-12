import subprocess

def copy_file(input_file, output_file):
    subprocess.run("cp " + input_file + " " + output_file, shell=True)



def rename_file(input_file, output_file):
    subprocess.run("mv " + input_file + " " + output_file, shell=True)



def read_file(path):
    with open(path, 'r') as file:
        return file.read()



def write_in_file(path, data):
    with open(path, 'w') as file:
        file.write(data)



def append_in_file(path, data):
    with open(path, 'a') as file:
        file.write(data)



def replace_in_file(path, str_in, str_out):
    with open(path, 'r') as file:
        data = file.read()

    data = data.replace(str_in, str_out)

    with open(path, 'w') as file:
        file.write(data)



def remove_lines_containing_string(path, delete):
    with open(path, 'r') as file:
        lines = file.readlines()

    with open(path, 'w') as file:
        for index, line in enumerate(lines):
            if line.find(delete) == -1:
                file.write(line)



def remove_all_lines_from_position(path, position):
    with open(path, 'r') as file:
        lines = file.readlines()

    with open(path, 'w') as file:
        for index, line in enumerate(lines):
            if index < position:
                file.write(line)
            else:
                break



def remove_all_lines_before_pattern(path, pattern):
    with open(path, 'r') as file:
        lines = file.readlines()

    pattern_detected = 0
    with open(path, 'w') as file:
        for index, line in enumerate(lines):
            if line.find(pattern) != -1:
                pattern_detected = 1
            if pattern_detected == 1:
                file.write(line)



def remove_duplicates(result):
    lines=[]
    new_result = ""
    for line in result.split('\n'):
         if line not in lines:
             lines.append(line)
             new_result += line
             new_result += '\n'

    return new_result



def append_file_to_file(path1, path2):
    with open(path1, 'r') as file1, open(path2, 'a') as file2:
        for line in file1:
            file2.write(line)



def remove_list_duplicates(list1):
    return list(set(list1))



def intersect_list_preserving_order(list1, list2):
    return [x for x in list1 if x in set(list2)]



