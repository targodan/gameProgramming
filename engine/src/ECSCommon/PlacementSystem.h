#ifndef PLACEMENTSYSTEM_H
#define PLACEMENTSYSTEM_H

#include "../ECS/EntityManager.h"
#include "../ECS/System.h"

namespace engine {
    namespace ECSCommon {
        using namespace engine::ECS;
        
        class PlacementSystem : public System {
        private:
            static systemId_t systemId;
            
        public:
            PlacementSystem() {}
            PlacementSystem(const PlacementSystem& orig) {}
            virtual ~PlacementSystem() {}
            
            void run(EntityManager& em, float dT) override;
            
            bool isUpdateSystem() const override {
                return true;
            }
            bool isRenderSystem() const override {
                return false;
            }
            
            Array<systemId_t> getOptionalDependencies() const override;
            
            systemId_t getSystemTypeId() const override;
            
            std::string getSystemName() const override {
                return "PlacementSystem";
            }
            
            static systemId_t systemTypeId();
            static void setSystemTypeId(systemId_t id);
        };
    }
}

#endif /* PLACEMENTSYSTEM_H */

