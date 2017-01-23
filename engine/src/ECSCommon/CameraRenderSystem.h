#ifndef CAMERARENDERSYSTEM_H
#define CAMERARENDERSYSTEM_H

#include "../ECS/EntityManager.h"
#include "../ECS/System.h"
#include "PlacementSystem.h"

namespace engine {
    namespace ECSCommon {
        using namespace engine::ECS;
        
        class CameraRenderSystem : public System {
        public:
            CameraRenderSystem();
            CameraRenderSystem(const CameraRenderSystem& orig);
            virtual ~CameraRenderSystem();
            
            virtual void run(EntityManager& em, float deltaTimeSeconds) override;
            virtual bool isUpdateSystem() const override {
                return false;
            }
            virtual bool isRenderSystem() const override {
                return true;
            }
            
            std::string getSystemName() const override {
                return "CameraRenderSystem";
            }
            
            systemId_t getSystemTypeId() const override;
            
            static systemId_t systemTypeId();
            static void setSystemTypeId(systemId_t id);
        private:
            static systemId_t systemId;
            
            
        };        
    }
}

#endif /* CAMERARENDERSYSTEM_H */

