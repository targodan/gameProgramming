#include "PerformanceMetricsSystem.h"

#include "../ECS/SystemRegisterer.h"
#include "ECS/SystemType.h"
#include "ECS/Entity.h"

#include "../renderer/FontRegistry.h"

#include <iomanip>
#include <GLFW/glfw3.h>

namespace engine {
    namespace ECSCommon {
        ECS_REGISTER_SYSTEM(PerformanceMetricsSystem);
        
        systemId_t PerformanceMetricsSystem::systemId = 0;
            
        void PerformanceMetricsSystem::run(EntityManager& em, float deltaTimeSeconds) {
            // Don't use deltaTimeSeconds as that does not always represent the real time.
            float newTime = glfwGetTime();
            if(this->getCurrentRunType() == SystemType::UPDATE) {
                float dT = newTime - this->lastUpdateTime;
                this->lastUpdateTime = newTime;
                
                ++this->numUpdates;
                this->timeInUpdates += dT;
                
                this->resetUpdateTimers();
            } else {
                float dT = newTime - this->lastRenderTime;
                this->lastRenderTime = newTime;
                
                ++this->numRenders;
                this->timeInRenders += dT;
                
                this->resetRenderTimers();
                
                this->text.getComponent<TextComponent>().setText(this->renderText());
            }
        }
        
        void PerformanceMetricsSystem::resetUpdateTimers() {
            if(this->numUpdates > 60) {
                float timePerUpdate = this->timeInUpdates / this->numUpdates;
                this->numUpdates = 10;
                this->timeInUpdates = timePerUpdate * this->numUpdates;
            }
        }
        void PerformanceMetricsSystem::resetRenderTimers() {
            if(this->numRenders > 60) {
                float timePerRender = this->timeInRenders / this->numRenders;
                this->numRenders = 10;
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
