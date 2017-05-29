#!/usr/bin/python3

import sys
import os
import subprocess

if len(sys.argv) != 3:
    print("python3 scale.py direktorij faktor")
    exit()

path = os.path.abspath(sys.argv[1])
scale = sys.argv[2]

for root, dirs, files in os.walk(path):
    for file in files:
        name = file.split('.')[0]
        imagePath = os.path.join(root, name)
        if file.split('.')[-1] == 'png':
            # scale it
            subprocess.call(["../scaleImage", imagePath+".png", str(scale), imagePath+"_scaled.png"])
            print("Scaling ", imagePath+".png")

