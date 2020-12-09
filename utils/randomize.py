import random
import sys

filename = sys.argv[-1]
if filename[-4:] != ".txt":
    sys.exit("You must provide a .txt file.")

words = []
with open(filename, "r") as f:
    for line in f:
        wil = line.split()
        for i, val in enumerate(wil):
            if val[0] in ['(', '[', '{', '\'', '"']:
                val = val[1:]
            if val[-1] in ['.', '!', '?', ',', ';', ':', 
                    ')', ']', '}', '\'', '"']:
                val = val[:-1]
            words.append(val)

write_order = []
for i in enumerate(words):
    write_order.append(i[0])
random.shuffle(write_order)

with open(filename, "w+") as f:
    line_length = 0
    for i, val in enumerate(write_order):
        line_length += len(words[val]) + 1
        if line_length > 80:
            f.write("\n")
            line_length = len(words[val]) + 1
        f.write(words[val] + " ")

