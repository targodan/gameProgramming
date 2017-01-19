#ifndef DEMO02_H
#define DEMO02_H

#include "Game.h"
#include "../../engine/src/renderer/ShaderProgram.h"
#include "../../engine/src/ECS/Entity.h"

namespace demo {
    using namespace engine::renderer;
    using engine::ECS::Entity;
    
    class Demo02 : public engine::Game {
    public:
        Demo02(int argc, char** argv, double ups=60);
        Demo02(const Demo02& orig) = delete;
        virtual ~Demo02();
        
        virtual void initialize() override;
        virtual void shutdown() override;

        virtual void processEvents() override;
    private:
        Entity triangle;
        Entity player;
        
        float lastX;
        float lastY;
        float firstMouseMovement;
    };
}

#endif /* DEMO02_H */

