#include "include/Simulation.h"
#include <iostream>
#include <fstream>
#include <cstdio>
#include <ctime>
#include <algorithm>

void init(int argc, char** argv){
    gConfig = iniparser_load(argv[1]);
    if(!gConfig){
        fprintf(stderr, "Failed to load simulation config\n");
        throw 1;
    }
    for(int i = 0; i < NUM_DIRS; i++){
        INI_KEY_ID_GEN(i, "d", "dir");
        dir_probs[i] = iniparser_getint(gConfig, P_BUFF, 0);
    }
    dir_distrib = new std::discrete_distribution<int>(dir_probs.begin(), dir_probs.end());
}

void doVisual(){
    Simulation sim;

    // Screen should never be bigger than desktop, and it must be square.
    SCREEN_WIDTH  = sf::VideoMode::getDesktopMode().height - 40;
    SCREEN_HEIGHT = sf::VideoMode::getDesktopMode().height - 40;
    SCREEN_MIN    = std::min(SCREEN_WIDTH, SCREEN_HEIGHT);

    sf::RenderWindow w(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT),
            "Infection Model", sf::Style::Close);
    w.setFramerateLimit(60);

    while(w.isOpen()){
        // handle window events so that the operating system doesn't decide
        // that this program is unresponsive
        sf::Event e;
        while(w.pollEvent(e)){
            if(e.type == sf::Event::Closed){
                w.close();
            }
        }
        sim.tick();

        // render    
        w.clear();
        w.draw(sim);
        w.display();
    }
}

void doConsole(){
    Simulation sim;
    int ticks = iniparser_getint(gConfig, "world:ticks", 100);
    for(int i = 0; i < ticks; i++){
        sim.tick();
    }
}

int main(int argc, char**argv){
    init(argc, argv);
    
    if(iniparser_getboolean(gConfig, ":visual", false) == true){
        // doing it visually
        printf("Visual\n");
        doVisual();
    }else{
        // set logging to true
        iniparser_set(gConfig, ":log", "true");
        // run the simulation
        doConsole();
    }

    iniparser_freedict(gConfig);
    delete dir_distrib;
}
