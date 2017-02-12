#ifndef FORCETIMERSYSTEM_H
#define FORCETIMERSYSTEM_H

#include "../ECS/EntityManager.h"
#include "../ECS/System.h"
#include "TimerSystem.h"

namespace engine {
    namespace ECSCommon {
        using namespace engine::ECS;

        class ForceTimerSystem : public System{
        public:
            ForceTimerSystem();
            ForceTimerSystem(const ForceTimerSystem& orig);
            virtual ~ForceTimerSystem();
            
            
            void run(EntityManager& em, float deltaTimeSeconds) override;
            
            bool isUpdateSystem() const override {
                return true;
            }
            bool isRenderSystem() const override {
                return false;
            }
            
            virtual Array<systemId_t> getDependencies() const {
                return {TimerSystem::systemTypeId()};
            }
            
            systemId_t getSystemTypeId() const override;
            
            std::string getSystemName() const override {
                return "ForceTimerSystem";
            }
            
            static systemId_t systemTypeId();
            static void setSystemTypeId(systemId_t id);
            
        private:
            static systemId_t systemId;

        };
    }
}


#endif /* FORCETIMERSYSTEM_H */

