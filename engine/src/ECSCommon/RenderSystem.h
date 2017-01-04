#ifndef RENDERSYSTEM_H
#define RENDERSYSTEM_H

#include "../ECS/EntityManager.h"
#include "../ECS/System.h"
#include "VisualComponent.h"

namespace engine {
    namespace ECSCommon {
        using namespace engine::ECS;
        class RenderSystem : public System {
        private:
            static systemId_t systemId;
            
            void render(VisualComponent& comp);
        public:
            RenderSystem();
            RenderSystem(const RenderSystem& orig);
            virtual ~RenderSystem();
            
            virtual void run(EntityManager& em, float deltaTimeSeconds) override;
            virtual bool isUpdateSystem() const override {
                return true;
            }
            virtual bool isRenderSystem() const override {
                return true;
            }
            
            std::string getSystemName() const override {
                return "RenderSystem";
            }
            
            static systemId_t systemTypeId();
            static void setSystemTypeId(systemId_t id);
        };        
    }
}



#endif /* RENDERSYSTEM_H */

