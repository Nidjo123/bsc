#!/usr/bin/python3

import sys
import subprocess
import os

if len(sys.argv) != 2:
    print("Usage: python3 middlebury_2006.py path_to_pictures")
    exit()

path = os.path.abspath(sys.argv[1])
max_disparity = 100
max_window = 34
P1 = 3
P2 = 22

correspondencies = ['SSD', 'ZSAD', 'Census', 'BT']
    
def name(correspondence, window, picture):
    return "_".join([picture, correspondence, str(window), str(max_disparity)]) + ".png"

for directory in os.listdir(path):
    dirPath = os.path.join(path, directory)

    if os.path.isdir(dirPath):
        # call stereoMatching on pictures
        # and then calculate error

        leftImage = os.path.join(dirPath, "view1.png")
        rightImage = os.path.join(dirPath, "view5.png")

        leftTruth = os.path.join(dirPath, "disp1.png")
        rightTruth = os.path.join(dirPath, "disp5.png")

        resultPath = os.path.join(".", "middlebury_2006_results", directory)

        if not os.path.exists(resultPath):
            os.makedirs(resultPath)
    
        for correspondence in correspondencies:
            for window in range(1, max_window, 2):
                localPath = os.path.abspath(os.path.join(resultPath, "local_" + name(correspondence, window, directory)))
                sgmPath = os.path.abspath(os.path.join(resultPath, "sgm_" + name(correspondence, window, directory)))
                subprocess.call(["../stereoMatching", leftImage, rightImage, correspondence, str(window), str(max_disparity), str(P1), str(P2), localPath, sgmPath])
                if correspondence == "BT": break
