#!/usr/bin/python3

import matplotlib.pyplot as plt
import itertools
import collections

marker = itertools.cycle(('+', '.', 'o', ',', '*', '^', '-'))
markers = {}

def sortDict(d):
    for key in d:
        d[key] = sorted(d[key], key=lambda x: x[0])

def toArrays(ts):
    a = []
    b = []
    for x, y in ts:
        a.append(x)
        b.append(y)
    return a, b

def show(data, title):
    sortDict(data)
    
    for key in sorted(data.keys()):
        value = data[key]
        x, y = toArrays(value)
        plt.plot(x, y, markers[key], label=key)
        plt.title(title)
        plt.xlabel('Veličina kvadratnog prozora u pikselima')
        plt.ylabel('Pogreška u %')

    plt.legend(bbox_to_anchor=(1, 1))
    plt.show()

SSD = collections.defaultdict(lambda: [])
ZSAD = collections.defaultdict(lambda: [])
Census = collections.defaultdict(lambda: [])

with open("middlebury_2006.out", "r") as file:
    local = []
    sgm = []
    for line in file:
        name, error = line.split()
        name = name.split('.')[0] # remove .png
        matching, picName, corresp, window, max_disp = name.split('_')[:-1]

        if matching == 'local':
            if corresp == 'SSD':
                SSD[picName].append((int(window), float(error)*100))
            elif corresp == 'ZSAD':
                ZSAD[picName].append((int(window), float(error)*100))
            elif corresp == 'Census':
                Census[picName].append((int(window), float(error)*100))

    for key in SSD:
        markers[key] = '-' + next(marker)
                
    show(SSD, 'SSD')
    show(ZSAD, 'ZSAD')
    show(Census, 'Census')

