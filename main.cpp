#include "include/Simulation.h"
#include <iostream>
#include <fstream>
#include <cstdio>

int main(int argc, char**argv){
    gConfig = iniparser_load(argv[1]);
    if(!gConfig){
        fprintf(stderr, "Failed to load simulation config\n");
        return 0;
    }
    Simulation sim;
    
    printf("Person Size: %d\n", sizeof(Person));
    int ticks = iniparser_getint(gConfig, "world:ticks", 100);
    for(int i = 0; i < ticks; i++){
        if(sim.tick() != 0)
            break;
    }

    iniparser_freedict(gConfig);
}
