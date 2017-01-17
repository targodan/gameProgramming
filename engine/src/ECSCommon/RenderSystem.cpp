#include "RenderSystem.h"
#include "VisualComponent.h"
#include "TextComponent.h"

#include "../ECS/SystemRegisterer.h"

#include "../renderer/TextRenderer.h"

using engine::renderer::TextRenderer;

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
            
            auto& fontRenderer = TextRenderer::getInstance();
            fontRenderer.enableBatchMode();
            for(auto it = em.begin({TextComponent::getComponentTypeId()}); it != em.end(); ++it) {
                auto& comp = it->to<TextComponent>();
                fontRenderer.renderRichText(comp.getText(), comp.getXPixel(), comp.getYPixel());
            }
            fontRenderer.endBatchMode();
        }
        
        systemId_t RenderSystem::systemTypeId() {
            return RenderSystem::systemId;
        }
        
        systemId_t RenderSystem::getSystemTypeId() const {
            return RenderSystem::systemId;
        }
        
        void RenderSystem::setSystemTypeId(systemId_t id) {
            RenderSystem::systemId = id;
        }
        
        void RenderSystem::render(VisualComponent& comp) {
            comp.getMesh().render();
        }
    }
}