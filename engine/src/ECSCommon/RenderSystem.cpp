#include "RenderSystem.h"
#include "VisualComponent.h"
#include "TextComponent.h"
#include "CameraComponent.h"
#include "PerformanceMetricsSystem.h"

#include "../ECS/SystemRegisterer.h"

#include "../renderer/TextRenderer.h"
#include "../WindowResizeMessage.h"

#include "glm/gtx/transform.hpp"
#include "glm/gtx/string_cast.hpp"

using engine::renderer::TextRenderer;

namespace engine {
    namespace ECSCommon {
        using glm::mat4;
        
        ECS_REGISTER_SYSTEM(RenderSystem);
        
        systemId_t RenderSystem::systemId = 0;
        
        RenderSystem::RenderSystem(MessageHandler& handler) {
            handler.registerReceiver(WindowResizeMessage::getMessageTypeId(), this);
        }

        RenderSystem::RenderSystem(const RenderSystem& orig) {
        }

        RenderSystem::~RenderSystem() {
        }
        
        void RenderSystem::run(EntityManager& em, float deltaTimeSeconds) {
            for(auto itCamera = em.begin({CameraComponent::getComponentTypeId(), PlacementComponent::getComponentTypeId()}); itCamera != em.end(); ++itCamera) {
                auto& placement = itCamera[1]->to<PlacementComponent>();
                auto& camera = itCamera[0]->to<CameraComponent>();
                
                if(this->aspectRatioUpdated) {
                    camera.setAspectRatio(this->aspectRatio);
                }
                
                camera.setViewMatrix(placement.getPosition());
                int i = 0;
                for(auto itVisual = em.begin({VisualComponent::getComponentTypeId()}); itVisual != em.end(); ++itVisual) {
                    auto& visual = (*itVisual)->to<VisualComponent>();
                    try{ 
                        auto& placement = em.getEntity(visual.getEntityId()).getComponent<PlacementComponent>();
                        mat4 modelMatrix = glm::translate(placement.getPosition()); // Ignore rotation for now
                        visual.setShaderUniform("modelMatrix", modelMatrix);
                    } catch(...) { LOG(INFO) << "Check";}
                    
                    visual.setShaderUniform("projectionMatrix", camera.getProjectionMatrix());
                    visual.setShaderUniform("viewMatrix", camera.getViewMatrix());
                    
                    this->render(visual);
                }
                
                // TODO: If a second camera is present: ... take split-screen into account?
            }
            
            this->aspectRatioUpdated = false;
            
            auto it = em.begin({TextComponent::getComponentTypeId()});
            auto end = em.end();
            
            if(it != end) {
                auto& fontRenderer = TextRenderer::getInstance();
                fontRenderer.enableBatchMode();
                for(; it != end; ++it) {
                    auto& comp = it->to<TextComponent>();
                    fontRenderer.renderRichText(comp.getText(), comp.getXPixel(), comp.getYPixel());
                }
                fontRenderer.endBatchMode();
            }
        }
        
        Array<systemId_t> RenderSystem::getDependencies() const {
            return {};
        }
            
        Array<systemId_t> RenderSystem::getOptionalDependencies() const {
            return {PerformanceMetricsSystem::systemTypeId()};
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
        
        void RenderSystem::receive(shared_ptr<Message> msg) {
            if(msg->getId() == WindowResizeMessage::getMessageTypeId()) {
                auto& message = msg->to<WindowResizeMessage>();
                
                glViewport(0, 0, message.getNewWidth(), message.getNewHeight());
                this->aspectRatio = static_cast<float>(message.getNewWidth()) / message.getNewHeight();
                this->aspectRatioUpdated = true;
            }
        }
        
        void RenderSystem::render(VisualComponent& comp) {
            comp.getVisualObject().render();
        }
    }
}
