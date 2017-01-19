#ifndef FORCECOMPONENT_H
#define FORCECOMPONENT_H

#include "../ECS/Component.h"
#include "../physics/Force.h"

namespace engine {
    namespace ECSCommon {
        using namespace engine::ECS;
        
        class ForceComponent : public Component {
        private:
            static componentId_t typeId;
            
        protected:
            std::shared_ptr<engine::physics::Force> force;
            
        public:
            ForceComponent() {}
            ForceComponent(const std::shared_ptr<engine::physics::Force>& force) : force(force) {}
            ForceComponent(const ForceComponent& orig) = delete;
            virtual ~ForceComponent() {}
            
            const engine::physics::Force& getForce() const {
                return *this->force;
            }
            
            engine::physics::Force& getForce() {
                return *this->force;
            }
            
            componentId_t getComponentId() const override;
            std::string getComponentName() const override;
            std::string toString() const override;

            static void setComponentTypeId(componentId_t id);
            static componentId_t getComponentTypeId();
        };
    }
}

#endif /* FORCECOMPONENT_H */

