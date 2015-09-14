
all: main

main: main.cpp $(wildcard include/*.h)
	g++ -o $@ -std=c++11 $< -liniparser

.PHONY: test
test: main
	./main example-sim.ini
