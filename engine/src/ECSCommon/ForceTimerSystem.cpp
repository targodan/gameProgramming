#include "ForceTimerSystem.h"

#include "../ECS/SystemRegisterer.h"

#include "ForceComponent.h"
#include "TimerComponent.h"

namespace engine {
    namespace ECSCommon {  

        ECS_REGISTER_SYSTEM(ForceTimerSystem);
        
        systemId_t ForceTimerSystem::systemId = 0;
        
        ForceTimerSystem::ForceTimerSystem() {
        }

        ForceTimerSystem::ForceTimerSystem(const ForceTimerSystem& orig) {
        }

        ForceTimerSystem::~ForceTimerSystem() {
        }
        
        void ForceTimerSystem::run(EntityManager& em, float deltaTimeSeconds) {
            for(auto itForces = em.begin({ForceComponent::getComponentTypeId(), TimerComponent::getComponentTypeId()}); itForces != em.end(); ++itForces) {
                auto& force = itForces[0]->to<ForceComponent>();
                auto& timer = itForces[1]->to<TimerComponent>();

                force.getForce().setSecondsSinceStart(timer.getTime());
            }
        }
        
        systemId_t ForceTimerSystem::getSystemTypeId() const {
            return ForceTimerSystem::systemId;
        }
        
        systemId_t ForceTimerSystem::systemTypeId() {
            return ForceTimerSystem::systemId;
        }
        
        void ForceTimerSystem::setSystemTypeId(systemId_t id) {
            ForceTimerSystem::systemId = id;
        }
    }
}

