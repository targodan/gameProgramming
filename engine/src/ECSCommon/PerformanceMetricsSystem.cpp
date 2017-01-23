#include "PerformanceMetricsSystem.h"

#include "../ECS/SystemRegisterer.h"
#include "ECS/SystemType.h"
#include "ECS/Entity.h"

#include "../renderer/FontRegistry.h"

#include <iomanip>

namespace engine {
    namespace ECSCommon {
        ECS_REGISTER_SYSTEM(PerformanceMetricsSystem);
        
        systemId_t PerformanceMetricsSystem::systemId = 0;
            
        void PerformanceMetricsSystem::run(EntityManager& em, float deltaTimeSeconds) {
            if(this->getCurrentRunType() == SystemType::UPDATE) {
                ++this->numUpdates;
                this->timeInUpdates += deltaTimeSeconds;
                
                this->resetUpdateTimers();
            } else {
                ++this->numRenders;
                this->timeInRenders += deltaTimeSeconds;
                
                this->resetRenderTimers();
                
                this->text.getComponent<TextComponent>().setText(this->renderText());
            }
        }
        
        void PerformanceMetricsSystem::resetUpdateTimers() {
            if(this->numUpdates > 120) {
                float timePerUpdate = this->timeInUpdates / this->numUpdates;
                this->numUpdates = 60;
                this->timeInUpdates = timePerUpdate * this->numUpdates;
            }
        }
        void PerformanceMetricsSystem::resetRenderTimers() {
            if(this->numRenders > 120) {
                float timePerRender = this->timeInRenders / this->numRenders;
                this->numRenders = 60;
                this->timeInRenders = timePerRender * this->numRenders;
            }
        }
        
        RichText PerformanceMetricsSystem::renderText() const {
            RichText text(FontRegistry::getDefaultFontFamily(), FontType::Regular, 16, Color::BLUE);
            
            float ups = this->numUpdates / this->timeInUpdates;
            float fps = this->numRenders / this->timeInRenders;
            
            text << "UPS: " << std::fixed << std::setprecision(2) << ups << " | FPS: " << fps;
            
            return text;
        }
        
        systemId_t PerformanceMetricsSystem::getSystemTypeId() const {
            return PerformanceMetricsSystem::systemId;
        }
        
        systemId_t PerformanceMetricsSystem::systemTypeId() {
            return PerformanceMetricsSystem::systemId;
        }
        
        void PerformanceMetricsSystem::setSystemTypeId(systemId_t id) {
            PerformanceMetricsSystem::systemId = id;
        }
    }
}
