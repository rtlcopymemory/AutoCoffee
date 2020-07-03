#!/usr/bin/env python3

import os
import re
path = os.path.dirname(os.path.abspath(__file__))

files: list = []  # This will contain names of all the files .html
for file in os.listdir(path):
    if file.endswith(".html"):
        files.append(file)

for page in files:
    with open(path + "/" + page, 'r+') as source:
        with open(path + "/../AutoCoffee/" + page[:-4] + "h", 'w+') as dest:
            dest.write("char *" + page[:-5] + " =\n")
            for line in source.readlines():
                line = re.sub("\"", "\\\"", line)
                dest.write('"' + line[:-1] + '"\n')
            dest.write(";")
