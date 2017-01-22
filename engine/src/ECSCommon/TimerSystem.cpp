#include "TimerSystem.h"
#include "TimerComponent.h"

#include "../ECS/SystemRegisterer.h"

#include <easylogging++.h>

namespace engine {
    namespace ECSCommon {
        ECS_REGISTER_SYSTEM(TimerSystem);
        
        systemId_t TimerSystem::systemId = 0;
            
        void TimerSystem::run(EntityManager& em, float deltaTimeSeconds) {
            for(auto it = em.begin({TimerComponent::getComponentTypeId()}); it != em.end(); ++it) {
                auto& timer = it->to<TimerComponent>();
                timer.addTime(deltaTimeSeconds);
            }
        }
        
        systemId_t TimerSystem::getSystemTypeId() const {
            return TimerSystem::systemId;
        }
        
        systemId_t TimerSystem::systemTypeId() {
            return TimerSystem::systemId;
        }
        
        void TimerSystem::setSystemTypeId(systemId_t id) {
            TimerSystem::systemId = id;
        }
    }
}