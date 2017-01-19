#ifndef GAME_H
#define GAME_H

#include "engine/Game.h"
#include "engine/physics/DeformableBody.h"

namespace demoSimulation {
    class Game : public engine::Game {
    public:
        virtual void initialize() override;
    };
}

#endif /* GAME_H */

