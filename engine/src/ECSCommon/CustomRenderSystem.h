#ifndef CUSTOMRENDERSYSTEM_H
#define CUSTOMRENDERSYSTEM_H

#include "../ECS/System.h"

#include <functional>

namespace engine {
    namespace ECSCommon {
        using namespace engine::ECS;
        
        class CustomRenderSystem : public System {
        private:
            static systemId_t systemId;
            
            std::string name;
            std::function<void(EntityManager&, float)> function;
            
        public:
            CustomRenderSystem(const std::string name, const std::function<void(EntityManager&, float)>& function)
                : name(name), function(function) {}
            CustomRenderSystem(const CustomRenderSystem& orig) {}
            virtual ~CustomRenderSystem() {}
            
            virtual void run(EntityManager& em, float deltaTimeSeconds) override;
            virtual bool isUpdateSystem() const override {
                return false;
            }
            virtual bool isRenderSystem() const override {
                return true;
            }
            
            std::string getSystemName() const override {
                return this->name;
            }
            
            systemId_t getSystemTypeId() const override;
            
            static systemId_t systemTypeId();
            static void setSystemTypeId(systemId_t id);
        };        
    }
}

#endif /* CUSTOMRENDERSYSTEM_H */

