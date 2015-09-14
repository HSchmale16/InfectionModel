#include "include/Simulation.h"
#include <iostream>
#include <fstream>
#include <cstdio>
#include <unistd.h>

int main(int argc, char**argv){
    gConfig = iniparser_load(argv[1]);
    if(!gConfig){
        fprintf(stderr, "Failed to load simulation config\n");
        return 0;
    }
    Simulation sim;

    sf::RenderWindow w(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT),
            "Infection Model", sf::Style::Fullscreen);

    printf("Person Size: %d\n", sizeof(Person));
    int ticks = iniparser_getint(gConfig, "world:ticks", 100);
    for(int i = 0; (i < ticks && w.isOpen()); i++){
        sf::Event e;
        while(w.pollEvent(e)){
            if(e.type == sf::Event::Closed){
                w.close();
            }
        }
        if(sim.tick() != 0)
            break;
        w.clear();
        w.draw(sim);
        w.display();
        usleep(1000000 / 60);
    }
    iniparser_freedict(gConfig);
}
