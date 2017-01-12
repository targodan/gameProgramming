#include "CameraRenderSystem.h"
#include "CameraComponent.h"
#include "PlacementComponent.h"

#include "../ECS/SystemRegisterer.h"

namespace engine {
    namespace ECSCommon {
        ECS_REGISTER_SYSTEM(CameraRenderSystem);
        
        systemId_t CameraRenderSystem::systemId = 0;
        
        CameraRenderSystem::CameraRenderSystem() {
        }

        CameraRenderSystem::CameraRenderSystem(const CameraRenderSystem& orig) {
        }

        CameraRenderSystem::~CameraRenderSystem() {
        }   
        
        void CameraRenderSystem::run(EntityManager& em, float deltaTimeSeconds) {
            for(auto it = em.begin({CameraComponent::getComponentTypeId(), PlacementComponent::getComponentTypeId()}); it != em.end(); ++it) {
                auto& placement = it[1]->to<PlacementComponent>();
                auto& camera = it[0]->to<CameraComponent>();
                // do stuff
            }
        }
        
        systemId_t CameraRenderSystem::systemTypeId() {
            return CameraRenderSystem::systemId;
        }
        
        systemId_t CameraRenderSystem::getSystemTypeId() const {
            return CameraRenderSystem::systemId;
        }
        
        void CameraRenderSystem::setSystemTypeId(systemId_t id) {
            CameraRenderSystem::systemId = id;
        }
    }
}