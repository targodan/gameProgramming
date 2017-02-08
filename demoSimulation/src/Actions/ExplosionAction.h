#ifndef EXPLACTION_H
#define EXPLACTION_H

#include "../../../engine/src/IO/Action.h"
#include "../../../engine/src/ECS/EntityManager.h"
#include "../../../engine/src/ECSCommon/TimerComponent.h"

namespace demo {
    namespace IO {
        
        using engine::IO::Action;
        using engine::ECS::EntityManager;
        using engine::ECSCommon::TimerComponent;
        
        class ResetTimerAction : public Action {
        public:
            ResetTimerAction(int device, int button, TimerComponent& timer) : Action(device, button), timer(timer) {}
            ResetTimerAction(const ResetTimerAction& orig) : Action(orig), timer(orig.timer) {}
            void execute(EntityManager& em) override {
                this->timer.reset();
            }
        private:
            TimerComponent& timer;
        };
    }
}

#endif /* BOOMACTION_H */

