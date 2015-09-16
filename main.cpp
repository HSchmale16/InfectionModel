#include "include/Simulation.h"
#include <iostream>
#include <fstream>
#include <cstdio>
#include <ctime>
#include <unistd.h>
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
    printf("%d\n", (*dir_distrib)(generator));
}

int main(int argc, char**argv){
    init(argc, argv);
    Simulation sim;

    sf::RenderWindow w(sf::VideoMode::getDesktopMode(),
            "Infection Model", sf::Style::Close);
    SCREEN_WIDTH  = w.getSize().x;
    SCREEN_HEIGHT = w.getSize().y;
    SCREEN_MIN    = std::min(SCREEN_WIDTH, SCREEN_HEIGHT);

    printf("Person: %d Bytes\n", sizeof(Person));
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
            goto done;
        w.clear();
        w.draw(sim);
        w.display();
        usleep(1000000 / 60);
    }
done:
    iniparser_freedict(gConfig);
    delete dir_distrib;
}
