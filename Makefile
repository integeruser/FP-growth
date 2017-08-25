CXX=clang++
CXXFLAGS=-std=c++11 -O2 -Wall

main:
	$(CXX) $(CXXFLAGS) -o main -I include src/fptree.cpp src/main.cpp

clean:
	rm -rf main
