#ifndef RESETACTION_H
#define RESETACTION_H

#include "../../../engine/src/IO/Action.h"
#include "../../../engine/src/ECS/EntityManager.h"
#include "../../../engine/src/physics/Force.h"

namespace demoSimulation {
    using engine::IO::Action;
    using engine::ECS::EntityManager;
    using engine::physics::Force;

    class ResetAction : public Action {
    public:
        ResetAction(int device, int button, engine::physics::DeformableBody& defBody) : Action(device, button), defBody(defBody) {}
        ResetAction(const ResetAction& orig) : Action(orig), defBody(orig.defBody), count(orig.count) {}
        void execute(EntityManager& em, float deltaT) override {
            ++this->count;
            if(this->count >= 7) {
                this->count = 0;
                this->defBody.resetSimulation();
            }
        }
        
    private:
        engine::physics::DeformableBody& defBody;
        int count = 0;
    };
}

#endif /* RESETACTION_H */

