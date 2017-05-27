#!/bin/bash

g++ -g -O3 -std=c++11 -o stereoMatching main.cpp Correspondence.cpp SSD.cpp ZSAD.cpp Census.cpp Matching.cpp LocalMatching.cpp BirchfieldTomasi.cpp SemiGlobalMatching.cpp $(libpng-config --ldflags) -fopenmp
