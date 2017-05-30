#!/usr/bin/python3

import sys
import subprocess
import os

if len(sys.argv) != 2:
    print("Usage: python3 kitti_2015_sgm.py path_to_pictures")
    exit()

path = os.path.abspath(sys.argv[1])
path = os.path.join(path, "training")
max_disparity = 150
P1 = 3
P2 = 22

def name(picture):
    return "_".join(["sgmbt", picture.split(".")[0], str(max_disparity)]) + ".png"

for image in os.listdir(os.path.join(path, "image_2")):
    leftImage = os.path.join(path, "image_2", image)
    rightImage = os.path.join(path, "image_3", image)
    resultPath = os.path.join(".", "kitti_2015_sgmbt8_results")

    if not os.path.exists(resultPath):
        os.makedirs(resultPath)

    sgmPath = os.path.abspath(os.path.join(resultPath, name(image)))
    subprocess.call(["../sgm", leftImage, rightImage, str(max_disparity), str(P1), str(P2), sgmPath])
