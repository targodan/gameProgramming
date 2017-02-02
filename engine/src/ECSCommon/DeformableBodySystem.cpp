#include "DeformableBodySystem.h"

#include "../ECS/SystemRegisterer.h"

#include "DeformableBodyComponent.h"
#include "ForceComponent.h"
#include "TimerComponent.h"

namespace engine {
    namespace ECSCommon {
        ECS_REGISTER_SYSTEM(DeformableBodySystem);
        
        systemId_t DeformableBodySystem::systemId = 0;
            
        void DeformableBodySystem::run(EntityManager& em, float deltaTimeSeconds) {
            for(auto itForces = em.begin({ForceComponent::getComponentTypeId(), TimerComponent::getComponentTypeId()}); itForces != em.end(); ++itForces) {
                auto& force = itForces[0]->to<ForceComponent>();
                auto& timer = itForces[1]->to<TimerComponent>();

                force.getForce().setSecondsSinceStart(timer.getTime());
            }
            for(auto itBodies = em.begin({DeformableBodyComponent::getComponentTypeId()}); itBodies != em.end(); ++itBodies) {
                auto& body = itBodies->to<DeformableBodyComponent>();
                
                Eigen::VectorXf forces = Eigen::VectorXf::Zero(body.getDeformableBody().getExpectedForceVectorSize());
                for(auto itForces = em.begin({ForceComponent::getComponentTypeId()}); itForces != em.end(); ++itForces) {
                    auto& force = (*itForces)->to<ForceComponent>();
                    
                    auto f = force.getForce().getForceOnVertices(body.getDeformableBody().getProperties());
                    if(f.rows() > 0) {
                        forces += f;
                    }
                }
                body.getDeformableBody().step(deltaTimeSeconds, forces);
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