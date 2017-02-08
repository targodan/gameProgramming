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
            ToggleForceAction(int device, int button, Force& force) : Action(device, button), force(force) {}
            ToggleForceAction(const ToggleForceAction& orig) : Action(orig), force(orig.force) {}
            void execute(EntityManager& em) override {
                ++this->count;
                if(this->count >= 10) {
                    this->count = 0;
                    if(this->force.isEnabled()) {
                        this->force.disable();
                    } else {
                        this->force.enable();
                    }
                }
            }
        private:
            Force& force;
            int count = 0;
        };
    }
}

#endif /* BOOMACTION_H */

