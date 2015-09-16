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
    printf("%d\n", (*dir_distrib)(generator));
}

int main(int argc, char**argv){
    init(argc, argv);
    Simulation sim;

    // Screen should never be bigger than desktop, and it must be square.
    SCREEN_WIDTH  = sf::VideoMode::getDesktopMode().height - 40;
    SCREEN_HEIGHT = sf::VideoMode::getDesktopMode().height - 40;
    SCREEN_MIN    = std::min(SCREEN_WIDTH, SCREEN_HEIGHT);

    sf::RenderWindow w(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT),
            "Infection Model", sf::Style::Close);
    w.setFramerateLimit(60);

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
        if(sim.tick() != 0){
            printf("All done\n");
            goto done;
        }
        w.clear();
        w.draw(sim);
        w.display();
    }
done:
    iniparser_freedict(gConfig);
    delete dir_distrib;
}
