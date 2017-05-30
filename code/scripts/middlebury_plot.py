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

    plt.gca().set_ylim([0, 100])
    
    for key in sorted(data.keys()):
        value = data[key]
        x, y = toArrays(value)
        plt.plot(x, y, markers[key], label=key)
        plt.title(title)
        plt.xlabel('Veličina kvadratnog prozora u pikselima')
        plt.ylabel('Pogreška u %')

    plt.legend(bbox_to_anchor=(1, 1))
    plt.show()

def showAvg(*args, title=""):
    plt.title(title)
    plt.gca().set_ylim([0, 100])
    plt.xlabel("Veličina kvadratnog prozora u pikselima")
    plt.ylabel("Pogreška u %")
    for d, label in args:
        sortDict(d)
        nKeys = len(d.keys())
        
        x = []
        y = []
        
        for key in d:
            for i, v in enumerate(d[key]):
                if len(y) <= i:
                    y.append(v[1])
                    x.append(v[0])
                
                y[i] += v[1]
                x[i] = v[0]
        for i, v in enumerate(y):
            y[i] = v / nKeys

        plt.plot(x, y, label=label)

    plt.legend()
    plt.show()

localSSD = collections.defaultdict(lambda: [])
localZSAD = collections.defaultdict(lambda: [])
localCensus = collections.defaultdict(lambda: [])
sgmSSD = collections.defaultdict(lambda: [])
sgmZSAD = collections.defaultdict(lambda: [])
sgmCensus = collections.defaultdict(lambda: [])
sgmBT = collections.defaultdict(lambda: [])

with open("middlebury_2006.out", "r") as file:
    local = []
    sgm = []
    for line in file:
        name, error = line.split()
        name = name.split('.')[0] # remove .png
        matching, picName, corresp, window, max_disp = name.split('_')[:-1]

        if matching == 'local':
            if corresp == 'SSD':
                localSSD[picName].append((int(window), float(error)*100))
            elif corresp == 'ZSAD':
                localZSAD[picName].append((int(window), float(error)*100))
            elif corresp == 'Census':
                localCensus[picName].append((int(window), float(error)*100))
        elif matching == 'sgm':
            if corresp == 'SSD':
                sgmSSD[picName].append((int(window), float(error)*100))
            elif corresp == 'ZSAD':
                sgmZSAD[picName].append((int(window), float(error)*100))
            elif corresp == 'Census':
                sgmCensus[picName].append((int(window), float(error)*100))
            elif corresp == 'BT':
                sgmBT[picName].append(float(error)*100)
                
    for key in localSSD:
        markers[key] = '-' + next(marker)
    
    show(localSSD, 'SSD lokalno')
    show(localZSAD, 'ZSAD lokalno')
    show(localCensus, 'Census lokalno')

    show(sgmSSD, 'SSD + SGM')
    show(sgmZSAD, 'ZSAD + SGM')
    show(sgmCensus, 'Census + SGM')
    
    showAvg((localSSD, 'SSD'), (localZSAD, 'ZSAD'), (localCensus, 'Census'), title='Prosječna pogreška lokalnih metoda')

    showAvg((sgmSSD, 'SSD'), (sgmZSAD, 'ZSAD'), (sgmCensus, 'Census'), title='Prosječna pogreška lokalno + SGM')
    
    xLabels = list(sgmBT.keys())
    y = []
    for key in xLabels:
        y.append(sgmBT[key][0])
    
    plt.xticks(range(len(xLabels)), xLabels, rotation=90)
    plt.stem(range(len(xLabels)), y)
    plt.show()
        
