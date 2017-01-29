#ifndef MODELTRANSFORMATIONSYSTEM_H
#define MODELTRANSFORMATIONSYSTEM_H

#include "../ECS/EntityManager.h"
#include "../ECS/System.h"
#include "PlacementSystem.h"

namespace engine {
    namespace ECSCommon {
        using namespace engine::ECS;
        
        class ModelTransformationSystem : public System {
        private:
            static systemId_t systemId;
            
        public:
            ModelTransformationSystem() {}
            ModelTransformationSystem(const ModelTransformationSystem& orig) {}
            virtual ~ModelTransformationSystem() {}
            
            void run(EntityManager& em, float deltaTimeSeconds) override;
            
            bool isUpdateSystem() const override {
                return true;
            }
            bool isRenderSystem() const override {
                return true;
            }
            
            Array<systemId_t> getOptionalDependencies() const override;
            systemId_t getSystemTypeId() const override;
            std::string getSystemName() const override {
                return "ModelTransformationSystem";
            }
            
            static systemId_t systemTypeId();
            static void setSystemTypeId(systemId_t id);
        };
    }
}

#endif /* MODELTRANSFORMATIONSYSTEM_H */

