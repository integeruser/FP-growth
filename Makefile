default:
	mkdir -p bin
	g++ -std=c++11 -O2 -Wall -o bin/main -I include src/*

clean:
	rm -rf bin
