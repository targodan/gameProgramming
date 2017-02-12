#ifndef BOOMACTION_H
#define BOOMACTION_H

#include "../../../engine/src/IO/Action.h"
#include "../../../engine/src/ECS/EntityManager.h"
#include "../OneShotForce.h"
#include "../../../engine/src/physics/ParticleSystem.h"

namespace demo {
    namespace IO {
        
        using engine::IO::Action;
        using engine::ECS::EntityManager;
        using demoSimulation::OneShotForce;
        using engine::physics::ParticleSystem;
        
        class BoomAction : public Action {
        public:
            BoomAction(int device, int button, OneShotForce& force, shared_ptr<ParticleSystem> particles) : Action(device, button), force(force), pats(particles) {}
            BoomAction(const BoomAction& orig) : Action(orig), force(orig.force), pats(orig.pats) {}
            void execute(EntityManager& em, float deltaT) override {
                this->force.resetShot();
                this->pats->enable();
            }
        private:
            OneShotForce& force;
            shared_ptr<ParticleSystem> pats;
        };
    }
}

#endif /* BOOMACTION_H */

