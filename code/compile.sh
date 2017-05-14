#!/bin/bash

g++ -g -O3 -std=c++11 -o stereoMatching *.cpp $(libpng-config --ldflags) -fopenmp
