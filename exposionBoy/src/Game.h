#ifndef GAME_H
#define GAME_H

#include "engine/Game.h"

namespace explosionBoy {
    class Game : public engine::Game {
    public:
        Game(int argc, char** argv) : engine::Game(argc, argv) {}
    };
}

#endif /* GAME_H */
