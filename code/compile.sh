#!/bin/bash

g++ -g -std=c++11 -o stereoMatching *.cpp $(libpng-config --ldflags) -fopenmp
