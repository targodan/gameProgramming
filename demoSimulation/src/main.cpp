#include <easylogging++.h>
#include <iostream>

#include "Game.h"

INITIALIZE_EASYLOGGINGPP

int main(int argc, char** argv) {
    demoSimulation::Game game(argc, argv);
    
    game.initialize();
    game.run();
    game.shutdown();
    
    return 0;
}
