#ifndef GAME_H
#define GAME_H

#include "engine/Game.h"

#include "Level.h"

namespace explosionBoy {
    class Game : public engine::Game {
    protected:
        std::unique_ptr<Level> currentLevel;
        
    public:
        Game(int argc, char** argv) : engine::Game(argc, argv) {}
        
        void initialize() override;
    };
}

#endif /* GAME_H */

