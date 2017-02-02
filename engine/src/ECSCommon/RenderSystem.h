#ifndef RENDERSYSTEM_H
#define RENDERSYSTEM_H

#include "../ECS/EntityManager.h"
#include "../ECS/System.h"
#include "VisualComponent.h"
#include "../ECS/MessageHandler.h"
#include "../ECS/MessageReceiver.h"

namespace engine {
    namespace ECSCommon {
        using namespace engine::ECS;
        
        class RenderSystem : public System, MessageReceiver {
        private:
            static systemId_t systemId;
            
            bool aspectRatioUpdated = false;
            float aspectRatio = 1;
            
            void render(VisualComponent& comp);
        public:
            RenderSystem(MessageHandler& handler);
            RenderSystem(const RenderSystem& orig);
            virtual ~RenderSystem();
            
            virtual void run(EntityManager& em, float deltaTimeSeconds) override;
            virtual bool isUpdateSystem() const override {
                return false;
            }
            virtual bool isRenderSystem() const override {
                return true;
            }
            
            void receive(shared_ptr<Message> msg) override;
            
            virtual Array<systemId_t> getDependencies() const override;
            
            virtual Array<systemId_t> getOptionalDependencies() const override;
            
            std::string getSystemName() const override {
                return "RenderSystem";
            }
            
            systemId_t getSystemTypeId() const override;
            
            static systemId_t systemTypeId();
            static void setSystemTypeId(systemId_t id);
        };        
    }
}



#endif /* RENDERSYSTEM_H */

