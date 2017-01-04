#include "RenderSystem.h"
#include "VisualComponent.h"

#include "../ECS/SystemRegisterer.h"

namespace engine {
    namespace ECSCommon {
        ECS_REGISTER_SYSTEM(RenderSystem);
        
        systemId_t RenderSystem::systemId = 0;
        
        RenderSystem::RenderSystem() {
        }

        RenderSystem::RenderSystem(const RenderSystem& orig) {
        }

        RenderSystem::~RenderSystem() {
        }
        
        void RenderSystem::run(EntityManager& em, float deltaTimeSeconds) {
            for(auto it = em.begin({VisualComponent::getComponentTypeId()}); it != em.end(); ++it) {
                auto ptr = *it;
                auto comp = dynamic_cast<VisualComponent*>(ptr.get());
                this->render(*comp);
            }
        }
        
        systemId_t RenderSystem::systemTypeId() {
            return RenderSystem::systemId;
        }
        
        void RenderSystem::setSystemTypeId(systemId_t id) {
            RenderSystem::systemId = id;
        }
        
        void RenderSystem::render(VisualComponent& comp) {
            
        }
    }
}