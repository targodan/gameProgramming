#ifndef DEFORMABLEBODYCOMPONENT_H
#define DEFORMABLEBODYCOMPONENT_H

#include "../ECS/Component.h"
#include "../physics/DeformableBody.h"

namespace engine {
    namespace ECSCommon {
        using namespace engine::ECS;
        
        class DeformableBodyComponent : public Component {
        private:
            static componentId_t typeId;
            
        protected:
            std::shared_ptr<engine::physics::DeformableBody> deformableBody;
            
        public:
            DeformableBodyComponent() {}
            DeformableBodyComponent(const std::shared_ptr<engine::physics::DeformableBody>& deformableBody) : deformableBody(deformableBody) {}
            DeformableBodyComponent(const DeformableBodyComponent& orig) = delete;
            virtual ~DeformableBodyComponent() {}
            
            const engine::physics::DeformableBody& getDeformableBody() const {
                return *this->deformableBody;
            }
            
            engine::physics::DeformableBody& getDeformableBody() {
                return *this->deformableBody;
            }
            
            componentId_t getComponentId() const override;
            std::string getComponentName() const override;
            std::string toString() const override;

            static void setComponentTypeId(componentId_t id);
            static componentId_t getComponentTypeId();
        };
    }
}

#endif /* DEFORMABLEBODYCOMPONENT_H */

