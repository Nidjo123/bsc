#!/bin/bash

g++ -g -O3 -std=c++11 -o scaleImage ScaleValues.cpp $(libpng-config --ldflags)
