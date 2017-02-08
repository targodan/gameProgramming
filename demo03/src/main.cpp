#include <easylogging++.h>

#include "demo03.h"

INITIALIZE_EASYLOGGINGPP

using namespace demo;
int main(int argc, char** argv) {
    LOG(INFO) << "Staring Main";
    Demo03 game(argc, argv);
    
    game.initialize();
    game.run();
    game.shutdown();
    
    return 0;
}
