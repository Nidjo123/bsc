#!/usr/bin/python3

import sys
import subprocess
import os

if len(sys.argv) != 2:
    print("Usage: python3 kitti_2015.py path_to_pictures")
    exit()

path = os.path.abspath(sys.argv[1])
path = os.path.join(path, "training")
max_disparity = 140
max_window = 34
P1 = 3
P2 = 22

correspondencies = ['SSD', 'ZSAD', 'Census', 'BT']
    
def name(correspondence, window, picture):
    return "_".join([picture.split(".")[0], correspondence, str(window), str(max_disparity)]) + ".png"

for image in os.listdir(os.path.join(path, "image_2")):
    # call stereoMatching on pictures

    leftImage = os.path.join(path, "image_2", image)
    rightImage = os.path.join(path, "image_3", image)
    resultPath = os.path.join(".", "kitti_2015_results")

    if not os.path.exists(resultPath):
        os.makedirs(resultPath)

    for correspondence in correspondencies:
        for window in range(1, max_window, 2):
            localPath = os.path.abspath(os.path.join(resultPath, "local_" + name(correspondence, window, image)))
            sgmPath = os.path.abspath(os.path.join(resultPath, "sgm_" + name(correspondence, window, image)))
            subprocess.call(["../stereoMatching", leftImage, rightImage, correspondence, str(window), str(max_disparity), str(P1), str(P2), localPath, sgmPath])

            print(localPath, sgmPath)
            if correspondence == "BT": break # doesn't depend on window size
