#!/bin/bash

g++ -g -O3 -std=c++11 -o sgm sgm_bt.cpp Correspondence.cpp Matching.cpp BirchfieldTomasi.cpp SemiGlobalMatching.cpp $(libpng-config --ldflags) -fopenmp
