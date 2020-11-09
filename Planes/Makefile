# Makefile for Planes

# ***************************************
# Variables to control Makefile operation

CXX = g++
CXXFLAGS = -Wpedantic -g

# ***************************************
# Targets needed to bring the executable up to date

Planes: Planes.o
	$(CXX) $(CXXFLAGS) -o Planes Planes.o

Planes.o: Planes.cpp
	$(CXX) $(CXXFLAGS) -c Planes.cpp
