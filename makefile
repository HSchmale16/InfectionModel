CXX_FLAGS := -std=c++11 -O3 -march=native -mtune=intel
LD_FLAGS  := -liniparser -lsfml-graphics -lsfml-window -lsfml-system
EXE       := infection-model


all: $(EXE)

$(EXE): main.cpp $(wildcard include/*.h)
	g++ $(CXX_FLAGS) -o $@ $< $(LD_FLAGS)
	wc $@

.PHONY: test
test: $(EXE)
	./$(EXE) example-sim.ini

.PHONY: clean
clean:
	rm -f $(EXE)
