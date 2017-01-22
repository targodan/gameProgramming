#include "CustomUpdateSystem.h"

#include "../ECS/SystemRegisterer.h"

#include <easylogging++.h>

namespace engine {
    namespace ECSCommon {
        ECS_REGISTER_SYSTEM(CustomUpdateSystem);
        
        systemId_t CustomUpdateSystem::systemId = 0;
            
        void CustomUpdateSystem::run(EntityManager& em, float deltaTimeSeconds) {
            this->function(em, deltaTimeSeconds);
        }
        
        systemId_t CustomUpdateSystem::getSystemTypeId() const {
            return CustomUpdateSystem::systemId;
        }
        
        systemId_t CustomUpdateSystem::systemTypeId() {
            return CustomUpdateSystem::systemId;
        }
        
        void CustomUpdateSystem::setSystemTypeId(systemId_t id) {
            CustomUpdateSystem::systemId = id;
        }
    }
}