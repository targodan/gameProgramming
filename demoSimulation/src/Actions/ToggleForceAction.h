#ifndef TOGGLEFORCEACTION_H
#define TOGGLEFORCEACTION_H

#include "../../../engine/src/IO/Action.h"
#include "../../../engine/src/ECS/EntityManager.h"
#include "../../../engine/src/physics/Force.h"

namespace demo {
    namespace IO {
        
        using engine::IO::Action;
        using engine::ECS::EntityManager;
        using engine::physics::Force;
        
        class ToggleForceAction : public Action {
        public:
            ToggleForceAction(int device, int button, Force& force, demoSimulation::StatusDisplay* sd) : Action(device, button), force(force), sd(sd) {}
            ToggleForceAction(const ToggleForceAction& orig) : Action(orig), force(orig.force) {}
            void execute(EntityManager& em, float deltaT) override {
                ++this->count;
                if(this->count >= 7) {
                    this->count = 0;
                    if(this->force.isEnabled()) {
                        this->force.disable();
                        sd->setGravity(false);
                    } else {
                        this->force.enable();
                        sd->setGravity(true);
                    }
                }
            }
        private:
            Force& force;
            demoSimulation::StatusDisplay* sd;
            int count = 0;
        };
    }
}

#endif /* TOGGLEFORCEACTION_H */

