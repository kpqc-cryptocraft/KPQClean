import re

f = open(r'tiger_256.massif.out.217625.txt')

heap = 0
stack = 0

max_heap = 0
max_stack = 0

while True:
    line = f.readline()
    
    if not line:
        break

    if "mem_heap_extra_B=" in line:
        heap = int(re.sub(r'[^0-9]', '', line))

    if "mem_stacks_B=" in line:
        stack = int(re.sub(r'[^0-9]', '', line))
        if stack > max_stack:
            max_heap = heap
            max_stack = stack

memory = max_heap + max_stack

print("heap {}".format(max_heap))
print("stack {}".format(max_stack))
print("memory consumption {}".format(memory))
