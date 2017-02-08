#ifndef LIGHTINGSYSTEM_H
#define LIGHTINGSYSTEM_H

#include "../ECS/EntityManager.h"
#include "../ECS/System.h"
#include "../renderer/Mapping.h"

namespace engine {
    namespace ECSCommon {
        using namespace engine::ECS;
        using util::Array;
        using renderer::Mapping;
        
        class LightingSystem : public System {
        private:
            static systemId_t systemId;
            
        public:
            LightingSystem() {}
            LightingSystem(const LightingSystem& orig) {}
            virtual ~LightingSystem() {}
            
            virtual void run(EntityManager& em, float deltaTimeSeconds) override;
            virtual bool isUpdateSystem() const override {
                return false;
            }
            virtual bool isRenderSystem() const override {
                return true;
            }
            
            std::string getSystemName() const override {
                return "LightingSystem";
            }
            
            systemId_t getSystemTypeId() const override;
            
            virtual Array<systemId_t> getDependencies() const override;
            virtual Array<systemId_t> getOptionalDependencies() const override;
            
            unsigned int nPreviousPointLights = 0;
            Mapping mapping = Mapping::NONE;
            
            static systemId_t systemTypeId();
            static void setSystemTypeId(systemId_t id);
        };        
    }
}

#endif /* LIGHTINGSYSTEM_H */

