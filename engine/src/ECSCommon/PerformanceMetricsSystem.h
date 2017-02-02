#ifndef PERFORMACEMETRICSSYSTEM_H
#define PERFORMACEMETRICSSYSTEM_H

#include "../ECS/System.h"
#include "../ECS/EntityManager.h"
#include "../ECS/Entity.h"

#include "TextComponent.h"

namespace engine {
    namespace ECSCommon {
        using namespace engine::ECS;
        
        class PerformanceMetricsSystem : public System {
        private:
            static systemId_t systemId;
            
        protected:
            Entity text;
            
            float lastUpdateTime = 0;
            float lastRenderTime = 0;
            
            int numUpdates = 0;
            float timeInUpdates = 0;
            
            int numRenders = 0;
            float timeInRenders = 0;
            
            void resetUpdateTimers();
            void resetRenderTimers();
            
            RichText renderText() const;
            
        public:
            PerformanceMetricsSystem(EntityManager& em) : text(em.createEntity("PerformanceMetrics")) {
                this->text.addComponent<TextComponent>();
                this->text.getComponent<TextComponent>().setXPixel(10);
                this->text.getComponent<TextComponent>().setYPixel(20);
            }
            PerformanceMetricsSystem(const PerformanceMetricsSystem& orig) = delete;
            virtual ~PerformanceMetricsSystem() {}
            
            void run(EntityManager& em, float deltaTimeSeconds) override;

            bool isUpdateSystem() const override {
                return true;
            }
            bool isRenderSystem() const override {
                return true;
            }
            
            systemId_t getSystemTypeId() const override;
            
            std::string getSystemName() const override {
                return "PerformanceMetricsSystem";
            }
            
            static systemId_t systemTypeId();
            static void setSystemTypeId(systemId_t id);
        };
    }
}

#endif /* PERFORMACEMETRICSSYSTEM_H */

