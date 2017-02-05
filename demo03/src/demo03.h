#ifndef DEMO02_H
#define DEMO02_H

#include "Game.h"
#include "../../engine/src/renderer/ShaderProgram.h"
#include "../../engine/src/ECS/Entity.h"

namespace demo {
    using namespace engine::renderer;
    using engine::ECS::Entity;
    
    class Demo03 : public engine::Game {
    public:
        Demo03(int argc, char** argv, double ups=60);
        Demo03(const Demo03& orig) = delete;
        virtual ~Demo03();        
        virtual void initialize() override;
        virtual void shutdown() override;
    private:
        Entity PatSys;
        Entity floor;
        Entity player;
        
        float lastX;
        float lastY;
        float firstMouseMovement;
    };
}

#endif /* DEMO02_H */

