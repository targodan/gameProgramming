#include "CameraRenderSystem.h"
#include "CameraComponent.h"
#include "PlacementComponent.h"
#include "VisualComponent.h"

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
                camera.setViewMatrix(placement.getPosition());
                
                
                for(auto itVisual = em.begin({VisualComponent::getComponentTypeId()}); itVisual != em.end(); ++itVisual) { // Each and every visual component has to be rendered from the camera's perspective
                    auto& visual = (*itVisual)->to<VisualComponent>();
                    
                    auto shaderPtr = visual.getMaterial().getShader();
                    shaderPtr->useProgram();
                    shaderPtr->setUniform("projectionMatrix", camera.getProjectionMatrix());
                    shaderPtr->setUniform("viewMatrix", camera.getViewMatrix());
                }
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