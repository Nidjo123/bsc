#!/usr/bin/python3

import sys
import subprocess
import os

if len(sys.argv) != 2:
    print("Usage: python3 kitti_2015_results.py path_to_kitti_2015")
    exit()
    
path = os.path.abspath(sys.argv[1])
path = os.path.join(path, "training")
leftDispPath = os.path.join(path, "disp_noc_0")
rightDispPath = os.path.join(path, "disp_noc_1")

results = []

for image in os.listdir(leftDispPath):
    leftImage = os.path.join(leftDispPath, image)
    rightImage = os.path.join(rightDispPath, image)

    name = image.split('.')[0]
    name += "_150.png"
    resultPath = os.path.join(".", "kitti_2015_sgmbt_results", "sgmbt_" + name)
    resultPath = os.path.abspath(resultPath)
    
    res = subprocess.check_output(["../evaluator", leftImage, rightImage, resultPath])

    res = res.decode("utf8")

    print(res)

    results.append((image, float(res)))

results = sorted(results, key=lambda x: x[1])
    
with open("kitti_2015_sgm.out", "w") as file:
    for img, error in results:
        s = img + " " + str(error) + "\n"
        file.write(s)

print("Minimal error: ", results[0])
print("Maximal error: ", results[-1])
