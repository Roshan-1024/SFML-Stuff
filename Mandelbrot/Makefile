CXX = g++
CXXFLAGS = -std=c++20 -Wall
LIBS = $(shell pkg-config --cflags --libs sfml-graphics)

all:
	$(CXX) mandelbrot.cpp $(CXXFLAGS) $(LIBS) -o mandelbrot
