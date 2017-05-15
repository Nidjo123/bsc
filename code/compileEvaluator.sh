#!/bin/bash

g++ -std=c++11 -O3 -o evaluator Evaluator.cpp $(libpng-config --ldflags)
