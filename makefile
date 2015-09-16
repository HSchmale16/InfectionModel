CXX_FLAGS := -std=c++11 -O3 -march=native -mtune=intel
LD_FLAGS  := -liniparser -lsfml-graphics -lsfml-window -lsfml-system

all: main

main: main.cpp $(wildcard include/*.h)
	g++ $(CXX_FLAGS) -o $@ $< $(LD_FLAGS)
	wc $@

.PHONY: test
test: main
	./main example-sim.ini

.PHONY: clean
clean:
	rm -f main
