CXX=clang++
CXXFLAGS=-std=c++11 -O2 -Wall

default:
	mkdir -p bin
	$(CXX) $(CXXFLAGS) -o bin/main -I include src/fptree.cpp src/main.cpp

clean:
	rm -rf bin
