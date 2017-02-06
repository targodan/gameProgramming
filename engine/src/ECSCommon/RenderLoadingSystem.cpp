#include "RenderLoadingSystem.h"

#include "../ECS/SystemRegisterer.h"
#include "VisualComponent.h"

#include <easylogging++.h>

namespace engine {
    namespace ECSCommon {
        ECS_REGISTER_SYSTEM(RenderLoadingSystem);
        
        systemId_t RenderLoadingSystem::systemId = 0;
            
        void RenderLoadingSystem::run(EntityManager& em, float deltaTimeSeconds) {
            for(auto itVisual = em.begin({VisualComponent::getComponentTypeId()}); itVisual != em.end(); ++itVisual) {
                auto& object = itVisual->to<VisualComponent>().getVisualObject();
                if(!object.isLoaded()) {
                    object.loadObject();
                }
            }
        }
        
        systemId_t RenderLoadingSystem::getSystemTypeId() const {
            return RenderLoadingSystem::systemId;
        }
        
        systemId_t RenderLoadingSystem::systemTypeId() {
            return RenderLoadingSystem::systemId;
        }
        
        void RenderLoadingSystem::setSystemTypeId(systemId_t id) {
            RenderLoadingSystem::systemId = id;
        }
    }
}