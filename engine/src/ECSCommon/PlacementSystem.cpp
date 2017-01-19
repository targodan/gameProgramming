#include "PlacementSystem.h"
#include "PlacementComponent.h"

#include "../ECS/SystemRegisterer.h"
#include "InputSystem.h"

namespace engine {
    namespace ECSCommon {
        using engine::ECSCommon::InputSystem;
        ECS_REGISTER_SYSTEM(PlacementSystem);
        
        systemId_t PlacementSystem::systemId = 0;
            
        void PlacementSystem::run(EntityManager& em, float deltaTimeSeconds) {
            for(auto it = em.begin({PlacementComponent::getComponentTypeId()}); it != em.end(); ++it) {
                auto& comp = (*it)->to<PlacementComponent>();
                if(!((comp.getVelocity().x == 0) && (comp.getVelocity().y == 0) && (comp.getVelocity().z ==  0))) {
                    comp.getPosition() += 1.0f * glm::normalize(comp.getVelocity()) * deltaTimeSeconds;
                    comp.setLastVelocity(comp.getVelocity());
                    comp.setVelocity(vec3(0,0,0));
                }
            }
            
        }
        
        Array<systemId_t> PlacementSystem::getOptionalDependencies() const {
            return {InputSystem::systemTypeId()};
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