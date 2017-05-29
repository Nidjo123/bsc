#!/usr/bin/python3

import os
import sys
import subprocess

if len(sys.argv) != 2:
    print("python3 middlebury_2006_directory")
    exit()

dispPath = os.path.abspath(sys.argv[1])
resultPath = os.path.abspath("./middlebury_2006_results/")

res = []

output = open("middlebury_2006.out", "w")

for root, dirs, files in os.walk(resultPath):
    for file in files:
        if not "scaled" in file: continue
        
        name = file.split('_')[1]
        currDispPath = os.path.join(dispPath, name)
        leftDisp = os.path.join(currDispPath, "disp1.png")
        rightDisp = os.path.join(currDispPath, "disp5.png")
        path = os.path.join(root, file)
        
        print(path)
        if file.split('.')[-1] == 'png':
            # scale it
            result = subprocess.check_output(["../evaluator", leftDisp, rightDisp, path])
            error = result.decode("utf8").split("\n")[-1]
            res.append((error, path))

            output.write(file + " " + error + "\n")

print(max(res), min(res))

output.close()

