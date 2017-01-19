#ifndef TIMERSYSTEM_H
#define TIMERSYSTEM_H

#include "../ECS/System.h"

namespace engine {
    namespace ECSCommon {
        using namespace engine::ECS;
        
        class TimerSystem : public System {
        private:
            static systemId_t systemId;
            
        public:
            TimerSystem() {}
            TimerSystem(const TimerSystem& orig) {}
            virtual ~TimerSystem() {}
            
            virtual void run(EntityManager& em, float deltaTimeSeconds) override;
            virtual bool isUpdateSystem() const override {
                return true;
            }
            virtual bool isRenderSystem() const override {
                return false;
            }
            
            std::string getSystemName() const override {
                return "TimerSystem";
            }
            
            systemId_t getSystemTypeId() const override;
            
            static systemId_t systemTypeId();
            static void setSystemTypeId(systemId_t id);
        };        
    }
}

#endif /* TIMERSYSTEM_H */

