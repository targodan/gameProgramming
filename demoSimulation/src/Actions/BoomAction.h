#ifndef BOOMACTION_H
#define BOOMACTION_H

#include "../../../engine/src/IO/Action.h"
#include "../../../engine/src/ECS/EntityManager.h"
#include "../../../engine/src/util/vec3.h"
#include "../../../engine/src/ECSCommon/PlacementComponent.h"
#include "../OneShotForce.h"
#include "../../../engine/src/physics/ParticleSystem.h"

namespace demo {
    namespace IO {

        using engine::IO::Action;
        using engine::ECS::EntityManager;
        using engine::ECSCommon::PlacementComponent;
        using demoSimulation::OneShotForce;
        using engine::physics::ParticleSystem;

        class BoomAction : public Action {
        public:
            BoomAction(int device, int button, OneShotForce& force, shared_ptr<ParticleSystem> particles, PlacementComponent& bombPlace) : Action(device, button), force(force), pats(particles), bombPlace(bombPlace) {}
            BoomAction(const BoomAction& orig) : Action(orig), force(orig.force), pats(orig.pats), bombPlace(orig.bombPlace) {}
            void execute(EntityManager& em, float deltaT) override {
                this->force.resetShot();
                // this->pats->enable();
                // this->bombPlace.setPosition(engine::util::vec3(0, -5, 0));
            }
        private:
            OneShotForce& force;
            shared_ptr<ParticleSystem> pats;
            PlacementComponent& bombPlace;
        };
    }
}

#endif /* BOOMACTION_H */
