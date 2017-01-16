#include "RenderSystem.h"
#include "VisualComponent.h"

#include "../ECS/SystemRegisterer.h"

#include "../renderer/FontRenderer.h"
#include "renderer/Font.h"

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
            engine::renderer::Font font("/usr/share/fonts/TTF/DejaVuSans.ttf");
            font.setSizeInPixels(50);
            engine::renderer::FontRenderer::getInstance().renderText(u8"Dies ist ein Test.", font, engine::renderer::Color::GREEN, 800, 600);
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