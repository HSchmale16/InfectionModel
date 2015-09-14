
all: main

main: main.cpp $(wildcard include/*.h)
	g++ -o $@ -std=c++11 $< -liniparser -lsfml-graphics -lsfml-window -lsfml-system

.PHONY: test
test: main
	./main example-sim.ini
