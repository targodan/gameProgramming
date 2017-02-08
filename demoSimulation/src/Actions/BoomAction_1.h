#ifndef BOOMACTION_H
#define BOOMACTION_H

#include "../../../engine/src/IO/Action.h"
#include "../../../engine/src/ECS/EntityManager.h"
#include "../OneShotForce.h"

namespace demo {
    namespace IO {
        
        using engine::IO::Action;
        using engine::ECS::EntityManager;
        using demoSimulation::OneShotForce;
        
        class BoomAction : public Action {
        public:
            BoomAction(int device, int button, OneShotForce& force) : Action(device, button), force(force) {}
            BoomAction(const BoomAction& orig) : Action(orig), force(orig.force) {}
            void execute(EntityManager& em) override {
                this->force.resetShot();
            }
        private:
            OneShotForce& force;
        };
    }
}

#endif /* BOOMACTION_H */

