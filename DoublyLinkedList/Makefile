# Makefile for DoubleLinkedList

# ***************************************
# Variables to control Makefile operation

CXX = g++
CXXFLAGS = -Wpedantic -g

# ***************************************
# Targets needed to bring the executable up to date

DoublyLinkedList: main.o DoublyLinkedList.o
	$(CXX) $(CXXFLAGS) -o DoublyLinkedList main.o DoublyLinkedList.o

main.o: main.cpp DoublyLinkedList.h
	$(CXX) $(CXXFLAGS) -c main.cpp

DoublyLinkedList.o: DoublyLinkedList.h