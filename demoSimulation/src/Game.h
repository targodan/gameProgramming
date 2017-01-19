#ifndef GAME_H
#define GAME_H

#include "engine/Game.h"
#include "engine/physics/DeformableBody.h"
#include "engine/ECS/Entity.h"

namespace demoSimulation {
    class Game : public engine::Game {
    protected:
        engine::ECS::Entity tetrahedron;
        
    public:
        Game(int argc, char** argv, double ups=60) : engine::Game(argc, argv, ups) {}
        
        virtual void initialize() override;
    };
}

#endif /* GAME_H */

