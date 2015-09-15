#include "include/Simulation.h"
#include <iostream>
#include <fstream>
#include <cstdio>
#include <ctime>
#include <unistd.h>
#include <algorithm>
    
int main(int argc, char**argv){
    gConfig = iniparser_load(argv[1]);
    if(!gConfig){
        fprintf(stderr, "Failed to load simulation config\n");
        return 0;
    }
    Simulation sim;

    sf::RenderWindow w(sf::VideoMode::getDesktopMode(),
            "Infection Model", sf::Style::Close);
    SCREEN_WIDTH  = w.getSize().x;
    SCREEN_HEIGHT = w.getSize().y;
    SCREEN_MIN    = std::min(SCREEN_WIDTH, SCREEN_HEIGHT);

    int ticks = iniparser_getint(gConfig, "world:ticks", 100);
    for(int i = 0; (i < ticks && w.isOpen()); i++){
        // handle window events so that the operating system doesn't decide
        // that this program is unresponsive
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
