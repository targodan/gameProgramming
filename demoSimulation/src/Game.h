#ifndef GAME_H
#define GAME_H

#include "engine/Game.h"
#include "engine/physics/DeformableBody.h"
#include "engine/ECS/Entity.h"

#include "engine/renderer/Font.h"
#include "StatusDisplay.h"

namespace demoSimulation {
    class Game : public engine::Game {
    protected:
        engine::ECS::Entity player;
        engine::ECS::Entity tetrahedron;
        StatusDisplay* statusDisplay;
        
    public:
        Game(int argc, char** argv) : engine::Game(argc, argv, 60) {}
        
        virtual void initialize() override;
        void render(float deltaTimeSeconds) override;
    };
}

#endif /* GAME_H */

