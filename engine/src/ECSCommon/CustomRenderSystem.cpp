#include "CustomRenderSystem.h"

#include "../ECS/SystemRegisterer.h"

#include <easylogging++.h>

namespace engine {
    namespace ECSCommon {
        ECS_REGISTER_SYSTEM(CustomRenderSystem);
        
        systemId_t CustomRenderSystem::systemId = 0;
            
        void CustomRenderSystem::run(EntityManager& em, float deltaTimeSeconds) {
            this->function(em, deltaTimeSeconds);
        }
        
        systemId_t CustomRenderSystem::getSystemTypeId() const {
            return CustomRenderSystem::systemId;
        }
        
        systemId_t CustomRenderSystem::systemTypeId() {
            return CustomRenderSystem::systemId;
        }
        
        void CustomRenderSystem::setSystemTypeId(systemId_t id) {
            CustomRenderSystem::systemId = id;
        }
    }
}