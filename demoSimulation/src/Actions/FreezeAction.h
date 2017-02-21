#ifndef FREEZEACTION_H
#define FREEZEACTION_H

#include "../../../engine/src/IO/Action.h"
#include "../../../engine/src/ECS/EntityManager.h"
#include "../../../engine/src/physics/Force.h"

#include "../freezer.h"

namespace demoSimulation {
    using engine::IO::Action;
    using engine::ECS::EntityManager;
    using engine::physics::Force;

    class FreezeAction : public Action {
    public:
        FreezeAction(int device, int button, engine::physics::DeformableBody& defBody, engine::physics::TetrahedronizedObject& tMesh, demoSimulation::StatusDisplay* sd) : Action(device, button), defBody(defBody), tMesh(tMesh), sd(sd) {}
        FreezeAction(const FreezeAction& orig) : Action(orig), defBody(orig.defBody), tMesh(orig.tMesh), count(orig.count) {}
        void execute(EntityManager& em, float deltaT) override {
            ++this->count;
            if(this->count >= 7) {
                this->count = 0;
                this->frozen = !this->frozen;
                if(this->frozen) {
                    switch(this->bi.buttonID) {
                        case GLFW_KEY_H:
                            freezeAllWith(defBody, tMesh, -2, NAN, NAN);
                            this->sd->setLeftFrozen(this->frozen);
                            break;
                        case GLFW_KEY_J:
                            freezeAllWith(defBody, tMesh, 2, NAN, NAN);
                            this->sd->setRightFrozen(this->frozen);
                            break;
                        case GLFW_KEY_K:
                            freezeAllWith(defBody, tMesh, NAN, 4, NAN);
                            this->sd->setTopFrozen(this->frozen);
                            break;
                        case GLFW_KEY_L:
                            freezeAllWith(defBody, tMesh, NAN, 0, NAN);
                            this->sd->setBottomFrozen(this->frozen);
                            break;
                    }
                } else {
                    switch(this->bi.buttonID) {
                        case GLFW_KEY_H:
                            unfreezeAllWith(defBody, tMesh, -2, NAN, NAN);
                            this->sd->setLeftFrozen(this->frozen);
                            break;
                        case GLFW_KEY_J:
                            unfreezeAllWith(defBody, tMesh, 2, NAN, NAN);
                            this->sd->setRightFrozen(this->frozen);
                            break;
                        case GLFW_KEY_K:
                            unfreezeAllWith(defBody, tMesh, NAN, 4, NAN);
                            this->sd->setTopFrozen(this->frozen);
                            break;
                        case GLFW_KEY_L:
                            unfreezeAllWith(defBody, tMesh, NAN, 0, NAN);
                            this->sd->setBottomFrozen(this->frozen);
                            break;
                    }
                }
            }
        }
        
    private:
        engine::physics::DeformableBody& defBody;
        engine::physics::TetrahedronizedObject tMesh;
        
        bool frozen = false;
        
        demoSimulation::StatusDisplay* sd;
        int count = 0;
    };
}

#endif /* FREEZEACTION_H */

