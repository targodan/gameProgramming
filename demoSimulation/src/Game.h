#ifndef GAME_H
#define GAME_H

#include "engine/Game.h"
#include "engine/physics/DeformableBody.h"
#include "engine/ECS/Entity.h"

#include "engine/renderer/Font.h"

namespace demoSimulation {
    class Game : public engine::Game {
    protected:
        engine::ECS::Entity player;
        engine::ECS::Entity tetrahedron;
        
    public:
        Game(int argc, char** argv) : engine::Game(argc, argv, 120) {}
        
        virtual void initialize() override;
        void render(float deltaTimeSeconds) override;
    };
}

#endif /* GAME_H */

