#include "PlacementSystem.h"
#include "PlacementComponent.h"

#include "../ECS/SystemRegisterer.h"

namespace engine {
    namespace ECSCommon {
        ECS_REGISTER_SYSTEM(PlacementSystem);
        
        systemId_t PlacementSystem::systemId = 0;
            
        void PlacementSystem::run(EntityManager& em) {
            auto it = em.getComponentIterator({PlacementComponent::getComponentTypeId()});
        }
        
        Array<systemId_t> PlacementSystem::getOptionalDependencies() const {
            return {/* GravitySystem (?) */};
        }
        
        systemId_t PlacementSystem::getSystemTypeId() const {
            return PlacementSystem::systemId;
        }
        
        systemId_t PlacementSystem::systemTypeId() {
            return PlacementSystem::systemId;
        }
        
        void PlacementSystem::setSystemTypeId(systemId_t id) {
            PlacementSystem::systemId = id;
        }
    }
}