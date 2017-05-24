
PNGLIB=$(shell libpng-config --ldflags)
CPPFLAGS=-g -std=c++11 -O3

scaleValues: scaleValues.o
	g++ $(CPPFLAGS) -o scaledValues scaleValues.o $(PNGLIB)

scaleValues.o: ScaleValues.cpp
	g++ $(CPPFLAGS) -c ScaleValues.cpp $(PNGLIB)

stereoMatching: correspondence.o ssd.o birchfieldtomasi.o zsad.o census.o
	g++ $(CPPFLAGS) -o stereoMatching correspondence.o ssd.o birchfieldtomasi.o zsad.o census.o $(PNGLIB)

correspondence.o: Correspondence.cpp
	g++ $(CPPFLAGS) -c correspondence.cpp

birchfieldtomasi.o: Correspondence.cpp BirchfieldTomasi.cpp
	g++ $(CPPFLAGS) -c birchfieldtomasi.cpp

ssd.o: Correspondence.cpp SSD.cpp
	g++ $CPPFLAGS) -c ssd.cpp

zsad.o: Correspondence.cpp ZSAD.cpp
	g++ $CPPFLAGS) -c zsad.cpp

census.o: Correspondence.cpp Census.cpp
	g++ $CPPFLAGS) -c census.cpp
