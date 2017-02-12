#ifndef DEFORMABLEBODYSYSTEM_H
#define DEFORMABLEBODYSYSTEM_H

#include "../ECS/EntityManager.h"
#include "../ECS/System.h"
#include "ForceTimerSystem.h"

namespace engine {
    namespace ECSCommon {
        using namespace engine::ECS;
        
        class DeformableBodySystem : public System {
        private:
            static systemId_t systemId;
            
        public:
            DeformableBodySystem() {}
            DeformableBodySystem(const DeformableBodySystem& orig) {}
            virtual ~DeformableBodySystem() {}
            
            void run(EntityManager& em, float deltaTimeSeconds) override;
            
            bool isUpdateSystem() const override {
                return true;
            }
            bool isRenderSystem() const override {
                return false;
            }
            
            virtual Array<systemId_t> getOptionalDependencies() const {
                return {ForceTimerSystem::systemTypeId()};
            }
            
            systemId_t getSystemTypeId() const override;
            
            std::string getSystemName() const override {
                return "DeformableBodySystem";
            }
            
            static systemId_t systemTypeId();
            static void setSystemTypeId(systemId_t id);
        };
    }
}

#endif /* DEFORMABLEBODYSYSTEM_H */

