#ifndef GAME_H
#define GAME_H

#include "engine/Game.h"
#include "engine/physics/DeformableBody.h"
#include "engine/ECS/Entity.h"

#include "engine/renderer/Font.h"

namespace demoSimulation {
    class Game : public engine::Game {
    protected:
        engine::ECS::Entity camera;
        engine::ECS::Entity tetrahedron;
        
        engine::renderer::Font font;
        
    public:
        Game(int argc, char** argv, double ups=60) : engine::Game(argc, argv, ups), font("/usr/share/fonts/TTF/DejaVuSans.ttf") {}
        
        virtual void initialize() override;
    };
}

#endif /* GAME_H */

