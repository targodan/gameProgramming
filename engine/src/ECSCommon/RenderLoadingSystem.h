#ifndef RENDERLOADINGSYSTEM_H
#define RENDERLOADINGSYSTEM_H

#include "../ECS/System.h"

namespace engine {
    namespace ECSCommon {
        using namespace engine::ECS;
        
        class RenderLoadingSystem : public System {
        private:
            static systemId_t systemId;
            
        public:
            RenderLoadingSystem() {}
            RenderLoadingSystem(const RenderLoadingSystem& orig) {}
            virtual ~RenderLoadingSystem() {}
            
            virtual void run(EntityManager& em, float deltaTimeSeconds) override;
            virtual bool isUpdateSystem() const override {
                return false;
            }
            virtual bool isRenderSystem() const override {
                return true;
            }
            
            std::string getSystemName() const override {
                return "RenderLoadingSystem";
            }
            
            systemId_t getSystemTypeId() const override;
            
            static systemId_t systemTypeId();
            static void setSystemTypeId(systemId_t id);
        };        
    }
}

#endif /* RENDERLOADINGSYSTEM_H */

