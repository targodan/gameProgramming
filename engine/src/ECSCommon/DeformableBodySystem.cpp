#include "DeformableBodySystem.h"

#include "../ECS/SystemRegisterer.h"

#include "DeformableBodyComponent.h"
#include "ForceComponent.h"

namespace engine {
    namespace ECSCommon {
        ECS_REGISTER_SYSTEM(DeformableBodySystem);
        
        systemId_t DeformableBodySystem::systemId = 0;
            
        void DeformableBodySystem::run(EntityManager& em, float deltaTimeSeconds) {
            for(auto itBodies = em.begin({DeformableBodyComponent::getComponentTypeId()}); itBodies != em.end(); ++itBodies) {
                auto& body = itBodies->to<DeformableBodyComponent>();
                
                for(auto itForces = em.begin({ForceComponent::getComponentTypeId()}); itForces != em.end(); ++itForces) {
                    auto& force = itForces->to<ForceComponent>();
                    // TODO: The following line is just a workaround
                    force.getForce().addSecondsSinceStart(deltaTimeSeconds);
                    body.getDeformableBody().step(deltaTimeSeconds, force.getForce());
                }
            }
        }
        
        systemId_t DeformableBodySystem::getSystemTypeId() const {
            return DeformableBodySystem::systemId;
        }
        
        systemId_t DeformableBodySystem::systemTypeId() {
            return DeformableBodySystem::systemId;
        }
        
        void DeformableBodySystem::setSystemTypeId(systemId_t id) {
            DeformableBodySystem::systemId = id;
        }
    }
}